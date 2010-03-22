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
#include "HUtils/idfas.h"
#include "../../dictionary/op_map.h"
#include "../../dictionary/entity_map.h"
#include "../../dictionary/op_autokeyword.h"
#include "n_cm_treetemplate.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

#define FBL_VAL_table					_T("table")
#define FBL_VAL_bracket					_T("bracket")
#define FBL_VAL_center					"center"		
#define FBL_VAL_baseline				"baseline"

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml( long level, long& nodesCount )
{
	nodesCount = 0;
	//CFormulaNode *singleChildNode = getSingleContentFormulaChild();
	CNode *singleChildNode = getSingleContentFormulaChild();

	QString ret = _T("");
	long dummy = 0;
	std::vector<QString> dest_str;
	std::vector<long> dest_style;
	QString body = _T(""), name = _T(""), sattr = _T("");
	int isAutodetect = isAutodetectContent() && !isIgnoreContentConversion();
	if( isAutodetect )
	{
		long is_ok = 0, is_newline = 0;
		if( singleChildNode != 0 )
			return CParentNode::to_mathml( level, nodesCount );
		body = to_mathml__content_harvest_plaintext( 0, is_ok, level, is_newline );
		QString sV(FBL_TAG_mtext);
		::getMathMLChar2Style( dest_str, dest_style, body, sV, __GLOBAL_WORK_MODE );
		if( is_ok && (dest_str.size() == dest_style.size()) )
		{
			if( dest_str.size() == 1 )
			{
				if( ::detectContentTag( name, body, sattr, dest_str[ 0 ], dest_style[ 0 ] ) != -1 )
				{
					nodesCount = 1;
					return body.length() ? 
							::mml_tag_no_nl( name, body, sattr, level ) : 
							::mml_tag_self_nl( name, sattr, level );
				}
			}
			else if( dest_str.size() == 2 && dest_style[ 0 ] == FSTYLE_TEXT_BUTTON )
			{
				FXmlBtn *ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( dest_str[ 0 ] );
				if( ptrFXmlBtn && (ptrFXmlBtn->getID() == _T("@minus") || ptrFXmlBtn->getID() == _T("@plus")) )
				{
					LSDigit ldigit;
					long result = ldigit.Analisys( dest_str[ 1 ].midRef(0) );
					if( result && ::detectContentTag( name, dest_str[ 1 ], sattr, dest_str[ 1 ], FSTYLE_NUMBER ) != -1 )
					{
						nodesCount = 1;
						return body.length() ? 
								::mml_tag_no_nl( name, body, sattr, level ) : 
								::mml_tag_self_nl( name, sattr, level );
					}
				}
			}
		}
	}
	
	if( to_mathml_data.is_content || isIgnoreContentConversion() )
	{
		int retry = 0;
		ret = to_mathml__content( retry, level, nodesCount );
		switch( retry )
		{
		case 0:
			return ret;
		}
		nodesCount = 0;
		ret = _T("");
	}

	if( isAutodetect )
	{
		CFormulaNode *singleChild = (CFormulaNode*) getSingleFormulaChild();
		if( singleChild && singleChild->isAutodetectContent() )
		{
			return singleChild->to_mathml( level, nodesCount );
		}

		QString detectedTag( FBL_TAG_csymbol );
		if( dest_str.size() == 1 && dest_style.size() == 1 &&
			(::detectContentTag( name, body, sattr, dest_str[ 0 ], dest_style[ 0 ] ) != -1) )
		{
			detectedTag = name;
		}
		ret = ::mml_tag_nl( detectedTag, CParentNode::to_mathml( level + 1, nodesCount ), QString( _T("") ), level );
		nodesCount = 1;
	}
	else
	{
		if( !isRWMMLSupport() )
			ret = CParentNode::to_mathml( level, nodesCount );
		else if( to_mathml_data.mathml_template_string.length() && !isReferenced() )
			ret = to_mathml__template( level, nodesCount ); // mathml conversion template, ??? strange attributes ???
		else if( to_mathml_data.type.length() && !xml_strcmp(to_mathml_data.type, FBL_VAL_bracket) )
		{
			nodesCount = 1;
			ret = ::mml_fence( 
				CParentNode::to_mathml( level + 1, dummy ),
				to_mathml_data.left,
				to_mathml_data.right,
				level
			);
		}
		else if( to_mathml_data.value.length() && to_mathml_data.type.length() )
			ret = to_mathml__subsup( level, nodesCount );
		else if( to_mathml_data.type.length() && 
				 !xml_strcmp(to_mathml_data.type, FBL_VAL_table) &&
				 to_mathml_data.name.length() )
			ret = to_mathml__table( level, nodesCount );
		else if( to_mathml_data.name.length() )
			ret = to_mathml__tagname( level, nodesCount );
		else
			ret = _T("");
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml__template( long level, long& nodesCount )
{
	nodesCount = -1;

	long i, dummy = 0;
	int argc = GetChildCount();
	QVector<QString> replace_array;
	for( i = 0; i < argc; i++ )
		replace_array.push_back( QString() );

	std::vector<long> arg_level;
	QString str = to_mathml_data.mathml_template_string;
	long npos = 0, cb = 0, k = 0;
	while( (npos = str.indexOf( __HERMITECH_TEMPLATE_MARK, npos )) != -1 )
	{
		for( cb = npos - 1; cb < str.length() && cb > 0 && str.at(cb) == ' '; cb-- ) ;
		k = npos - 1 - cb;
		arg_level.push_back( k / 2 );
		if( k )
		{
			str.remove( cb + 1, k );
			npos = cb + 1 + QString(__HERMITECH_TEMPLATE_MARK).length();
		}
		else
			npos++;
	}
	
	std::vector<QString> arg;
	std::vector<long> buffer_cb;
	cb = 0;
	k = 0;
	buffer_cb.push_back( k );
	CNode *pCurNode = GetFirstChild();
	for( i = 0; i < GetChildCount() && pCurNode; i++ )
	{
		k = level + (i < (long) arg_level.size() ? arg_level[ i ] : 0);
		if( !k ) k = 1;
		QString tstr = pCurNode->to_mathml( k, dummy );
		if( tstr.length() > 0 && tstr[ tstr.length() - 1 ] == '\n' )
			tstr.truncate(tstr.length() - 1);
		arg.push_back( tstr );
		cb += (long) tstr.length() + 1;
		buffer_cb.push_back( cb );
		pCurNode = pCurNode->GetNext();
	}

	for( i = 0; i < (long) arg.size(); i++ )
		replace_array[ i ] = arg.at(i);

	QString pre;
	pre.fill( ' ', MML_FORMAT_INDENT_SPACE * level );
	str.insert( 0, pre );
	npos = 0;
	while ( (npos = (long) str.indexOf( _T('\n'), npos )) != -1 )
	{
		if( npos + 1 < str.length() && str.at(npos + 1) != QChar(0) )
		{
			for( i = npos + 1; i < (long) str.length() && str.at(i) == _T(' '); i++ ) ;
			if( str.midRef(i, QString(__HERMITECH_TEMPLATE_MARK).length()) != __HERMITECH_TEMPLATE_MARK )
				str.insert( npos + 1, pre );
		}
		npos++;
	}

	HKMatchTemplate templ;
	/*int ret = */templ.Treate( str, replace_array );
	return templ.getBuffer();
}

///////////////////////////////////////////////////////////////////////////////

void constructAlign_MTable( QString& attrStr, CFormulaNode *frmNode )
{
	attrStr = _T("");

	int isValidChildNo = (frmNode->getAlignmentValue() >= 1 && frmNode->getAlignmentValue() <= frmNode->GetChildCount());
	int rowNo = (frmNode->getAlignmentValue() - 1) / frmNode->GetNumColumn() + 1;
	switch( frmNode->getAlignmentType() )
	{
	case FBtnChildPos::TableAxis:
		if( isValidChildNo )
			attrStr = MAKE_MATHML_ATTR( FBL_ATTR_align, QString("%1 %2").arg(FBL_VAL_axis).arg(rowNo) );
		break;
	case FBtnChildPos::TableCenter2Baseline:
		attrStr = MAKE_MATHML_ATTR( FBL_ATTR_align, 
			isValidChildNo ? (QString("%1 %2").arg(FBL_VAL_center).arg(rowNo)) : (QString("%1").arg(FBL_VAL_center)) );
		break;
	case FBtnChildPos::TableTop2Baseline:
		attrStr = MAKE_MATHML_ATTR( FBL_ATTR_align, 
			isValidChildNo ? (QString("%1 %2").arg(FBL_VAL_top).arg(rowNo)) : (QString("%1").arg(FBL_VAL_top)) );
		break;
	case FBtnChildPos::TableBottom2Baseline:
		attrStr = MAKE_MATHML_ATTR( FBL_ATTR_align, 
			isValidChildNo ? (QString("%1 %2").arg(FBL_VAL_bottom).arg(rowNo)) : (QString("%1").arg(FBL_VAL_bottom)) );
		break;
	default:
		break;
	}
}

void calculateElemAlign_MTable( int& isEach, int& isDefault, std::vector<long>& xAlign, 
								long nElem, long nStep, CFormulaNode *frmNode,
								long defaultAlign, long mask )
{
	isEach = 0;
	isDefault = 1;
	xAlign.clear();
	xAlign.resize( nElem, defaultAlign );

	for( long i = 0; i < nElem; i++ )
	{
		xAlign[ i ] = frmNode->GetChildFrameAlignType( i * nStep ) & mask;
		if( isDefault )
			isDefault = (xAlign[ i ] == defaultAlign);
		if( i && !isEach )
			isEach = (xAlign[ i - 1 ] != xAlign[ i ]);
	}
}

QString convertVAlign2Str_MTable( long valign )
{
	switch( valign & VERTICAL_MASK )
	{
	case TO_TOP:
		return FBL_VAL_top;
	case TO_BOTTOM:
		return FBL_VAL_bottom;
	case TO_BASELINE:
		return FBL_VAL_baseline;
	case VERTICAL_CENTER:
		return FBL_VAL_center;
	default:
		break;
	}
	return _T("");
}

QString convertHAlign2Str_MTable( long halign )
{
	switch( halign & HORISONTAL_MASK )
	{
	case TO_LEFT:
		return FBL_VAL_left;
	case TO_RIGHT:
		return FBL_VAL_right;
	case HORISONTAL_CENTER:
		return FBL_VAL_center;
	default:
		break;
	}
	return _T("");
}

void convertVAlign2Str_MTable( QString& dest, std::vector<long> xAlign )
{
	if( xAlign.size() < 1 )
	{
		dest = _T("");
		return;
	}
	else
		dest = ::convertVAlign2Str_MTable( xAlign[ 0 ] );

	for( long i = 1; i < (long) xAlign.size(); i++ )
		dest += QString( _T(" ") ) + ::convertVAlign2Str_MTable( xAlign[ i ] );
}

void convertHAlign2Str_MTable( QString& dest, std::vector<long> xAlign )
{
	if( xAlign.size() < 1 )
	{
		dest = _T("");
		return;
	}
	else
		dest = ::convertHAlign2Str_MTable( xAlign[ 0 ] );

	for( long i = 1; i < (long) xAlign.size(); i++ )
		dest += QString( _T(" ") ) + ::convertHAlign2Str_MTable( xAlign[ i ] );
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml__table( long level, long& nodesCount )
{
	QString dest = _T("");
	printUnfilteredAttr( dest );

	QString attrStr = _T("");
	::constructAlign_MTable( attrStr, this );
	dest += attrStr;

	int isEach = 0, isDefault = 1;
	std::vector<long> hAlign, vAlign;

	attrStr = _T("");
	::calculateElemAlign_MTable( isEach, isDefault, vAlign, GetNumLine(), GetNumColumn(), this, TO_BASELINE, VERTICAL_MASK );
	if( isEach )
		::convertVAlign2Str_MTable( attrStr, vAlign );
	else if( !isDefault && vAlign.size() > 0 )
		attrStr = ::convertVAlign2Str_MTable( vAlign[ 0 ] );
	if( attrStr.length() )
		dest += MAKE_MATHML_ATTR( FBL_ATTR_rowalign, attrStr );

	attrStr = _T("");
	::calculateElemAlign_MTable( isEach, isDefault, hAlign, GetNumColumn(), 1, this, HORISONTAL_CENTER, HORISONTAL_MASK );
	if( isEach )
		::convertHAlign2Str_MTable( attrStr, hAlign );
	else if( !isDefault && hAlign.size() > 0 )
		attrStr = ::convertHAlign2Str_MTable( hAlign[ 0 ] );
	if( attrStr.length() )
		dest += MAKE_MATHML_ATTR( FBL_ATTR_columnalign, attrStr );

	nodesCount = 1;

	QString _t, _l, sattr;
	long i, j, dummy = 0, idx, v, h;
	CNode *pCurNode = GetFirstChild();
	_t = _T("");
	idx = 0;
	Q_ASSERT( (long) hAlign.size() == GetNumColumn() && (long) vAlign.size() == GetNumLine() );
	for( i = 0; pCurNode && i < GetNumLine(); i++ )
	{
		v = (i < (long) vAlign.size()) ? vAlign[ i ] : TO_BASELINE;
		_l = _T("");
		for( j = 0; pCurNode && j < GetNumColumn(); j++ )
		{
			h = (j < (long) hAlign.size()) ? hAlign[ j ] : HORISONTAL_CENTER;
			sattr = _T("");
			long cellAlign = GetChildFrameAlignType( idx );
			if( (cellAlign & VERTICAL_MASK) != v )
			{
				attrStr = ::convertVAlign2Str_MTable( cellAlign );
				sattr += MAKE_MATHML_ATTR( FBL_ATTR_rowalign, attrStr );
			}
			if( (cellAlign & HORISONTAL_MASK) != h )
			{
				attrStr = ::convertHAlign2Str_MTable( cellAlign );
				sattr += MAKE_MATHML_ATTR( FBL_ATTR_columnalign, attrStr );
			}

			_l += ::mml_tablecell_tag( pCurNode->to_mathml( level + 3, dummy ), level + 2, sattr );
			pCurNode = pCurNode->GetNext();
			idx++;
		}
		_t += ::mml_tablerow_tag( _l, level + 1 );
	}

	return ::mml_table_tag( _t, dest, level );
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::to_mathml__subsup( long level, long& nodesCount )
{
	nodesCount = 1;

	long dummy = 0;
	if( to_mathml_data.frame.length() )
	{
		QString ret = mml_tag_no_nl( 
						to_mathml_data.type, 
						to_mathml_data.value, 
						QString( _T("") ), 
						level + 1 );

		QString arg = _T("");
		CNode *pCurNode = GetFirstChild();
		for( long j = 0; j < GetChildCount() && pCurNode; j++ )
		{
			if( j > 0 && isRWMMLInfix() )
				arg += ret;
			arg += pCurNode->to_mathml( level + 1, dummy );
			pCurNode = pCurNode->GetNext();
		}

		QString body = arg;
		QString sarg = _T(""), cur_attr = _T("");
		printUnfilteredAttr( sarg );

		if( isRWMMLPostfix() )
			body = mml_tag_nl( 
					to_mathml_data.frame, 
					ret + body,
					cur_attr + sarg,
					level );
		else if( isRWMMLPrefix() )
			body = mml_tag_nl( 
					to_mathml_data.frame, 
					body + ret,
					cur_attr + sarg,
					level );
		else
			body = mml_tag_nl( 
					to_mathml_data.frame, 
					body,
					cur_attr + sarg,
					level );

		return body;
	}
	else
	{
		QString ret = mml_tag_no_nl( 
						to_mathml_data.type, 
						to_mathml_data.value, 
						QString( _T("") ), 
						level + 1 );
		QString arg = _T("");
		CNode *pCurNode = GetFirstChild();
		for( long j = 0; j < GetChildCount() && pCurNode; j++ )
		{
			if( j > 0 && isRWMMLInfix() )
				arg += ret;
			arg += pCurNode->to_mathml( GetChildCount() == 1 ? level + 1 : level + 2, dummy );
			pCurNode = pCurNode->GetNext();
		}

		if( GetChildCount() > 1 )
			arg = mml_row_tag_nl( arg, level + 1 );

		if( isRWMMLPostfix() )
			return mml_row_tag_nl( arg + ret, level );
		else if( isRWMMLPrefix() )
			return mml_row_tag_nl( ret + arg, level );
		else
			return mml_row_tag_nl( arg, level );
	}
}

QString CFormulaNode::to_mathml__tagname__MFRAC_getAttr( QString& sattr, long /*level*/ )
{
	long n;
	long numalign = TO_BOTTOM | HORISONTAL_CENTER;
	long denomalign = TO_TOP | HORISONTAL_CENTER;

	n = GetChildFrameAlignType( 0 );
	if( n != numalign )
	{
		switch( n & HORISONTAL_MASK )
		{
		case TO_LEFT:
			sattr += MAKE_MATHML_ATTR( FBL_ATTR_numalign, FBL_VAL_left );
			break;
		case TO_RIGHT:
			sattr += MAKE_MATHML_ATTR( FBL_ATTR_numalign, FBL_VAL_right );
			break;
		default:
			break;
		}
	}

	n = GetChildFrameAlignType( 1 );
	if( n != denomalign )
	{
		switch( n & HORISONTAL_MASK )
		{
		case TO_LEFT:
			sattr += MAKE_MATHML_ATTR( FBL_ATTR_denomalign, FBL_VAL_left );
			break;
		case TO_RIGHT:
			sattr += MAKE_MATHML_ATTR( FBL_ATTR_denomalign, FBL_VAL_right );
			break;
		default:
			break;
		}
	}

	RealFde thickness = getLinethickness( 0 );
	if( thickness != LINETHICKNESS_MEDIUM )
		sattr += MAKE_MATHML_ATTR( FBL_ATTR_linethickness, QString("%1px").arg(thickness) );
	return sattr;
}

QString CFormulaNode::to_mathml__tagname( long level, long& nodesCount )
{
	nodesCount = 1;

	long cb = 0;
	QString sattr = _T(""), body = _T("");
	if( ::mml_is_frac_tag( to_mathml_data.name ) )
		to_mathml__tagname__MFRAC_getAttr( sattr, level );

	QString tagname = to_mathml_data.name;

	if( !xml_strcmp( tagname, FBL_TAG_mroot ) ||
		!xml_strcmp( tagname, FBL_TAG_mfrac ) ||
		!xml_strcmp( tagname, FBL_TAG_msup ) ||
		!xml_strcmp( tagname, FBL_TAG_msub ) ||
		!xml_strcmp( tagname, FBL_TAG_msubsup ) ||
		!xml_strcmp( tagname, FBL_TAG_munder ) ||
		!xml_strcmp( tagname, FBL_TAG_mover ) ||
		!xml_strcmp( tagname, FBL_TAG_munderover ) )
	{
		CNode *pCurNode = GetFirstChild();
		for( long j = 0; j < GetChildCount() && pCurNode; j++ )
		{
			cb = 0;
			QString arg = pCurNode->to_mathml( level + 1, cb );

			if( cb > 1 )
			{
				arg = pCurNode->to_mathml( level + 2, cb );	// just to get pretty-printing, could be erased to speed up converting to MathML
				body += mml_row_tag_nl( arg, level + 1 );
			}
			else
				body += arg;

			pCurNode = pCurNode->GetNext();
		}
	}
	else if( !xml_strcmp( tagname, FBL_TAG_menclose ) )
	{
		sattr = MAKE_MATHML_ATTR( FBL_ATTR_notation, hstr_escape( to_mathml_data.type ) );
		body = CParentNode::to_mathml( level + 1, cb );
	}
	else if( !xml_strcmp( tagname, FBL_TAG_mmultiscripts ) )
	{
		long nType = to_mathml_data.type.toLong() & 0xF;
		QString buttonID = _T("");
		long base = -1, swcb = -1, nwcb = -1, secb = -1, necb = -1;
		::getMMultiscriptsAttr( nType, buttonID, base, swcb, nwcb, secb, necb );

		CNode *bs = GetChild( base );
		CNode *sw = GetChild( swcb ), *nw = GetChild( nwcb );
		CNode *se = GetChild( secb ), *ne = GetChild( necb );
		std::vector<QString> strSE, strNE, strSW, strNW;

		cb = 0;
		body = bs ? bs->to_mathml( level + 1, cb ) : ::mml_row_tag_no_nl( QString( _T("") ), level + 1 );
		if( bs && cb > 1 )
		{
			body = bs->to_mathml( level + 2, cb );	// just to get pretty-printing, could be erased to speed up converting to MathML
			body = mml_row_tag_nl( body, level + 1 );
		}

		if( se && se->GetType() == NODE_FRAME ) ((CFrameNode*)se)->setMathML2StringVector( strSE, level + 1 );
		if( ne && ne->GetType() == NODE_FRAME ) ((CFrameNode*)ne)->setMathML2StringVector( strNE, level + 1 );
		if( sw && sw->GetType() == NODE_FRAME ) ((CFrameNode*)sw)->setMathML2StringVector( strSW, level + 1 );
		if( nw && nw->GetType() == NODE_FRAME ) ((CFrameNode*)nw)->setMathML2StringVector( strNW, level + 1 );

		long i, mx = (long) (strSE.size() > strNE.size() ? strSE.size() : strNE.size());
		if( mx > 0 )
		{
			for( i = 0; i < mx; i++ )
			{
				body += (i < (long) strSE.size()) ? strSE[ i ] : ::mml_tag_self_nl( QString(FBL_TAG_none), QString( _T("") ), level + 1 );
				body += (i < (long) strNE.size()) ? strNE[ i ] : ::mml_tag_self_nl( QString(FBL_TAG_none), QString( _T("") ), level + 1 );
			}
		}

		mx = (long) (strSW.size() > strNW.size() ? strSW.size() : strNW.size());
		if( mx > 0 )
		{
			body += ::mml_tag_self_nl( QString(FBL_TAG_mprescripts), QString( _T("") ), level + 1 );
			for( i = 0; i < mx; i++ )
			{
				body += (i < (long) strSW.size()) ? strSW[ i ] : ::mml_tag_self_nl( QString(FBL_TAG_none), QString( _T("") ), level + 1 );
				body += (i < (long) strNW.size()) ? strNW[ i ] : ::mml_tag_self_nl( QString(FBL_TAG_none), QString( _T("") ), level + 1 );
			}
		}
	}
	else
	{
		body = CParentNode::to_mathml( level + 1, cb );
	}

	printUnfilteredAttr( sattr );

	if( to_mathml_data.name == FBL_TAG_none && body.length() == 0 )
		return ::mml_tag_self_nl( to_mathml_data.name, sattr, level );

	return mml_tag_nl( to_mathml_data.name, body, sattr, level );
}

///////////////////////////////////////////////////////////////////////////////
