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

#include "node.h"

//////////////////////////////////////////////////////////////////////

// SetParentFormulaSize
void CNode::UseBtnCalc_InitByThisSize( FBtn& btn )
{
	// set coordinates that relate us to the parent, therefore X = Y = 0 always

	btn.getCalc()->SetLocal( SVG_NodeTemplate_Parameter_Left, 0 );
	btn.getCalc()->SetLocal( SVG_NodeTemplate_Parameter_Top, 0 );
	btn.getCalc()->SetLocal( SVG_NodeTemplate_Parameter_Right, GetCoord(SVG_NodeTemplate_Parameter_CX) );
	btn.getCalc()->SetLocal( SVG_NodeTemplate_Parameter_Bottom, GetCoord(SVG_NodeTemplate_Parameter_CY) );

	btn.getCalc()->SetLocal( 4 + SVG_NodeTemplate_Parameter_Left, getLeftIndent() );
	btn.getCalc()->SetLocal( 4 + SVG_NodeTemplate_Parameter_Top, getTopIndent() );
	btn.getCalc()->SetLocal( 4 + SVG_NodeTemplate_Parameter_Right, getRightIndent() );
	long i;
	btn.getCalc()->SetLocal( i = 4 + SVG_NodeTemplate_Parameter_Bottom, getBottomIndent() );
	btn.getCalc()->SetLocal( ++i, ::getCurrentFormulatorStyle().getEM() );

	CPlaneText *textNode = (GetType() == NODE_PLANETEXT) ? ((CPlaneText*) this) : new CPlaneText();
	btn.getCalc()->SetLocal( ++i, textNode->getAscent() );
	btn.getCalc()->SetLocal( ++i, textNode->getDescent() );
	if( textNode != this ) delete textNode;
}

// GetParentFormulaSize
void CNode::UseBtnCalc_ChangeThisMargin( FBtn& btn )
{
	// set parent margins
	setLeftIndent( (RealFde) btn.getCalc()->GetLocal( 4 + SVG_NodeTemplate_Parameter_Left ) );
	setTopIndent( (RealFde) btn.getCalc()->GetLocal( 4 + SVG_NodeTemplate_Parameter_Top ) );
	setRightIndent( (RealFde) btn.getCalc()->GetLocal( 4 + SVG_NodeTemplate_Parameter_Right ) );
	setBottomIndent( (RealFde) btn.getCalc()->GetLocal( 4 + SVG_NodeTemplate_Parameter_Bottom ) );
}

// GetParentFormulaSize2ThisSize
void CNode::UseBtnCalc_ChangeThisSize( FBtn& btn )
{
	// change parent size
	RealFde width = (RealFde) (btn.getCalc()->GetLocal( SVG_NodeTemplate_Parameter_Right ) - btn.getCalc()->GetLocal( SVG_NodeTemplate_Parameter_Left ));
	RealFde height = (RealFde) (btn.getCalc()->GetLocal( SVG_NodeTemplate_Parameter_Bottom ) - btn.getCalc()->GetLocal( SVG_NodeTemplate_Parameter_Top ));
	if( width >= 0.0 && height >= 0.0 )
		SetSize( SizeFde( width, height ) );
}

//////////////////////////////////////////////////////////////////////

void CFormulaNode::UpdateFormulaNode( const FBtn& btn )
{
	if( btn.getNodeType() == NODE_FORMULA || btn.getNodeType() == NODE_MATHML )
	{
		HMathMLAttr backupAttr( getMathMLAttr() );
		CLineNode *backupAddonNode = 0;
		if( getAddonNode() != 0 && getAddonNode()->GetType() == NODE_LINE )
		{
			CLineNode *srcLine = (CLineNode*) getAddonNode();
			backupAddonNode = new CLineNode();
			backupAddonNode->PumpLine2Line( srcLine );
		}
		setNodeInfo_Property( btn.getPropList() );
		setUnfilteredAttr( backupAttr );
		if( backupAddonNode )
		{
			setAddonNode( backupAddonNode );
			backupAddonNode = 0;
		}
		setSpecialVerticalCenter( btn.getAlignmentType(), btn.getAlignmentValue() );
	}
}

CFormulaNode* CNode::CreateFormulaNode( const FBtn& btn )
{
	if( btn.getNodeType() != NODE_FORMULA ) return 0;

	CFormulaNode *pNodeFormula = NULL;
	if( !::xml_strcmp( btn.getTextValue(), "table" ) && btn.getRows() && btn.getColumns() )
	{
		pNodeFormula = new CFormulaNode();

		// create table cells
		RealFde frameDX = 0.4, frameDY = 0.1;
		RealFde CellsInWidth  = btn.getColumns() * (1.0 + 2.0 * frameDX);
		RealFde CellsInHeight = btn.getRows() * (1.0 + 2.0 * frameDY);
		RealFde CellX = HF_BINBUTTON_HORIZONTAL_SCALE / CellsInWidth;
		RealFde CellY = HF_BINBUTTON_VERTICAL_SCALE / CellsInHeight;
		RealFde SpaceX = 2.0 * frameDX * CellX;
		RealFde SpaceY = 2.0 * frameDY * CellY;

		long i, j;
		RectFde rc;

		rc.setTop( frameDY * CellY );
		for( i = 0; i < btn.getRows(); i++ )
		{
			rc.setBottom( rc.top() + CellY );
			rc.setLeft( frameDX * CellX );
			for( j = 0; j < btn.getColumns(); j++ )
			{
				rc.setRight( rc.left() + CellX );
				pNodeFormula->AddSlot( CFormulaPos( rc, HORISONTAL_CENTER | VERTICAL_CENTER, 0 ) );
				pNodeFormula->AddChild( new CFrameNode() );
				rc.setLeft( rc.right() + SpaceX );
			}
			rc.setTop( rc.bottom() + SpaceY );
		}
	}
	else if( btn.getChildCount() > 0 )
	{
		pNodeFormula = new CFormulaNode();
		for( long i = 0; i < btn.getChildCount(); i++ )
		{
			pNodeFormula->AddSlot( CFormulaPos( btn.getChildItem(i).pos, btn.getChildItem(i).align, btn.getChildItem(i).level ) );
			pNodeFormula->AddChild( new CFrameNode() );
			pNodeFormula->FlushAltGroup();
		}
		if( btn.getGraphicsCount() > 0 )
		{
			QVector<CGraphPrimFormula> srcGraphics( btn.getGraphicsCount() );
			for( long i = 0; i < btn.getGraphicsCount(); i++ )
			{
				srcGraphics[ i ].Create( btn.getGraphicsItem(i) );
				pNodeFormula->AddAltGroup( btn.getGraphicsItem(i).altGroup );
			}
			pNodeFormula->CreateGraphPrimIndirect( srcGraphics );
		}
	}
	if( pNodeFormula )
	{
		pNodeFormula->setNodeInfo_Property( btn.getPropList() );
		pNodeFormula->SetName( btn.getName() );
		pNodeFormula->setMathMLFormat( btn.getMathMLData() );
		pNodeFormula->setSpecialVerticalCenter( btn.getAlignmentType(), btn.getAlignmentValue() );
		pNodeFormula->VerifyAltGroup();
		pNodeFormula->setCalcCoord( btn.getID() );
		if( btn.getRows() ) pNodeFormula->SetNumLine( btn.getRows() );
		if( btn.getColumns() ) pNodeFormula->SetNumColumn( btn.getColumns() );
	}
	return pNodeFormula;
}

int CNode::UpdateTextNode( const FBtn& btn, QString& text, long& style, CNodeInfo& _info, CNodeExInfo& _ex_info )
{
	if( btn.getNodeType() != NODE_PLANETEXT ) return -1;

	_info.setMathMLFormat( btn.getMathMLData() );
	_info.setNodeInfo_Property( btn.getPropList() );

	if( btn.getTextValue().length() )
		text = btn.getTextValue();
	else
	{
		text = HF_PLANETEXT_HUB_SYMBOL;
		_info.setCannotDrawText();
	}
	style = btn.getTextStyle();
	_ex_info.setCalcCoord( btn.getID() );

	if( btn.getGraphicsCount() > 0 )
	{
		QVector<CGraphPrimFormula> srcGraphics( btn.getGraphicsCount() );
		for( long i = 0; i < btn.getGraphicsCount(); i++ )
			srcGraphics[ i ].Create( btn.getGraphicsItem(i) );
		_ex_info.CreateGraphPrimIndirect( srcGraphics );
		_ex_info.VerifyAltGroup();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
