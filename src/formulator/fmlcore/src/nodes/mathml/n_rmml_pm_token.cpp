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
#include "../../dictionary/op_map.h"
#include "../../dictionary/entity_map.h"
#include "../../mathml/mml_style.h"
#include "n_cm_treetemplate.h"
#include "../node.h"
#include "../extcontent/ext_node.h"
#include "../extcontent/extc_classid.h"

///////////////////////////////////////////////////////////////////////////////
// Returns: not NULL - only when a linebreak is needed after the returned node.
///////////////////////////////////////////////////////////////////////////////
CNode* CLineNode::InsertMMLTokenNode( AST_TXML_Tag *node, std::vector<CPlaneText*> *nodes_dump )
{
	if( !node ) return 0;

	CNode *nodeLinebreak = 0;
	FS_LogFont_Color dest_lfs, to_lfs;
	RealFde dest_pointsize = 0.0;

	long fstyle;
	QString t, body, rbody /*recursive body*/;
	std::vector<QString> dest_str;
	std::vector<long> dest_style;
	int mv = 0;	// mathvariant
	int mc = 0;	// mathcolor
	int ms = 0;	// mathsize
	int is_set_secondary_style = 0, is_set_mglyph = 0, is_set_mspace = 0;
	long j;
	long _toolbar_no = -1, _button_no = -1;
	FXmlBtn *ptrFXmlBtn = 0;

	QString op_text = _T("");
	long op_style = FSTYLE_NONE;

	node->formatBlank();
	body = node->getText();
	struct HMathMLAttr mml_attr( node->getTagName(), body );
   	CNodeExInfo ex_ni;
	CNodeInfo ni;
	if( ::mml_is_mglyph_tag( node->getTagName() ) )
	{
		AST_TXML_Attribute *_fontfamily = node->getAttribute( FBL_ATTR_fontfamily );
		AST_TXML_Attribute *_index = node->getAttribute( FBL_ATTR_index );
		AST_TXML_Attribute *_alt = node->getAttribute( FBL_ATTR_alt );
		if( _fontfamily && _index && _alt && 
			_fontfamily->getValueType() == ASTVALT_TXML_String &&
			_alt->getValueType() == ASTVALT_TXML_String &&
			(_index->getValueType() == ASTVALT_TXML_Int ||
			 _index->convert2Integer() == ELH_NO_ERROR ) )
		{
			FS_LogFont_Color default_lfs;
			default_lfs.m_name = _fontfamily->getStringValue();
			default_lfs.m_hint = QFont::Times;
			default_lfs.m_isFixed = false;
			default_lfs.m_isBold = false;
			default_lfs.m_isItalic = false;
			default_lfs.m_color = QColor( 0, 0, 0 );

			op_text = QString(QChar((int) _index->getIntValue()));
			op_style = ::getCurrentFormulatorStyle().getStyleNo( default_lfs );
			if( op_style == -1 )
				op_style = ::getCurrentFormulatorStyle().addStyle( default_lfs );
			is_set_mglyph = 1;
		}
		else
		{
		}
	}
	else
	{
		HMathOpAttr *opinfo = ::mml_is_token_op_tag( node->getTagName() ) ? ::getOperatorEntryByMML( body ) : 0;
		if( opinfo ) mml_attr.setMODefault( *opinfo );

		mml_attr.set( ::getCurrentFormulatorStyle().getUnit2PX(), node, body );

		ni.setMathMLAttr( mml_attr );
		if( ::mml_is_token_string_tag( node->getTagName() ) )
			body =	mml_attr.getFilteredAttr( FBL_ATTR_lquote ) + 
					::hstr_escape( body ) + 
					mml_attr.getFilteredAttr( FBL_ATTR_rquote );
		else if( ::mml_is_token_space_tag( node->getTagName() ) )
		{
			is_set_mspace = 1;
			op_text = HF_PLANETEXT_HUB_SYMBOL;
			op_style = FSTYLE_OP;
			ni.setCannotDrawText();
		}

		CNodeInfo _info;	// ignoring node info
		if( opinfo && opinfo->dsrname.length() )
		{
			if( ::getCurrentToolBarSet()->getTextButtonCoordByID( opinfo->dsrname, _toolbar_no, _button_no ) != -1 )
			{
				ptrFXmlBtn = ::getCurrentToolBarSet()->
				  getButtonCalc( _toolbar_no, _button_no );
				if( ptrFXmlBtn )
				{
					if( ptrFXmlBtn->getNodeType() == NODE_PLANETEXT )
					{
						int ret = CNode::UpdateTextNode( *ptrFXmlBtn, op_text, op_style, _info, ex_ni );
						if( ret != -1 )
						{
							is_set_secondary_style = 1;
							if( _info.isUnicode() )
								ni.setUnicode();
							else
								ni.clearUnicode();

							if( _info.isCannotDrawText() )
								ni.setCannotDrawText();

							ni.setNodeInfo_MathMLFormat( _info );
							ni.setNodeInfo_Alignment( _info );
						}
					}
				}
			}
		}
	}

	if( !is_set_secondary_style && !is_set_mglyph && !is_set_mspace )
	{
		// set secondary (by tag & body) HF style
		if( !::mml_is_token_tag_correct( node ) )
		{
			rbody = node->getDeepChildValueText();
			if( rbody == body )
				::getMathMLChar2Style( dest_str, dest_style, body, node->getTagName(), __GLOBAL_WORK_MODE );
			else
			{
				dest_str.clear();
				dest_style.clear();
				fstyle = FS_TextStyles::getMathMLTag2Style( rbody, node->getTagName() );
				dest_str.push_back( rbody );
				dest_style.push_back( fstyle );
			}
		}
		else
			::getMathMLChar2Style( dest_str, dest_style, body, node->getTagName(), __GLOBAL_WORK_MODE );
	}
	else
	{
		dest_str.clear();
		dest_style.clear();
		dest_str.push_back( op_text );
		dest_style.push_back( op_style );
	}

	if( dest_str.size() != dest_style.size() )
	{
		Q_ASSERT( 0 );
		return 0;
	}
	
	int nodeinfo_was_set;
	for( j = 0; j < (long) dest_str.size(); j++ )
	{
		CNodeInfo _save_ni( ni );

		nodeinfo_was_set = 0;
		CPlaneText *pTextNode = new CPlaneText();

		if( dest_str[ j ].length() == 0 && dest_style[ j ] != FSTYLE_TEXT_BUTTON )
		{
			ni = _save_ni;
			continue;
		}

		if( dest_style[ j ] == FSTYLE_TEXT_BUTTON )
		{
			ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( dest_str[ j ] );
			if( ptrFXmlBtn && ptrFXmlBtn->getNodeType() == NODE_PLANETEXT )
			{
				CNodeInfo _info;
				CNodeExInfo _ex_ni;
				int ret = CNode::UpdateTextNode( *ptrFXmlBtn, op_text, op_style, _info, _ex_ni );
				if( ret != -1 )
				{
					dest_str[ j ] = op_text;
					dest_style[ j ] = op_style;

					if( _info.isUnicode() )
						ni.setUnicode();
					else
						ni.clearUnicode();

					if( _info.isCannotDrawText() )
						ni.setCannotDrawText();

					ni.setNodeInfo_MathMLFormat( _info );
					if( ni.to_mathml_data.value.length() && ni.to_mathml_data.type.length() )
					{
						ni.to_mathml_data.type = node->getTagName();
					}
					ni.setNodeInfo_Alignment( _info );

					pTextNode->setNodeInfo( ni );
					pTextNode->setNodeExInfo( _ex_ni );
					nodeinfo_was_set = 1;
				}
			}
		}

		if( !nodeinfo_was_set )
		{
			pTextNode->setNodeInfo( ni );
			pTextNode->setNodeExInfo( ex_ni );
		}

		if( dest_style[ j ] == FSTYLE_UNICODE )
		{
			pTextNode->setUnicode();
			dest_style[ j ] = fstyle = FSTYLE_UNICODE_DEFAULT;
		}
		else if( dest_style[ j ] == FSTYLE_TEXT_BUTTON )
		{
			ni = _save_ni;
			Q_ASSERT( 0 );
			delete pTextNode;
			continue;
		}

		mv = ::mml_mathvariant2font( ::isMathMLStylePriority(), dest_style[ j ], ::getCurrentFormulatorStyle().getAllFontStyles(), mml_attr, dest_lfs );
		QColor default_color = ::isMathMLStylePriority() ? QColor( 0, 0, 0 ) : ::getCurrentFormulatorStyle().getFontStyle( dest_style[ j ] ).m_color;
		mc = ::mml_mathcolor2color( mml_attr.getFilteredAttr( FBL_ATTR_mathcolor ), dest_lfs.m_color, default_color );

		to_lfs = ::getCurrentFormulatorStyle().getFontStyle( dest_style[ j ] );
		to_lfs.m_color = (mc != -1 ? dest_lfs.m_color : ::getCurrentFormulatorStyle().getFontStyle( dest_style[ j ] ).m_color );
		if( mv != -1 )
		{
			long onlydue2fontfamily = 0;
			if( !is_set_secondary_style ||
				(is_set_secondary_style && ::mml_mathvariant2font( node, &onlydue2fontfamily ) != -1) )
			{
				to_lfs.m_isFixed = dest_lfs.m_isFixed;
				to_lfs.m_hint = dest_lfs.m_hint;
				if( !onlydue2fontfamily )
				{
					to_lfs.m_isBold = dest_lfs.m_isBold;
					to_lfs.m_isItalic = dest_lfs.m_isItalic;
				}
			}
			if( !is_set_mglyph && 
				(!::getCurrentFormulatorStyle().isFaceNameCriticalDrawing( dest_style[ j ] ) || !::isMathMLStylePriority()/*2DO new option?*/) )
			{
				to_lfs.m_name = dest_lfs.m_name;
			}
		}
		if( mv != -1 || mc != -1 )
		{
			fstyle = ::getCurrentFormulatorStyle().getStyleNo( to_lfs, dest_style[ j ] );
			if( fstyle == -1 )
			{
				fstyle = ::getCurrentFormulatorStyle().addStyle( to_lfs );
			}
		}
		else
			fstyle = dest_style[ j ];

		pTextNode->setFStyle( fstyle, ::getCurrentFormulatorStyle().getFontStyle( fstyle ) );

		if( is_set_mspace )
		{
			AST_TXML_Attribute *_image_path = node->getAttribute( FBL_ATTR_image_path );
			int image_fault = 1;
			if( _image_path && pTextNode )
			{
				void *pData = ::PrepareExtNodeData( EXTNODECONTENT_ID_IMG, _image_path->getStringValue(), NULL );
				if( pData )
				{
					delete pTextNode;
					pTextNode = ::CreateExtNode( EXTNODECONTENT_ID_IMG, pData );
					image_fault = 0;
				}
			}
			if( image_fault )
			{
				fstyle = op_style;
				pTextNode->setMathSpace( ni );

				AST_TXML_Attribute *_linebreak = 0;
				static QString mspaceDimAttr[ 3 ] = { FBL_ATTR_width, FBL_ATTR_height, FBL_ATTR_depth };
				for( long icb = 0; icb < 3 && _linebreak == 0; icb++ )
					_linebreak = node->getAttribute( mspaceDimAttr[ icb ] );
				if( _linebreak == 0 &&
					(_linebreak = node->getAttribute( FBL_ATTR_linebreak )) != 0 )
				{
					QString value = _linebreak->getStringValue();
					if( value == FBL_VAL_newline || value == FBL_VAL_indentingnewline || value == FBL_VAL_goodbreak )
						nodeLinebreak = pTextNode;
				}
			}
		}

		if( pTextNode->isUnicode() )
		{
			pTextNode->to_mathml_data.name = node->getTagName();
			pTextNode->CopyText( dest_str[ j ], fstyle, ::getCurrentFormulatorStyle().getFontStyle( fstyle ), 1 );
		}
		else
		{
			pTextNode->CopyText( dest_str[ j ], fstyle, ::getCurrentFormulatorStyle().getFontStyle( fstyle ), 0 );
		}

		CFrameNode *pFrameNode = (CFrameNode*) GetFrameNode();
		if( !pFrameNode )
			pTextNode->SetLevel( FTEXT_HEIGHT_FRAME );
		else
			pTextNode->SetLevel( pFrameNode->GetLevel() );

		dest_pointsize = ::getCurrentFormulatorStyle().getKegl( pTextNode->GetLevel() );
		ms = ::mml_mathsize2pointsize( 
				::getCurrentFormulatorStyle().getUnit2PX(), 
				mml_attr.getFilteredAttr( FBL_ATTR_mathsize ), dest_pointsize );
		if( ms == -1 )
			dest_pointsize = ::getCurrentFormulatorStyle().getKegl( pTextNode->GetLevel() );
		else if( ms == 1 )
		{
			if( dest_pointsize == (RealFde) MML_MATHSIZE_SMALL )
				dest_pointsize = ::getCurrentFormulatorStyle().getKegl( FTEXT_HEIGHT_CHILD1 );
			else if( dest_pointsize == (RealFde) MML_MATHSIZE_NORMAL )
				dest_pointsize = ::getCurrentFormulatorStyle().getKegl( FTEXT_HEIGHT_FRAME );
			else if( dest_pointsize == (RealFde) MML_MATHSIZE_BIG )
				dest_pointsize = ::getCurrentFormulatorStyle().getKegl( FTEXT_HEIGHT_LARGEOP );
			else
				dest_pointsize = NODE_PLANETEXT__POINT_SIZE_DEFAULT;
		}
		pTextNode->setTextKeglSize( dest_pointsize );

		AddChild( pTextNode );
		if( nodes_dump )
			nodes_dump->push_back( pTextNode );

		ni = _save_ni;
	}

	return nodeLinebreak;
}

///////////////////////////////////////////////////////////////////////////////
