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

#include <QCursor>
#include <QPrinter>
#include <QApplication>
#include <QMessageBox>

#include "HUtils/ihkstring.h"

#include "rules/appstyle.h"
#include "rules/copy.h"
#include "rules/edit.h"
#include "rules/insert.h"
#include "rules/mouse.h"
#include "rules/nav.h"
#include "rules/sel.h"

#include "ni_root.h"
#include "dump/dumptags.h"
#include "../version/version.h"
#include "../dictionary/op_iddefs.h"

#include "../gui/widget/fmlbasewidget.h"
#include "../gui/dlg/nodeinputdlg.h"

/////////////////////////////////////////////////////////////////////////////

CRootNode::CRootNode( QFormulatorBaseWidget *owner )
	: CFrameNode()
	, CLRBracketSelection( *this )
	, m_shFlag( FORMULIB_ROOTNODE_ONCHAR_FLAG )
	, m_sn()
	, m_si()
	, m_sndrag()
	, m_rcdrag( 0.0, 0.0, 0.0, 0.0 )
	, m_AutodetectMulticharOp( NULL )
{
	m_owner = owner;

	m_sn.SetNode( GetFirstChild() );
	m_sn.SetPosition( 0 );

	setLeftIndent( DEFAULT_INDENTS );
	setTopIndent( DEFAULT_INDENTS );
	setRightIndent( DEFAULT_INDENTS );
	setBottomIndent( DEFAULT_INDENTS );
}

CRootNode::~CRootNode()
{
	deleteAutodetectMulticharOp();
}

/////////////////////////////////////////////////////////////////////////////


void CRootNode::Store( QDataStream& ar ) const
{
	ar << m_lineSpacing;

	// store brackets
	int shouldStore = -1;
	const std::vector<SelectNode>& tostore = getSNVector( shouldStore );
	std::vector<SelectNode> tostore_empty;
	qint32 value2ar = shouldStore;
	ar << value2ar;

	SelectNode tmpSn = m_sn;
	CFrameNode::Store( ar, tmpSn, (shouldStore == 0 || shouldStore == 1) ? tostore : tostore_empty );
}

void CRootNode::Load( QDataStream& ar )
{
	QApplication::setOverrideCursor( Qt::WaitCursor );

	GetSelectInfo().SetEmpty();

	ar >> m_lineSpacing;

	// load brackets
	qint32 value2ar;
	ar >> value2ar;
	int isLeftBrLoad = value2ar;
	std::vector<SelectNode> toload;
	if( isLeftBrLoad == 0 || isLeftBrLoad == 1 )
		ClearBrackets_All();
	CFrameNode::Load( ar, GetCaret(), toload, isLeftBrLoad );
	if( isLeftBrLoad == 0 || isLeftBrLoad == 1 )
		RefreshBracketsSN( isLeftBrLoad, toload );

	QApplication::restoreOverrideCursor();
}

/////////////////////////////////////////////////////////////////////

int CRootNode::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc )
{
	CFrameNode::Draw( fde, tl, rc );
	if( !fde.IsPrinting() )
	{
		// draw focus rect
		CNode *pNode = GetCaret().GetNode();
		if( pNode != NULL )
		{
			// draw focus rect
			if( pNode->GetType() == NODE_PLANETEXT ) pNode = pNode->GetParent();
			//RectFde rcItem = rc.intersected( RectFde( pNode->GetAbsolutePosition(), pNode->GetSize() ) );
			RectFde rcItem = RectFde( pNode->GetAbsolutePosition(), pNode->GetSize() );
			fde.DrawRectangle( rcItem.left(), rcItem.top(), rcItem.width(), rcItem.height(), FS_LogPen(QColor(0x60, 0x60, 0x60, 0xc0)) );
		}
		// draw drag rect
		if( !GetDrag().IsEmpty() )
		{
			fde.FillRectangle( m_rcdrag.left(), m_rcdrag.top(), m_rcdrag.width(), m_rcdrag.height(), FS_LogBrush(QColor(0, 0, 0, 0x7F), Qt::SolidPattern) );
		}
	}
#ifdef _DEBUG
		fde.DrawRectangle( tl.x(), tl.y(), GetSize().width(), GetSize().height(), FS_LogPen( QColor(0, 0, 0) ) );
#endif
	return 1;
}

/////////////////////////////////////////////////////////////////////

void CRootNode::RecalculateSize( int bRecalc )
{
	CFrameNode::RecalculateSize( bRecalc );
}

void CRootNode::RecalculateSizeFull()
{
	CFrameNode::RecalculateSizeFull();
}

/////////////////////////////////////////////////////////////////////////////

CFormulaNode* CRootNode::GetContentFormulaUnderCaret()
{
	CNode *pElementNode = 0, *pElementNode2 = 0;
	CFormulaNode* formulaNode = 0;
	CNode *pParentNode = GetCaret().GetNode();
	if( pParentNode )
	{
		if( pParentNode->GetType() == NODE_PLANETEXT )
		{
			pElementNode = pParentNode;
			pParentNode = pParentNode->GetParent();
		}
		else if( pParentNode->GetType() == NODE_LINE )
		{
			long pos = GetCaret().GetPosition();
			pElementNode = ((CLineNode*) pParentNode)->GetChild( pos );
			if( pos > 0 )
				pElementNode2 = ((CLineNode*) pParentNode)->GetChild( pos - 1 );
		}
	}
	if( pParentNode && pParentNode->GetType() == NODE_LINE )
	{
		formulaNode = pParentNode->getParentContentFormulaNode( 0 );
		if( !formulaNode )
		{
			if( pElementNode )
				formulaNode = pElementNode->getParentContentFormulaNode( 0 );
			else if( pElementNode2 )
				formulaNode = pElementNode2->getParentContentFormulaNode( 0 );
		}
	}
	if( formulaNode == 0 || formulaNode->GetParent() == 0 )
	{
		//QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Evaluation is unavailable.\n\nFailed to find Content MathML node under the caret.")));
		return NULL;
	}

	for( CFormulaNode *frmNode = formulaNode->GetParent()->getParentContentFormulaNode( 0 );
			frmNode && frmNode->GetParent();
			frmNode = formulaNode->GetParent()->getParentFormulaNode( 0 ) )
	{
		formulaNode = frmNode;
	}
	return formulaNode;
}

bool CRootNode::PrepareContentEqual2Evaluate( CFormulaNode *&formulaNode, CUndoRedo * /*pUndoRedo*/ )
{
	bool recalc = false;
	CFormulaNode *innerNode = formulaNode->passAutodetectSingleFormulaChild();
	if( innerNode->isContentApply() && innerNode->GetChildCount() == 1 )
	{
		CLineNode *line = innerNode->getContentFormulaSingleLineChild();
		if( line && line->GetChildCount() == 3 )
		{
			CNode *op1 = line->GetFirstChild(), *op2 = line->GetLastChild(), *sign = line->GetChild( 1 );
			if( op1 && op2 && sign && sign->isContentTextOperator() && ((CPlaneText*) sign)->getCalcButtonID() == HF_BTNID_EQUAL )
			{
				op1 = op1->getParentContentFormulaNode( 0 );
				if( op1 && op1 != formulaNode )
				{
					if( op2->GetType() == NODE_FORMULA )
						op2 = ((CFormulaNode*) op2)->passAutodetectSingleFormulaChild();
					if( MoveCaret( op2, 1 ) )
					{
						if( GetCaret().GetNode() && GetCaret().GetNode()->GetType() == NODE_LINE )
						{
							CLineNode *editedLine = (CLineNode*) GetCaret().GetNode();
							if( editedLine->GetChildCount() > 0 )
							{
								if( editedLine != line && editedLine->GetChildIndex(op1) == -1 && editedLine->GetChildIndex(sign) == -1 )
								{
									editedLine->DeleteAllChild();
									GetCaret().SetPosition( 0 );
								}
							}
							op1 = line->GetFirstChild();
							if( op1 ) op1 = op1->getParentContentFormulaNode( 0 );
							if( op1 ) formulaNode = (CFormulaNode*) op1;

							recalc = true;
						}
					}
				}
			}
		}
	}
	return recalc;
}

/////////////////////////////////////////////////////////////////////////////
//

int CRootNode::GetCaretInfo( PointFde &pt, SizeFde &sz )
{
	if( !GetDrag().IsEmpty() )
		return -1;

	::GetCaretInfo( GetCaret(), pt, sz );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//

int CRootNode::GetCaretMessage( QString& rMessage )
{
	if( !::isOption_ShowNavInfo() )
	{
		rMessage = "";
		return 0;
	}

	if( GetDrag().IsEmpty() )
	{
		rMessage = "Navigation: ";

		CNode *pElementNode = NULL;
		CNode *pNode = GetCaret().GetNode();
		Q_ASSERT( pNode != NULL );
		if( pNode->GetType() == NODE_PLANETEXT )
		{
			pElementNode = pNode;
			pNode = pNode->GetParent();
		}
		else if( pNode->GetType() == NODE_LINE )
		{
			long pos = GetCaret().GetPosition();
			pElementNode = ((CLineNode*) pNode)->GetChild( pos );
		}

		Q_ASSERT( pNode != NULL );
		Q_ASSERT( pNode->GetType() == NODE_LINE );

		rMessage += ((CLineNode*) pNode)->getMathMLNodeDescription();
		if( pElementNode )
		{
			QString resStr = pElementNode->getMathMLNodeDescription();
			if( resStr.length() )
				rMessage += QString("; current node - ") + resStr;
		}
		return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::MoveLeftRight( int fLeft, int fSel, int fWord )
{
	ClearOnCharFlag();

	int nRet = ::MoveLeftRight( GetCaret(), GetSelectInfo(), fLeft, fSel, fWord, 1 );
	Q_ASSERT( isValidPosition( GetCaret() ) );
	return nRet;
}

int CRootNode::MoveHomeEnd( int fHome, int fSel, int fCtrl )
{
	ClearOnCharFlag();

	int nRet = ::MoveHomeEnd( this, GetCaret(), GetSelectInfo(), fHome, fSel, fCtrl );
	Q_ASSERT( isValidPosition( GetCaret() ) );
	return nRet;
}

int CRootNode::MoveUpDown( RectFde& rc, PointFde& pt, SizeFde& sz, int fUp, int fSel )
{
	ClearOnCharFlag();

	if( fUp == 0 )
		pt.ry() += sz.height() - 1.0;

	int nRet = ::MoveUpDown( rc, pt, GetCaret(), GetSelectInfo(), fUp, fSel );
	Q_ASSERT( isValidPosition( GetCaret() ) );

	return nRet;
}

int CRootNode::MovePgUpDown( RectFde& rc, SizeFde& ofs, int fUp, int fSel )
{
	ClearOnCharFlag();

	SizeFde sz;
	PointFde pt;
	GetCaretInfo( pt, sz );

	if( fUp )
	{
		pt.ry() -= rc.height() - getTopIndent();
		if( pt.y() < getTopIndent() ) 
			pt.ry() = getTopIndent();
	}
	else
	{
		pt.ry() += rc.height() - getBottomIndent();
		if( pt.y() > GetSize().height() - getBottomIndent() ) 
			pt.ry() = GetSize().height() - getBottomIndent();
	}

	rc.translate( 0.0, (fUp ? -ofs.height() : ofs.height()) );

	int fRet = ::MoveCaret( fSel, pt, rc, GetCaret(), GetSelectInfo(), this );
	Q_ASSERT( isValidPosition( GetCaret() ) );

	return fRet;
}

/////////////////////////////////////////////////////////////////////////////
//

int CRootNode::GetSelectInfo( PointFde &sb, PointFde &se )
{
	SizeFde sz;
	::GetCaretInfo( GetSelectInfo().GetBegin(), sb, sz );
	::GetCaretInfo( GetSelectInfo().GetEnd(), se, sz );
	return 0;
}

int CRootNode::DragCaret( RectFde& rc, PointFde pt )
{
	SelectNode sndrag = GetDrag();
	int fRet = ::MoveCaret( pt, rc, GetDrag(), this );
	Q_ASSERT( isValidPosition( GetDrag() ) );

	if( sndrag == GetDrag() )
		return -1;

	PointFde ptf;
	SizeFde szf;
	::GetCaretInfo( GetDrag(), ptf, szf );
	::assign( m_rcdrag, ptf );
	m_rcdrag += szf;

	return fRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::MoveCaret( RectFde& rc, PointFde pt, int fSel )
{
	int fRet = ::MoveCaret( fSel, pt, rc, GetCaret(), GetSelectInfo(), this );
	Q_ASSERT( isValidPosition( GetCaret() ) );
	return fRet;
}

int CRootNode::MoveCaret( CNode *pNode, int isRight )
{
	return ::MoveCaret( pNode, isRight, GetCaret() );
}

int CRootNode::DblClk( RectFde& rc, PointFde pt )
{
	int fRet = ::DblClk( pt, rc, GetCaret(), GetSelectInfo(), this );
	Q_ASSERT( isValidPosition( GetCaret() ) );
	return fRet;
}

/////////////////////////////////////////////////////////////////////////////

CFrameNode* CRootNode::RemoveOrCopySelection( int fRemove )
{
	if( IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();

		int nRet = -1;
		if( fRemove )
		{
			if( GetDrag().IsEmpty() )
			{
				nRet = ::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 1 );
				Q_ASSERT( isValidPosition( GetCaret() ) );
			}
			else
			{
				nRet = ::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 1, &GetDrag() );
				Q_ASSERT( isValidPosition( GetCaret() ) );
				Q_ASSERT( isValidPosition( GetDrag() ) );
			}
		}
		else
		{
			nRet = ::CopyToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 1 );
			Q_ASSERT( isValidPosition( GetCaret() ) );
		}

		return pFrameNode;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::MoveDel( CUndoRedo *pUndoRedo )
{
	ClearOnCharFlag();

	if( !getAutodetectMulticharOp() )
		createAutodetectMulticharOp();

	int nRet = ::MoveDel( GetCaret(), GetSelectInfo(), getAutodetectMulticharOp(), pUndoRedo );
	Q_ASSERT( isValidPosition( GetCaret() ) );

	CLineNode *pLine = NULL;
	if( GetCaret().GetNode()->GetType() == NODE_PLANETEXT )
		pLine = (CLineNode*)(GetCaret().GetNode()->GetParent());
	else if( GetCaret().GetNode()->GetType() == NODE_LINE )
		pLine = (CLineNode*)(GetCaret().GetNode());
	CNode *pNode = NULL;
	if( pLine != NULL && pLine->GetParent() &&
		(pNode = pLine->GetParent()->GetParent()) != NULL && pNode->GetParent() != NULL &&
		pNode->GetType() == NODE_FORMULA && pNode->to_mathml_data.is_content != 0 &&
		((CFormulaNode*)pNode)->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
	{
		Q_ASSERT( pNode->GetParent()->GetType() == NODE_LINE );
	}

	return nRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::MoveBs( CUndoRedo *pUndoRedo )
{
	ClearOnCharFlag();

	if( !getAutodetectMulticharOp() )
		createAutodetectMulticharOp();

	if( GetDrag().GetNode() != NULL )
		Q_ASSERT( isValidPosition( GetDrag() ) );
 
	int nRet = ::MoveBs( GetCaret(), GetSelectInfo(), getAutodetectMulticharOp(), pUndoRedo );
	Q_ASSERT( isValidPosition( GetCaret() ) );

	if( GetDrag().GetNode() != NULL )
		Q_ASSERT( isValidPosition( GetDrag() ) );

	return nRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::MoveReturn()
{
	ClearOnCharFlag();

	if( GetDrag().GetNode() != NULL )
		Q_ASSERT( isValidPosition( GetDrag() ) );

	if( IsSelection() )
	{
		CFrameNode *pFrameNode = RemoveOrCopySelection( -1 );
		if( pFrameNode )
			delete pFrameNode;
	}

#ifdef _DEBUG
	if( GetDrag().GetNode() != NULL )
		Q_ASSERT( isValidPosition( GetDrag() ) );
#endif

	::BreakLine( GetCaret(), 1 );
	Q_ASSERT( isValidPosition( GetCaret() ) );

#ifdef _DEBUG
	if( GetDrag().GetNode() != NULL )
		Q_ASSERT( isValidPosition( GetDrag() ) );
#endif

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::Char( unsigned int nChar, CUndoRedo *pUndoRedo )
{
	Q_ASSERT( isValidPosition( GetCaret() ) );

	if( !getAutodetectMulticharOp() )
		createAutodetectMulticharOp();

	if( ::isInputMode_Content() )
	{
		if( SetBracketsByChar( nChar ) == 0 )
			return 0;
	}
		
	int nRet = ::OnChar( nChar, GetCaret(), GetSelectInfo() );
	if( ::isInputMode_Content() && getAutodetectMulticharOp() )
	{
		QString btnID = _T(""), btnIDParam = _T("");
		if( (nRet = ::OnOperator( nChar, GetCaret(), GetSelectInfo(), btnID, btnIDParam, 
				getAutodetectMulticharOp(), IsOnCharFlag(), GetLBMark(), pUndoRedo )) != -1 )
		{
			if( btnID.length() )
			{
				long tbr, btn;
				if( ::getCurrentToolBarSet()->getButtonCoordByID( btnID, tbr, btn ) == -1 )
				{
					SetOnCharFlag();
					return -1;
				}

				FXmlBtn *ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( tbr, btn );
				if( ptrFXmlBtn )
				{
					if( ptrFXmlBtn->getNodeType() == NODE_MATHML && btnIDParam.length() > 0 )
					{
						nRet = InsertMathML( ptrFXmlBtn, btnIDParam );
					}
					else
						nRet = Insert( ptrFXmlBtn );
				}
			}
		}
	}

	Q_ASSERT( isValidPosition( GetCaret() ) );
	SetOnCharFlag();
	return nRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::Escape()
{
	if( ::isInputMode_Content() )
	{
		ClearBrackets_All();
		ClearOnCharFlag();
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::ApplyStyle( long style )
{
	return ::ApplyUserOtherStyle( style, GetCaret(), GetSelectInfo() );
}

int CRootNode::ApplyKegl( RealFde kegl )
{
	return ::ApplyUserOtherKegl( kegl, GetCaret(), GetSelectInfo() );
}

int CRootNode::IncDecKegl( RealFde value )
{
	return ::ApplyUserOtherKegl_IncDec( value, GetCaret(), GetSelectInfo() );
}

/////////////////////////////////////////////////////////////////////////////

RealFde CRootNode::GetCurrentKegl()
{
	return ::GetCurrentKegl( GetCaret() );
}

/////////////////////////////////////////////////////////////////////////////

CNode* CRootNode::GetPropertyNode()
{
	return ::GetPropertyNode( GetCaret(), GetSelectInfo() );
}

int CRootNode::SelectPropertyNode( CNode *pNode )
{
	return ::SelectPropertyNode( pNode, GetCaret(), GetSelectInfo() );
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::SelectAll()
{
	if( GetChildCount() || ((CLineNode*) GetFirstChild())->GetChildCount() )
	{
		GetSelectInfo().GetBegin().SetNode( GetFirstChild() );
		GetSelectInfo().GetBegin().SetPosition( 0 );
		GetSelectInfo().GetEnd().SetNode( GetLastChild() );
		GetSelectInfo().GetEnd().SetPosition( ((CLineNode*) GetLastChild())->GetChildCount() );

		if( GetSelectInfo().IsSelection() )
		{
			GetCaret() = GetSelectInfo().GetEnd();
			return 0;
		}
		GetSelectInfo().SetEmpty();
	}
	return -1;
}

int CRootNode::ClearAll()
{
	if( SelectAll() == 0 && IsSelection() )
	{
		CFrameNode *pFrameNode = RemoveOrCopySelection( -1 );
		if( pFrameNode ) delete pFrameNode;
		return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::PtInSelection( RectFde& rc, PointFde& pt )
{
	ClearOnCharFlag();

	return ::TestSelection( rc, GetSelectInfo(), pt );
}

int CRootNode::DrInSelection()
{
	ClearOnCharFlag();

	return ::DragInSelection( GetDrag(), GetSelectInfo() );
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertExternalNode( CExtNode *pNode )
{
	if( pNode == NULL )
		return -1;

	SetOnCharFlag();

	if( IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 0 );
		delete pFrameNode;
	}

	return (::InsertPlaneText( GetCaret(), pNode ) == NULL ? -1 : 0);
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertPlaneText( FXmlBtn *ptrFXmlBtn )
{
	Q_ASSERT( ptrFXmlBtn != NULL );

	if( IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 0 );
		delete pFrameNode;
	}

	CNodeInfo ni;
	CNodeExInfo ex_ni;
	QString strText;
	long nStyle = ::getCurrentFormulatorStyle_OnChar();

	int fRet = -1;
	if( CNode::UpdateTextNode( *ptrFXmlBtn, strText, nStyle, ni, ex_ni ) != -1 )
	{
		RealFde kegl = ::getKegl( GetCaret().GetNode()->GetLevel() );
		fRet = (::InsertPlaneText( GetCaret(), strText, nStyle, kegl, ni, ex_ni ) == NULL ? -1 : 0);
	}

	return fRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertPlaneText( QString &strText, int fUnicode )
{
	if( IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 0 );
		delete pFrameNode;
	}

	CNodeInfo ni;

	if( fUnicode )
		ni.setUnicode();

	RealFde kegl = ::getKegl( GetCaret().GetNode()->GetLevel() );
	CNodeExInfo exniV;
	return (::InsertPlaneText( GetCaret(), strText, -1, kegl, ni/*CNodeInfo()*/, exniV ) == NULL ? -1 : 0);
}

int CRootNode::InsertPlaneText( QString &strText, long style, RealFde kegl )
{
	if( IsSelection() )
	{
		CFrameNode *pFrameNode = new CFrameNode();
		::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 0 );
		delete pFrameNode;
	}
	CNodeInfo niV;
	CNodeExInfo exniV;
	return (::InsertPlaneText( GetCaret(), strText, style, kegl, niV, exniV ) == NULL ? -1 : 0);
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertPlaneTextByDFA( const QString& strText )
{
	QString sV(strText);
	return InsertPlaneText( sV, 0 );
}

/////////////////////////////////////////////////////////////////////////////
//

int CRootNode::InsertMathML( FXmlBtn *ptrFXmlBtn, QString strSubstitution, int shouldMakeReadOnly )
{
	int nRet = -1;
	CFormulaNode *pInsertedFormula = 0;
	QString taskname = __HERMITECH_TOOLBAR_MATHML__;

	if( strSubstitution.length() )
	{
		nRet = InsertMathML( (taskname + _T("=")),
								strSubstitution, &pInsertedFormula,
								ptrFXmlBtn ? &ptrFXmlBtn : 0, 1 /* do not make read-only */ );
	}
	else if( ptrFXmlBtn )
	{			
		nRet = InsertMathML( taskname, 
								ptrFXmlBtn->getNodeMathMLText(),
								&pInsertedFormula, &ptrFXmlBtn, shouldMakeReadOnly );
	}

	if( pInsertedFormula )
	{
		if( ptrFXmlBtn ) pInsertedFormula->UpdateFormulaNode( *ptrFXmlBtn );
		pInsertedFormula->RecalculateSize( 0 );
		pInsertedFormula->GetLineNode()->RecalculateSize( 1 );
	}
	return nRet;
}

/////////////////////////////////////////////////////////////////////////////
//

int CRootNode::InsertMathML( const QString& strTask, const QString& strText, 
	CFormulaNode** pInsertedFormula, FXmlBtn **ptrFXmlBtn, int shouldMakeReadOnly )
{
	if( strText.length() == 0 )
		return -1;

	TXML_Storage* bms = CompileMathML( strTask, strText );
	if( bms == NULL )
		return -1;

	return InsertMathML( strTask, bms, pInsertedFormula, ptrFXmlBtn, shouldMakeReadOnly );
}

/////////////////////////////////////////////////////////////////////////////
//

int CRootNode::InsertMathML( const QString& strTask, const QString& strText, int fExclamation )
{
	TXML_Storage* bms = CompileMathML( strTask, strText );
	if( !bms )
	{
		if( fExclamation && m_owner->isVerbose() )
		{
			bool shouldOverride = false;
			QCursor* currentCursor = QApplication::overrideCursor();
			if( currentCursor && currentCursor->shape() == Qt::WaitCursor )
				shouldOverride = true;

			if( shouldOverride ) QApplication::setOverrideCursor( Qt::ArrowCursor );
			QMessageBox::StandardButton ret = QMessageBox::question( 0, __FMLCORE_Product_Title,
				QObject::tr("Current MathML text cannot be converted into expression because of error(s). Do you wish to insert this MathML text as a plain text?"),
				QMessageBox::Yes | QMessageBox::No );
			if( shouldOverride ) QApplication::restoreOverrideCursor();

			if( ret == QMessageBox::Yes )
			{
				if( IsSelection() ) MoveDel( NULL );
				return InsertPlaneTextByDFA( strText );
			}
		}
		return -1;
	}
	if( IsSelection() ) MoveDel( NULL );
	return InsertMathML( strTask, bms );
}

int CRootNode::ReplaceMathML( const QString& strTask, const QString& strText, int fExclamation )
{
	TXML_Storage* bms = CompileMathML( strTask, strText );
	if( !bms )
	{
		if( fExclamation && m_owner->isVerbose() )
		{
			bool shouldOverride = false;
			QCursor* currentCursor = QApplication::overrideCursor();
			if( currentCursor && currentCursor->shape() == Qt::WaitCursor )
				shouldOverride = true;

			if( shouldOverride ) QApplication::setOverrideCursor( Qt::ArrowCursor );
			QMessageBox::StandardButton ret = QMessageBox::question( 0, __FMLCORE_Product_Title,
				QObject::tr("Current MathML text cannot be converted into expression because of error(s). Do you wish to insert this MathML text as a plain text?"),
				QMessageBox::Yes | QMessageBox::No );
			if( shouldOverride ) QApplication::restoreOverrideCursor();

			if( ret == QMessageBox::Yes )
			{
				if( IsSelection() ) MoveDel( NULL );
				return InsertPlaneTextByDFA( strText );
			}
		}
		return -1;
	}
	ClearAll();
	InsertMathML( strTask, bms );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//

static QString mathV(FBL_TAG_math);
int CRootNode::InsertMathML( const QString& strTask, TXML_Storage* bms, 
	CFormulaNode** pInsertedFormula, FXmlBtn **p_ptrFXmlBtn, int shouldMakeReadOnly )
{
	if( bms == NULL )
		return -1;

	SetOnCharFlag();

	int fToolbar = (strTask == __HERMITECH_TOOLBAR_MATHML__);
	int _root_free = 0;
	AST_TXML_Tag* root = bms->getFirstTag( FBL_TAG_math );
	
	if( !root )
	{
		root = new AST_TXML_Tag( &mathV, 0, bms );
		_root_free = 1;
	}

	if( root )
	{
		if( p_ptrFXmlBtn && *p_ptrFXmlBtn )
		{
			// call misc. dialogs to ask a user for details of Content MathML buttons
			if( InsertMathML_PreprocessButtonContent( *p_ptrFXmlBtn, bms ) == -1 )
			{
				root = new AST_TXML_Tag( &mathV, 0, (AST_TXML_Compound*)0 );
				_root_free = 1;
			}
		}

		root->recursiveProcessing( &fToolbar, mmlTreeIterator__mapUnicodeValue2EntityName );

		if( fToolbar )
		{
			//=== to not delete mrow
			AST_TXML_Compound *_tags = root->getTags();
			if( _tags && _tags->getChildNumber() == 1 && _tags->getChild( 0 ) &&
				_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
			{
				AST_TXML_Tag* _mrow = (AST_TXML_Tag*)(_tags->getChild( 0 ));
				if( _mrow->getTagName() == FBL_TAG_mrow )
					_mrow->getFlags().setFlag( OLIMP_MATHML_FLAG__MROW_KEEP1CHILD );
			}
		}
	}

	CFrameNode *pFrameNode = new CFrameNode();
	if( pFrameNode->GetChildCount() ) pFrameNode->DeleteAllChild();	
	pFrameNode->read_mathml( root );
	useLinebreakPosition( pFrameNode );

	if( root && _root_free )
	{
		root->setTags( 0 );
		delete root;
		root = 0;
	}
	delete bms;

	int nRet = (pFrameNode->GetChildCount() ? 0 : -1);
	if( nRet == 0 )
	{
		int bContent = 0;
		if( p_ptrFXmlBtn && *p_ptrFXmlBtn && (*p_ptrFXmlBtn)->isContentEatLeftOp() )
		{
			bContent = 1;
		}
		else if( !IsSelection() )
		{
			CNode *whereTo = GetCaret().GetNode();
			if( whereTo )
			{
				CFormulaNode *tokenNode = whereTo->getParentContentFormulaNode( 0 /*do not use 'ignored' nodes also*/ );
				if( tokenNode )
				{
					if( ::mml_is_cm_token( tokenNode->to_mathml_data.name ) && !tokenNode->isAutodetectContent() )
						tokenNode->setAutodetectContent();
				}
			}
		}

		CFrameNode *pFrameCopyL = NULL;
		CFrameNode *pFrameCopyR = NULL;
		if( IsSelection() )
		{
			pFrameCopyL = new CFrameNode();
			::RemoveToFrame( pFrameCopyL, GetCaret(), GetSelectInfo() );
		}
		else 
		{
			if( bContent )
			{
				::CutLeftRight( GetCaret(), GetSelectInfo(), &pFrameCopyL, &pFrameCopyR );
			}
		}

		pFrameNode->RecalculateSizeFull();

		CFormulaNode *pFormulaNode = NULL;
		if( fToolbar )
		{
			if( pFrameNode->GetFirstChild() && pFrameNode->GetFirstChild()->GetType() == NODE_LINE &&
				((CLineNode*) pFrameNode->GetFirstChild())->GetFirstChild() && 
				((CLineNode*) pFrameNode->GetFirstChild())->GetFirstChild()->GetType() == NODE_FORMULA )
			{
				Q_ASSERT( pFrameNode->GetChildCount() == 1 );
				Q_ASSERT( ((CLineNode*) pFrameNode->GetFirstChild())->GetChildCount() == 1 );

				pFormulaNode = (CFormulaNode*) ((CLineNode*) pFrameNode->GetFirstChild())->GetFirstChild();
				if( pInsertedFormula )
					*pInsertedFormula = pFormulaNode;
			}
		}
		else
		{
			pFrameNode->setReadOnlyAttribute4AllNonReadOnlyNodes();
			for( long i = 0; i < pFrameNode->GetChildCount(); i++ )
			{
				CNode *pNode2Treat = pFrameNode->GetChild( i );
				if( pNode2Treat && pNode2Treat->GetType() == NODE_LINE && pNode2Treat->isReadOnly() )
					pNode2Treat->clearReadOnly();
			}
		}

		nRet = ::InsertFromFrame( pFrameNode, GetCaret(), 1 );

		if( pFormulaNode != NULL )
		{
			if( fToolbar )
			{
				if( shouldMakeReadOnly )
				{
					pFormulaNode->setReadOnlyAttribute4AllNonEmptyNodes();
				}
				pFormulaNode->CM_WrapIntoBracketsOnPrecedence();
				pFormulaNode->CM_WrapChildrenIntoBracketsOnPrecedence( pFrameCopyL, pFrameCopyR );
				::PasteLeftRight( GetCaret(), GetSelectInfo(), pFrameCopyL, pFrameCopyR, pFormulaNode );
			}

			if( ::mml_is_cm_token( pFormulaNode->to_mathml_data.name ) )
				pFormulaNode->setContentTokenParent_Transparent();
		}

		if( pFrameCopyL != NULL )
		{
			delete pFrameCopyL;
			pFrameCopyL = NULL;
		}
		if( pFrameCopyR != NULL )
		{
			delete pFrameCopyR;
			pFrameCopyR = NULL;
		}
	}

	delete pFrameNode;
	pFrameNode = NULL;

	return nRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertMathML_Force( const QString& strTask, const QString& strText )
{
	TXML_Storage* bms = CompileMathML( strTask, strText );

	if( bms == NULL )
		return -1;

	int _root_free = 0;
	AST_TXML_Tag* root = bms->getFirstTag( FBL_TAG_math );
	if( !root )
	{
		root = new AST_TXML_Tag( &mathV, 0, bms );
		_root_free = 1;
	}

	if( root )
	{
		root->recursiveProcessing( NULL, mmlTreeIterator__mapUnicodeValue2EntityName );
	}

	GetSelectInfo().SetEmpty();
	DeleteAllChild();
	read_mathml( root );
	useLinebreakPosition( this );

	if( root && _root_free )
	{
		root->setTags( 0 );
		delete root;
		root = 0;
	}
	if( bms ) 
		delete bms;

	if( GetChildCount() == 0 )
		AddChild( new CLineNode() );
	GetCaret().SetNode( GetFirstChild() );
	GetCaret().SetPosition( 0 );
	RecalculateSizeFull();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::InsertFormula( FXmlBtn *ptrFXmlBtn, CFormulaNode **pFormulaNode )
{
	Q_ASSERT( ptrFXmlBtn != NULL );

	int nRet = -1;
	CFormulaNode *pNode = ptrFXmlBtn ? CreateFormulaNode( *ptrFXmlBtn ) : NULL;
	if( pNode != NULL )
	{
		pNode->SetLevel( GetCaret().GetNode()->GetFrameNode()->GetLevel() );
		pNode->RecalculateSizeFull();

		CFrameNode *pFrameNode = NULL;
		if( IsSelection() )
		{
			pFrameNode = new CFrameNode();
			::RemoveToFrame( pFrameNode, GetCaret(), GetSelectInfo(), 0 );
		}

		nRet = ::InsertFromFormula( pNode, GetCaret(), 0 );

		if( CaretToFirstEmptyLine( pNode, GetCaret() ) == 0 )
		{
			if( pFrameNode && !pNode->isFrameHidden() )
			{
				::InsertFromFrame( pFrameNode, GetCaret(), 1 );
				delete pFrameNode;
			}

			CaretToFirstEmptyLine( pNode, GetCaret() );
		}

		if( pNode->isFrameHidden() && pNode->GetParent() && pNode->GetParent()->GetType() == NODE_LINE )
		{
			long tbr, btn;
			QString buttonID = QString(_T("disabled ")) + pNode->getCalcButtonID();
			if( ::getCurrentToolBarSet()->getButtonCoordByID( buttonID, tbr, btn ) != -1 )
			{
				FXmlBtn *hiddenTextContent = ::getCurrentToolBarSet()->getButtonCalc( tbr, btn );
				if( hiddenTextContent && hiddenTextContent->getNodeType() == NODE_PLANETEXT )
				{
					InsertPlaneText( hiddenTextContent );
					GetCaret().SetNode( pNode->GetParent() );
					GetCaret().SetPosition( pNode->GetParent()->GetChildIndex( pNode ) + 1 );
				}
			}
			pNode->iterateNodeTree( setRecursiveReadOnly, 0 );
			pNode->RecalculateSizeFull();
		}

		pNode->GetParent()->RecalculateSize( 1 );
	}

	if( pFormulaNode )
		*pFormulaNode = pNode;
	return nRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::Insert( CNode* pNode )
{
	int nRet = -1;

	switch ( pNode->GetType() )
	{
	case NODE_FRAME:
		if( GetDrag().IsEmpty() )
		{
			nRet = ::InsertFromFrame( (CFrameNode*) pNode, GetCaret(), 1 );
		}
		else
		{
			Q_ASSERT( isValidPosition( GetDrag() ) );
			nRet = ::InsertFromFrame( (CFrameNode*) pNode, GetDrag(), 1, &GetSelectInfo() );
			GetCaret() = GetDrag();
			GetDrag().SetEmpty();
		}
		break;

	case NODE_LINE:
		Q_ASSERT( GetDrag().IsEmpty() );
		nRet = ::InsertFromLine( (CLineNode*) pNode, GetCaret(), 1 );
		break;

	case NODE_PLANETEXT:
		Q_ASSERT( GetDrag().IsEmpty() );
		nRet = ::InsertFromText( (CPlaneText*) pNode, GetCaret(), 1 );
		break;

	case NODE_FORMULA:
		Q_ASSERT( GetDrag().IsEmpty() );
		nRet = ::InsertFromFormula( (CFormulaNode*) pNode, GetCaret(), 1 );
		break;

	default:
		break;
	}

	return nRet;
}

/////////////////////////////////////////////////////////////////////////////

int CRootNode::Insert( FXmlBtn *ptrFXmlBtn, int shouldMakeReadOnly )
{
	if( ptrFXmlBtn == NULL ) 
		return -1;

	SetOnCharFlag();

	if( ptrFXmlBtn->getNodeType() == NODE_PLANETEXT )
	{
		if( InsertPlaneText( ptrFXmlBtn ) == -1 )
			return -1;
	}
	else if( ptrFXmlBtn->getNodeType() == NODE_MATHML )
	{
		if( InsertMathML( ptrFXmlBtn, NULL, shouldMakeReadOnly ) == -1 )
			return -1;
	}
	else if( ptrFXmlBtn->getNodeType() == NODE_FORMULA )
	{
		if( ptrFXmlBtn->isPropertyInput() )
		{
			int a = ptrFXmlBtn->isPropertyInput( FBL_ATTR_rows );
			int b = ptrFXmlBtn->isPropertyInput( FBL_ATTR_columns );
			if( a || b )
			{
				CMmlTableDlg dlg( a, a ? 5 : ptrFXmlBtn->getRows(), b, b ? 5 : ptrFXmlBtn->getColumns() );
				if( owner()->callDialog( &dlg ) == QDialog::Accepted )
				{
					ptrFXmlBtn->setRows( dlg.rows() );
					ptrFXmlBtn->setColumns( dlg.columns() );
				}
				else
					return -1;
			}
		}

		if( InsertFormula( ptrFXmlBtn, NULL ) == -1 )
			return -1;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

QString CRootNode::to_xhtml( const QString& path2xsl, const QString& originator, const QString& title )
{
	static QString strResult = 
"<?xml version='1.0'?>\n"
"<?xml-stylesheet type='text/xsl' href='%1'?>\n"
"<html xmlns='http://www.w3.org/1999/xhtml'\n"
"      xmlns:pref='http://www.w3.org/2002/Math/preference'\n"
"      pref:renderer='hermitech'>\n"
"<head>\n"
"  <meta http-equiv='Content-Type' content='text/html' />\n"
"  <meta name='ProgId' content='Formulator.Document' />\n"
"  <meta name='Generator' content='%2' />\n"
"  <meta name='Originator' content='%3' />\n"
"  <title>%4</title>\n"
"</head>\n"
"\n"
"<body>\n"
"<p>\n"
"%5</p>\n"
"</body>\n"
"</html>\n"
"";
	return strResult.arg(path2xsl).arg(originator).arg(originator).arg(title).arg(to_mathml_4_html( 0, ::isOption_XHTMLAsBlock() ));
}

/////////////////////////////////////////////////////////////////////////////

void CRootNode::useLinebreakPosition( CFrameNode *node )
{
	node->iterateNodeTree( accountLinebreaks, 0 );
}

///////////////////////////////////////////////////////////////////////////////

void searchRecursiveFindAllBrackets( CNode* node, void* arg )
{
	if( node && arg )
	{
		std::vector<SelectNode>* snb = (std::vector<SelectNode>*) arg;
		for( long i = 0; i < (long) snb->size(); i++ )
		{
			// если на скобку есть ссылка из документа, отметим этот факт
			if( (*snb)[ i ].GetNode() == node )
				(*snb)[ i ].SetEmpty();
		}
	}
}

int CLRBracketSelection::OnCharBracket( unsigned int nChar, const SelectNode& sn )
{
	if( nChar == _T('(') )
		m_isSNLeft = 1;
	else if( nChar == _T(')') )
		m_isSNLeft = 0;
	else
		return -1;

	if( m_snb.size() > 0 && m_isSNVectorLeft == (m_isSNLeft ? 0 : 1) )
	{
		m_sn = sn;
		return 1;
	}

	if( m_snb.size() == 0 || m_isSNVectorLeft == -1 )
	{
		m_isSNVectorLeft = m_isSNLeft;
		notifyNodeOnRemoveBracket( m_snb );
		m_snb.clear();
		notifyNodeOnCreateBracket( sn, m_isSNVectorLeft );
		m_snb.push_back( sn );
	}
	else if( m_isSNVectorLeft == m_isSNLeft )
	{
		notifyNodeOnCreateBracket( sn, m_isSNVectorLeft );
		m_snb.push_back( sn );
	}
	else if( m_snb.size() > 0 /*always true*/ && m_snb[ m_snb.size() - 1 ].IsEmpty() )
	{
		Q_ASSERT( 0 );
		DecBracketsStack();
	}
	ClearBrackets_SN();
	return 0;
}

int CRootNode::SetBracketsByChar( unsigned int nChar )
{
	int isLeft = -1;
	std::vector<SelectNode>& snb = getSNVectorRef( isLeft );
	for( long i = 0; i < (long) snb.size(); )
	{
		CNode *pNode = snb[ i ].GetNode();
		if( pNode && haveChild( pNode ) && pNode->hasBracket( snb[ i ].GetPosition(), isLeft ) )
			i++;
		else
			snb.erase( snb.begin() + i );
	}
	if( snb.size() == 0 )
		ClearBrackets_SNVector();

	int ret = OnCharBracket( nChar, GetCaret() );
	if( ret == 1 )
	{
		SetContentBrackets( isSNLeft() );
		ClearBrackets_OnePair();
		return 0;
	}
	return ret;
}

int CRootNode::SetContentBrackets( int isLeft )
{
	std::vector<CBracketCoord> leftCut, rightCut;
	if( SetContentBrackets_traversal( 1, GetLBMark(), leftCut, rightCut /*пустой массив*/ ) < 0 ) return -1;
	long leftIdx = SetContentBrackets_traversal( 0, GetRBMark(), rightCut, leftCut ), rightIdx = rightCut.size() - 1;
	if( leftIdx < 0 || leftCut.size() == 0 || rightCut.size() == 0 || leftIdx - 1 >= (long) leftCut.size() )
		return -1;
	else if( leftIdx == 0 )
		return -1;

	--leftIdx;
	long lbPos = leftCut[ leftIdx ].second >> 1, rbPos = rightCut[ rightIdx ].second >> 1;
	if( lbPos >= rbPos )
		return -1;
	CLineNode *sharedLine = leftCut[ leftIdx ].first;
	Q_ASSERT( sharedLine == rightCut[ rightIdx ].first );
	bool lbBroken = (leftCut[ leftIdx ].second % 2 == 1), rbBroken = (rightCut[ rightIdx ].second % 2 == 1);

	CFormulaNode *sharedFormula = sharedLine->getParentFormulaNode( 0 );
	if( sharedFormula && sharedFormula->GetParent() )
	{
		sharedFormula = sharedFormula->passParentApplyBrackets( NULL );
	}
	if( !sharedFormula || sharedFormula->GetParent() == 0 || sharedFormula->GetParent()->GetType() != NODE_LINE ) return -1;
	CLineNode *parentLine = (CLineNode*) sharedFormula->GetParent();
	if( !parentLine ) return -1;

	long lslotPos = 0, rslotPos = 0;
	CFormulaNode *lslot = 0, *rslot = 0;
	CFormulaNode *plslot = 0, *prslot = 0;
	if( lbBroken )
	{
		int iRet = SetContentBrackets_breakLine( 1, leftCut, lslotPos, lslot, plslot );
		if( iRet == -1 ) return -1;

	}
	if( rbBroken )
	{
		int iRet = SetContentBrackets_breakLine( 0, rightCut, rslotPos, rslot, prslot );
		if( iRet == -1 ) return -1;
	}
	
	CFormulaNode *activeSlot = sharedFormula;
	if( lslot && plslot )
	{
		activeSlot = SetContentBrackets_reorder( lslot, plslot, activeSlot );
	}
	if( rslot && prslot )
	{
		activeSlot = SetContentBrackets_reorder( rslot, prslot, activeSlot );
	}
	
	std::vector<CFormulaNode*> checklist;
	if( lslot )
		checklist.push_back( lslot );
	if( rslot )
		checklist.push_back( rslot );
	checklist.push_back( sharedFormula );
	if( plslot )
		checklist.push_back( plslot );
	if( prslot )
		checklist.push_back( prslot );

	CLineNode *focusLine = 0;
	long focusPos = 0;
	CNode *focusNode = GetCaret().GetNode();
	if( focusNode->GetType() == NODE_PLANETEXT && focusNode->GetParent() && focusNode->GetParent()->GetType() == NODE_LINE )
	{
		focusLine = (CLineNode*) focusNode->GetParent();
		focusPos = focusLine->GetChildIndex( focusNode );
		if( !isLeft ) focusPos++;
	}
	else if( focusNode->GetType() == NODE_LINE )
	{
		focusLine = (CLineNode*) focusNode;
		focusPos = GetCaret().GetPosition();
		if( focusPos == 0 )
		{
			focusNode = focusLine->GetFirstChild();
			isLeft = 1;
		}
		else if( focusPos >= focusLine->GetChildCount() )
		{
			focusNode = focusLine->GetLastChild();
			isLeft = 0;
		}
		else
		{
			focusNode = focusLine->GetChild( isLeft ? focusPos : (focusPos - 1) );
		}
		if( focusNode && focusNode->GetType() == NODE_FORMULA )
			focusNode = ((CFormulaNode*) focusNode)->passAutodetectSingleFormulaChildren();
	}
	else
	{
		Q_ASSERT( 0 );
	}

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

	parentLine->cleanSpareAutodetect();
	parentLine->RecalculateSizeFull();
	parentLine->RecalculateSize( 1 );

	if( focusNode && focusNode->GetParent() && focusNode->GetParent()->GetType() == NODE_LINE )
	{
		const SelectNode& upperSn = isLeft ? GetRBMark() : GetLBMark();
		CLineNode *pairLine = 0, *upperLine = 0;
		CNode *pairLineFocus = 0;
		long childIndexOfRequest = 0;
		if( upperSn.GetNode() != 0 && this->haveChild( upperSn.GetNode() ) )
		{
			if( upperSn.GetNode()->GetType() == NODE_PLANETEXT &&
				upperSn.GetNode()->GetParent() &&
				upperSn.GetNode()->GetParent()->GetType() == NODE_LINE )
			{
				upperLine = (CLineNode*) upperSn.GetNode()->GetParent();
			}
			else if( upperSn.GetNode()->GetType() == NODE_LINE )
			{
				upperLine = (CLineNode*) upperSn.GetNode();
			}
			for( pairLine = focusNode->getParentLineNode( 0, &childIndexOfRequest);
					pairLine != 0; 
					pairLine = pairLine->getParentLineNode( 0, &childIndexOfRequest) )
			{
				if( pairLine == upperLine ) break;
			}
		}
		if( isAutographEditing() && pairLine && (pairLineFocus = pairLine->GetChild( childIndexOfRequest )) != 0 &&
			!pairLineFocus->isCannotBeEdited() && !pairLineFocus->isCannotBeMoved() )
		{
			GetCaret().SetPosition( childIndexOfRequest + (isLeft ? 0 : 1) );
			GetCaret().SetNode( pairLine );
		}
		else
		{
			GetCaret().SetPosition( focusNode->GetParent()->GetChildIndex( focusNode ) + (isLeft ? 0 : 1) );
			GetCaret().SetNode( focusNode->GetParent() );
			::MoveLeftRight( GetCaret(), GetSelectInfo(), isLeft ? 1 : 0, 0, 0, 1 );
		}
	}

	return 0;
}

long CRootNode::SetContentBrackets_traversal(
						int isLeft, const SelectNode& bracket, 
						std::vector<CBracketCoord>& cutted,
						const std::vector<CBracketCoord>& stopLineNode )
{
	const int isMultipleChildFormulaAllowed = 1;
	CLineNode *lineNode = 0;
	long position = bracket.GetPosition();

	if( bracket.GetNode()->GetType() == NODE_PLANETEXT && bracket.GetNode()->GetParent() )
	{
		position = bracket.GetNode()->GetParent()->GetChildIndex( bracket.GetNode() );
		if( !isLeft ) position++;
		lineNode = (CLineNode*) bracket.GetNode()->GetParent();
	}
	else if( bracket.GetNode()->GetType() == NODE_LINE )
	{
		lineNode = (CLineNode*) bracket.GetNode();
	}
	if( !lineNode ) return -1;

	long childIndexOfRequest = 0;
	CFormulaNode *pFormulaNode = lineNode->getParentContentFormulaNode( 1, &childIndexOfRequest );
	if( pFormulaNode == 0 )
	{
		childIndexOfRequest = 0;
		if( position == 0 && lineNode->GetFirstChild() && lineNode->GetFirstChild()->GetType() == NODE_FORMULA )
			pFormulaNode = (CFormulaNode*) lineNode->GetFirstChild();
		else if( position == lineNode->GetChildCount() && lineNode->GetLastChild() && lineNode->GetLastChild()->GetType() == NODE_FORMULA )
			pFormulaNode = (CFormulaNode*) lineNode->GetLastChild();

		if( pFormulaNode && pFormulaNode->GetChildCount() == 1 )
		{
			lineNode = pFormulaNode->getContentFormulaSingleLineChild();
			if( !lineNode ) return -1;
			if( position ) position = lineNode->GetChildCount();
		}
		else
			return -1;
	}
	else
	{
		if( !isMultipleChildFormulaAllowed && pFormulaNode->GetChildCount() != 1 ) return -1;
	}
	Q_ASSERT( pFormulaNode && lineNode && (isMultipleChildFormulaAllowed || pFormulaNode->GetChildCount() == 1) );

	int inner = ((childIndexOfRequest == 0 && position == 0) || 
				 (childIndexOfRequest == pFormulaNode->GetChildCount() - 1 && position >= lineNode->GetChildCount())) ? 0 : 1;

	cutted.push_back( std::make_pair( lineNode, (position << 1) + inner ) );
	for( long i = 0; i < (long) stopLineNode.size(); i++ )
	{
		if( stopLineNode[ i ].first == lineNode )
			return i + 1;
	}

	for(	CFormulaNode *parentFormulaNode = 0;
			(isMultipleChildFormulaAllowed || pFormulaNode->GetChildCount() == 1) && 
			pFormulaNode->GetParent() && pFormulaNode->GetParent()->GetType() == NODE_LINE;
			pFormulaNode = parentFormulaNode )
	{
		Q_ASSERT( pFormulaNode );
		lineNode = (CLineNode*) pFormulaNode->GetParent();
		position = lineNode->GetChildIndex( pFormulaNode );
		if( !isLeft ) position++;
		if( (parentFormulaNode = lineNode->getParentContentFormulaNode( 1, &childIndexOfRequest )) == 0 )
			break;
		if( inner == 0 )
		{
			inner = ((childIndexOfRequest == 0 && position == 0) || 
					 (childIndexOfRequest == parentFormulaNode->GetChildCount() - 1 && position >= lineNode->GetChildCount())) ? 0 : 1;
		}
		cutted.push_back( std::make_pair( lineNode, (position << 1) + inner ) );
		for( long i = 0; i < (long) stopLineNode.size(); i++ )
		{
			if( stopLineNode[ i ].first == lineNode )
				return i + 1;
		}
	}
	return 0;
}

int CRootNode::SetContentBrackets_breakLine( 
				int isLeft, const std::vector<CBracketCoord>& cutted, long& slotPos,
				CFormulaNode *&slot, CFormulaNode *&pslot )
{
	for( long i = 0; i < (long) cutted.size(); i++ )
	{
		if( cutted[ i ].second % 2 )
		{
			slotPos = cutted[ i ].second >> 1;
			if( !isLeft ) --slotPos;
			CLineNode *slotLine = cutted[ i ].first;
			CNode *pNode = slotLine->GetChild( slotPos );
			if( !pNode || pNode->GetType() != NODE_FORMULA || 
				pNode != (isLeft ? slotLine->GetLastChild() : slotLine->GetFirstChild()) )
			{
				return -1;
			}
			slot = ((CFormulaNode*) pNode)->passParentApplyBrackets( NULL );
			pslot = ((CLineNode*) slot->GetParent())->getParentFormulaNode( 0 );
			if( !pslot || pslot->GetParent() == 0 ) return -1;
			pslot = pslot->passParentApplyBrackets( NULL );
			break;
		}
	}
	return 0;
}

CFormulaNode* CRootNode::SetContentBrackets_reorder( CFormulaNode *slot, CFormulaNode *pslot, CFormulaNode *sharedFormula )
{
	return CFormulaNode::reorderFormulaBracketsTree( slot, pslot, sharedFormula, 1 );
}

/////////////////////////////////////////////////////////////////////////////
