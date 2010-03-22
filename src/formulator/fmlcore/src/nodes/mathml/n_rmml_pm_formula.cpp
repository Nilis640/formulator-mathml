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

#include "HUtils/idfas.h"
#include "../../dictionary/entity_map.h"
#include "../../dictionary/op_iddefs.h"
#include "n_cm_treetemplate.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

#define FBL_VAL_center					"center"		
#define FBL_VAL_baseline				"baseline"

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode( AST_TXML_Tag *node )
{
	CFormulaNode *ret_node = NULL;
	if( !node ) return ret_node;

	// all non-tag children are ignored
	node->deleteValueChildren();

	QString tagname = node->getTagName();

	long i;
	AST_TXML_Compound *_inner = 0, *node_tags = 0;
	AST_TXML_Tag *t_mrow = 0;

	if( (node_tags = node->getTags()) != NULL && 
		(node_tags->getChildNumber() == 0 ||
		 node_tags->getChildNumber() > 1) )
	{
		if( !xml_strcmp( tagname, FBL_TAG_mstyle ) ||
			!xml_strcmp( tagname, FBL_TAG_msqrt ) ||
			!xml_strcmp( tagname, FBL_TAG_merror ) ||
			!xml_strcmp( tagname, FBL_TAG_menclose ) ||
			!xml_strcmp( tagname, FBL_TAG_mpadded ) ||
			!xml_strcmp( tagname, FBL_TAG_mphantom ) ||
			!xml_strcmp( tagname, FBL_TAG_mtd ) ||
			!xml_strcmp( tagname, FBL_TAG_math ) )
		{
			_inner = new AST_TXML_Compound();
			for( i = 0; i < node_tags->getChildNumber(); i++ )
			{
				_inner->Add( node_tags->getChild( i ) );
				node_tags->ReplaceChild( i, NULL );
			}
			node_tags->stmt.Flush();
			t_mrow = new AST_TXML_Tag( &mrowV, 0, _inner );
			t_mrow->getFlags().setFlag(OLIMP_MATHML_FLAG__MROW_ARTIFICIAL);
			node_tags->Add( t_mrow );
		}
	}
 
	//=== TABLE
	if( ::mml_is_table_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MTABLE( node );
	else if( ::mml_is_fenced_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MFENCED( node );
	else if( ::mml_is_frac_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MFRAC( node );
	else if( ::mml_is_style_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MSTYLE( node );
	else if( ::mml_is_under_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MUNDER( node );
	else if( ::mml_is_over_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MOVER( node );
	else if( ::mml_is_underover_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MUNDEROVER( node );
	else if( ::mml_is_mphantom_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MPHANTOM( node );
	else if( ::mml_is_row_tag( tagname ) || ::mml_is_tablecell_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MROW( node );
	else if( ::mml_is_maction_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MACTION( node );
	else if( ::mml_is_mmultiscripts_tag( tagname ) )
		ret_node = InsertMMLFormulaNode_MMULTISCRIPTS( node );
	else if( tagname == FBL_TAG_menclose )
		ret_node = InsertMMLFormulaNode_MENCLOSE( node );
	else if( tagname == FBL_TAG_mpadded )
		ret_node = InsertMMLFormulaNode_MPADDED( node );
	else
	{
		ret_node = InsertMMLFormulaNode_OTHERS_byMML( node );
		if( ret_node )
		{
			ret_node->readUnfilteredAttr( node );
		}
	}

	if( ret_node ) ret_node->setPrecedenceAsFirstChild();

	return ret_node;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MERROR( QString message )
{
	CFormulaNode *ret = NULL;
	//FXmlBtn *ptrFXmlBtn = 0;
	//long _toolbar_no = -1, _button_no = -1;

	AST_TXML_Tag *t_mrow = new AST_TXML_Tag( &mtextV, 0, new AST_TXML_Value( &message ) );
	AST_TXML_Compound *_inner = new AST_TXML_Compound();
	_inner->Add( t_mrow );
	AST_TXML_Tag *e_node = new AST_TXML_Tag( &merrorV, 0, _inner );
	
	//ret = InsertMMLFormulaNode_OTHERS( ptrFXmlBtn, e_node );
	ret = InsertMMLFormulaNode_OTHERS_byMML( e_node );

	if( e_node ) delete e_node;
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_OTHERS( FXmlBtn *ptrFXmlBtn, AST_TXML_Compound *node, int is_hidden )
{
	CFormulaNode *new_node = 0;
	CFrameNode *pFrameNode = 0;
	CNode *pCurNode = 0;

	if( !ptrFXmlBtn )
		return NULL; // error: Internal error

	if( ptrFXmlBtn->getNodeType() == NODE_FORMULA )
	{
		new_node = CreateFormulaNode( *ptrFXmlBtn );
		if( !new_node ) return NULL; // error: Internal error

		pFrameNode = (CFrameNode *)GetFrameNode();
		if( pFrameNode ) new_node->SetLevel( pFrameNode->GetLevel() );
		AddChild( new_node );

		if( node )
		{
			pCurNode = new_node->GetFirstChild();
			if( pCurNode && pCurNode->GetType() == NODE_FRAME )
			{
				if( ((CFrameNode*)pCurNode)->GetFirstChild() )
					((CFrameNode*)pCurNode)->DeleteChild( ((CFrameNode*)pCurNode)->GetFirstChild() );
				pCurNode->read_mathml( node );
			}
		}
	}
	if( new_node && is_hidden )
		new_node->setHidden();
	return new_node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_OTHERS( FXmlBtn *ptrFXmlBtn, AST_TXML_Tag *node, int is_hidden )
{
	long cb = 0;
	CFormulaNode *new_node = 0/*, *box_node = 0, *child_node = 0*/;
	CFrameNode *pFrameNode = 0;
	//CLineNode *parent_line = 0;
	CNode *pCurNode = 0;

	if( !ptrFXmlBtn )
		return NULL; // error: Internal error

//	if( !node->getTags() || node->getTags()->getChildNumber() == 0 )
//		return NULL;	//error: Error encountered and repaired: Too few Child node->getTagName() in node

	if( ptrFXmlBtn->getNodeType() == NODE_FORMULA )
	{
		new_node = CreateFormulaNode( *ptrFXmlBtn );
		if( !new_node ) return NULL; // error: Internal error

		AST_TXML_Attribute *colorAttr = node->getAttribute( FBL_ATTR_color );
		QColor grPrimColor = QColor(0, 0, 0);
		if( colorAttr &&
			::mml_mathcolor2color( colorAttr->getStringValue(), grPrimColor, QColor(0, 0, 0) ) == 0 )
		{
			new_node->setLineColor( grPrimColor );
		}

		pFrameNode = (CFrameNode *)GetFrameNode();
		if( pFrameNode ) new_node->SetLevel( pFrameNode->GetLevel() );
		AddChild( new_node );

		if( node->getTags() && node->getTags()->getChildNumber() > 0 )
		{
			if( new_node->GetChildCount() < node->getTags()->getChildNumber() )
			{	//error: Error encountered and repaired: Too many Child node->getTagName() in node
				//delete new_node;
				//return;
				if( xml_strcmp( node->getTagName(), FBL_TAG_merror ) )
				{
					QString mess = _T("Too many children in ") + node->getTagName();
					InsertMMLFormulaNode_MERROR( mess );
				}
			}

			if( new_node->GetChildCount() > node->getTags()->getChildNumber() )
			{	//error: Error encountered and repaired: Too few Child node->getTagName() in node
				//delete new_node;
				//return;
				if( xml_strcmp( node->getTagName(), FBL_TAG_merror ) )
				{
					QString mess = _T("Too few children in ") + node->getTagName();
					InsertMMLFormulaNode_MERROR( mess );
				}
			}

			pCurNode = new_node->GetFirstChild();
			cb = 0;
			while( pCurNode && cb < new_node->GetChildCount() && cb < node->getTags()->getChildNumber() )
			{
				if( pCurNode->GetType() == NODE_FRAME && 
					node->getTags()->getChild( cb ) &&
					node->getTags()->getChild( cb )->getNodeType() == ASTT_TXML_Tag )
				{
					
					if( ((CFrameNode*)pCurNode)->GetFirstChild() )
						((CFrameNode*)pCurNode)->DeleteChild( ((CFrameNode*)pCurNode)->GetFirstChild() );
					pCurNode->read_mathml( (AST_TXML_Tag*)node->getTags()->getChild( cb ) );
				}
				pCurNode = pCurNode->GetNext();
				cb++;
			}
		}
	}
	if( new_node && is_hidden )
		new_node->setHidden();
	return new_node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_OTHERS_byID( const QString& _id, AST_TXML_Tag *node )
{
	FXmlBtn *ptrFXmlBtn = 0;
	long _toolbar_no = -1, _button_no = -1;

	if( ::getCurrentToolBarSet()->getButtonCoordByID( _id, _toolbar_no, _button_no ) == -1 )
	{
		if( xml_strcmp( node->getTagName(), FBL_TAG_merror ) )
		{
			QString mess = _T("Unexpected tag \"") + node->getTagName() + _T("\"");
			InsertMMLFormulaNode_MERROR( mess );
		}
		return NULL;
	}
	ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
	return InsertMMLFormulaNode_OTHERS( ptrFXmlBtn, node );
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_OTHERS_byMML( AST_TXML_Tag *node )
{
	FXmlBtn *ptrFXmlBtn = 0;
	long _toolbar_no = -1, _button_no = -1;

	if( ::getCurrentToolBarSet()->getButtonCoordByMathMLTag( node->getTagName(), _toolbar_no, _button_no ) == -1 )
	{
		if( xml_strcmp( node->getTagName(), FBL_TAG_merror ) )
		{
			QString mess = _T("Unexpected tag \"") + node->getTagName() + _T("\"");
			InsertMMLFormulaNode_MERROR( mess );
		}
		return NULL;
	}
	ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
	return InsertMMLFormulaNode_OTHERS( ptrFXmlBtn, node );
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_OTHERS_byMML( QString mml_name, AST_TXML_Compound *node )
{
	FXmlBtn *ptrFXmlBtn = 0;
	long _toolbar_no = -1, _button_no = -1;

	if( ::getCurrentToolBarSet()->getButtonCoordByMathMLTag( mml_name, _toolbar_no, _button_no ) == -1 )
	{
		if( xml_strcmp( mml_name, FBL_TAG_merror ) )
		{
			QString mess = _T("Unexpected tag \"") + mml_name + _T("\"");
			InsertMMLFormulaNode_MERROR( mess );
		}
		return NULL;
	}
	ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
	return InsertMMLFormulaNode_OTHERS( ptrFXmlBtn, node );
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MTABLE( AST_TXML_Tag *node )
{
	FXmlBtn *ptrFXmlBtn = 0;
	long _toolbar_no = -1, _button_no = -1, cb = 0;
	CFormulaNode *new_node = 0;
	CFrameNode *pFrameNode = 0;
	//CLineNode *parent_line = 0;
	CNode *pCurNode = 0;

	std::vector<long> line_length;
	AST_TXML_Compound *stmt = 0;

	if( ::mml_is_table_tag_correct( node, line_length ) )
	{
		long i, j, nColumn = 0, nRow = (long) line_length.size();
		for( i = 0; i < nRow; i++ )
			if( line_length[ i ] > nColumn ) nColumn = line_length[ i ];
		if( nRow <= 0 && nColumn <= 0 )
			return 0; // error: Internal error

		if( ::getCurrentToolBarSet()->getButtonCoordByID( QString(HF_BTNID_DEFAULT_TABLE_ASK_X_ASK), _toolbar_no, _button_no ) == -1 )
			return 0; // error: Error encountered: HF table formula is absent
		ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
		if( !ptrFXmlBtn )
			return 0; // error: Internal error
		if( ptrFXmlBtn->getNodeType() != NODE_FORMULA )
			return 0; // error: Error encountered: HF table formula has a wrong format
		if( ptrFXmlBtn->isPropertyInput( FBL_ATTR_rows ) )
			ptrFXmlBtn->setRows( nRow );
		if( ptrFXmlBtn->isPropertyInput( FBL_ATTR_columns ) )
			ptrFXmlBtn->setColumns( nColumn );
		new_node = CreateFormulaNode( *ptrFXmlBtn );
		if( !new_node || new_node->GetChildCount() != nRow * nColumn )
			return 0; // error: Internal error
		pFrameNode = (CFrameNode*)GetFrameNode();
		if( pFrameNode ) new_node->SetLevel( pFrameNode->GetLevel() );

		// read attributes
		InsertMMLFormulaNode_MTABLE_readAttr( node, new_node );

		AddChild( new_node );
		pCurNode = new_node->GetFirstChild();
		cb = 0;
		for( i = 0; i < nRow; i++ )
		{
			for( j = 0; j < nColumn; j++ )
			{
				if( !pCurNode ) return 0; // error: Internal error
				if( pCurNode->GetType() == NODE_FRAME )
				{
					stmt = mml_get_table_tag_cell( node, i, j );
					pCurNode->read_mathml( stmt );
					if( ((CFrameNode*)pCurNode)->GetChildCount() > 1 &&
						((CFrameNode*)pCurNode)->GetFirstChild() )
						((CFrameNode*)pCurNode)->DeleteChild( ((CFrameNode*)pCurNode)->GetFirstChild() );
				}
				pCurNode = pCurNode->GetNext();
			}
		}
	}
	return new_node;
}

void CLineNode::InsertMMLFormulaNode_MTABLE_readAttr( AST_TXML_Tag *node, CFormulaNode *new_node )
{
	struct HMathMLAttr mml_attr( _T(""), _T("") );	// not one of the predefined tokens
	mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), node, _T("") /*has no body*/ );
	mml_attr.accountAttr( FBL_ATTR_align );
	mml_attr.accountAttr( FBL_ATTR_columnalign );
	mml_attr.accountAttr( FBL_ATTR_rowalign );
	new_node->CNodeInfo_Property::setUnfilteredAttr( mml_attr );


	AST_TXML_Attribute *tblAttr = 0;
	long i, j, idx;

	tblAttr = node->getAttribute( FBL_ATTR_align );
	if( tblAttr && tblAttr->getStringValue().length() > 0 )
	{
		idx = -1;
		i = 0;
		QString tmp = _T(""), body = tblAttr->getStringValue();
		do
		{
			while( i < body.length() && body.at(i).isSpace() ) i++;
			while( i < body.length() && body.at(i).isLetter() )
			{
				tmp.append( body.at(i) );
				i++;
			}
			if( tmp.length() == 0 ) break;
			while( i < body.length() && body.at(i).isSpace() ) i++;
			if( i >= body.length() ) break;
			if( !(body.at(i).isDigit() || body.at(i) == QChar('-')) ) break;

			LSDigit ldigit;
			long result = ldigit.Analisys( body.midRef(i) );
			if( result != LSDigit::Integer ) break;
			if( ldigit.mnt_int >= 1 && ((long) ldigit.mnt_int) <= new_node->GetNumLine() )
			{
				if( ldigit.asign < 0 )
					idx = (new_node->GetNumLine() - ldigit.mnt_int) * new_node->GetNumColumn();
				else
					idx = (ldigit.mnt_int - 1) * new_node->GetNumColumn();
			}
		}
		while( 0 );

		if( tmp == FBL_VAL_top )
			new_node->setSpecialVerticalCenter( FBtnChildPos::TableTop2Baseline, idx + 1 );
		else if( tmp == FBL_VAL_bottom )
			new_node->setSpecialVerticalCenter( FBtnChildPos::TableBottom2Baseline, idx + 1 );
		else if( tmp == FBL_VAL_center || tmp == FBL_VAL_baseline )
			new_node->setSpecialVerticalCenter( FBtnChildPos::TableCenter2Baseline, idx + 1 );
		else
			new_node->setSpecialVerticalCenter( FBtnChildPos::TableAxis, idx + 1 );
	}

	std::vector<long> vAlign, hAlign;
	long v = TO_BASELINE, h = HORISONTAL_CENTER;
	::bb_read_attr_align_horizontal( node, FBL_ATTR_columnalign, hAlign, h );
	::bb_read_attr_align_vertical( node, FBL_ATTR_rowalign, vAlign, v );

	if( hAlign.size() > 0 )	h = hAlign[ hAlign.size() - 1 ];
	if( vAlign.size() > 0 ) v = vAlign[ vAlign.size() - 1 ];

	vAlign.resize( new_node->GetNumLine(), v );
	hAlign.resize( new_node->GetNumColumn(), h );

	long effV, cellEffV, cellEffH;
	AST_TXML_Tag *mtr = 0, *mtd = 0;
	idx = 0;
	for( i = 0; i < new_node->GetNumLine() && i < (long) vAlign.size(); i++ )
	{
		mtr = ::mml_get_table_tag_mtr( node, i );

		std::vector<long> hRowAlign;
		long rowH = LONG_MAX;
		::bb_read_attr_align_horizontal( mtr, FBL_ATTR_columnalign, hRowAlign, LONG_MAX );
		if( hRowAlign.size() > 0 ) rowH = hRowAlign[ hRowAlign.size() - 1 ];
		hRowAlign.resize( hAlign.size(), rowH );
		for( j = 0; j < (long) hRowAlign.size() && j < (long) hAlign.size(); j++ )
			if( hRowAlign[ j ] == LONG_MAX ) hRowAlign[ j ] = hAlign[ j ];

		effV = LONG_MAX;
		::bb_read_attr_align_vertical( mtr, FBL_ATTR_rowalign, effV );
		if( effV == LONG_MAX ) effV = vAlign[ i ];

		for( j = 0; j < new_node->GetNumColumn() && j < (long) hAlign.size(); j++ )
		{
			mtd = ::mml_get_table_tag_mtd( node, i, j );

			cellEffH = LONG_MAX;
			::bb_read_attr_align_horizontal( mtd, FBL_ATTR_columnalign, cellEffH );
			if( cellEffH == LONG_MAX ) cellEffH = hRowAlign[ j ];

			cellEffV = LONG_MAX;
			::bb_read_attr_align_vertical( mtd, FBL_ATTR_rowalign, cellEffV );
			if( cellEffV == LONG_MAX ) cellEffV = effV;

			Q_ASSERT( cellEffV != LONG_MAX && cellEffH != LONG_MAX );
			new_node->SetChildFrameAlignType( idx, cellEffV | cellEffH );
			idx++;
		}
	}

	std::vector<FBtnGraphics> frameLine;
	RealFde x0 = 0.0;
	RealFde y0 = 0.0;
	RealFde x100 = -1;//HF_BINBUTTON_VERTICAL_SCALE;
	RealFde y100 = -1;//HF_BINBUTTON_VERTICAL_SCALE;

	tblAttr = node->getAttribute( FBL_ATTR_frame );
	if( tblAttr && tblAttr->getStringValue().length() > 0 )
	{
		int penStyle = -1;
		if( tblAttr->getStringValue() == FBL_VAL_solid )
			penStyle = Qt::SolidLine;
		else if ( tblAttr->getStringValue() == FBL_VAL_dashed )
			penStyle = Qt::DashLine;

		if( penStyle != -1 )
		{
			FBtnGraphics oneLine;
			
			RectFde rect1(   PointFde(x0,   y0), PointFde(x100,   y0) );
			RectFde rect2( PointFde(x100,   y0), PointFde(x100, y100) );
			RectFde rect3( PointFde(x100, y100),   PointFde(x0, y100) );
			RectFde rect4(   PointFde(x0, y100),   PointFde(x0,   y0) );
			oneLine.createLine( FBtnGraphics::GRAPH_PRIM_LINE, penStyle, rect1 );
			frameLine.push_back( oneLine );
			oneLine.createLine( FBtnGraphics::GRAPH_PRIM_LINE, penStyle, rect2 );
			frameLine.push_back( oneLine );
			oneLine.createLine( FBtnGraphics::GRAPH_PRIM_LINE, penStyle, rect3 );
			frameLine.push_back( oneLine );
			oneLine.createLine( FBtnGraphics::GRAPH_PRIM_LINE, penStyle, rect4 );
			frameLine.push_back( oneLine );
		}
	}

	RealFde pos;
	std::vector<long> penStyleList;

	if( new_node->GetNumLine() > 1 )
	{
		::bb_read_attr_table_lines( node, FBL_ATTR_rowlines, penStyleList );
		if( penStyleList.size() > 0 )
		{
			penStyleList.resize( new_node->GetNumLine() - 1, penStyleList[ penStyleList.size() - 1 ] );

			FBtnGraphics oneLine;
			for( i = 0; i < (long) penStyleList.size(); i++ )
			{
				RectFde rc( x0, 0.0, x100, 0.0 );
				if( penStyleList[ i ] == -1 ) continue;
				pos = new_node->GetSlot( i * new_node->GetNumColumn() ).GetPecentPosition().bottom();
				pos += new_node->GetSlot( (i + 1) * new_node->GetNumColumn() ).GetPecentPosition().top();
				rc.setTop( pos / 2.0 );
				rc.setBottom( pos / 2.0 );

				oneLine.createLine( FBtnGraphics::GRAPH_PRIM_LINE, penStyleList[ i ], rc );
				frameLine.push_back( oneLine );
			}
		}
	}

	if( new_node->GetNumColumn() > 1 )
	{
		::bb_read_attr_table_lines( node, FBL_ATTR_columnlines, penStyleList );
		if( penStyleList.size() > 0 )
		{
			penStyleList.resize( new_node->GetNumColumn() - 1, penStyleList[ penStyleList.size() - 1 ] );

			FBtnGraphics oneLine;
			for( i = 0; i < (long) penStyleList.size(); i++ )
			{
				RectFde rc( 0.0, y0, 0.0, y100 );
				if( penStyleList[ i ] == -1 ) continue;
				pos = new_node->GetSlot( i ).GetPecentPosition().left();
				pos += new_node->GetSlot( i + 1 ).GetPecentPosition().right();
				rc.setLeft( pos / 2.0 );
				rc.setRight( pos / 2.0 );

				oneLine.createLine( FBtnGraphics::GRAPH_PRIM_LINE, penStyleList[ i ], rc );
				frameLine.push_back( oneLine );
			}
		}
	}

	if( frameLine.size() > 0 )
	{
		CGraphPrimFormula *primList = new CGraphPrimFormula[ frameLine.size() ];
    	new_node->FlushAltGroup();

		for( i = 0; i < (long) frameLine.size(); i++ )
		{
			primList[ i ].Create( frameLine[ i ] );
			new_node->AddAltGroup( frameLine[ i ].altGroup );
		}

		new_node->CreateGraphPrimIndirect( (int) frameLine.size(), primList );
		delete[] primList;
	}
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MFENCED( AST_TXML_Tag *node )
{
	AST_TXML_Attribute *_attr;
	QString attr_open, attr_close, b_attr_separator, attr_separator = _T("");
	long i;

	//=== 'open', 'close' and 'separator' attributes (or their defaults)
	_attr = node->getAttribute( FBL_ATTR_open );
	attr_open = ::formatBlankAsXML( _attr  ? _attr->getStringValue() : FBL_VAL_DEFAULT_mfenced_open  );
	_attr = node->getAttribute( FBL_ATTR_close );
	attr_close = ::formatBlankAsXML( _attr ? _attr->getStringValue() : FBL_VAL_DEFAULT_mfenced_close );
	_attr = node->getAttribute( FBL_ATTR_separators );

	if( node->getTags() && 
		node->getTags()->getChildNumber() == 1 &&
		node->getTags()->getChild( 0 ) &&
		node->getTags()->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
	{
		QString fid = _T("");
		if( ::isXMLSymbol__BracketL( attr_open ) && 
			::isXMLSymbol__BracketR( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_LR;
		else if( ::isXMLSymbol__BracketLB( attr_open ) && 
				 ::isXMLSymbol__BracketRB( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_LRB;
		else if( ::isXMLSymbol__BracketLF( attr_open ) && 
				 ::isXMLSymbol__BracketRF( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_LRF;
		else if( ::isXMLSymbol__BracketLA( attr_open ) && 
				 ::isXMLSymbol__BracketRA( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_LRA;
		else if( ::isXMLSymbol__BracketVerBar( attr_open ) && 
				 ::isXMLSymbol__BracketVerBar( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_ABS;
		else if( ::isXMLSymbol__BracketDblVerBar( attr_open ) && 
				 ::isXMLSymbol__BracketDblVerBar( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_NORM;
		else if( ::isXMLSymbol__BracketLFloor( attr_open ) && 
				 ::isXMLSymbol__BracketRFloor( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_FLOOR;
		else if( ::isXMLSymbol__BracketLCeil( attr_open ) && 
				 ::isXMLSymbol__BracketRCeil( attr_close ) )
			fid = HF_BTNID_DEFAULT_BRACKET_CEIL;
		else if( attr_close.length() == 0 )
		{
			if( ::isXMLSymbol__BracketL( attr_open ) )
				fid = HF_BTNID_DEFAULT_BRACKET_L;
			else if( ::isXMLSymbol__BracketLB( attr_open ) )
				fid = HF_BTNID_DEFAULT_BRACKET_LB;
			else if( ::isXMLSymbol__BracketLF( attr_open ) )
				fid = HF_BTNID_DEFAULT_BRACKET_LF;
			else if( ::isXMLSymbol__BracketLA( attr_open ) )
				fid = HF_BTNID_DEFAULT_BRACKET_LA;
			else if( ::isXMLSymbol__BracketVerBar( attr_open ) )
				fid = HF_BTNID_DEFAULT_BRACKET_ABS_L;
			else if( ::isXMLSymbol__BracketDblVerBar( attr_open ) )
				fid = HF_BTNID_DEFAULT_BRACKET_NORM_L;
		}
		else  if( attr_open.length() == 0 )
		{
			if( ::isXMLSymbol__BracketR( attr_close ) )
				fid = HF_BTNID_DEFAULT_BRACKET_R;
			else if( ::isXMLSymbol__BracketRB( attr_close ) )
				fid = HF_BTNID_DEFAULT_BRACKET_RB;
			else if( ::isXMLSymbol__BracketRF( attr_close ) )
				fid = HF_BTNID_DEFAULT_BRACKET_RF;
			else if( ::isXMLSymbol__BracketRA( attr_close ) )
				fid = HF_BTNID_DEFAULT_BRACKET_RA;
			else if( ::isXMLSymbol__BracketVerBar( attr_close ) )
				fid = HF_BTNID_DEFAULT_BRACKET_ABS_R;
			else if( ::isXMLSymbol__BracketDblVerBar( attr_close ) )
				fid = HF_BTNID_DEFAULT_BRACKET_NORM_R;
		}

		CFormulaNode *r;
		if( fid.length() &&
			(r = InsertMMLFormulaNode_OTHERS_byID( fid, node )) != 0 )
			return r;
	}

	b_attr_separator = _attr ? _attr->getStringValue() : FBL_VAL_DEFAULT_mfenced_separator;
	for( i = 0; i < (long) b_attr_separator.length(); i++ )
		if( !_isspace( b_attr_separator.at(i) ) )
			attr_separator += b_attr_separator.at(i);

	AST_TXML_Compound *_tags = new AST_TXML_Compound();

	AST_TXML_Compound *_a;
	AST_TXML_Value *_v;
	AST_TXML_Compound *_innner_tags = new AST_TXML_Compound();
	AST_TXML_Tag *node_inner_mrow;

	_a = new AST_TXML_Compound();
	_a->Add( new AST_TXML_Attribute( &fenceV, &trueV ) );
	QString sV( attr_open ), s2V( attr_close );
	_v = new AST_TXML_Value( &sV );
	_tags->Add( new AST_TXML_Tag( &moV, _a, _v ) );
	_tags->Add( node_inner_mrow = new AST_TXML_Tag( &mrowV, 0, _innner_tags ) );
	_a = new AST_TXML_Compound();
	_a->Add( new AST_TXML_Attribute( &fenceV, &trueV ) );
	_v = new AST_TXML_Value( &s2V );
	_tags->Add( new AST_TXML_Tag( &moV, _a, _v ) );
	
	std::vector<long> loan;
	if( node->getTags() && node->getTags()->getChildNumber() )
	{
		if( attr_separator.length() && 
			((long) attr_separator.length()) < node->getTags()->getChildNumber() - 1 )
			attr_separator.append( QString( node->getTags()->getChildNumber() - 1 - attr_separator.length(), attr_separator.at(attr_separator.length() - 1) ) );
		for( i = 0; i < node->getTags()->getChildNumber() - 1; i++ )
		{
			loan.push_back( _innner_tags->Add( node->getTags()->getChild( i ) ) );
			if( i < (long) attr_separator.length() )
			{
				_a = new AST_TXML_Compound();
				_a->Add( new AST_TXML_Attribute( &separatorV, &trueV ) );
				QString s3V(1, attr_separator.at(i));
				_v = new AST_TXML_Value( &s3V );
				_innner_tags->Add( new AST_TXML_Tag( &moV, _a, _v ) );
			}
		}
		loan.push_back( _innner_tags->Add( node->getTags()->getChild( i ) ) );
	}

	AST_TXML_Tag *node_mrow = new AST_TXML_Tag( &mrowV, 0, _tags );
	node_mrow->mode.setFlag(OLIMP_MATHML_FLAG__MROW_KEEP1CHILD);
	CFormulaNode *_node = InsertMMLFormulaNode( node_mrow );

	for( i = 0; i < (long) loan.size(); i++ )
		_innner_tags->ReplaceChild( loan[ i ], 0 );
	delete node_mrow;

	return _node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MFRAC( AST_TXML_Tag *node )
{
	long numalign;
	long denomalign;
	unsigned char bevelled = 0;
	RealFde attr_width = LINETHICKNESS_DEFAULT;
	::bb_read_attr_linethickness( node, FBL_ATTR_linethickness, attr_width );

	long v, h;
	v = TO_BOTTOM;
	h = HORISONTAL_CENTER;
	::bb_read_attr_align_horizontal( node, FBL_ATTR_numalign, h );
	numalign = v | h;

	v = TO_TOP;
	h = HORISONTAL_CENTER;
	::bb_read_attr_align_horizontal( node, FBL_ATTR_denomalign, h );
	denomalign = v | h;

	AST_TXML_Attribute *_attr = node->getAttribute( FBL_ATTR_bevelled );
	if( _attr && 
		_attr->getValueType() == ASTVALT_TXML_String &&
		!xml_strcmp( _attr->getStringValue(), FBL_VAL_true ) )
		bevelled = 1;

	CFormulaNode* mfrac_node = bevelled ?
		InsertMMLFormulaNode_OTHERS_byID( HF_BTNID_DEFAULT_BEVELLED_MFRAC, node ) :
		InsertMMLFormulaNode_OTHERS_byMML( node );
	if( mfrac_node )
	{
		mfrac_node->SetChildFrameAlignType( 0, numalign );
		mfrac_node->SetChildFrameAlignType( 1, denomalign );
		mfrac_node->setLinethickness( 0, attr_width );

		struct HMathMLAttr mml_attr(_T( ""), _T("") );	// not one of the predefined tokens
		mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), node, _T("") /*has no body*/ );
		mml_attr.accountAttr( FBL_ATTR_linethickness );
		mml_attr.accountAttr( FBL_ATTR_numalign );
		mml_attr.accountAttr( FBL_ATTR_denomalign );
		mfrac_node->CNodeInfo_Property::setUnfilteredAttr( mml_attr );
	}
	return mfrac_node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MSTYLE( AST_TXML_Tag *node )
{
	const AST_TXML_Compound* _attr = node->getAttributes();
	AST_TXML_Compound* _Child = node->getTags();
	if( !_Child ) return 0;
	if( _attr && _attr->getChildNumber() )
	{
		static const int mstyleOwn_Length = 12;
		static QString mstyleOwn[ mstyleOwn_Length ] = 
		{
			_T("scriptlevel"),
			_T("displaystyle"),
			_T("scriptsizemultiplier"),
			_T("scriptminsize"),
			_T("background"),
			_T("veryverythinmathspace"),
			_T("verythinmathspace"),
			_T("thinmathspace"),
			_T("mediummathspace"),
			_T("thickmathspace"),
			_T("verythickmathspace"),
			_T("veryverythickmathspace")
		};
		std::map<QString, int> map_isMStyleOwn;
		long i;
		for( i = 0; i < mstyleOwn_Length; i++ )
			map_isMStyleOwn[ mstyleOwn[ i ] ] = 1;

		for( i = 0; i < _Child->getChildNumber(); i++ )
		{
			if( _Child->getChild( i ) &&
				_Child->getChild( i )->getNodeType() == ASTT_TXML_Tag )
				InsertMMLFormulaNode_MSTYLE_recursive( 
					(AST_TXML_Tag*)_Child->getChild( i ), _attr, map_isMStyleOwn
				);
		}
	}

	if( _Child->getChildNumber() == 1 && _Child->getChild( 0 ) &&
		_Child->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
	{
		AST_TXML_Tag* _msp = (AST_TXML_Tag*)(_Child->getChild( 0 ));
		if( _msp->getTagName() == FBL_TAG_mspace )
		{
			AST_TXML_Attribute *_background = node->getAttribute( FBL_ATTR_background );
			if( _background && _msp->getAttribute( FBL_ATTR_background ) == 0 )
			{
				AST_TXML_Compound *_msp_attr = _msp->getAttributes();
				if( _msp_attr == 0 ) _msp_attr = new AST_TXML_Compound();
				_msp_attr->Add( _background->Clone() );
				_msp->setAttributes( _msp_attr );
			}
			// mspace: 'mstyle' is ignored and deal with contents only
			read_mathml( _msp );
			return 0;
		}
	}

	QString oldname = node->getTagName();
	node->setTagName( FBL_TAG_mrow );
	CFormulaNode* new_node = InsertMMLFormulaNode_MROW( node );
	node->setTagName( oldname );
	if( !new_node ) return new_node;

	struct HMathMLAttr mml_attr( _T(""), _T("") );	// not one of the predefined tokens
	mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), node, _T("") /*has no body*/ );
	new_node->setColorAttr( mml_attr );
	new_node->to_mathml_data.reserved = new_node->to_mathml_data.name;
	new_node->to_mathml_data.name = node->getTagName();

	return new_node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MSTYLE_recursive( AST_TXML_Tag *node, const AST_TXML_Compound* _new_attr, std::map<QString, int>& map_isMStyleOwn )
{
	if( !node->getAttributes() )
		node->setAttributes( new AST_TXML_Compound() );

	long i;
	QString newname;
	for( i = 0; i < _new_attr->getChildNumber(); i++ )
	{
		if( _new_attr->stmt.at(i) == NULL || 
			_new_attr->stmt.at(i)->getNodeType() != ASTT_TXML_Attribute )
			continue;
		newname = ((AST_TXML_Attribute*)(_new_attr->stmt[ i ]))->getAttributeName();

		// reserved for MSTYLE, existing or non-supporting attribute
		if( map_isMStyleOwn.find( newname ) != map_isMStyleOwn.end() ||
			node->getAttributes()->getFirstAttribute( newname ) != NULL ||
			!::getFullMathMLAttrList().isSupportedTag2Attr( node->getTagName(), newname ) )
			continue;

		// attribute with this name is already exists
		if( ::mml_has_equivalent_attr( node->getAttributes(), newname ) )
			continue;
		node->getAttributes()->stmt.Add( _new_attr->stmt[ i ]->Clone() );
	}

	AST_TXML_Compound* _Child = node->getTags();
	if( !_Child ) return 0;
	for( i = 0; i < _Child->getChildNumber(); i++ )
	{
		if( _Child->getChild( i ) && _Child->getChild( i )->getNodeType() == ASTT_TXML_Tag )
			InsertMMLFormulaNode_MSTYLE_recursive( (AST_TXML_Tag*)_Child->getChild( i ), _new_attr, map_isMStyleOwn );
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MACTION( AST_TXML_Tag *node )
{
	QString oldname = node->getTagName();
	node->setTagName( FBL_TAG_mrow );
	CFormulaNode* new_node = InsertMMLFormulaNode_MROW( node );
	node->setTagName( oldname );
	return new_node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MPHANTOM( AST_TXML_Tag *node )
{
	AST_TXML_Compound* _Child = node->getTags();
	if( !_Child ) return 0;
	return InsertMMLFormulaNode_HIDDEN_EMPTY_SLOT( node );
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_HIDDEN_EMPTY_SLOT( AST_TXML_Tag *node )
{
	FXmlBtn *ptrFXmlBtn = 0;
	long _toolbar_no = -1, _button_no = -1;

	//static QString tagname = FBL_TAG_mrow;
	if( ::getCurrentToolBarSet()->getButtonCoordByMathMLTag( node->getTagName(), _toolbar_no, _button_no ) == -1 )
		return 0;
	ptrFXmlBtn = ::getCurrentToolBarSet()->
	  getButtonCalc( _toolbar_no, _button_no );
	CFormulaNode *new_node = InsertMMLFormulaNode_OTHERS( ptrFXmlBtn, node, 1/*hidden*/ );

	if( new_node )
	{
		struct HMathMLAttr mml_attr(_T( ""), _T("") );
		mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), node, _T("") /*has no body*/ );
		new_node->CNodeInfo_Property::setUnfilteredAttr( mml_attr );
		new_node->CNodeInfo_Property::setColorAttr( mml_attr );
	}

	return new_node;
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MROW( AST_TXML_Tag *node )
{
	AST_TXML_Compound *node_tags = node->getTags();

	/*if( node_tags == NULL || node_tags->getChildNumber() == 0 )
		return;*/

	CFormulaNode *new_node = InsertMMLFormulaNode_OTHERS_byMML( QString(FBL_TAG_mrow), node_tags );

	if( new_node )
	{
		struct HMathMLAttr mml_attr(_T( ""), _T("") );
		mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), node, _T("") /*has no body*/ );
		new_node->CNodeInfo_Property::setUnfilteredAttr( mml_attr );
		if( node->getFlags().isSetFlag(OLIMP_MATHML_FLAG__MROW_CONTENT_AUTODETECT) )
			new_node->setAutodetectContent();
	}

	return new_node;
}

void getMMultiscriptsAttr( long nChildCoded, QString& buttonID, long& base, long& sw, long& nw, long& se, long& ne )
{
	static QString IDS[ 16 ] =
	{
		_T("@empty_frame"),				// 0
		_T("@sup"),                     // 1
		_T("@sub"),                     // 2
		_T("@subsup"),                  // 3
		_T("@presup"),                  // 4
		_T("@suppresup"),               // 5
		_T("@sub_presup"),              // 6
		_T("@supsub_presup"),           // 7
		_T("@presub"),                  // 8
		_T("@sup_presub"),              // 9
		_T("@subpresub"),               // 10
		_T("@supsub_presub"),           // 11
		_T("@presubsup"),               // 12
		_T("@sup_presupsub"),           // 13
		_T("@sub_presupsub"),           // 14
		_T("@suppresup_subpresub")      // 15
	};
	long btnSW[ 16 ]   = {  0, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0 };
	long btnNW[ 16 ]   = { -1, -1, -1, -1,  0,  0,  0,  0, -1, -1, -1, -1,  1,  1,  1,  1 };
	long btnBASE[ 16 ] = { -1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2 };
	long btnSE[ 16 ]   = { -1, -1,  1,  1, -1, -1,  2,  2, -1, -1,  2,  2, -1, -1,  3,  3 };
	long btnNE[ 16 ]   = { -1,  1, -1,  2, -1,  2, -1,  3, -1,  2, -1,  3, -1,  3, -1,  4 };

	buttonID = IDS[ nChildCoded];
	base = btnBASE[ nChildCoded ];
	sw = btnSW[ nChildCoded ];
	nw = btnNW[ nChildCoded ];
	se = btnSE[ nChildCoded];
	ne = btnNE[ nChildCoded];
}

CFormulaNode* CLineNode::InsertMMLFormulaNode_MMULTISCRIPTS( AST_TXML_Tag *xml_node )
{
	if( !xml_node->getTags() || !xml_node->isExistTagChild() )
		return 0;
	xml_node->deleteValueChildren();

	CLineNode *baseLine = new CLineNode();
	baseLine->read_mathml( xml_node->getTagChild( 0 ) );

	CLineNode* east[ 2 ] = {new CLineNode(), new CLineNode()};	// sub sup
	CLineNode* west[ 2 ] = {new CLineNode(), new CLineNode()};	// sub sup
	long east_cb[ 2 ] = {0, 0};
	long west_cb[ 2 ] = {0, 0};
	CFormulaNode *frmNode = 0;
	MMLContent_TT_mrow item2insert;
	AST_TXML_Tag *spaceTag = new AST_TXML_Tag( &mspaceV, 0, new AST_TXML_Value( &blankV ) );
	AST_TXML_Tag *_n1 = 0, *_n2 = 0;

	CLineNode** toread = east;
	long *toread_cb = east_cb;
	for( long j = 1; j + 1 < xml_node->getChildNumber(); j++ )
	{
		if( (_n1 = xml_node->getTagChild( j )) == 0 ) continue;
		if( _n1->getTagName() == FBL_TAG_mprescripts )
		{
			if( toread == east ) 
			{
				toread = west;
				toread_cb = west_cb;
			}
			continue;
		}
		if( (_n2 = xml_node->getTagChild( j + 1 )) == 0 ) { j++; continue; }

		if( _n1->getTagName() != FBL_TAG_none )
		{
			item2insert.set( _n1 );
			frmNode = toread[ 0 ]->InsertMMLFormulaNode( item2insert.getRoot() );
			if( frmNode ) frmNode->clearRWMMLSupport();
			toread_cb[ 0 ]++;
		}
		else
		{
			item2insert.set( spaceTag );
			frmNode = toread[ 0 ]->InsertMMLFormulaNode( item2insert.getRoot() );
			if( frmNode )
			{
				frmNode->iterateNodeTree( setRecursiveReadOnly, 0 );
				frmNode->to_mathml_data.name = FBL_TAG_none;
			}
		}
		item2insert.reset();

		if( _n2->getTagName() != FBL_TAG_none )
		{
			item2insert.set( _n2 );
			frmNode = toread[ 1 ]->InsertMMLFormulaNode( item2insert.getRoot() );
			if( frmNode ) frmNode->clearRWMMLSupport();
			toread_cb[ 1 ]++;
		}
		else
		{
			item2insert.set( spaceTag );
			frmNode = toread[ 1 ]->InsertMMLFormulaNode( item2insert.getRoot() );
			if( frmNode )
			{
				frmNode->iterateNodeTree( setRecursiveReadOnly, 0 );
				frmNode->to_mathml_data.name = FBL_TAG_none;
			}
		}
		item2insert.reset();

		j++;
	}
	delete spaceTag;

	long buttonIDIdx = 0;
	if( west_cb[ 0 ] > 0 && west[ 0 ]->GetChildCount() > 0 ) buttonIDIdx |= 8;
	if( west_cb[ 1 ] > 0 && west[ 1 ]->GetChildCount() > 0 ) buttonIDIdx |= 4;
	if( east_cb[ 0 ] > 0 && east[ 0 ]->GetChildCount() > 0 ) buttonIDIdx |= 2;
	if( east_cb[ 1 ] > 0 && east[ 1 ]->GetChildCount() > 0 ) buttonIDIdx |= 1;

	try
	{
		frmNode = 0;
		QString buttonID;
		long base = -1, sw = -1, nw = -1, se = -1, ne = -1;
		long nChildCoded = buttonIDIdx & 0xF;
		::getMMultiscriptsAttr( nChildCoded, buttonID, base, sw, nw, se, ne );

		FXmlBtn *ptrFXmlBtn = 0;
		long _toolbar_no = -1, _button_no = -1;

		if( ::getCurrentToolBarSet()->getButtonCoordByID( buttonID, _toolbar_no, _button_no ) == -1 ) throw -1;
		ptrFXmlBtn = ::getCurrentToolBarSet()->
			getButtonCalc( _toolbar_no, _button_no );
		if( !ptrFXmlBtn || ptrFXmlBtn->getNodeType() != NODE_FORMULA ) throw -1;
		if( (frmNode = CreateFormulaNode( *ptrFXmlBtn )) == 0 ) throw -1;
		CFrameNode *pFrameNode = (CFrameNode *)GetFrameNode();
		if( pFrameNode ) frmNode->SetLevel( pFrameNode->GetLevel() );
		AddChild( frmNode );

		CLineNode *childLineNode = 0;
		if( sw != -1 && west[ 0 ]->GetChildCount() > 0 )
		{
			if( (childLineNode = frmNode->getFormulaLineChild( sw, 0 )) != 0 )
				childLineNode->PumpLine2Line( west[ 0 ] );
		}
		if( nw != -1 && west[ 1 ]->GetChildCount() > 0 )
		{
			if( (childLineNode = frmNode->getFormulaLineChild( nw, 0 )) != 0 )
				childLineNode->PumpLine2Line( west[ 1 ] );
		}
		if( se != -1 && east[ 0 ]->GetChildCount() > 0 )
		{
			if( (childLineNode = frmNode->getFormulaLineChild( se, 0 )) != 0 )
				childLineNode->PumpLine2Line( east[ 0 ] );
		}
		if( ne != -1 && east[ 1 ]->GetChildCount() > 0 )
		{
			if( (childLineNode = frmNode->getFormulaLineChild( ne, 0 )) != 0 )
				childLineNode->PumpLine2Line( east[ 1 ] );
		}
		if( base != -1 && baseLine->GetChildCount() > 0 )
		{
			if( (childLineNode = frmNode->getFormulaLineChild( base, 0 )) != 0 )
				childLineNode->PumpLine2Line( baseLine );
		}

		frmNode->to_mathml_data.name = FBL_TAG_mmultiscripts;
		frmNode->to_mathml_data.type = QString("1").arg(nChildCoded);
	}
	catch(...)
	{
	}

	delete baseLine;
	delete east[ 0 ];
	delete east[ 1 ];
	delete west[ 0 ];
	delete west[ 1 ];
	return frmNode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MENCLOSE( AST_TXML_Tag *xml_node )
{
	FXmlBtn *ptrFXmlBtn = 0;
	long _toolbar_no = -1, _button_no = -1;

	AST_TXML_Attribute *_attr = xml_node->getAttribute( FBL_ATTR_notation );
	QString tagtype = _attr ? _attr->getStringValue() : FBL_VAL_longdiv;

	if( ::getCurrentToolBarSet()->getButtonCoordByMathMLNameType( xml_node->getTagName(), tagtype, _toolbar_no, _button_no ) == -1 )
		return InsertMMLFormulaNode_MROW( xml_node );

	ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
	return InsertMMLFormulaNode_OTHERS( ptrFXmlBtn, xml_node );
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLFormulaNode_MPADDED( AST_TXML_Tag *node )
{
	CFormulaNode* frmNode = InsertMMLFormulaNode_MROW( node );
	if( !frmNode ) return frmNode;
	frmNode->to_mathml_data.reserved = frmNode->to_mathml_data.name;
	frmNode->to_mathml_data.name = node->getTagName();
	frmNode->setSpecialVerticalCenter( FBtnChildPos::MSPACE_Exact_HeightDepth, 1 );
	return frmNode;
}

///////////////////////////////////////////////////////////////////////////////
