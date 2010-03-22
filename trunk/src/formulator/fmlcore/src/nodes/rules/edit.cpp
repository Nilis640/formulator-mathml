/****************************************************************************
**
** Copyright (C) 2010 Andriy Kovalchuk, Vyacheslav Levytskyy,
** Igor Samolyuk, Valentyn Yanchuk (aka "Hermitech Laboratory")
**
** All rights reserved.
** Contact: Hermitech Laboratory (info@mmlsoft.com)
**
** This file is a part of the Formulator MathML Editor project
** (http://www.mmlsoft.com).
**
** Commercial Usage
** Licensees holding valid Formulator Commercial licenses may use this 
** file in accordance with the Formulator Commercial License Agreement 
** provided with the Software or, alternatively, in accordance with the 
** terms contained in a written agreement between you and 
** Hermitech Laboratory.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Hermitech Laboratory at info@mmlsoft.com.
**
****************************************************************************/

#include "HUtils/iundoredo.h"
#include "HUtils/idfas.h"

#include "../ni_root.h"
#include "../mathml/n_cm_treetemplate.h"
#include "../../dictionary/op_autokeyword.h"
#include "../../dictionary/op_iddefs.h"

#include "copy.h"
#include "edit.h"
#include "nav.h"
#include "insert.h"
#include "mouse.h"

/////////////////////////////////////////////////////////////////////////////

inline
int UpdateBeginEndTextPosition( SelectNode &sn )
{
	if( sn.GetNode() && sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		if( sn.GetPosition() == 0 )
		{
			sn.SetPosition( sn.GetNode()->GetParent()->GetChildIndex( sn.GetNode() ) );
			sn.SetNode( sn.GetNode()->GetParent() );
		}
		else if( sn.GetPosition() == ((CPlaneText*) sn.GetNode())->GetStringLength() )
		{
			sn.SetPosition( sn.GetNode()->GetParent()->GetChildIndex( sn.GetNode() ) + 1 );
			sn.SetNode( sn.GetNode()->GetParent() );
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void BreakPlaneText( SelectNode &sn, int fUpCalc, SelectNode *psn, SelectNode* /*pse*/ )
{
	Q_ASSERT( sn.GetNode()->canBeEdited() );
	Q_ASSERT( sn.GetNode()->GetType() == NODE_PLANETEXT );

	CPlaneText *pSrcPlaneText = (CPlaneText*) sn.GetNode();
	if( sn.GetPosition() == 0 )
	{
		CParentNode *pParentNode = pSrcPlaneText->GetParent();
		long nPosition = pParentNode->GetChildIndex( pSrcPlaneText );

		sn.SetNode( pParentNode );
		sn.SetPosition( nPosition );
	}
	else if( sn.GetPosition() == pSrcPlaneText->GetStringLength() )
	{
		CParentNode *pParentNode = pSrcPlaneText->GetParent();
		long nPosition = pParentNode->GetChildIndex( pSrcPlaneText );

		sn.SetNode( pParentNode );
		sn.SetPosition( nPosition + 1 );
	}
	else
	{
		CPlaneText *pDstPlaneText = new CPlaneText( pSrcPlaneText, sn.GetPosition(), -1 );
		pDstPlaneText->SetLevel( pSrcPlaneText->GetLevel() );
		
		CParentNode *pParentNode = pSrcPlaneText->GetParent();
		long nPosition = pParentNode->GetChildIndex( pSrcPlaneText );

		pParentNode->InsertChild( nPosition + 1, pDstPlaneText );

		pSrcPlaneText->RecalculateSize( 0 );
		pDstPlaneText->RecalculateSize( 0 );

		if( fUpCalc )
			pParentNode->RecalculateSize( fUpCalc );

		if( psn )
		{
			if( sn.GetNode() == psn->GetNode() )
			{
				if( sn.GetPosition() <= psn->GetPosition() )
				{
					psn->SetNode( pDstPlaneText );
					psn->SetPosition( psn->GetPosition() - sn.GetPosition() );
				}
			}
			else if( sn.GetNode()->GetParent() == psn->GetNode() )
			{
				long nIndex = sn.GetNode()->GetParent()->GetChildIndex( sn.GetNode() );
				if( nIndex < psn->GetPosition() )
				{
					psn->IncPosition();
				}
			}
		}

		sn.SetNode( pParentNode );
		sn.SetPosition( nPosition + 1 );
	}

	if( psn ) UpdateBeginEndTextPosition( *psn );
}

/////////////////////////////////////////////////////////////////////////////

void BreakLine( SelectNode &sn, int fUpCalc, SelectNode *psn, SelectNode* /*pse*/ )
{
	Q_ASSERT( sn.GetNode()->canBeEdited() );

	if( sn.GetNode()->GetType() == NODE_PLANETEXT ) BreakPlaneText( sn, -1, psn );
	
	Q_ASSERT( sn.GetNode()->canBeEdited() );
	Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );
	
	CLineNode *pSrcLineNode = (CLineNode*) sn.GetNode();
	CLineNode *pDstLineNode = new CLineNode( pSrcLineNode, sn.GetPosition(), -1 );
	pDstLineNode->SetLevel( pSrcLineNode->GetLevel() );

	pSrcLineNode->GetParent()->AddChildAfter( pDstLineNode, pSrcLineNode );

	pSrcLineNode->RecalculateSize( 0 );
	pDstLineNode->RecalculateSize( 0 );

	if( psn && sn.GetNode() == psn->GetNode() && sn.GetPosition() < psn->GetPosition() )
	{
		psn->SetNode( pDstLineNode );
		psn->SetPosition( psn->GetPosition() - sn.GetPosition() );
	}

	sn.SetNode( pDstLineNode );
	sn.SetPosition( 0 );

	pDstLineNode->GetParent()->RecalculateSize( fUpCalc );
}

/////////////////////////////////////////////////////////////////////////////

inline 
void UpdateSelectNode4GlueText( CPlaneText *pPlaneText, SelectNode &sn )
{
	Q_ASSERT( pPlaneText != NULL && pPlaneText->GetNext() != NULL );
	if( pPlaneText->GetNext() == sn.GetNode() )
	{
		sn.SetNode( pPlaneText );
		sn.SetPosition( pPlaneText->GetStringLength() + sn.GetPosition() );
	}
	else if( pPlaneText->GetParent() == sn.GetNode() )
	{
		long nPos = pPlaneText->GetParent()->GetChildIndex( pPlaneText->GetNext() );
		if( nPos < sn.GetPosition() ) sn.DecPosition();
	}
}

/////////////////////////////////////////////////////////////////////////////

int GlueText( SelectNode &sn, CNode *pPrev, int fUpCalc, SelectNode *psn, SelectNode* /*pse*/ )
{
	if( pPrev && pPrev->GetType() == NODE_PLANETEXT && 
		pPrev->GetNext() && pPrev->GetNext()->GetType() == NODE_PLANETEXT && 
		((CPlaneText*) pPrev)->isGluing( (CPlaneText*) pPrev->GetNext() ) )
	{
		if( psn ) UpdateSelectNode4GlueText( (CPlaneText*) pPrev, *psn );

		sn.SetNode( pPrev );
		sn.SetPosition( ((CPlaneText*) pPrev)->GetStringLength() );

		((CPlaneText*) pPrev)->Glue( (CPlaneText*) pPrev->GetNext() );
		pPrev->GetParent()->DeleteChild( pPrev->GetNext() );

		pPrev->RecalculateSize( fUpCalc );
		if( fUpCalc == 0 )
			pPrev->GetParent()->RecalculateSize( fUpCalc );

		if( psn ) UpdateBeginEndTextPosition( *psn );
		return 0;
	}
	return -1;
}

int GlueLine( SelectNode &sn, CNode *pPrev, int fUpCalc, SelectNode *psn, SelectNode* /*pse*/ )
{
	if( pPrev && pPrev->GetType() == NODE_LINE && 
		pPrev->GetNext() && pPrev->GetNext()->GetType() == NODE_LINE &&
		((CLineNode*) pPrev)->isGluing( (CLineNode*) pPrev->GetNext() ) )
	{
		sn.SetNode( pPrev );
		sn.SetPosition( ((CLineNode*) pPrev)->GetChildCount() );

		if( psn && pPrev->GetNext() == psn->GetNode() )
		{
			psn->SetNode( pPrev );
			psn->SetPosition( ((CLineNode*) pPrev)->GetChildCount() + psn->GetPosition() );
		}

		CNode *pLast = ((CLineNode*) pPrev)->GetLastChild();

		QVector<CNode*> children;
		((CLineNode*) pPrev->GetNext())->RemoveOrCopyChildren( children );
		((CLineNode*) pPrev)->InsertChildren( children );

		Q_ASSERT( pPrev->GetParent() != NULL );
		pPrev->GetParent()->DeleteChild( pPrev->GetNext() );

		if( GlueText( sn, pLast, fUpCalc, psn ) == -1 )
		{
			pPrev->RecalculateSize( fUpCalc );
			pPrev->GetParent()->RecalculateSize( fUpCalc );
		}
		return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

inline 
void UpdateSelectNode4GlueTextInLine( CLineNode *pLineNode, long nIndex, SelectNode &sn )
{
	if( sn.GetNode() == pLineNode )
	{
		if( sn.GetPosition() == nIndex )
		{
			sn.SetNode( pLineNode->GetChild( nIndex - 1 ) );
			sn.SetPosition( ((CPlaneText*) pLineNode->GetChild( nIndex - 1 ))->GetStringLength() );
		}
		else if( sn.GetPosition() > nIndex )
		{
			sn.DecPosition();
		}
	}
}

int GlueTextInLine( CLineNode *pLineNode, SelectInfo *psi )
{
	if( pLineNode->GetChildCount() == 0 ) return -1;

	long nIndex = 1;
	CNode *pPrev = pLineNode->GetFirstChild();
	while ( pPrev && pPrev->GetNext() )
	{
		if( pPrev->GetType() == NODE_PLANETEXT && 
			pPrev->GetNext()->GetType() == NODE_PLANETEXT && 
			((CPlaneText*) pPrev)->isGluing( (CPlaneText*) pPrev->GetNext() ) )
		{
			if( psi )
			{
				UpdateSelectNode4GlueTextInLine( pLineNode, nIndex, psi->GetBegin() );
				UpdateSelectNode4GlueTextInLine( pLineNode, nIndex, psi->GetEnd() );
			}

			((CPlaneText*) pPrev)->Glue( (CPlaneText*) pPrev->GetNext() );
			pLineNode->DeleteChild( pPrev->GetNext() );
	    
			pPrev->RecalculateSize( 0 );
		}
		else
		{
			pPrev = pPrev->GetNext();
			nIndex++;
		}
	}

	if( psi )
	{
		UpdateBeginEndTextPosition( psi->GetBegin() );
		UpdateBeginEndTextPosition( psi->GetEnd() );
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int MoveDelBs_SetNewFocus( CPlaneText *pOperatorNode, SelectNode &sn, int isSNBeforeOpNode )
{
	if( !pOperatorNode ) return 0;

	CNode *pNode = 0;
	if( isSNBeforeOpNode )
	{
		if( pOperatorNode->GetPrev() )
			pNode = pOperatorNode->GetPrev();
	}
	else
	{
		if( pOperatorNode->GetNext() )
			pNode = pOperatorNode->GetNext();
	}

	return MoveCaret( pNode, isSNBeforeOpNode, sn );
}

int MoveDelBs_RemoveOperatorFromApply( CPlaneText *pOperatorNode, CLineNode *pParentLine, SelectNode &sn, int isSNBeforeOpNode, HAutodetectMulticharOp *autoMulticharOp )
{
	if( pOperatorNode->isFence() ) return 0;

	CParentNode *pParentNode = pParentLine->GetParent();
	if( pParentNode && pParentNode->GetParent() && pParentNode->GetParent()->GetType() == NODE_FORMULA )
	{
		CFormulaNode *applyNode = (CFormulaNode*) (pParentNode->GetParent());
		if( applyNode->to_mathml_data.is_content &&
			(::mml_is_cm_apply( applyNode->to_mathml_data.name ) || 
			applyNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR) )
		{
			CPlaneText *nopNode = autoMulticharOp ? autoMulticharOp->getOperator1( HF_BTNID_NOP_VALUE ) : 0;
			Q_ASSERT( nopNode != NULL );
			if( nopNode )
			{
				sn.SetPosition( applyNode->GetParent()->GetChildIndex( applyNode ) + (isSNBeforeOpNode ? 0 : 1) );
				sn.SetNode( applyNode->GetParent() );

				pOperatorNode->SetString( nopNode->GetString() );
				pOperatorNode->setNodeInfo( nopNode );
				pOperatorNode->setNodeExInfo( nopNode );

				pOperatorNode->setFStyle( nopNode->getFStyle(), pOperatorNode->getOtherStyleFont() );
				pOperatorNode->RecalculateSize( 0 );
				applyNode->to_mathml_data.precedence = (short) ::mml_getPrecedence( TAG2MML_nop, 2, QString() );
				applyNode->to_mathml_data.assoc = (short) ::mml_getAssoc( TAG2MML_nop );
				applyNode->CM_WrapChildrenIntoBracketsOnPrecedence();
				applyNode->CM_WrapIntoBracketsOnPrecedence();
				applyNode->RecalculateSizeFull();

				::MoveDelBs_SetNewFocus( pOperatorNode, sn, isSNBeforeOpNode );
				return 1;
			}
		}
	}
	return 0;
}

int MoveDelBs_RemoveOperatorFromApply( CFormulaNode * /*pOperatorNode*/, CLineNode *pParentLine, SelectNode &sn, int isSNBeforeOpNode, HAutodetectMulticharOp *autoMulticharOp )
{
	CParentNode *pParentNode = pParentLine->GetParent();
	if( pParentNode && pParentNode->GetParent() && pParentNode->GetParent()->GetType() == NODE_FORMULA )
	{
		CFormulaNode *applyNode = (CFormulaNode*) (pParentNode->GetParent());
		if( applyNode->to_mathml_data.is_content && (::mml_is_cm_apply( applyNode->to_mathml_data.name ) || 
													 applyNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR) )
		{
			CPlaneText *nopNode = autoMulticharOp ? autoMulticharOp->getOperator1( HF_BTNID_NOP_VALUE ) : 0;
			Q_ASSERT( nopNode != NULL );
			if( nopNode )
			{
				sn.SetPosition( applyNode->GetParent()->GetChildIndex( applyNode ) + (isSNBeforeOpNode ? 0 : 1) );
				sn.SetNode( applyNode->GetParent() );

				CNode *todeleteNode = 0;
				pParentLine->ExchangeChild( 0, new CPlaneText( nopNode ), todeleteNode );
				if( todeleteNode ) delete todeleteNode;
				applyNode->to_mathml_data.precedence = (short) ::mml_getPrecedence( TAG2MML_nop, 1, QString() );
				applyNode->to_mathml_data.assoc = (short) ::mml_getAssoc( TAG2MML_nop );
				applyNode->CM_WrapChildrenIntoBracketsOnPrecedence();
				applyNode->CM_WrapIntoBracketsOnPrecedence();
				applyNode->RecalculateSizeFull();

				if( pParentLine->GetFirstChild() && pParentLine->GetFirstChild()->GetType() == NODE_PLANETEXT )
					::MoveDelBs_SetNewFocus( (CPlaneText*) pParentLine->GetFirstChild(), sn, isSNBeforeOpNode );
				else
					Q_ASSERT( 0 );
				return 1;
			}
		}
	}
	return 0;
}

int MoveDelBs_RemoveBrackets( CFormulaNode* bracketsNode, SelectNode &sn, int isSNBeforeBracket )
{
	if( bracketsNode == 0 || bracketsNode->GetParent() == 0 || bracketsNode->GetParent()->GetType() != NODE_LINE ||
		bracketsNode->to_mathml_data.is_content == 0 ||
		bracketsNode->getCalcButtonID() != HF_BTNID_DEFAULT_BRACKET_LR )
	{
		return 0;
	}
	CLineNode *parentBracketedLineNode = (CLineNode*) bracketsNode->GetParent();
	CLineNode *removeBrackets_Parent = 0, *removeBrackets_Content = 0;

	CLineNode *focusLine = 0;

	CFormulaNode *focusFormula = (CFormulaNode*) bracketsNode->getSingleFormulaChild(), *operandNode = 0;
	if( focusFormula )
	{
		focusFormula = focusFormula->passAutodetectSingleFormulaChildren();
		focusLine = focusFormula->getContentFormulaSingleLineChild();
		operandNode = bracketsNode->passAutodetectParent( NULL );

		removeBrackets_Parent = 0;
		removeBrackets_Content = bracketsNode->getContentFormulaSingleLineChild();
	}
	else if( parentBracketedLineNode->GetChildCount() == 1 )
	{
		focusFormula = parentBracketedLineNode->getParentFormulaNode( 0 );
		if( focusFormula && focusFormula->getContentFormulaSingleLineChild() == parentBracketedLineNode )
		{
			focusLine = bracketsNode->passAutodetectSingleFormulaChildren()->getContentFormulaSingleLineChild();
			operandNode = focusFormula->passAutodetectParent( NULL );

			removeBrackets_Parent = parentBracketedLineNode;
			removeBrackets_Content = focusLine;
		}
	}

	if( operandNode == 0 || operandNode->GetParent() == 0 ||
		focusLine == 0 ||
		focusFormula == 0 || focusFormula->GetChildCount() != 1 ||
		focusFormula->to_mathml_data.is_content == 0 || !::mml_is_cm_apply( focusFormula->to_mathml_data.name ) )
	{
		return 0;
	}

	if( /*removeBrackets_Parent == 0 || */removeBrackets_Content == 0 )
	{
		return 0;
	}

	if( focusLine->GetChildCount() == 1 )
	{
		return 0;
	}

	long bracketsPosition = -1;
	CFormulaNode *parentApply = operandNode->GetParent()->getParentFormulaNode( 0 );
	CLineNode *parentApplyLine = parentApply ? parentApply->getContentFormulaSingleLineChild() : 0;
	if( parentApplyLine && parentApplyLine == operandNode->GetParent() &&
		parentApply && parentApply->GetParent() && parentApply->to_mathml_data.is_content )
	{
		if( parentApply->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
		{
			CFormulaNode *pFormulaNode = parentApply->GetParent()->getParentFormulaNode( 0 );
			if( pFormulaNode && ((CFormulaNode*) pFormulaNode)->getSingleFormulaChild() == parentApply && pFormulaNode->to_mathml_data.is_content )
				parentApply = pFormulaNode;
		}
		if( ::mml_is_cm_apply( parentApply->to_mathml_data.name ) )
			bracketsPosition = parentApplyLine->GetChildIndex( operandNode );
	}
	if( bracketsPosition == -1 )
	{
		return 0;
	}

	long parentApplyPosition = -1;
	CFormulaNode *frm = parentApply->passAutodetectParent( NULL );
	CFormulaNode *grandParentApply = frm->GetParent() ? frm->GetParent()->getParentFormulaNode( 0 ) : 0;
	CLineNode *grandParentApplyLine = grandParentApply ? grandParentApply->getContentFormulaSingleLineChild() : 0;
	if( grandParentApplyLine && grandParentApplyLine == frm->GetParent() &&
		grandParentApply && grandParentApply->GetParent() && grandParentApply->to_mathml_data.is_content )
	{
		if( grandParentApply->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
		{
			CFormulaNode *pFormulaNode = grandParentApply->GetParent()->getParentFormulaNode( 0 );
			if( pFormulaNode && ((CFormulaNode*) pFormulaNode)->getSingleFormulaChild() == grandParentApply && pFormulaNode->to_mathml_data.is_content )
				grandParentApply = pFormulaNode;
		}
		if( ::mml_is_cm_apply( grandParentApply->to_mathml_data.name ) )
			parentApplyPosition = grandParentApplyLine->GetChildIndex( frm );
	}

	CFormulaNode /* *activeSlot = 0, */*subst_1 = 0, *subst_2 = 0, *slot_1 = 0, *slot_2 = 0, *newFocusNode = 0;
	std::vector<CFormulaNode*> checklist;
	if( bracketsPosition == 0 )
	{
		if( focusLine->GetLastChild() && focusLine->GetLastChild()->GetType() == NODE_FORMULA )
		{
			subst_1 = parentApply;
			slot_1 = (CFormulaNode*) focusLine->GetLastChild();
			if( grandParentApply && grandParentApplyLine && parentApplyPosition == grandParentApplyLine->GetChildCount() - 1 )
			{
				if( focusLine->GetFirstChild() && focusLine->GetFirstChild()->GetType() == NODE_FORMULA )
				{
					subst_2 = grandParentApply;
					slot_2 = (CFormulaNode*) focusLine->GetFirstChild();
				}
			}
		}
		else
		{
		}
	}
	else if( bracketsPosition == parentApplyLine->GetChildCount() - 1 )
	{
		if( focusLine->GetFirstChild() && focusLine->GetFirstChild()->GetType() == NODE_FORMULA )
		{
			subst_1 = parentApply;
			slot_1 = (CFormulaNode*) focusLine->GetFirstChild();
			if( grandParentApply && grandParentApplyLine && parentApplyPosition == 0 )
			{
				if( focusLine->GetLastChild() && focusLine->GetLastChild()->GetType() == NODE_FORMULA )
				{
					subst_2 = grandParentApply;
					slot_2 = (CFormulaNode*) focusLine->GetLastChild();
				}
			}
		}
		else
		{
		}
	}
	else if( bracketsPosition > 0 && bracketsPosition < parentApplyLine->GetChildCount() - 1 )
	{
		return 0;
	}
	else
	{
		Q_ASSERT( 0 );
		return 0;
	}

	if( subst_1 && slot_1 )
	{
		newFocusNode = isSNBeforeBracket ? focusLine->getFirstFormulaChild() : focusLine->getLastFormulaChild();

		if( subst_1->GetParent() )
		{
			sn.SetPosition( subst_1->GetParent()->GetChildIndex( subst_1 ) );
			sn.SetNode( subst_1->GetParent() );
		}

		if( removeBrackets_Parent == 0 )
		{
			focusFormula->adjustReadOnlyAttr( parentBracketedLineNode );

			CNode *deepChild = focusFormula->GetParent()->RemoveChild( 0 );
			Q_ASSERT( deepChild );
			parentBracketedLineNode->ExchangeChild( bracketsNode, deepChild );
			delete bracketsNode;
			bracketsNode = 0;
		}
		else
		{
			removeBrackets_Parent->InsertMML_CM_remove_outof_brackets( removeBrackets_Content );
		}

		CFormulaNode *activeSlot = CFormulaNode::reorderFormulaBracketsTree( slot_1, focusFormula, subst_1, 0 );

		if( !slot_1->isAutodetectContent() )
			slot_1->adjustReadOnlyAttr4Parent();
		if( !focusFormula->isAutodetectContent() )
			focusFormula->adjustReadOnlyAttr4Parent();
		if( !subst_1->isAutodetectContent() )
			subst_1->adjustReadOnlyAttr4Parent();

		checklist.push_back( slot_1 );
		checklist.push_back( subst_1 );
		
		if( subst_2 && slot_2 )
		{
			activeSlot = CFormulaNode::reorderFormulaBracketsTree( slot_2, activeSlot, subst_2, 0 );

			if( !slot_2->isAutodetectContent() )
				slot_2->adjustReadOnlyAttr4Parent();
			if( !activeSlot->isAutodetectContent() )
				activeSlot->adjustReadOnlyAttr4Parent();
			if( !subst_2->isAutodetectContent() )
				subst_2->adjustReadOnlyAttr4Parent();

			checklist.push_back( slot_2 );
			checklist.push_back( subst_2 );
		}
		checklist.push_back( activeSlot );

		for( long i = 0; i < (long) checklist.size(); i++ )
		{
			CFormulaNode *checkBrackets = checklist[ i ]->passAutodetectSingleFormulaChildren();
			CLineNode *bracketLine = checkBrackets->CM_WrapIntoBracketsOnPrecedence();
			if( bracketLine && checkBrackets->GetParent() )
			{
				CLineNode* currentParentLine = (CLineNode*) checkBrackets->GetParent();
				currentParentLine->InsertMML_CM_remove_outof_brackets( bracketLine );
			}
		}

		activeSlot->cleanSpareAutodetect();

		activeSlot->RecalculateSizeFull();
		activeSlot->RecalculateSize( 1 );

		if( newFocusNode )
			newFocusNode = newFocusNode->passAutodetectSingleFormulaChildren();
		long childIndexOfRequest = 0;
		CLineNode *newFocusLine = newFocusNode ? newFocusNode->getParentLineNode( 0 /*don't use read-only nodes*/, &childIndexOfRequest ) : 0;
		if( newFocusLine )
		{
			sn.SetPosition( childIndexOfRequest + (isSNBeforeBracket ? 0 : 1) );
			sn.SetNode( newFocusLine );
		}
		else
		{
			sn.SetPosition( activeSlot->GetParent()->GetChildIndex( activeSlot ) );
			sn.SetNode( activeSlot->GetParent() );
		}

		return 1;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int MoveDel( SelectNode &sn, SelectInfo &si, HAutodetectMulticharOp *autoMulticharOp, CUndoRedo *pUndoRedo )
{
	if( si.IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		int nRet = ::RemoveToFrame( pFrameNode, sn, si, 1 );
		delete pFrameNode;
		return nRet;
	}
	else if( sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		Q_ASSERT( sn.GetNode()->canBeEdited() );

		Q_ASSERT( ((CPlaneText*) sn.GetNode())->GetStringLength() > 1 );

		((CPlaneText*) sn.GetNode())->Delete( sn.GetPosition() );
		sn.GetNode()->RecalculateSize( 1 );

		if( sn.GetPosition() == ((CPlaneText*) sn.GetNode())->GetStringLength() )
		{
			if( GlueText( sn, sn.GetNode() ) == -1 )
			{
				sn.SetPosition( 
					sn.GetNode()->GetParent()->GetChildIndex( sn.GetNode() ) + 1);
				sn.SetNode( sn.GetNode()->GetParent() );
			}
		}

		return 0;
	}
	else if( sn.GetNode()->GetType() == NODE_LINE )
	{
		Q_ASSERT( sn.GetNode()->canBeEdited() );

		CLineNode *pLineNode = (CLineNode*) sn.GetNode();

		CNode *pNode = (sn.GetPosition() < pLineNode->GetChildCount() ? pLineNode->GetChild( sn.GetPosition() ) : NULL);
		if( pNode != NULL )
		{
			if( pNode->GetType() == NODE_PLANETEXT )
			{
				if( pNode->isCannotBeEdited() )
				{
					if( pNode->canBeMoved() )
					{
						::MoveLeftRight( sn, si, 0, -1, 0, 0 );
						sn = si.GetBegin();
					}
					else
					{
						if( ::MoveDelBs_RemoveOperatorFromApply( (CPlaneText*) pNode, pLineNode, sn, 1, autoMulticharOp ) )
						{
							sn.GetNode()->RecalculateSize( 1 );
							return 0;
						}
					}
					return -1;
				}

				if( ((CPlaneText*) pNode)->GetStringLength() == 1 )
				{
					pNode = pNode->GetPrev();
					pLineNode->DeleteChild( sn.GetPosition() );
					if( GlueText( sn, pNode ) == -1 )
					{
						pLineNode->RecalculateSize( 1 );
					}
				}
				else
				{
					((CPlaneText*) pNode)->Delete( 0 );
					pNode->RecalculateSize( 1 );
				}

				return 0;
			}
			else
			{
				::MoveLeftRight( sn, si, 0, -1, 0, 0 );
				sn = si.GetBegin();

				CFormulaNode *pBracketNode = 0;
				if( pNode->GetType() == NODE_FORMULA )
				{
					CLineNode *lineNode = 0;
					for( CFormulaNode *pFormulaNode = (CFormulaNode*) pNode;
						pFormulaNode && (pFormulaNode->to_mathml_data.is_content || pFormulaNode->isAutodetectContent()); )
					{
						pFormulaNode = pFormulaNode->passAutodetectSingleFormulaChildren();
						if( pFormulaNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
						{
							pBracketNode = pFormulaNode;
							break;
						}
						else
						{
							lineNode = pFormulaNode->getContentFormulaSingleLineChild();
							if( lineNode && lineNode->GetFirstChild() && lineNode->GetFirstChild()->GetType() == NODE_FORMULA )
								pFormulaNode = (CFormulaNode*) lineNode->GetFirstChild();
							else
								break;
						}
					}

					if( pBracketNode )
					{
						if( pUndoRedo != NULL )
							pUndoRedo->CheckPoint( 1 );
						::MoveLeftRight( sn, si, 1, 0, 0, 0 );
						int wasDeleted = ::MoveDelBs_RemoveBrackets( pBracketNode, sn, 1 );
						if( !wasDeleted )
						{
							if( pUndoRedo != NULL && pUndoRedo->CanUndo() )
								pUndoRedo->Undo();
						}
						else
						{
							return 0;
						}
					}
				}

				return -1;
			}
		}
		else
		{
			if( ::GlueLine( sn, pLineNode, 1 ) == 0 )
			{
				// удалось
				return 0;
			}
			else
			{
/*
				Q_ASSERT( pLineNode->GetParent() != NULL );
				CParentNode *pParentNode = pLineNode->GetParent()->GetParent();
				if( pParentNode != NULL && 
					pParentNode->GetType() == NODE_FORMULA &&
					pParentNode->canBeMoved() && 
					pParentNode->GetParent() && 
					pParentNode->GetParent()->canBeEdited() )
				{
					si.GetBegin().SetPosition( 
						pParentNode->GetParent()->GetChildIndex( pParentNode ) );
					si.GetBegin().SetNode( pParentNode->GetParent() );
					si.GetEnd().SetPosition( 
						 si.GetBegin().GetPosition() + 1 );
					si.GetEnd().SetNode( pParentNode->GetParent() );

					sn = si.GetBegin();
				}
*/

				Q_ASSERT( pLineNode->GetParent() != NULL );
				CParentNode *pParentNode = pLineNode->GetParent()->GetParent();
				if( pParentNode && pParentNode->GetType() == NODE_FORMULA 
					&& pParentNode->GetParent() && pParentNode->GetParent()->GetType() == NODE_LINE )
				{
					long frameIdx = pParentNode->GetChildIndex( pLineNode->GetParent() );
					long childPos = pParentNode->GetParent()->GetChildIndex( pParentNode );
					bool the2ndFrame = (((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP ||
										((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_SUP) && (frameIdx == 0);
					bool isDivision = false, isSup = false;
					if( pParentNode->canBeMoved() && pParentNode->GetParent()->canBeEdited() && !the2ndFrame )
					{
						si.GetBegin().SetPosition( childPos );
						si.GetBegin().SetNode( pParentNode->GetParent() );
						si.GetEnd().SetPosition( 
							 si.GetBegin().GetPosition() + 1 );
						si.GetEnd().SetNode( pParentNode->GetParent() );

						sn = si.GetBegin();
					}
					else
					{
						if( childPos + 1 == pParentNode->GetParent()->GetChildCount() )
						{
							if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP &&
								frameIdx == 0 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
							{
								isDivision = true;
							}
							else if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_SUP &&
								frameIdx == 0 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
							{
								isSup = true;
							}
							else
							{
								do
								{
									pParentNode = pParentNode->GetParent()->GetParent();
									if( pParentNode && pParentNode->GetParent() )
									{
										frameIdx = pParentNode->GetParent()->GetChildIndex( pParentNode );
										pParentNode = pParentNode->GetParent();
									}

									if( pParentNode && pParentNode->GetType() == NODE_FORMULA 
										&& pParentNode->GetParent() && pParentNode->GetParent()->GetType() == NODE_LINE /*&&
										!pParentNode->canBeMoved() || !pParentNode->GetParent()->canBeEdited()*/ )
									{
										if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP &&
											frameIdx == 0 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
										{
											childPos = pParentNode->GetParent()->GetChildCount() - 1;
											isDivision = true;
											break;
										}
										else if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_SUP &&
											frameIdx == 0 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
										{
											childPos = pParentNode->GetParent()->GetChildCount() - 1;
											isSup = true;
											break;
										}
										else
											childPos = pParentNode->GetParent()->GetChildIndex( pParentNode );
									}
									else
										break;
								}
								while( childPos + 1 == pParentNode->GetParent()->GetChildCount() );
								if( pParentNode && pParentNode->GetParent() &&
									childPos + 1 != pParentNode->GetParent()->GetChildCount() &&
									pParentNode->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
								{
									childPos = pParentNode->GetParent()->GetChildCount() - 1;
								}
							}
						}

						if( (isDivision || isSup) && pParentNode && pParentNode->GetParent() )
						{
							sn.SetNode( pParentNode->GetParent() );
							sn.SetPosition( 0 );
							enum TAG2MML op_id = isDivision ? TAG2MML_divide : (isSup ? TAG2MML_power : TAG2MML_nop);
							CFormulaNode* newFormulaNode = ((CLineNode*) pParentNode->GetParent())->convertDivSup2Nop( (CFormulaNode*&) pParentNode, op_id, 0 );
							if( newFormulaNode && newFormulaNode->GetParent() && newFormulaNode->GetParent()->GetType() == NODE_LINE )
							{
								CLineNode *newFormulaLine = newFormulaNode->getContentFormulaSingleLineChild();
								CNode *opNode = newFormulaLine ? newFormulaLine->GetChild( 1 ) : NULL;
								if( opNode && opNode->GetType() == NODE_PLANETEXT )
								{
									if( ::MoveDelBs_RemoveOperatorFromApply( (CPlaneText*) opNode, newFormulaLine, sn, 1 /**/, autoMulticharOp ) )
									{
										((CLineNode*) newFormulaNode->GetParent())->RecalculateSize( 1 );
										return 0;
									}
								}
								newFormulaNode = ((CLineNode*) newFormulaNode->GetParent())->convertDivSup2Nop( newFormulaNode, op_id, 1 );
								if( newFormulaNode && newFormulaNode->GetParent() )
								{
									newFormulaNode->RecalculateSizeFull();
									((CLineNode*) newFormulaNode->GetParent())->RecalculateSize( 1 );
								}
							}
						}
						else if( pParentNode && pParentNode->GetParent() &&
							(childPos + 1) < pParentNode->GetParent()->GetChildCount() &&
							((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
						{
							CLineNode *pParentLine = (CLineNode*) pParentNode->GetParent();
							CNode *opNode = pParentLine->GetChild( childPos + 1 /**/);
							if( opNode && opNode->GetType() == NODE_PLANETEXT )
							{
								if( ::MoveDelBs_RemoveOperatorFromApply( (CPlaneText*) opNode, pParentLine, sn, 1, autoMulticharOp ) )
								{
									sn.GetNode()->RecalculateSize( 1 );
									return 0;
								}
							}
						}
					}
				}

				return -1;
			}
		}
	}
	else
	{
		Q_ASSERT( 0 );
	}

	sn.GetNode()->RecalculateSize( 0 );
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveBs( SelectNode &sn, SelectInfo &si, HAutodetectMulticharOp *autoMulticharOp, CUndoRedo *pUndoRedo )
{
	if( si.IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		int nRet = ::RemoveToFrame( pFrameNode, sn, si, 1 );
		delete pFrameNode;
		return nRet;
	}
	else if( sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		Q_ASSERT( sn.GetNode()->canBeEdited() );

		Q_ASSERT( ((CPlaneText*) sn.GetNode())->GetStringLength() > 1 );

		sn.SetPosition( sn.GetPosition() - 1 );
		((CPlaneText*) sn.GetNode())->Delete( sn.GetPosition() );
		sn.GetNode()->RecalculateSize( 1 );

		if( sn.GetPosition() == 0 )
		{
			if( GlueText( sn, sn.GetNode()->GetPrev() ) == -1 )
			{
				sn.SetPosition( 
					sn.GetNode()->GetParent()->GetChildIndex( sn.GetNode() ) );
				sn.SetNode( sn.GetNode()->GetParent() );
			}
		}

		return 0;
	}
	else if( sn.GetNode()->GetType() == NODE_LINE )
	{
		Q_ASSERT( sn.GetNode()->canBeEdited() );

		CLineNode *pLineNode = (CLineNode*) sn.GetNode();

		CNode *pNode = (sn.GetPosition() ? pLineNode->GetChild( sn.GetPosition() - 1 ) : NULL);
		if( pNode != NULL )
		{
			if( pNode->GetType() == NODE_PLANETEXT )
			{
				if( pNode->isCannotBeEdited() )
				{
					if( pNode->canBeMoved() )
					{
						::MoveLeftRight( sn, si, -1, -1, 0, 0 );
						sn = si.GetEnd();
					}
					else
					{
						if( ::MoveDelBs_RemoveOperatorFromApply( (CPlaneText*) pNode, pLineNode, sn, 0, autoMulticharOp ) )
						{
							sn.GetNode()->RecalculateSize( 1 );
							return 0;
						}
					}
					return -1;
				}

				if( ((CPlaneText*) pNode)->GetStringLength() == 1 )
				{
					pNode = pNode->GetPrev();

					sn.SetPosition( sn.GetPosition() - 1 );
					pLineNode->DeleteChild( sn.GetPosition() );

					if( GlueText( sn, pNode ) == -1 )
					{
						pLineNode->RecalculateSize( 1 );
					}
				}
				else
				{
					((CPlaneText*) pNode)->DeleteLast();
					pNode->RecalculateSize( 1 );
				}

				return 0;
			}
			else
			{
				if( pUndoRedo != NULL )
					pUndoRedo->CheckPoint( 1 );

				::MoveLeftRight( sn, si, -1, -1, 0, 0 );
				sn = si.GetEnd();

				CFormulaNode *pBracketNode = 0;
				if( pNode->GetType() == NODE_FORMULA )
				{
					CLineNode *lineNode = 0;
					for( CFormulaNode *pFormulaNode = (CFormulaNode*) pNode;
							pFormulaNode && (pFormulaNode->to_mathml_data.is_content || pFormulaNode->isAutodetectContent()); )
					{
						pFormulaNode = pFormulaNode->passAutodetectSingleFormulaChildren();
						if( pFormulaNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
						{
							pBracketNode = pFormulaNode;
							break;
						}
						else
						{
							lineNode = pFormulaNode->getContentFormulaSingleLineChild();
							if( lineNode && lineNode->GetLastChild() && lineNode->GetLastChild()->GetType() == NODE_FORMULA )
								pFormulaNode = (CFormulaNode*) lineNode->GetLastChild();
							else
								break;
						}
					}

					if( pBracketNode )
					{
						if( pUndoRedo != NULL )
							pUndoRedo->CheckPoint( 1 );
						::MoveLeftRight( sn, si, 0, 0, 0, 0 );
						int wasDeleted = ::MoveDelBs_RemoveBrackets( pBracketNode, sn, 0 );
						if( !wasDeleted )
						{
							if( pUndoRedo != NULL && pUndoRedo->CanUndo() )
								pUndoRedo->Undo();
						}
						else
						{
							return 0;
						}
					}
				}

				return -1;
			}
		}
		else
		{
			if( ::GlueLine( sn, pLineNode->GetPrev(), 1 ) == 0 )
			{
				return 0;
			}
			else
			{
				Q_ASSERT( pLineNode->GetParent() != NULL );
				CParentNode *pParentNode = pLineNode->GetParent()->GetParent();
				if( pParentNode && pParentNode->GetType() == NODE_FORMULA 
					&& pParentNode->GetParent() && pParentNode->GetParent()->GetType() == NODE_LINE )
				{
					long frameIdx = pParentNode->GetChildIndex( pLineNode->GetParent() );
					long childPos = pParentNode->GetParent()->GetChildIndex( pParentNode );
					bool the2ndFrame = (((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP ||
										((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_SUP) && (frameIdx > 0);
					bool isDivision = false, isSup = false;
					if( pParentNode->canBeMoved() && pParentNode->GetParent()->canBeEdited() && !the2ndFrame )
					{
						si.GetBegin().SetPosition( childPos );
						si.GetBegin().SetNode( pParentNode->GetParent() );
						si.GetEnd().SetPosition( 
							 si.GetBegin().GetPosition() + 1 );
						si.GetEnd().SetNode( pParentNode->GetParent() );

						sn = si.GetEnd();
					}
					else 
					{
						if( childPos == 0 )
						{
							if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP &&
								frameIdx == 1 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
							{
								isDivision = true;
							}
							else if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_SUP &&
								frameIdx == 1 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
							{
								isSup = true;
							}
							else
							{
								do
								{
									pParentNode = pParentNode->GetParent()->GetParent();
									if( pParentNode && pParentNode->GetParent() )
									{
										frameIdx = pParentNode->GetParent()->GetChildIndex( pParentNode );
										pParentNode = pParentNode->GetParent();
									}
									if( pParentNode && pParentNode->GetType() == NODE_FORMULA 
										&& pParentNode->GetParent() && pParentNode->GetParent()->GetType() == NODE_LINE /*&&
										!pParentNode->canBeMoved() || !pParentNode->GetParent()->canBeEdited()*/ )
									{
										if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP &&
											frameIdx == 1 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
										{
											childPos = 0;
											isDivision = true;
											break;
										}
										else if( ((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_SUP &&
											frameIdx == 1 && pParentNode->GetParent()->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
										{
											childPos = 0;
											isSup = true;
											break;
										}
										else
											childPos = pParentNode->GetParent()->GetChildIndex( pParentNode );
									}
									else
										break;
								}
								while( childPos == 0 );
								if( childPos && pParentNode && pParentNode->GetParent() &&
									pParentNode->canBeMoved() && pParentNode->GetParent()->canBeEdited() )
									childPos = 0;
							}
						}

						if( (isDivision || isSup) && pParentNode && pParentNode->GetParent() )
						{
							sn.SetNode( pParentNode->GetParent() );
							sn.SetPosition( 0 );
							enum TAG2MML op_id = isDivision ? TAG2MML_divide : (isSup ? TAG2MML_power : TAG2MML_nop);
							CFormulaNode* newFormulaNode = ((CLineNode*) pParentNode->GetParent())->convertDivSup2Nop( (CFormulaNode*&) pParentNode, op_id, 0 );
							if( newFormulaNode && newFormulaNode->GetParent() && newFormulaNode->GetParent()->GetType() == NODE_LINE )
							{
								CLineNode *newFormulaLine = newFormulaNode->getContentFormulaSingleLineChild();
								CNode *opNode = newFormulaLine ? newFormulaLine->GetChild( 1 ) : NULL;
								if( opNode && opNode->GetType() == NODE_PLANETEXT )
								{
									if( ::MoveDelBs_RemoveOperatorFromApply( (CPlaneText*) opNode, newFormulaLine, sn, 0, autoMulticharOp ) )
									{
										((CLineNode*) newFormulaNode->GetParent())->RecalculateSize( 1 );
										return 0;
									}
								}
								newFormulaNode = ((CLineNode*) newFormulaNode->GetParent())->convertDivSup2Nop( newFormulaNode, op_id, 1 );
								if( newFormulaNode && newFormulaNode->GetParent() )
								{
									newFormulaNode->RecalculateSizeFull();
									((CLineNode*) newFormulaNode->GetParent())->RecalculateSize( 1 );
								}
							}
						}
						else if( childPos > 0 && pParentNode && pParentNode->GetParent() &&
							((CFormulaNode*) pParentNode)->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
						{
							CLineNode *pParentLine = (CLineNode*) pParentNode->GetParent();
							CNode *opNode = pParentLine->GetChild( childPos - 1 );
							if( opNode )
							{
								if( opNode->GetType() == NODE_PLANETEXT )
								{
									if( ::MoveDelBs_RemoveOperatorFromApply( (CPlaneText*) opNode, pParentLine, sn, 0, autoMulticharOp ) )
									{
										sn.GetNode()->RecalculateSize( 1 );
										return 0;
									}
								}
								else if( opNode->GetType() == NODE_FORMULA && opNode->isCannotBeEdited() && childPos == 1 &&
										((CFormulaNode*) opNode)->getCalcButtonID() == HF_BTNID_SUP )
								{
									if( ::MoveDelBs_RemoveOperatorFromApply( (CFormulaNode*) opNode, pParentLine, sn, 0, autoMulticharOp ) )
									{
										sn.GetNode()->RecalculateSize( 1 );
										return 0;
									}
								}
							}
						}
					}
				}
				return -1;
			}
		}
	}
	else
	{
		Q_ASSERT( 0 );
	}

	sn.GetNode()->RecalculateSize( 0 );
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int CutLeftRight( SelectNode &sn, SelectInfo &si, 
	CFrameNode **ppFrameNodeL, CFrameNode **ppFrameNodeR )
{
	if( si.IsSelection() || ppFrameNodeL == NULL || ppFrameNodeR == NULL )
		return -1;

	*ppFrameNodeL = NULL;
	*ppFrameNodeR = NULL;

	if( sn.GetNode() != NULL )
	{
		if( sn.GetNode()->GetType() == NODE_PLANETEXT )
		{
			Q_ASSERT( sn.GetNode()->canBeEdited() );
			BreakPlaneText( sn, 0, NULL, NULL );
		}

		Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );

		if( sn.GetPosition() && 
			((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() - 1 ) != NULL && 
			((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() - 1 )->canBeMoved() )
		{
			si.GetBegin() = sn; 
			si.GetEnd() = sn; 
			if( MoveLeftRight( sn, si, 1, 1, 1, 0 ) == 0 )
			{
				*ppFrameNodeL = new CFrameNode();
				RemoveToFrame( *ppFrameNodeL, sn, si, 0, NULL );
			}
			Q_ASSERT( (*ppFrameNodeL)->GetChildCount() == 1 );
		}

		if( sn.GetPosition() < ((CLineNode*) sn.GetNode())->GetChildCount() &&
			((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() ) != NULL && 
			((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() )->canBeMoved() )
		{
			si.GetBegin() = sn; 
			si.GetEnd() = sn; 
			if( MoveLeftRight( sn, si, 0, 1, 1, 0 ) == 0 )
			{
				*ppFrameNodeR = new CFrameNode();
				RemoveToFrame( *ppFrameNodeR, sn, si, 0, NULL );
			}
			Q_ASSERT( (*ppFrameNodeR)->GetChildCount() == 1 );
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int PasteLeftRight( SelectNode &sn, SelectInfo &si, 
	CFrameNode *pFrameNodeL, CFrameNode *pFrameNodeR, CFormulaNode *pFormulaNode )
{
	if( pFrameNodeL || pFrameNodeR )
	{
		long nEmptyCount = GetEmptyLineCount( pFormulaNode );

		if( nEmptyCount == 0 )
		{
			Q_ASSERT( 0 );

			MoveLeftRight( sn, si, 1, 0, 1, 0 );
			InsertFromFrame( pFrameNodeL, sn, 1 );
			MoveLeftRight( sn, si, 0, 0, 1, 0 );
			InsertFromFrame( pFrameNodeR, sn, 1 );
		}
		else if( nEmptyCount == 1 )
		{
			CaretToFirstEmptyLine( pFormulaNode, sn );

			if( pFrameNodeL != NULL )
			{
				::InsertFromFrame( pFrameNodeL, sn, 1 );
			}
			if( pFrameNodeR != NULL )
			{
				::InsertFromFrame( pFrameNodeR, sn, 1 );
			}
		}
		else 
		{
			SelectNode sn1;
			CaretToFirstEmptyLine( pFormulaNode, sn1 );
			SelectNode sn2;
			CaretToSecondEmptyLine( pFormulaNode, sn2 );

			SelectNode sn3;
			if( nEmptyCount > 2 )
			{
				CaretToSecondEmptyLine( pFormulaNode, sn3 );
			}

			if( pFrameNodeL != NULL && pFrameNodeR != NULL )
			{
				sn = sn1;
				::InsertFromFrame( pFrameNodeL, sn, 1 );
				sn = sn2;
				::InsertFromFrame( pFrameNodeR, sn, 1 );

				if( nEmptyCount > 2 )
				{
					sn = sn3;
				}
			}
			else if( pFrameNodeL != NULL )
			{
				sn = sn1;
				::InsertFromFrame( pFrameNodeL, sn, 1 );
				sn = sn2;
			}
			else if( pFrameNodeR != NULL )
			{
				sn = sn2;
				::InsertFromFrame( pFrameNodeR, sn2, 1 );
				sn = sn1;
			}
		}
	}
	else
	{
		pFormulaNode->RecalculateSizeFull();
		CaretToFirstEmptyLine( pFormulaNode, sn );
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int OnChar( QChar ch, SelectNode &sn, SelectInfo &si )
{
	if( si.IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		::RemoveToFrame( pFrameNode, sn, si, 1 );
		delete pFrameNode;
	}
	Q_ASSERT( sn.GetNode()->canBeEdited() );

	if( ch.isLetter() || ch.isDigit() )
	{
		CNodeInfo ni;
		CNodeExInfo exni;
		::InsertPlaneText( sn, ch, ::getCurrentUserChoiceStyle(), ::getCurrentKegl(), ni, exni );
	}
	else 
		::InsertAlnumPlaneText( sn, ch, ::getCurrentKegl() );

	return -1;
}

#define __AUTOREPLACE_NONE					-1
#define __AUTOREPLACE_MULTICHAR				1
#define __AUTOREPLACE_UNARY					2
#define __AUTOREPLACE_BINARY				3
#define __AUTOREPLACE_CI					4
#define __AUTOREPLACE_CN					5
#define __AUTOREPLACE_ID2TIMES_LEFT			6
#define __AUTOREPLACE_ID2TIMES_RIGHT		7
#define __AUTOREPLACE_MATHML_CODE			8
#define __AUTOREPLACE_MATHML_CODE_POWBASE	9
#define __AUTOREPLACE_ALLDONE				10

int OnOperator_AutoMultiplication( CNode *pNode, QString keyword1char, QString& btnID, QString& btnIDParam, CPlaneText *&powBaseNode, long& precNew_MathMLCode )
{
	int iRet = __AUTOREPLACE_NONE;
	powBaseNode = NULL;

	if( keyword1char.length() == 0 ) return iRet;

	if( keyword1char.length() > 1 && !::isCMKeywordPrefix( keyword1char ) )
	{
		int isValidAlphabet = 1, isLetter = 0, isIdentical = 1;
		QChar aspect = 0;
		QString body = _T("");
		for(long ind = 0; ind < keyword1char.length(); )
		{
			if( keyword1char.at(ind).isLetter() )
			{
				body += _T("<ci>");
				body += keyword1char.at(ind);
				body += _T("</ci>");
				isLetter++;
				if( aspect == 0 )
					aspect = keyword1char.at(ind);
				else if( isIdentical )
					isIdentical = (aspect == keyword1char.at(ind));
				ind++;
			}
			else
			{
				long pos2 = ind;
				do { pos2++; } while( pos2 < keyword1char.length() && !keyword1char.at(pos2).isLetter() );
				if( pos2 - ind == 1 && !keyword1char.at(ind).isDigit() )
				{
					isValidAlphabet = 0;
					break;
				}
				body += _T("<cn>");
				body += keyword1char.mid(ind, pos2 - ind);
				body += _T("</cn>");
				ind = pos2;
			}
		}
		if( isValidAlphabet && isLetter )
		{
			if( isLetter == keyword1char.length() && isIdentical )
			{
				CFormulaNode *tryFormulaNode = pNode->getParentContentFormulaNode( 1 );
				if( tryFormulaNode && tryFormulaNode->GetParent() )
				{
					if( tryFormulaNode->isAutodetectContent() && 
						tryFormulaNode->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
					{
						tryFormulaNode = tryFormulaNode->GetParent()->getParentContentFormulaNode( 1 );
					}
					else if( ::mml_is_cm_token( tryFormulaNode->to_mathml_data.name ) )
					{
						tryFormulaNode = tryFormulaNode->GetParent()->getParentContentFormulaNode( 1 );
					}
				}

				if( tryFormulaNode && ::mml_is_cm_apply( tryFormulaNode->to_mathml_data.name ) &&
					tryFormulaNode->getCalcButtonID() == HF_BTNID_SUP )
				{
					LSDigit ldigit;
					CPlaneText* child0 = 0, *child1 = 0;
					CFormulaNode *node0 = tryFormulaNode->getSingleFormulaChild_ContentEmptyFrame( 0 );
					CFormulaNode *node1 = tryFormulaNode->getSingleFormulaChild_ContentEmptyFrame( 1 );
					if( node0 && node1 && (node1->isAutodetectContent() || node1->to_mathml_data.name == FBL_TAG_cn) )
					{
						child0 = node0->getSingleTextChild();
						child1 = node1->getSingleTextChild();
					}
					if( child0 && child0 == pNode && child0->GetStringLength() > 0 && child0->GetString()[ 0 ] == aspect &&
						child1 && ldigit.Analisys( child1->GetString().midRef(0) ) == LSDigit::Integer &&
						child1->GetStringLength() == ldigit.GetRead() && ldigit.mnt_int > 0 && ldigit.mnt_int < LONG_MAX )
					{
						child1->SetString(QString("%1").arg(ldigit.mnt_int + 1));
						child0->SetString(QString("%1").arg(aspect));

						tryFormulaNode->RecalculateSizeFull();
						if( tryFormulaNode->GetParent() )
							tryFormulaNode->GetParent()->RecalculateSize( 1 );

						btnID = _T("");
						btnIDParam = _T("");
						iRet = __AUTOREPLACE_ALLDONE;
					}
				}

				if( iRet != __AUTOREPLACE_ALLDONE )
				{
					btnID = HF_BTNID_MATHML_CODE;
					iRet = __AUTOREPLACE_MATHML_CODE;
					precNew_MathMLCode = mmlPrecedence_apply;
					QString frmBody = _T("<math display='block'><apply><power/><ci>%c</ci><cn>%u</cn></apply></math>");
					QString strBody;
					strBody = frmBody.arg(aspect).arg(isLetter);
					btnIDParam = strBody;
				}
			}
			else
			{
				btnID = HF_BTNID_MATHML_CODE;
				iRet = __AUTOREPLACE_MATHML_CODE;
				precNew_MathMLCode = mmlPrecedence_mult;
				btnIDParam = _T("<math display='block'><apply><times/>");
				btnIDParam += body;
				btnIDParam += _T("</apply></math>");
			}
		}
	}
	else
	{
		bool isPrevNode = true, autoreplace = true;
		CNode *tryNode = pNode->GetPrev();
		if( tryNode )
		{
			autoreplace = true;
			if( tryNode->GetType() == NODE_PLANETEXT && ((CPlaneText*) tryNode)->GetStringLength() > 0 )
			{
				QChar ch = ((CPlaneText*) tryNode)->GetString().at(((CPlaneText*) tryNode)->GetStringLength() - 1);
				if( !(ch.isLetter() || ch.isDigit()) )
					autoreplace = false;
			}
			if( autoreplace )
			{
				btnID = _T("@cm_times2");
				iRet = __AUTOREPLACE_ID2TIMES_LEFT;
			}
		}
		if( pNode->GetNext() && (!autoreplace || !tryNode) )
		{
			tryNode = pNode->GetNext();
			isPrevNode = false;
			autoreplace = true;
			if( tryNode->GetType() == NODE_PLANETEXT && ((CPlaneText*) tryNode)->GetStringLength() > 0 )
			{
				QChar ch = ((CPlaneText*) tryNode)->GetString().at(((CPlaneText*) tryNode)->GetStringLength() - 1);
				if( !(ch.isLetter() || ch.isDigit()) )
					autoreplace = false;
			}
			if( autoreplace )
			{
				btnID = _T("@cm_times2");
				iRet = __AUTOREPLACE_ID2TIMES_RIGHT;
			}
		}

		if( tryNode && tryNode->GetType() == NODE_FORMULA && tryNode->to_mathml_data.is_content != 0 )
		{
			CFormulaNode *tryFormulaNode = ((CFormulaNode*) tryNode)->getRecursiveApplyOperand( isPrevNode, HF_BTNID_SUP );
			if( tryFormulaNode->getCalcButtonID() == HF_BTNID_SUP &&
				::mml_is_cm_apply( tryFormulaNode->to_mathml_data.name ) )
			{
				LSDigit ldigit;
				CPlaneText* child0 = 0, *child1 = 0;

				CFormulaNode *node0 = tryFormulaNode->getSingleFormulaChild_ContentEmptyFrame( 0 );
				CFormulaNode *node1 = tryFormulaNode->getSingleFormulaChild_ContentEmptyFrame( 1 );
				if( node0 && node1 && (node1->isAutodetectContent() || node1->to_mathml_data.name == FBL_TAG_cn) )
				{
					child0 = node0->getSingleTextChild();
					child1 = node1->getSingleTextChild();
				}
				if( child0 && child0->GetStringLength() == 1 && child0->GetString() == keyword1char &&
					child1 && ldigit.Analisys( child1->GetString().midRef(0) ) == LSDigit::Integer &&
					child1->GetStringLength() == ldigit.GetRead() && ldigit.mnt_int > 0 && ldigit.mnt_int < LONG_MAX )
				{
					child1->SetString(QString("%1").arg(ldigit.mnt_int + 1));
					tryFormulaNode->RecalculateSizeFull();
					if( tryFormulaNode->GetParent() )
						tryFormulaNode->GetParent()->RecalculateSize( 1 );

					btnID = HF_BTNID_MATHML_CODE;
					btnIDParam = _T("<math display='block'></math>"); // игнорировать автозамену, она уже произошла
					iRet = __AUTOREPLACE_MATHML_CODE;
				}
			}
			else if( tryFormulaNode->getCalcButtonID() == HF_BTNID_EMPTY_FRAME &&
					 (tryFormulaNode->to_mathml_data.name == FBL_TAG_ci || tryFormulaNode->isAutodetectContent()) )
			{
				CPlaneText* child0 = tryFormulaNode->getSingleTextChild();
				if( child0 && child0->GetStringLength() == 1 && child0->GetString() == keyword1char && 
					child0->GetParent() && child0->GetParent()->GetType() == NODE_LINE )
				{
					powBaseNode = child0;
					btnID = HF_BTNID_MATHML_CODE;
					iRet = __AUTOREPLACE_MATHML_CODE_POWBASE;
					precNew_MathMLCode = mmlPrecedence_apply;
					QString frmBody = _T("<math display='block'><apply><power/><ci>%1</ci><cn>2</cn></apply></math>");
					QString strBody;
					strBody = frmBody.arg(keyword1char[ 0 ]);
					btnIDParam = strBody;
				}
			}
		}
	}
	return iRet;
}

CPlaneText* OnOperator_AutoReplaceOperator( CNode *&pNextNode, CPlaneText *currentOperatorEtalon, QString currentValue,
										    HAutodetectMulticharOp* autoMulticharOp, int search2left, CFormulaNode *frmParentNode = 0 )
{
	bool stopNOPSearch = true, stopMulticharSearch = false;
	if( !(pNextNode && pNextNode->isContentTextOperator()) )
	{
		if( frmParentNode )
		{
			stopMulticharSearch = true;
			CFormulaNode *upFormulaNode = 0;
			for( CNode *upParentNode = frmParentNode->GetParent(); 
				 upParentNode && (upFormulaNode = upParentNode->getParentContentFormulaNode( 1 )) != 0;
				 upParentNode = upFormulaNode->GetParent() )
			{
				if( (pNextNode = search2left ? upFormulaNode->GetPrev() : upFormulaNode->GetNext()) != 0 )
				{
					if( pNextNode && pNextNode->isContentTextOperator() )
						stopNOPSearch = false;
					break;
				}
			}
		}
		if( stopNOPSearch )
		{
			pNextNode = 0;
			return 0;
		}
	}
	QString opValue = ((CPlaneText*) pNextNode)->GetString();
	if( opValue == HF_BTNID_NOP_VALUE )
	{
		return currentOperatorEtalon;
	}
	else if( !stopMulticharSearch )
		return autoMulticharOp->getOperator2( search2left ? (opValue + currentValue) : (currentValue + opValue) );
	return 0;
}

void OnOperator_CalculateBrackets( CNode *pNode, short precedence, short assoc )
{
	CNode *curNode = 0;
	CFormulaNode *applyNode = 0/*, *bracketParentNode = 0*/;
	for( curNode = pNode; curNode != 0; curNode = curNode->GetParent() )
	{
		if( curNode->isContentApply() )
		{
			applyNode = (CFormulaNode*) curNode;
			break;
		}
	}
	if( !applyNode )
	{
		return;
	}

	if( precedence != -1 )
	{
		applyNode->to_mathml_data.precedence = precedence;
		applyNode->to_mathml_data.assoc = assoc;
	}

	applyNode->CM_WrapChildrenIntoBracketsOnPrecedence();
	applyNode->CM_WrapIntoBracketsOnPrecedence();
	applyNode->RecalculateSizeFull();
	applyNode->RecalculateSize( 1 );
}

int OnOperator( QChar ch, SelectNode &sn, SelectInfo &si, QString& btnID, QString& btnIDParam, 
	HAutodetectMulticharOp* autoMulticharOp, int isOnCharState, const SelectNode &snlb, CUndoRedo *pUndoRedo )
{
	Q_ASSERT( sn.GetNode() != NULL );

	int iRet = __AUTOREPLACE_NONE;
	long nodePos = 0;
	QString keyword1char, keyword2char;
	CNode *pNode = sn.GetNode(), *pLineNode = 0;
    
	switch( pNode->GetType() )
	{
	case NODE_LINE:
		nodePos = sn.GetPosition();
		if( nodePos )
		{
			pLineNode = pNode;
			pNode = ((CLineNode*) pNode)->GetChild( nodePos - 1 );
		}
		break;

	case NODE_PLANETEXT:
		pLineNode = pNode->GetParent();
		break;

	default:
		break;
	}

	if( !pLineNode || !pNode || pLineNode->GetType() != NODE_LINE || 
		pNode->GetType() != NODE_PLANETEXT || (keyword1char = ((CPlaneText*) pNode)->GetString()).length() == 0 )
		return iRet;

	if( keyword1char == QString(QChar(0x2212)) )
		keyword1char = _T("-");

	CFormulaNode *frmParentNode = pNode->getParentContentFormulaNode( 1 );
	int isContent = pNode->to_mathml_data.is_content || pNode->isIgnoreContentConversion() || pNode->isAutodetectContent();
	if( !isContent && frmParentNode )
		isContent = 1;
	CNode *pNodePrev = pNode->GetPrev(), *pNodeNext = pNode->GetNext();

	long precNew_MathMLCode = mmlPrecedence_Atom;
	CPlaneText *newOp = 0, *powBaseNode = 0;
	CNode *pNodeUpLeft = 0;

	QString newOpBody;
	enum TAG2MML newOpTagID = ::getCMKeyword2TagID( keyword1char, newOpBody );
	bool isDivSub_newOp = false, isArcAsPower_newOp = false;
	switch( newOpTagID )
	{
	case TAG2MML_arcsin:
	case TAG2MML_arccos:
	case TAG2MML_arctan: 
	case TAG2MML_arccosh:
	case TAG2MML_arccot:
	case TAG2MML_arccoth:
	case TAG2MML_arccsc:
	case TAG2MML_arccsch:
	case TAG2MML_arcsec:
	case TAG2MML_arcsech:
	case TAG2MML_arcsinh:
	case TAG2MML_arctanh:
		if( ::isRenderArcFunAsPower() )
			isArcAsPower_newOp = true;
		break;
	case TAG2MML_divide:
	case TAG2MML_power:
		isDivSub_newOp = true;
		break;
	default:
		break;
	}

	CPlaneText *currentOperatorEtalon = autoMulticharOp->getOperator1( keyword1char );
	int isLeft = -1;
	if( currentOperatorEtalon == NULL && isDivSub_newOp )
	{
		currentOperatorEtalon = autoMulticharOp->getOperator1( _T("+") ); // все равно какая операция, она будет проигнорирована
	}
	if( currentOperatorEtalon )
	{
		if( pNodePrev )
		{
			if( pNodePrev->GetType() == NODE_FORMULA )
			{
				pNodeUpLeft = ((CFormulaNode*) pNodePrev)->getFormulaDeepChild( 0, -1 );
				newOp = ::OnOperator_AutoReplaceOperator( pNodeUpLeft, currentOperatorEtalon, keyword1char, autoMulticharOp, 1, 0 );
				isLeft = 1;
			}
			else if( pNodePrev->GetType() == NODE_PLANETEXT )
			{
				pNodeUpLeft = pNodePrev;
				newOp = ::OnOperator_AutoReplaceOperator( pNodeUpLeft, currentOperatorEtalon, keyword1char, autoMulticharOp, 1, 0 );
				isLeft = 1;
			}
		}
		if( !newOp && pNodeNext )
		{
			if( pNodeNext->GetType() == NODE_FORMULA )
			{
				pNodeUpLeft = ((CFormulaNode*) pNodeNext)->getFormulaDeepChild( 0, 0 );
				newOp = ::OnOperator_AutoReplaceOperator( pNodeUpLeft, currentOperatorEtalon, keyword1char, autoMulticharOp, 0, 0 );
				isLeft = 0;
			}
			else if( pNodeNext->GetType() == NODE_PLANETEXT )
			{
				pNodeUpLeft = pNodeNext;
				newOp = ::OnOperator_AutoReplaceOperator( pNodeUpLeft, currentOperatorEtalon, keyword1char, autoMulticharOp, 0, 0 );
				isLeft = 0;
			}
		}
		if( !newOp && frmParentNode )
		{
			if( nodePos == 1 )
			{
				pNodeUpLeft = frmParentNode->GetPrev();
				newOp = ::OnOperator_AutoReplaceOperator( pNodeUpLeft, currentOperatorEtalon, keyword1char, autoMulticharOp, 1, frmParentNode /* look for NOP left&upward */ );
				isLeft = 1;
			}
			if( !newOp && nodePos == ((CLineNode*) pLineNode)->GetChildCount() )
			{
				pNodeUpLeft = frmParentNode->GetNext();
				newOp = ::OnOperator_AutoReplaceOperator( pNodeUpLeft, currentOperatorEtalon, keyword1char, autoMulticharOp, 0, frmParentNode /* look for NOP right&upward */ );
				isLeft = 0;
			}
		}
	}

	CFormulaNode *toreplaceFormula = 0;
	if( newOp && pNodeUpLeft )
	{
		if( isDivSub_newOp || isArcAsPower_newOp )
		{
			toreplaceFormula = pNodeUpLeft->getParentFormulaNode( 1, NULL );
			if( toreplaceFormula && toreplaceFormula->GetParent() && toreplaceFormula->GetParent()->GetType() == NODE_LINE &&
				toreplaceFormula->to_mathml_data.is_content )
			{
				if( toreplaceFormula->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR && toreplaceFormula->GetParent()->GetChildCount() == 1 )
				{
					toreplaceFormula = toreplaceFormula->GetParent()->getParentFormulaNode( 1, NULL );
					if( !(toreplaceFormula && toreplaceFormula->GetParent() && toreplaceFormula->GetParent()->GetType() == NODE_LINE &&
						  toreplaceFormula->to_mathml_data.is_content && ::mml_is_cm_apply( toreplaceFormula->to_mathml_data.name )) )
						toreplaceFormula = 0;
				}
				else if( !::mml_is_cm_apply( toreplaceFormula->to_mathml_data.name ) )
					toreplaceFormula = 0;
			}
			else
				toreplaceFormula = 0;

			if( toreplaceFormula == 0 )
				newOp = 0;
		}
	}

	btnID = _T("");
	if( newOp && pNodeUpLeft )
	{
		if( pUndoRedo != NULL )
			pUndoRedo->CheckPoint( 1 );

		if( isDivSub_newOp && toreplaceFormula )
		{
			pNode = sn.GetNode();
			if( pNode->GetType() != NODE_LINE )
				::MoveLeftRight( sn, si, 0, 0, 1, 0 );
			::MoveLeftRight( sn, si, 1, 1, 1, 0 );
			::MoveDel( sn, si, autoMulticharOp, pUndoRedo );
			CLineNode *toreplaceParentLine = (CLineNode*) toreplaceFormula->GetParent();
			Q_ASSERT( toreplaceParentLine );
			sn.SetNode( toreplaceParentLine );
			sn.SetPosition( 0 );

			pNodeUpLeft = 0;
			pLineNode = 0;
			pNode = 0;

			CFormulaNode* newFormulaNode = toreplaceParentLine->convertDivSup2Nop( toreplaceFormula, newOpTagID, 1 );
			if( newFormulaNode && newFormulaNode->GetParent() && newFormulaNode->GetParent()->GetType() == NODE_LINE )
			{
				CLineNode *newFocusLine = 0;
				if( isLeft == 1 )
					newFocusLine = newFormulaNode->getFormulaLineChild( 1, 0 );
				else if( isLeft == 0 )
					newFocusLine = newFormulaNode->getFormulaLineChild( 0, 0 );
				if( newFocusLine && newFocusLine->canBeEdited() )
				{
					sn.SetNode( newFocusLine );
					sn.SetPosition( isLeft == 1 ? 0 : newFocusLine->GetChildCount() );
				}

				newFormulaNode->CM_WrapChildrenIntoBracketsOnPrecedence();
				newFormulaNode->CM_WrapIntoBracketsOnPrecedence();

				newFormulaNode->RecalculateSizeFull();
				((CLineNode*) newFormulaNode->GetParent())->RecalculateSize( 1 );
			}
			else
			{
				Q_ASSERT( 0 );
			}
			toreplaceFormula = 0;
			return __AUTOREPLACE_ALLDONE;
		}
		else if( isArcAsPower_newOp && toreplaceFormula &&
				 pNodeUpLeft->GetParent() && pNodeUpLeft->GetParent()->GetType() == NODE_LINE &&
				 pNodeUpLeft->GetParent()->GetFirstChild() == pNodeUpLeft && pNodeUpLeft->GetParent() != pLineNode )
		{
			iRet = __AUTOREPLACE_MULTICHAR;

			CLineNode *toreplaceParentLine = (CLineNode*) pNodeUpLeft->GetParent();

			MMLContent_TT_MSup _msup_xml_tree;
			QString szBody = _T(""), szBase = _T("");
			::getSupContentByOperatorTagID( newOpTagID, szBody, szBase );
			_msup_xml_tree.setParent( szBody );
			_msup_xml_tree.setBase( szBase );

			CFormulaNode *newFormulaNode = toreplaceParentLine->InsertMMLFormulaNode( _msup_xml_tree.getRoot() );
			_msup_xml_tree.reset();

			long no = toreplaceParentLine->GetChildCount() - 1;
			if( newFormulaNode && toreplaceParentLine->GetChild( no ) == newFormulaNode )
			{
				newFormulaNode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
				newFormulaNode->clearReadOnly();
				newFormulaNode->clearIgnoreContentConversion();
				newFormulaNode->setCannotBeEdited();
				newFormulaNode->to_mathml_data.reserved = newFormulaNode->to_mathml_data.name;
				newFormulaNode->to_mathml_data.name = keyword1char;
				newFormulaNode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__SELF_CLOSING;

				bool shouldChangeSn = (sn.GetNode() == pNodeUpLeft || sn.GetNode() == pNodeUpLeft->GetParent()) ? true : false;
				CNode *checkNode = toreplaceParentLine->RemoveChild( no );
				Q_ASSERT( checkNode == newFormulaNode );
				if( checkNode == newFormulaNode )
				{
				}
				toreplaceParentLine->ExchangeChild( pNodeUpLeft, newFormulaNode );
				Q_ASSERT( newFormulaNode->GetParent() == toreplaceParentLine );

				delete pNodeUpLeft;
				pNodeUpLeft = 0;

				if( shouldChangeSn )
				{
					Q_ASSERT( 0 );
					sn.SetNode( toreplaceParentLine );
					sn.SetPosition( toreplaceParentLine->GetChildIndex( newFormulaNode ) + (isLeft ? 1 : 0) );
				}
			}
			else
			{
				Q_ASSERT( 0 );
			}
		}
		else
		{
			iRet = __AUTOREPLACE_MULTICHAR;
			toreplaceFormula = 0;
			((CPlaneText*) pNodeUpLeft)->SetString( newOp->GetString() );
			((CPlaneText*) pNodeUpLeft)->setNodeInfo( newOp );
			((CPlaneText*) pNodeUpLeft)->setNodeExInfo( newOp );
			((CPlaneText*) pNodeUpLeft)->setFStyle( newOp->getFStyle(), ((CPlaneText*) pNodeUpLeft)->getOtherStyleFont() );
			((CPlaneText*) pNodeUpLeft)->to_mathml_data.precedence = mmlPrecedence_Atom;	// так надо, в newOp приоритет запорчен при создании (HAutodetectMulticharOp::init)
			pNodeUpLeft->RecalculateSize( 0 );
		}
	}
	else
	{
		QString AutoOpUnary, AutoOpBinary;
		::getCMKeyword2ButtonID( _T(" ") + keyword1char, AutoOpUnary );
		::getCMKeyword2ButtonID( keyword1char, AutoOpBinary );

		if( !(pNodePrev && !pNodePrev->isCannotBeMoved()) && AutoOpUnary.length() )
		{
			btnID = AutoOpUnary;
			iRet = __AUTOREPLACE_UNARY;
		}
		else if( AutoOpBinary.length() )
		{
			btnID = AutoOpBinary;
			iRet = __AUTOREPLACE_BINARY;
		}
		else if( ch.isLetter() || ch.isDigit() )
		{
			if( ::isAutographEditing_OneCharID() )
			{
				iRet = ::OnOperator_AutoMultiplication( pNode, keyword1char, btnID, btnIDParam, powBaseNode, precNew_MathMLCode );
				Q_ASSERT( !(iRet == __AUTOREPLACE_MATHML_CODE_POWBASE && powBaseNode == NULL) );
				if( sn.GetNode()->GetType() == NODE_PLANETEXT && sn.GetPosition() >= ((CPlaneText*) sn.GetNode())->GetStringLength() )
				{
					Q_ASSERT( pLineNode && pLineNode->GetType() == NODE_LINE && pLineNode == pNode->GetParent() );
					sn.SetNode( (CLineNode *) pLineNode );
					sn.SetPosition( ((CLineNode *) pLineNode)->GetChildIndex( pNode ) + 1 );
				}
			}

			if( iRet == __AUTOREPLACE_NONE && !isContent )
			{
				if( ch.isLetter() )
				{
					btnID = HF_BTNID_CI;
					iRet = __AUTOREPLACE_CI;
				}
				else if( ch.isDigit() )
				{
					btnID = HF_BTNID_CN;
					iRet = __AUTOREPLACE_CN;
				}
			}
		}
	}

	if( btnID.length() && pUndoRedo != NULL )
		pUndoRedo->CheckPoint( 1 );

	if( iRet == __AUTOREPLACE_NONE || iRet == __AUTOREPLACE_ALLDONE )
		return iRet;
	else if( iRet == __AUTOREPLACE_ID2TIMES_LEFT )
	{
		::MoveLeftRight( sn, si, 1, 0, 0, 1 );
		return iRet;
	}
	else if( iRet == __AUTOREPLACE_ID2TIMES_RIGHT )
	{
		return iRet;
	}

	pNode = sn.GetNode();
	if( pNode->GetType() != NODE_LINE )
		::MoveLeftRight( sn, si, 0, 0, 1, 0 );
	::MoveLeftRight( sn, si, 1, 1, 1, 0 );
	switch( iRet )
	{
	case __AUTOREPLACE_UNARY:
	case __AUTOREPLACE_MULTICHAR:
		::MoveDel( sn, si, autoMulticharOp, pUndoRedo );
		if( newOp )
		{
			CNode *applyChild = pNodeUpLeft ? pNodeUpLeft : ((isArcAsPower_newOp && toreplaceFormula) ? toreplaceFormula : 0);
			if( applyChild )
				::OnOperator_CalculateBrackets( applyChild, newOp->to_mathml_data.precedence, newOp->to_mathml_data.assoc );
		}
		break;
	case __AUTOREPLACE_MATHML_CODE_POWBASE:
	case __AUTOREPLACE_MATHML_CODE:
		::MoveDel( sn, si, autoMulticharOp, pUndoRedo );
		if( sn.GetNode() )
		{
			CFormulaNode *tokenNode = sn.GetNode()->getParentContentFormulaNode( 0 /*do not use 'ignored' nodes also*/ );
			if( tokenNode && !tokenNode->isAutodetectContent() && ::mml_is_cm_token( tokenNode->to_mathml_data.name ) )
			{
				tokenNode->setAutodetectContent();
				tokenNode->to_mathml_data.name = _T("");
			}
		}
		if( iRet == __AUTOREPLACE_MATHML_CODE_POWBASE )
		{
			Q_ASSERT( powBaseNode && powBaseNode->GetParent() && powBaseNode->GetParent()->GetType() == NODE_LINE );
			CLineNode *lineNode = (CLineNode*) powBaseNode->GetParent();
			sn.SetNode( lineNode );
			sn.SetPosition( lineNode->GetChildIndex( powBaseNode ) + 1 );
			::MoveLeftRight( sn, si, 1, 1, 1, 0 );
			::MoveDel( sn, si, autoMulticharOp, pUndoRedo );
		}
		else if( precNew_MathMLCode == mmlPrecedence_mult && ::isAutographEditing_MoveOutOfHighPriorityOp() )
		{
		}
		break;
	case __AUTOREPLACE_BINARY:
		::MoveDel( sn, si, autoMulticharOp, pUndoRedo );
		if( sn.GetNode() )
		{
			CFormulaNode *tokenNode = sn.GetNode()->getParentContentFormulaNode( 0 /*do not use 'ignored' nodes also*/ );
			if( tokenNode && !tokenNode->isAutodetectContent() && ::mml_is_cm_token( tokenNode->to_mathml_data.name ) )
			{
				tokenNode->setAutodetectContent();
				tokenNode->to_mathml_data.name = _T("");
			}
		}
		if( ::isAutographEditing_MoveOutOfHighPriorityOp() )
		{
			CFormulaNode *opFormula = 0;
			if( isOnCharState )
			{
				for( CNode *curNode = pNode; curNode != 0; curNode = curNode->GetParent() )
				{
					if( curNode->GetType() == NODE_FORMULA && curNode->to_mathml_data.is_content != 0 &&
							::mml_is_cm_apply( curNode->to_mathml_data.name ) )
					{
						opFormula = (CFormulaNode*) curNode;
						break;
					}
				}
				if( opFormula )
				{
					CLineNode *lbLineNode = 0;
					if( !snlb.IsEmpty() && snlb.GetNode() )
					{
						switch( snlb.GetNode()->GetType() )
						{
						case NODE_LINE:
							lbLineNode = (CLineNode*) snlb.GetNode();
							break;

						case NODE_PLANETEXT:
							if( snlb.GetNode()->GetParent() && snlb.GetNode()->GetParent()->GetType() == NODE_LINE )
							lbLineNode = (CLineNode*) snlb.GetNode()->GetParent();
							break;
						}
					}
					long precNew = (newOpTagID == TAG2MML_divide) ? 
                                                                (long) mmlPrecedence_mult :
                                                                (long) ::mml_getPrecedence( newOpTagID, 2, newOpBody );
					long precOld = (opFormula->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP) ? 
                                                                (long) mmlPrecedence_mult :
                                                                (long) ::_CM_getPrecedence( opFormula );
					if( precNew >= precOld && precNew > mmlPrecedence_apply && ((CLineNode*) pLineNode) != lbLineNode )
					{
						if( precOld == mmlPrecedence_apply && opFormula->getCalcButtonID() != HF_BTNID_SUP )
						{
							bool shouldMoveOut = false;
							CLineNode *childContainer = opFormula->getContentFormulaSingleLineChild();
							CNode *firstChild = 0;
							QString tagName;
							enum TAG2MML tagID;

							if( childContainer && childContainer->GetChildCount() == 2 && 
								(firstChild = childContainer->GetFirstChild()) != 0 && firstChild->isCannotBeEdited() )
							{
								if( firstChild->GetType() == NODE_PLANETEXT )
								{
									tagName = ((CPlaneText*) firstChild )->GetString();
									tagID = ::mml_getTag2MML( tagName );
									if( ::mml_isContentTag( tagID ) && !::mml_isNAryOperator( tagID ) )
										shouldMoveOut = true;
								}
								else if( firstChild->GetType() == NODE_FORMULA &&
										((CFormulaNode*) firstChild)->getCalcButtonID() == HF_BTNID_SUP )
								{
									CLineNode *baseLine = ((CFormulaNode*) firstChild)->getFormulaLineChild( 0, 0 );
									if( baseLine && baseLine->GetChildCount() == 1 && baseLine->GetFirstChild() &&
										baseLine->GetFirstChild()->GetType() == NODE_PLANETEXT )
									{
										tagName = ((CPlaneText*) baseLine->GetFirstChild())->GetString();
										tagID = ::mml_getTag2MML( tagName );
										if( ::mml_isContentTag( tagID ) && !::mml_isNAryOperator( tagID ) )
											shouldMoveOut = true;
									}
								}
							}
							if( shouldMoveOut )
								::MoveLeftRight( sn, si, 0, 0, 0, 1 );
						}
						else
							::MoveLeftRight( sn, si, 0, 0, 0, 1 );
					}
				}
			}
		}
		break;
	default:
		break;
	}

	return iRet;
}

/////////////////////////////////////////////////////////////////////////////
