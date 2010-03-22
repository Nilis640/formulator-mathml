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

#include "mml_format.h"
#include "mml_defs.h"
#include "HUtils/idfas.h"
#include "../settings/options.h"

int mml_is_italic_token_mi( const QString& _tag_name, const QString& _tag_body )
{
	if( ::mml_is_token_identifier_tag( _tag_name ) 
		&& (_tag_body.length() == 1 || !::isOption_MulticharMINotItalic())
		&& _tag_body[ 0 ].isLetter() )
		return 1;
	return 0;
}

QString mml_tag_self_no_nl( const QString& name, const QString& attr, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(pre + QString( _T("<") ) + name + QString((attr == _T("") || attr[ 0 ] == _T(' ')) ? _T("") : _T(" ")) + attr + QString( _T("/>") ));
	return ret;
}

QString mml_tag_self_nl( const QString& name, const QString& attr, long level )
{
	return ::mml_tag_self_no_nl( name, attr, level ) + _T("\n");
}

QString mml_tag_nl( const QString& name, const QString& body, const QString& attr, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(
		pre + QString( _T("<") ) + name + QString((attr == _T("") || attr[ 0 ] == _T(' ') ) ? _T("") : _T(" ")) + attr + QString(_T(">\n") ) + 
		body + 
		pre + QString( _T("</") ) + name + QString( _T(">\n") )
	);
	return ret;
}

QString mml_tag_nl_part1( const QString& name, const QString& body, const QString& attr, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(
		pre + QString( _T("<") ) + name + QString((attr == _T("") || attr[ 0 ] == _T(' ') ) ? _T("") : _T(" ")) + attr + QString(_T(">\n") ) + 
		body
	);
	return ret;
}

QString mml_tag_nl_part2( const QString& name, const QString& body, const QString& /*attr*/, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(
		body + 
		pre + QString( _T("</") ) + name + QString( _T(">\n") )
	);
	return ret;
}

QString mml_tag_no_nl( const QString& name, const QString& body, const QString& attr, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(
		pre + QString( _T("<") ) + name + QString((attr == _T("") || attr[ 0 ] == _T(' ') ) ? _T("") : _T(" ")) + attr + QString( _T(">") ) + 
		body + 
		QString( _T("</") ) + name + QString( _T(">\n") )
	);
	return ret;
}

QString mml_tag_no_nl_part1( const QString& name, const QString& body, const QString& attr, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(
		pre + QString( _T("<") ) + name + QString((attr == _T("") || attr[ 0 ] == _T(' ') ) ? _T("") : _T(" ")) + attr + QString( _T(">") ) + 
		body
	);
	return ret;
}

QString mml_tag_no_nl_part2( const QString& name, const QString& body, const QString& /*attr*/, long level )
{
	QString pre(2 * level, ' ');
	QString ret = QString(
		body + 
		QString( _T("</") ) + name + QString( _T(">\n") )
	);
	return ret;
}

QString mml_tag_auto_nl( const QString& name, const QString& body, const QString& sattr, long level )
{
	if( 2 * level + (body + sattr).length() > 76 )
		return ::mml_tag_nl( name, body, sattr, level );
	else
		return ::mml_tag_no_nl( name, body, sattr, level );
}

QString mml_tag( const QString& name, const QString& body, const QString& attr, long level )
{
	return mml_tag_nl( name, body, attr, level );
}

QString mml_root_tag_xmlns( const QString& body, long level )
{
	return mml_tag_nl( 
		QString(FBL_TAG_math), 
		body, 
		QString(FBL_ATTR_STRING_root_xmlns),
		level
	);
}

QString mml_root_tag( const QString& body, long level )
{
	return mml_tag_nl( 
		QString(FBL_TAG_math), 
		body, 
		QString(FBL_ATTR_STRING_root),
		level
	);
}

QString mml_root_tag_4_html_mathml( int is_block, const QString& body, long level )
{
	return mml_tag_nl( 
		QString(FBL_TAG_math), 
		body, 
		is_block ?  QString(FBL_ATTR_STRING_root4html_block) : 
					QString(FBL_ATTR_STRING_root4html_inline),
		level
	);
}

QString mml_table_tag( const QString& body, const QString& attr, long level )
{
	return mml_tag_nl( QString(FBL_TAG_mtable), body, attr, level );
}

QString mml_tablerow_tag( const QString& body, long level )
{
	return ::mml_tag_nl( QString(FBL_TAG_mtr), body, QString(_T("")), level );
}

QString mml_tablecell_tag( const QString& body, long level, QString attr )
{
	return ::mml_tag_nl( QString(FBL_TAG_mtd), body, attr, level );
}

QString mml_row_tag_nl( const QString& body, long level )
{
	return ::mml_tag_nl( QString( FBL_TAG_mrow ), body, QString(_T("")), level );
}

QString mml_row_tag_no_nl( const QString& body, long level )
{
	return ::mml_tag_no_nl( QString( FBL_TAG_mrow ), body, QString(_T("")), level );
}

QString mml_fence( const QString& body, const QString& left, const QString& right, long level )
{
	return mml_tag_nl( 
		QString(FBL_TAG_mfenced), 
		body, 
		QString( QString( FBL_ATTR_open )  + QString( _T(" = \'") ) + QString( left ) + QString( _T("\'") ) +
			QString( _T(" ") ) +
			QString( FBL_ATTR_close ) + QString( _T(" = \'") ) + QString( right ) + QString( _T("\'") ) ),
		level
	);
}

void mml_validate4html( QString& src )
{
	static LCEntity la_ent;
	int ret;
	for( long i = 0; i < src.length(); i++ )
	{
		if( src.at(i) == QChar('&') )
		{
			ret = la_ent.Analisys( src.midRef( i ) );
			if( ret == LCEntity::Number && la_ent.GetRead() > 0 )
				i += la_ent.GetRead() - 1;
			else
			{
				src.insert( i + 1, MML_4HTML_AMP );
				i += strlen( MML_4HTML_AMP );
			}
			la_ent.Reset();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// the following 2 functions are rather "it can be rendered" 
// than "it is valid" checks
///////////////////////////////////////////////////////////////////////////////
int mml_is_table_tag_correct( AST_TXML_Tag *_mtable, std::vector<long>& line_length )
{
	AST_TXML_Compound *stmt;
	AST_TXML_Tag *_mtr, *_mtd;
	long i, j;

	line_length.clear();
	if( !_mtable || !_mtable->getTags() ) return 0;
	for( i = 0; i < _mtable->getTags()->getChildNumber(); i++ )
	{
		if( !_mtable->getTags()->getChild( i ) ||
			_mtable->getTags()->getChild( i )->getNodeType() != ASTT_TXML_Tag )
			continue;

		j = 0;
		line_length.push_back( j );
		_mtr = (AST_TXML_Tag*)_mtable->getTags()->getChild( i );

		if( !::mml_is_tablerow_tag( _mtr->getTagName() ) )
		{// fix the error
			_mtable->getTags()->ReplaceChild( i, 0 );
			stmt = new AST_TXML_Compound();
			stmt->Add( _mtr );
			QString mtrV(FBL_TAG_mtr);
			_mtr = new AST_TXML_Tag( &mtrV, 0, stmt );
			_mtable->getTags()->ReplaceChild( i, _mtr );
		}

		if( !_mtr->getTags() ) continue;
		QString mtdV(FBL_TAG_mtd);
		for( j = 0; j < _mtr->getTags()->getChildNumber(); j++ )
		{
			if( !_mtr->getTags()->getChild( j ) || 
			     _mtr->getTags()->getChild( j )->getNodeType() != ASTT_TXML_Tag )
				continue;

			line_length[ i ]++;
			_mtd = (AST_TXML_Tag*)_mtr->getTags()->getChild( j );
			if( !::mml_is_tablecell_tag( _mtd->getTagName() ) )
			{// fix the error
				_mtr->getTags()->ReplaceChild( j, 0 );
				stmt = new AST_TXML_Compound();
				stmt->Add( _mtd );
				_mtd = new AST_TXML_Tag( &mtdV, 0, stmt );
				_mtr->getTags()->ReplaceChild( j, _mtd );
			}
		}
	}
	return 1;
}

int mml_is_table_tag_correct( AST_TXML_Compound *_math_tags, std::vector<long>& line_length )
{
	AST_TXML_Tag *_mtable;
	line_length.clear();
	if( _math_tags->getChildNumber() == 1 && 
		_math_tags->getChild( 0 ) &&
		_math_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag &&
		::mml_is_table_tag( (_mtable = (AST_TXML_Tag*)_math_tags->getChild( 0 ))->getTagName() ) )
		return ::mml_is_table_tag_correct( _mtable, line_length );
	return 0;
}

AST_TXML_Tag* mml_get_table_tag_mtr( AST_TXML_Tag *_mtable, long row_no )
{
	AST_TXML_Node *_mtr = 0;
	if( !_mtable || 
		!_mtable->getTags() || 
		!(_mtr = _mtable->getTags()->getChild( row_no )) ||
		_mtr->getNodeType() != ASTT_TXML_Tag )
		return 0;
	return (AST_TXML_Tag*)_mtr;
}

AST_TXML_Tag* mml_get_table_tag_mtd( AST_TXML_Tag *_mtable, long row_no, long col_no )
{
	AST_TXML_Tag *_mtr = ::mml_get_table_tag_mtr( _mtable, row_no );
	AST_TXML_Node *_mtd = 0;
	if( !_mtr || !_mtr->getTags() || 
		!(_mtd = _mtr->getTags()->getChild( col_no )) ||
		_mtd->getNodeType() != ASTT_TXML_Tag )
		return 0;
	return (AST_TXML_Tag*)_mtd;
}

AST_TXML_Compound* mml_get_table_tag_cell( AST_TXML_Tag *_mtable, long row_no, long col_no )
{
	AST_TXML_Tag *_mtd = ::mml_get_table_tag_mtd( _mtable, row_no, col_no );
	if( !_mtd ) return 0;
	return _mtd->getTags();
}

int mml_is_token_tag_correct( AST_TXML_Tag *node )
{
	int ret = 1;
	if( node->getTags() )
	{
		for( long i = 0; i < node->getTags()->getChildNumber(); i++ )
		{
			if( node->getTags()->getChild( i )->getNodeType() != ASTT_TXML_Value )
			{
				ret = 0;
				break;
			}
		}
	}
	return ret;
}

int mml_is_table_tag_box_solid( AST_TXML_Tag* node )
{
	AST_TXML_Attribute *attr = node->getAttribute( FBL_ATTR_frame );
	if( !attr || xml_strcmp( attr->getStringValue(), FBL_VAL_solid ) ) return 0;
	return 1;
}

int mml_has_equivalent_attr( const AST_TXML_Compound* _new_attr, const QString attrname )
{
	if( attrname == FBL_ATTR_mathcolor && 
		((AST_TXML_Compound*)_new_attr)->getFirstAttribute(FBL_ATTR_color) )
		return 1;
	if( attrname == FBL_ATTR_color && 
		((AST_TXML_Compound*)_new_attr)->getFirstAttribute(FBL_ATTR_mathcolor) )
		return 1;
	if( attrname == FBL_ATTR_mathsize && 
		((AST_TXML_Compound*)_new_attr)->getFirstAttribute(FBL_ATTR_fontsize) )
		return 1;
	if( attrname == FBL_ATTR_fontsize && 
		((AST_TXML_Compound*)_new_attr)->getFirstAttribute(FBL_ATTR_mathsize) )
		return 1;
	if( (attrname == FBL_ATTR_fontweight || attrname == FBL_ATTR_fontstyle || attrname == FBL_ATTR_fontfamily) && 
		((AST_TXML_Compound*)_new_attr)->getFirstAttribute(FBL_ATTR_mathvariant) )
		return 1;
	return 0;
}

int mml_isAttrTakingNamedspace( QString attrName )
{
	if( attrName == FBL_ATTR_maxsize || attrName == FBL_ATTR_minsize ||
		attrName == FBL_ATTR_lspace || attrName == FBL_ATTR_rspace || 
		attrName == FBL_ATTR_width )
	{
		return 1;
	}
	return 0;
}

int mml_is_root_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_math );
}

int mml_is_table_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mtable );
}

int mml_is_fenced_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mfenced );
}

int mml_is_under_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_munder );
}

int mml_is_over_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mover );
}

int mml_is_underover_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_munderover );
}

int mml_is_under_n_over_tag( const QString& tag )
{
	return  !xml_strcmp( tag, FBL_TAG_munder ) ||
			!xml_strcmp( tag, FBL_TAG_mover ) ||
			!xml_strcmp( tag, FBL_TAG_munderover );
}

int mml_is_frac_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mfrac );
}

int mml_is_sup_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_msup );
}

int mml_is_sub_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_msub );
}

int mml_is_mmultiscripts_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mmultiscripts );
}

int mml_is_style_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mstyle );
}

int mml_is_mphantom_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mphantom );
}

int mml_is_maction_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_maction );
}

int mml_is_mglyph_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mglyph );
}

int mml_is_tablerow_tag( const QString& tag )
{
	return tag == FBL_TAG_mtr || tag == FBL_TAG_mlabeledtr;
}

int mml_is_tablecell_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mtd );
}

int mml_is_row_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mrow );
}

int mml_is_token_string_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_ms );
}

int mml_is_token_space_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mspace );
}

int mml_is_token_identifier_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mi );
}

int mml_is_token_number_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mn );
}

int mml_is_token_op_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mo );
}

int mml_is_token_text_tag( const QString& tag )
{
	return !xml_strcmp( tag, FBL_TAG_mtext );
}

int mml_is_token_tag( const QString& tag )
{
	return
		::mml_is_token_identifier_tag( tag ) ||
		::mml_is_token_number_tag( tag ) ||
		::mml_is_token_op_tag( tag ) ||
		::mml_is_token_string_tag( tag ) ||
		::mml_is_token_text_tag( tag ) ||
		::mml_is_token_space_tag( tag );
}

int mml_is_default_cn_base( const QString& src )
{
	return xml_strcmp( src, QString(CONTENT_MATHML_CN_DEFAULT_BASE_STR) ) == 0;
}

int mml_is_cm_token( const QString& str )
{
	return	xml_strcmp(str, QString(FBL_TAG_cn)) == 0 || 
			xml_strcmp(str, QString(FBL_TAG_ci)) == 0 || 
			xml_strcmp(str, QString(FBL_TAG_csymbol)) == 0;
}

int mml_is_cm_apply( const QString& str )
{
	return xml_strcmp(str, QString(FBL_TAG_apply)) == 0;
}
