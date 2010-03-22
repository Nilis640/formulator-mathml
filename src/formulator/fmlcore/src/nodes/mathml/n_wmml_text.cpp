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

#include "HUtils/imatchtemplate.h"
#include "../../dictionary/op_map.h"
#include "../../dictionary/entity_map.h"
#include "../../mathml/la_mmlchar.h"
#include "n_cm_treetemplate.h"
#include "../rules/appstyle.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

QString CPlaneText::to_mathml__get_attribute_str( QString& _tag, QString& _body, int *isDefaultMMLChanged )
{
	if( isDefaultMMLChanged ) *isDefaultMMLChanged = 0;

	QString new_attr = _T(""), tmp = _T("");

	struct HMathMLAttr mml_attr( _tag, _body );
	HMathOpAttr *opinfo = ::mml_is_token_op_tag( _tag ) ? ::getOperatorEntryByMML( _body ) : 0;
	if( opinfo ) mml_attr.setMODefault( *opinfo );

	struct HMathMLAttr mml_attr_by_this_node( getMathMLAttr() );

	FS_LogFont_Color current_lfs;
	long st = (getFStyle() == FSTYLE_NONE ? FSTYLE_DEFAULT : getFStyle());
	int wouldChangeStyle = 0;//!::isMathMLStylePriority() && ::wouldChangeStyleOnMathStyleAutodetect( this );
	if( st == FSTYLE_OTHER_STYLE )
	{
		if( isDefaultMMLChanged ) *isDefaultMMLChanged = 1;
		current_lfs = getOtherStyleFont();
	}
	else
	{
		current_lfs = ::getCurrentFormulatorStyle().getFontStyle( st );

		if( !::isMathMLStylePriority() )
		{
			FXmlBtn *btn = getCalc();
			if( btn )
			{
				if( btn->getTextStyle() != st )
					wouldChangeStyle = 1;
			}
			else
			{
				std::vector<QString> dest_str;
				std::vector<long> dest_style;
				::getMathMLChar2Style( dest_str, dest_style, _body, _tag, __GLOBAL_WORK_MODE );
				if( dest_style.size() > 1 )
					wouldChangeStyle = 1;
				else if( dest_style.size() == 1 && dest_style.at(0) != st )
					wouldChangeStyle = 1;
			}
		}
	}

	int ret;
	FS_LogFont_Color def_lfs;

	ret = ::mml_mathvariant2font( ::isMathMLStylePriority(), st, ::getCurrentFormulatorStyle().getAllFontStyles(), mml_attr, def_lfs );
	if( ret == -1 || 
		def_lfs.m_name != current_lfs.m_name || 
		def_lfs.m_isFixed != current_lfs.m_isFixed || 
		//def_lfs.m_hint != current_lfs.m_hint || 
		def_lfs.m_isBold != current_lfs.m_isBold || 
		def_lfs.m_isItalic != current_lfs.m_isItalic ||
		wouldChangeStyle )
	{
		tmp = _T("");
		::mml_getStyle2TextAttrFont( tmp, current_lfs, _tag, _body, getMathMLAttr(), (st == FSTYLE_OTHER_STYLE) || wouldChangeStyle );
		new_attr += tmp;
	}

	QColor default_color = ::isMathMLStylePriority() ? QColor( 0, 0, 0 ) : ::getCurrentFormulatorStyle().getFontStyle( st ).m_color;
	ret = ::mml_mathcolor2color( mml_attr.getFilteredAttr( FBL_ATTR_mathcolor ), def_lfs.m_color, default_color );
	if( ret == 0 || def_lfs.m_color != current_lfs.m_color )
	{
		tmp = _T("");
		::mml_getStyle2TextAttrColor( tmp, current_lfs.m_color, _tag, _body );
		new_attr += tmp; 
		if( isDefaultMMLChanged ) *isDefaultMMLChanged = 1;
	}

	if( mml_attr_by_this_node.getFilteredAttr( FBL_ATTR_mathbackground, tmp ) == 0 )
	{
		new_attr += MAKE_MATHML_ATTR( FBL_ATTR_mathbackground, tmp );
		if( isDefaultMMLChanged ) *isDefaultMMLChanged = 1;
	}

	if( getTextKeglSize() != ::getKegl( GetLevel() ) )
	{
		tmp = _T("");

		RealFde kegl = getTextKeglSize();
		if( kegl == ::getCurrentFormulatorStyle().getKegl( FTEXT_HEIGHT_CHILD1 ) )
			tmp = MAKE_MATHML_ATTR( FBL_ATTR_mathsize, FBL_VAL_small );
		else if( kegl == ::getCurrentFormulatorStyle().getKegl( FTEXT_HEIGHT_FRAME ) )
			tmp = MAKE_MATHML_ATTR( FBL_ATTR_mathsize, FBL_VAL_normal );
		else if( kegl == ::getCurrentFormulatorStyle().getKegl( FTEXT_HEIGHT_LARGEOP ) )
			tmp = MAKE_MATHML_ATTR( FBL_ATTR_mathsize, FBL_VAL_big );
		else
			::mml_getStyle2TextPointSize( tmp, kegl, _tag, _body );
		new_attr += tmp;
		if( isDefaultMMLChanged ) *isDefaultMMLChanged = 1;
	}

	tmp = _T("");
	if( mml_attr_by_this_node.getChangedAttr_2_String( ::getCurrentFormulatorStyle().getUnit2PX(), tmp, mml_attr, _tag ) )
	{
	}

	return new_attr + tmp;
}

//////////////////////////////////////////////////////////////////////////////////

int CPlaneText::to_mathml__getTag_n_Text( 
        std::vector<QString>& v_tag, std::vector<QString>& v_text, std::vector<QString>& v_attr, long /*level*/ )
{
	if( to_mathml_data.value.length() && to_mathml_data.type.length() )
	{
		v_tag.push_back( to_mathml_data.type );
		v_text.push_back( to_mathml_data.value );
	}
	else
	{
		QString bodytext( GetNodeText() );
		if( isUnicode() )
		{
			if( to_mathml_data.name.length() )
				v_tag.push_back( to_mathml_data.name );
			else
				v_tag.push_back( QString(FBL_TAG_mi) );
			v_text.push_back( bodytext );
		}
		else
		{
			int ret = 0;
			long cb, i;
			LMMLChar mmlsl( getFStyle() == FSTYLE_GREEK, getFStyle() == FSTYLE_NUMBER, getFStyle() == FSTYLE_VARIABLE || getFStyle() == FSTYLE_FUNCTION, getFStyle() == FSTYLE_TEXT );
			for(cb = 0;;)
			{
				ret = mmlsl.Analisys( bodytext.midRef(cb) );
				if( !ret ) break;
				if( mmlsl.GetRead() > 0 && !mmlsl.getTag().isEmpty() && !mmlsl.getText().isEmpty() )
				{
					v_tag.push_back( mmlsl.getTag() );
					v_text.push_back( mmlsl.getText() );
				}
				cb += mmlsl.GetRead();
				mmlsl.reset();
			}
			
			if( getFStyle() == FSTYLE_TEXT )
			{
				QString s_tag = FBL_TAG_mtext, s_text = _T("");
            
				for( i = 0; i < (long) v_text.size(); i++ )
					s_text += v_text[ i ];
            
				v_tag.erase( v_tag.begin(), v_tag.end() );
				v_tag.push_back( s_tag );
				v_text.erase( v_text.begin(), v_text.end() );
				v_text.push_back( s_text );
			}
			else if( getFStyle() == FSTYLE_OP )
			{
				QString s_tag = FBL_TAG_mo, s_text = _T("");
            
				for( i = 0; i < (long) v_text.size(); i++ )
					s_text += v_text[ i ];
            
				v_tag.erase( v_tag.begin(), v_tag.end() );
				v_tag.push_back( s_tag );
				v_text.erase( v_text.begin(), v_text.end() );
				v_text.push_back( s_text );
			}
		}
	}

	int isDefaultMMLChanged = 0, nTmp = 0;
	long j;
	for( long i = 0; i < (long) v_tag.size(); i++ )
	{
		if( v_text[ i ].length() )
		{
			QString& topr = v_text[ i ];
			if( topr[ 0 ] == _T(' ') )
			{
				topr[ 0 ] = _T('&');
				topr.insert(1, _T("nbsp;") );
			}
			if( topr.length() > 1 && topr[ topr.length() - 1 ] == ' ' )
			{
				topr[ topr.length() - 1 ] = _T('&');
				topr.append( _T("nbsp;") );
			}
			for( j = 0; j < (long) topr.length() - 1; j++ )
			{
				if( topr.at(j) == ' ' && topr.at(j + 1) == ' ' )
				{
					topr[ (int) (j + 1) ] = '&';
					topr.insert( j + 2, "nbsp;" );
					j += 6;
				}
			}
		}
		v_attr.push_back( to_mathml__get_attribute_str( v_tag[ i ], v_text[ i ], &nTmp ) );
		isDefaultMMLChanged = isDefaultMMLChanged || nTmp;
	}
	return isDefaultMMLChanged;
}

///////////////////////////////////////////////////////////////////////////////

QString CPlaneText::to_mathml( long level, long& nodesCount )
{
	nodesCount = 0;

	QString result = _T("");
	int isContent = to_mathml_data.is_content || isIgnoreContentConversion();
	if( isContent )
	{
		int retry = 0;
		result = to_mathml__content( retry, level, nodesCount );
		switch( retry )
		{
		case 0:
			return result;
		}
		nodesCount = 0;
		result = _T("");
	}

	if( to_mathml_data.mathml_template_string.length() )
	{
		QString str = to_mathml_data.mathml_template_string;
		QString pre;
		pre.fill( ' ', MML_FORMAT_INDENT_SPACE * level );
		str.insert( 0, pre );
		long npos = 0;
		while ( (npos = str.indexOf( '\n', npos )) != -1 )
		{
			if( npos + 1 < str.length() )
			{
				long i;
                                for( i = npos + 1; i < str.length() && str.at(i) == ' '; i++ ) ;
				if( str.mid(i, QString(__HERMITECH_TEMPLATE_MARK).length()) != __HERMITECH_TEMPLATE_MARK )
					str.insert( npos + 1, pre );
			}
			npos++;
		}
		nodesCount = -1;	// undefined
		return str;
	}

	std::vector<QString> v_tag, v_text, v_attr;
	to_mathml__getTag_n_Text( v_tag, v_text, v_attr, level );

	for( long i = 0; i < (long) v_tag.size() && i < (long) v_text.size() && i < (long) v_attr.size(); i++ )
		result += ::mml_tag_no_nl( v_tag[ i ], v_text[ i ], v_attr[ i ], level );

	nodesCount = (long) v_tag.size();
	return result;
}

QString CPlaneText::to_mathml__content( int& retry, long level, long& nodesCount )
{
	long /*is_ok = 0, */cb = 0;
	QString sattr = _T(""), result = _T(""), name = to_mathml_data.name;
	retry = 0;	// must NOT be re-read as a presentation MathML

	nodesCount = 0;
	printUnfilteredAttr( sattr );
	if( !isIgnoreContentConversion() )
	{
		switch( to_mathml_data.is_content )
		{
			case CONTENT_MATHML_CONVERTION2__SELF_CLOSING:
				nodesCount = 1;
				result = name.length() ? ::mml_tag_self_nl( name, sattr, level ) : _T("");
				if( getAddonNode() && getAddonNode()->GetType() == NODE_LINE )
				{
					result += ((CLineNode*)getAddonNode())->CParentNode::to_mathml( level, cb );
					if( cb != -1 )
						nodesCount += cb;
					else
						nodesCount = -1;
				}
				break;
    
			case CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE:
				if( name.length() )
				{
					nodesCount = 1;
					result = ::mml_tag_self_nl( name, sattr, level );
				}
				else if( to_mathml_data.reserved.length() )
				{
					nodesCount = 1;
					result = ::mml_tag_no_nl( to_mathml_data.reserved, to_mathml_data.value, sattr, level );
				}
				else
					result = _T("");
				break;

			case CONTENT_MATHML_CONVERTION2__ONLY_TEXT_BODY:
				nodesCount = 1;
				result = ::mml_tag_no_nl( name, to_mathml_data.value, sattr, level );
				break;

			default:
				break;
 	 	}
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////

QString CPlaneText::to_mathml__getOneTag( std::vector<QString>* pConstructedTags )
{
	QString result = _T("");
	if( isIgnoreContentConversion() || to_mathml_data.mathml_template_string.length() ) return result;
	if( to_mathml_data.is_content )
	{
		result = to_mathml_data.name;
		if( result.length() && pConstructedTags ) pConstructedTags->push_back( result );
	}
	else
	{
		std::vector<QString> v_tag, v_text, v_attr;
		to_mathml__getTag_n_Text( v_tag, v_text, v_attr, 1 );
		if( v_tag.size() == 1 ) result = v_tag[ 0 ];
		if( pConstructedTags )
		{
			for( long i = 0; i < (long) v_tag.size(); i++ )
				if( v_tag[ i ].length() ) pConstructedTags->push_back( v_tag[ i ] );
		}
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
