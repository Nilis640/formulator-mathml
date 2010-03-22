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

#include "../../mathml/mml_defs.h"
#include "../node.h"
#include "n_proplist.h"

///////////////////////////////////////////////////////////////////////////

CNode_TextPropertyList::CNode_TextPropertyList( void ) : 
	m_list_value(), m_list_name()
{
	m_nodeXMLTree = 0;
	m_rootAttrFocus = 0;
}

CNode_TextPropertyList::CNode_TextPropertyList( CNode_TextPropertyList& p ) : 
	m_list_value( p.m_list_value ), m_list_name( p.m_list_name )
{
	m_nodeXMLTree = 0;
	m_rootAttrFocus = 0;
}

CNode_TextPropertyList& CNode_TextPropertyList::operator = ( CNode_TextPropertyList& p ) 
{
	m_list_value = p.m_list_value;
	m_list_name = p.m_list_name;
	resetXMLPtrs();
	return *this;
}


void CNode_TextPropertyList::reset( void )
{
	m_list_value.clear();
	m_list_name.clear();
}

void CNode_TextPropertyList::setParameter( CNode_TextPropertyData& param )
{
	if( m_list_name.find( param.getKey() ) == m_list_name.end() )
	{
		m_list_value.push_back( param );
		m_list_name[ param.getKey() ] = (long) (m_list_value.size() - 1);
	}
	else
		m_list_value[ m_list_name[ param.getKey() ] ] = param; // assign a new parameter value
}

CNode_TextPropertyData* CNode_TextPropertyList::getParameter( unsigned long index )
{
	return &m_list_value[ index ];
}

long CNode_TextPropertyList::getParameterIndex( QString key )
{
	if ( m_list_name.is_find( key ) == 0 )
		return -1;
	return m_list_name[ key ];
}

CNode_TextPropertyData* CNode_TextPropertyList::getParameter( QString key )
{
	if ( m_list_name.is_find( key ) == 0 )
		return 0;
	return &m_list_value[ m_list_name[ key ] ];
}

///////////////////////////////////////////////////////////////////////////

void CNode_TextPropertyList::OnOKPropsEditing( std::vector<char>& supports )
{
	if( !m_rootAttrFocus ) return;

	AST_TXML_Compound *attrList = m_rootAttrFocus->getAttributes();
	if( !attrList )
	{
		attrList = new AST_TXML_Compound();
		m_rootAttrFocus->setAttributes( attrList );
	}
	else
		attrList->Flush();

	AST_TXML_Attribute *newAttr = 0;
	CNode_TextPropertyData *data = 0;
	for( long i = 0; i < (long) supports.size(); i++ )
	{
		if( supports[ i ] != 2 ) continue;
		if( (data = getParameter( i )) == 0 ) continue;
		newAttr = new AST_TXML_Attribute( &(data->m_name), &(data->m_value) );
		attrList->Add( newAttr );
	}
}

///////////////////////////////////////////////////////////////////////////

void addAttr2MStyle( HKMap<long,int>& mstyleAttr, std::multimap<QString, long>& m_tag2attrno, long idx )
{
	if( mstyleAttr.find( idx ) != mstyleAttr.end() ) return;
	mstyleAttr[ idx ] = 1;
	m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mstyle), idx) );
} 

void CMMLNodeAttrList::setMapTag2Attr()
{
	HKMap<long,int> mstyleAttr;

	static const int commonAttrsLength = 5;
	static QString commonAttrs[ commonAttrsLength ] =
		{FBL_ATTR_class, FBL_ATTR_style, FBL_ATTR_id, FBL_ATTR_xref, FBL_ATTR_xlink_href};

	long i, j;
	long idx = 0;
	QString tagName;

	static const int nPToken_cb = 7;
	static const QString namePToken[ nPToken_cb ] =
	{ FBL_TAG_mi, FBL_TAG_mn, FBL_TAG_mo, FBL_TAG_mtext, FBL_TAG_mspace, FBL_TAG_ms, FBL_TAG_mglyph };
	//
	static const int commonAttrsLength_P = 9;
	static QString commonAttrs_P[ commonAttrsLength_P ] = 
		{FBL_ATTR_mathvariant, FBL_ATTR_mathsize, FBL_ATTR_mathcolor, FBL_ATTR_mathbackground, 
		 FBL_ATTR_fontsize, FBL_ATTR_fontweight, FBL_ATTR_fontstyle, FBL_ATTR_fontfamily, FBL_ATTR_color};
	//
	static const int commonAttrsLength_Mo = 12;
	static QString commonAttrs_Mo[ commonAttrsLength_Mo ] = 
		{FBL_ATTR_form, FBL_ATTR_fence, FBL_ATTR_separator, FBL_ATTR_lspace, FBL_ATTR_rspace,
		 FBL_ATTR_stretchy, FBL_ATTR_symmetric, FBL_ATTR_maxsize, FBL_ATTR_minsize, 
		 FBL_ATTR_largeop, FBL_ATTR_movablelimits, FBL_ATTR_accent};
	//
	static const int commonAttrsLength_Mspace = 4;
	static QString commonAttrs_Mspace[ commonAttrsLength_Mspace ] = 
		{FBL_ATTR_width, FBL_ATTR_height, FBL_ATTR_depth, FBL_ATTR_linebreak};
	//
	static const int commonAttrsLength_Ms = 2;
	static QString commonAttrs_Ms[ commonAttrsLength_Ms ] = 
		{FBL_ATTR_lquote, FBL_ATTR_rquote};
	//
	static const int commonAttrsLength_Mglyph = 2;
	static QString commonAttrs_Mglyph[ commonAttrsLength_Mglyph ] = 
		{FBL_ATTR_alt, FBL_ATTR_index};

	for( j = 0; j < commonAttrsLength_P; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_P[ j ] )) == -1 )
			continue;
		for( i = 0; i < nPToken_cb; i++ )
		{
			m_tag2attrno.insert( std::pair<QString, long>(namePToken[ i ], idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//
	// Presentation: mo
	for( j = 0; j < commonAttrsLength_Mo; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mo[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mo), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//
	// Presentation: mspace
	for( j = 0; j < commonAttrsLength_Mspace; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mspace[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mspace), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//
	// Presentation: ms
	for( j = 0; j < commonAttrsLength_Ms; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Ms[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_ms), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//
	// Presentation: mglyph
	for( j = 0; j < commonAttrsLength_Mglyph; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mglyph[ j ] )) != -1 )
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mglyph), idx) );
	}
    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	// Presentation: layout schema
	//
	static const int nPElem_cb = 24;
	static const QString namePElem[ nPElem_cb ] =
	{ FBL_TAG_math, FBL_TAG_mrow, FBL_TAG_mfrac, FBL_TAG_msqrt, FBL_TAG_mroot, FBL_TAG_mstyle,
	  FBL_TAG_merror, FBL_TAG_mpadded, FBL_TAG_mphantom, FBL_TAG_mfenced, FBL_TAG_menclose,
	  FBL_TAG_msub, FBL_TAG_msup, FBL_TAG_msubsup, FBL_TAG_munder, FBL_TAG_mover,
	  FBL_TAG_munderover, FBL_TAG_mmultiscripts, FBL_TAG_mtable, FBL_TAG_mtr, FBL_TAG_mtd,
	  FBL_TAG_malignmark, FBL_TAG_maligngroup, FBL_TAG_maction };

	//
	// Element: mfrac
	//
	static const int commonAttrsLength_Mfrac = 5;
	static QString commonAttrs_Mfrac[ commonAttrsLength_Mfrac ] = 
		{FBL_ATTR_linethickness, FBL_ATTR_numalign, FBL_ATTR_denomalign, FBL_ATTR_bevelled, FBL_ATTR_color};
	//
	for( j = 0; j < commonAttrsLength_Mfrac; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mfrac[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mfrac), idx) );
			//m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mstyle), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//

	//
	// Element: math
	//
	static const int commonAttrsLength_Math = 6;
	static QString commonAttrs_Math[ commonAttrsLength_Math ] = 
		{FBL_ATTR_macros, FBL_ATTR_mode, FBL_ATTR_display, FBL_ATTR_overflow, FBL_ATTR_altimg, FBL_ATTR_alttext};
	//
	for( j = 0; j < commonAttrsLength_Math; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Math[ j ] )) != -1 )
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_math), idx) );
	}
	//

	//
	// Element: mstyle
	//
	static const int commonAttrsLength_Mstyle = 13;
	static QString commonAttrs_Mstyle[ commonAttrsLength_Mstyle ] = 
		{FBL_ATTR_scriptlevel, FBL_ATTR_displaystyle, FBL_ATTR_scriptsizemultiplier, 
		 FBL_ATTR_scriptminsize, FBL_ATTR_background, FBL_ATTR_veryverythinmathspace, 
		 FBL_ATTR_verythinmathspace, FBL_ATTR_thinmathspace, FBL_ATTR_mediummathspace, 
		 FBL_ATTR_thickmathspace, FBL_ATTR_verythickmathspace, FBL_ATTR_veryverythickmathspace, FBL_ATTR_color};
	//
	for( j = 0; j < commonAttrsLength_Mstyle; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mstyle[ j ] )) != -1 )
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
	}
	//

	//
	// Element: mpadded
	//
	static const int commonAttrsLength_Mpadded = 4;
	static QString commonAttrs_Mpadded[ commonAttrsLength_Mpadded ] = 
		{FBL_ATTR_width, FBL_ATTR_lspace, FBL_ATTR_height, FBL_ATTR_depth};
	//
	for( j = 0; j < commonAttrsLength_Mpadded; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mpadded[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mpadded), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//

	//
	// Element: mfenced
	//
	static const int commonAttrsLength_Mfenced = 4;
	static QString commonAttrs_Mfenced[ commonAttrsLength_Mfenced ] = 
		{FBL_ATTR_open, FBL_ATTR_close, FBL_ATTR_separators, FBL_ATTR_color};
	//
	for( j = 0; j < commonAttrsLength_Mfenced; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mfenced[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mfenced), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//

	//
	// Element: msqrt, mroot
	//
	if( (idx = getParameterIndex( FBL_ATTR_color )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_msqrt), idx) );
	if( (idx = getParameterIndex( FBL_ATTR_color )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mroot), idx) );
	//

	//
	// Element: menclose
	//
	if( (idx = getParameterIndex( FBL_ATTR_notation )) != -1 )
	{
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_menclose), idx) );
		::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
	}
	if( (idx = getParameterIndex( FBL_ATTR_color )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_menclose), idx) );
	//

	//
	// Element: msub
	//
	if( (idx = getParameterIndex( FBL_ATTR_subscriptshift )) != -1 )
	{
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_msub), idx) );
		::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
	}
	//

	//
	// Element: msup
	//
	if( (idx = getParameterIndex( FBL_ATTR_superscriptshift )) != -1 )
	{
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_msup), idx) );
		::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
	}
	//

	//
	// Element: msubsup
	//
	if( (idx = getParameterIndex( FBL_ATTR_subscriptshift )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_msubsup), idx) );
	if( (idx = getParameterIndex( FBL_ATTR_superscriptshift )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_msubsup), idx) );
	//

	//
	// Element: munder
	//
	if( (idx = getParameterIndex( FBL_ATTR_accentunder )) != -1 )
	{
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_munder), idx) );
		::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
	}
	//

	//
	// Element: mover
	//
	if( (idx = getParameterIndex( FBL_ATTR_accent )) != -1 )
	{
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mover), idx) );
		::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
	}
	//

	//
	// Element: munderover
	//
	if( (idx = getParameterIndex( FBL_ATTR_accentunder )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_munderover), idx) );
	if( (idx = getParameterIndex( FBL_ATTR_accent )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_munderover), idx) );
	//

	//
	// Element: mtable
	//
	static const int commonAttrsLength_Mtable = 19;
	static QString commonAttrs_Mtable[ commonAttrsLength_Mtable ] = 
		{FBL_ATTR_align, FBL_ATTR_rowalign, FBL_ATTR_columnalign,
		 FBL_ATTR_groupalign, FBL_ATTR_alignmentscope, FBL_ATTR_columnwidth,
		 FBL_ATTR_rowspacing, FBL_ATTR_columnspacing, FBL_ATTR_rowlines,
		 FBL_ATTR_columnlines, FBL_ATTR_frame, FBL_ATTR_framespacing,
		 FBL_ATTR_equalrows, FBL_ATTR_equalcolumns, FBL_ATTR_side,
		 FBL_ATTR_minlabelspacing, _T("table.width"), _T("table.displaystyle"), FBL_ATTR_color};
	//
	for( j = 0; j < commonAttrsLength_Mtable; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mtable[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mtable), idx) );
			if( commonAttrs_Mtable[ j ] != _T("table.width") )
				::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//

	//
	// Element: mtr
	//
	static const int commonAttrsLength_Mtr = 3;
	static QString commonAttrs_Mtr[ commonAttrsLength_Mtr ] = 
		{FBL_ATTR_rowalign, FBL_ATTR_columnalign, FBL_ATTR_groupalign};
	//
	for( j = 0; j < commonAttrsLength_Mtr; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mtr[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mtr), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//

	//
	// Element: mtd
	//
	static const int commonAttrsLength_Mtd = 5;
	static QString commonAttrs_Mtd[ commonAttrsLength_Mtd ] = 
		{FBL_ATTR_rowspan, FBL_ATTR_columnspan, FBL_ATTR_rowalign, FBL_ATTR_columnalign, FBL_ATTR_groupalign};
	//
	for( j = 0; j < commonAttrsLength_Mtd; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mtd[ j ] )) != -1 )
		{
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_mtd), idx) );
			::addAttr2MStyle( mstyleAttr, m_tag2attrno, idx );
		}
	}
	//

	//
	// Element: malignmark
	//
	if( (idx = getParameterIndex( FBL_ATTR_edge )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_malignmark), idx) );
	//

	//
	// Element: maligngroup
	//
	if( (idx = getParameterIndex( FBL_ATTR_groupalign )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_maligngroup), idx) );
	//

	//
	// Element: maction
	//
	if( (idx = getParameterIndex( FBL_ATTR_actiontype )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_maction), idx) );
	if( (idx = getParameterIndex( FBL_ATTR_selection )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_maction), idx) );
	//
    ///////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////
	// Content
	//
	static const int nCElem_cb = TAG2MML_infinity - TAG2MML_cn + 1;
	QString nameCElem[ nCElem_cb ];
	for( i = 0; i < nCElem_cb; i++ )
		nameCElem[ i ] = ::mml_getTagID2TagName( (enum TAG2MML)(i + TAG2MML_cn) );
	//
	static const int commonAttrsLength_C = 2;
	static QString commonAttrs_C[ commonAttrsLength_C ] = {FBL_ATTR_encoding, FBL_ATTR_definitionURL};
	//
	for( j = 0; j < commonAttrsLength_C; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_C[ j ] )) == -1 )
			continue;
		for( i = 0; i < nCElem_cb; i++ )
			m_tag2attrno.insert( std::pair<QString, long>(nameCElem[ i ], idx) );
	}
	//

	//
	// Element: cn
	//
	if( (idx = getParameterIndex( _T("cn.type") )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_cn), idx) );
	if( (idx = getParameterIndex( FBL_ATTR_base )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_cn), idx) );
	//

	//
	// Element: ci
	//
	if( (idx = getParameterIndex( _T("ci.type") )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_ci), idx) );
	//

	//
	// Element: set
	//
	if( (idx = getParameterIndex( _T("set.type") )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_set), idx) );
	//

	//
	// Element: tendsto
	//
	if( (idx = getParameterIndex( _T("tendsto.type") )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_tendsto), idx) );
	//

	//
	// Element: declare
	//
	static const int commonAttrsLength_Mdeclare = 4;
	static QString commonAttrs_Mdeclare[ commonAttrsLength_Mdeclare ] = 
		{ _T("declare.type"), FBL_ATTR_nargs, FBL_ATTR_occurrence, FBL_ATTR_scope };
	//
	for( j = 0; j < commonAttrsLength_Mdeclare; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs_Mdeclare[ j ] )) != -1 )
			m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_declare), idx) );
	}
	//

	//
	// Element: interval
	//
	if( (idx = getParameterIndex( FBL_ATTR_closure )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_interval), idx) );
	//

	//
	// Element: list
	//
	if( (idx = getParameterIndex( FBL_ATTR_order )) != -1 )
		m_tag2attrno.insert( std::pair<QString, long>(QString(FBL_TAG_list), idx) );
	//
    ///////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////
	// Common attributes
	//
	for( j = 0; j < commonAttrsLength; j++ )
	{
		if( (idx = getParameterIndex( commonAttrs[ j ] )) == -1 )
			continue;
		for( i = 0; i < nPToken_cb; i++ )
			m_tag2attrno.insert( std::pair<QString, long>(namePToken[ i ], idx) );
		for( i = 0; i < nPElem_cb; i++ )
			m_tag2attrno.insert( std::pair<QString, long>(namePElem[ i ], idx) );
		for( i = 0; i < nCElem_cb; i++ )
			m_tag2attrno.insert( std::pair<QString, long>(nameCElem[ i ], idx) );
	}
	//
    ///////////////////////////////////////////////////////////////////////////
}

void CMMLNodeAttrList::getTag2AttrList( QString tagname, std::vector<long>& attrIdx )
{
	attrIdx.clear();

	std::multimap<QString, long>::const_iterator fndIter = m_tag2attrno.find( tagname );
	if( fndIter == m_tag2attrno.end() ) return;
	do
	{
		Q_ASSERT( fndIter->first == tagname );
		Q_ASSERT( fndIter->second < (long) getParameterNumber() );

		attrIdx.push_back( fndIter->second );
		fndIter++;
	}
	while( fndIter != m_tag2attrno.end() && fndIter->first == tagname );
}

int CMMLNodeAttrList::isSupportedTag2Attr( QString tagName, QString attrName )
{
	CNode_TextPropertyData *data;
	std::multimap<QString, long>::const_iterator fndIter = m_tag2attrno.find( tagName );
	if( fndIter == m_tag2attrno.end() ) return 0;
	do
	{
		Q_ASSERT( fndIter->first == tagName );
		Q_ASSERT( fndIter->second < (long) getParameterNumber() );
		if( (data = getParameter( fndIter->second )) == 0 )
			continue;
		if( data->m_name == attrName )
			return 1;
		fndIter++;
	}
	while( fndIter != m_tag2attrno.end() && fndIter->first == tagName );
	return 0;
}

///////////////////////////////////////////////////////////////////////////

CMMLNodeAttrList& getFullMathMLAttrList()
{
	static CMMLNodeAttrList fullAttrList;
	return fullAttrList;
}

///////////////////////////////////////////////////////////////////////////

__HERMITECH_FORMULIB__
QString convertAttrName2Key( QString tagName, QString attrName )
{
	int isQualified = 0;
	enum TAG2MML tagid = ::mml_getTag2MML( tagName );
	switch( tagid )
	{
	case TAG2MML_mtable:
		if( attrName == FBL_ATTR_width || attrName == FBL_ATTR_displaystyle ) isQualified = 1;
		break;
	case TAG2MML_cn:
		if( attrName == FBL_ATTR_type ) isQualified = 1;
		break;
	case TAG2MML_ci:
		if( attrName == FBL_ATTR_type ) isQualified = 1;
		break;
	case TAG2MML_set:
		if( attrName == FBL_ATTR_type ) isQualified = 1;
		break;
	case TAG2MML_declare:
		if( attrName == FBL_ATTR_type ) isQualified = 1;
		break;
	case TAG2MML_tendsto:
		if( attrName == FBL_ATTR_type ) isQualified = 1;
		break;
	default:
		break;
	}
	return isQualified ? (tagName + _T(".") + attrName) : attrName;
}

///////////////////////////////////////////////////////////////////////////
