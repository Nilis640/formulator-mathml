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

#include <cstdlib>
#include <float.h>

#include "HUtils/idrawing.h"
#include "style.h"
#include "../settings/options.h"
#include "../mathml/mml_tag2mml.h"

///////////////////////////////////////////////////////////////////////////////

FS_TextStyles::FS_TextStyles( void ) 
	: m_names()
	, m_logFonts()
	, m_defKegl()
	, m_logPen()
	, m_logBrush()
	, m_defSize()
	, m_defAscent()
	, m_defDescent()
{
	InitDefaults();
}

FS_TextStyles::FS_TextStyles( const FS_TextStyles& fs )
	: m_names()
	, m_logFonts()
	, m_defKegl()
	, m_logPen()
	, m_logBrush()
	, m_defSize()
	, m_defAscent()
	, m_defDescent()
{
	*this = fs;
}

FS_TextStyles::~FS_TextStyles( void )
{
}

///////////////////////////////////////////////////////////////////////////////

const FS_TextStyles& FS_TextStyles::operator = ( const FS_TextStyles& fs )
{
	m_names = fs.m_names;
	m_logFonts = fs.m_logFonts;
	m_defKegl = fs.m_defKegl;
	m_unit2px = fs.m_unit2px;
	m_defSize = fs.m_defSize;
	m_defAscent = fs.m_defAscent;
	m_defDescent = fs.m_defDescent;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////

void FS_TextStyles::InitDefaults( void )
{
	//

	m_names.clear();
	m_names.push_back( _T("greek") );
	m_names.push_back( _T("fixed") ); 
	m_names.push_back( _T("variable") ); 
	m_names.push_back( _T("vector") ); 
	m_names.push_back( _T("number") ); 
	m_names.push_back( _T("keyword") ); 
	m_names.push_back( _T("remark") );
	m_names.push_back( _T("function") );
	m_names.push_back( _T("op") );
	m_names.push_back( _T("text") );
	m_names.push_back( _T("mathextra") );
	m_names.push_back( _T("user-1") );
	m_names.push_back( _T("user-2") );

	m_logFonts.clear();
	m_logFonts.push_back( FS_LogFont_Color( _T("Symbol"),				false,	false,	false,	QColor(   0,   0,   0 ), QFont::AnyStyle ) );	// FSTYLE_GREEK   	
	m_logFonts.push_back( FS_LogFont_Color( _T("Courier New"),			true,	false,	false,	QColor(   0,   0,   0 ), QFont::Courier ) );	// FSTYLE_FIXED   	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	true,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_VARIABLE	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	true,	false,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_VECTOR  	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	false,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_NUMBER  	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	true,	false,	QColor(   0,   0, 255 ), QFont::Times ) );	// FSTYLE_KEYWORD 	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	false,	QColor(   0, 128,   0 ), QFont::Times ) );	// FSTYLE_REMARK  	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	false,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_FUNCTION	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	false,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_OP      	
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	false,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_TEXT    	
	m_logFonts.push_back( FS_LogFont_Color( _T("Lucida Sans Unicode"),	false,	false,	false,	QColor(   0,   0,   0 ), QFont::AnyStyle ) );	// FSTYLE_MTEXTRA 	
	//m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),	false,	false,	false,	QColor(   0,   0,   0 ), QFont::AnyStyle ) );	// FSTYLE_MTEXTRA 	
	m_logFonts.push_back( FS_LogFont_Color( _T("Courier New"),			true,	false,	false,	QColor(   0,   0,   0 ), QFont::Courier ) );	// FSTYLE_USER1   	 
	m_logFonts.push_back( FS_LogFont_Color( _T("Times New Roman"),		false,	false,	true,	QColor(   0,   0,   0 ), QFont::Times ) );	// FSTYLE_USER2

	m_defKegl.clear();
	m_defKegl.push_back( FKEGL_LARGEOP );
	m_defKegl.push_back( FKEGL_FRAME );
	m_defKegl.push_back( FKEGL_CHILD1 );
	m_defKegl.push_back( FKEGL_CHILD2 );

	updateDefaults();
}

///////////////////////////////////////////////////////////////////////////////

void FS_TextStyles::updateDefaults()
{
	FS_LogFont_Color lfc = getFontStyle( FSTYLE_VARIABLE );
	QFont font( lfc.m_name, -1, lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
	font.setPointSizeF( getKegl( FTEXT_HEIGHT_FRAME ) );
	font.setFixedPitch( lfc.m_isFixed );
	font.setStyleHint( lfc.m_hint );
	QFontInfo fi( font );
	QFontMetricsF fm( font );

	CFmlDrawEngineQt::GetUnitsEx_( m_unit2px, font, fi, fm );

	m_defSize.clear();
	m_defAscent.clear();
	m_defDescent.clear();
	QVector<RealFde> keglToMeasure;
	for( long i = 0; i < FSTYLE_DEFAULT_MAX_LEVEL_PLAINTEXT; i++ )
	{
		//font.setPixelSize( (int) kegl2pixel( getKegl( i - 1 ) ) );
		font.setPointSizeF( getKegl( i - 1 ) );
		QFontMetricsF fmex( font );
		m_defSize.push_back( SizeFde( fmex.width(QChar('x')), fmex.height() ) );
		m_defAscent.push_back( fmex.ascent() );
		m_defDescent.push_back( fmex.descent() );
	}
}

///////////////////////////////////////////////////////////////////////////////

void FS_TextStyles::kegl2LevelOffset( RealFde kegl, qint32& level, RealFde& offset )
{
	level = -1;
	offset = FLT_MAX;

	RealFde dist;
	for( long i = 0; i < m_defKegl.size(); i++ )
	{
		dist = kegl - m_defKegl.at(i);
		if( fabs( offset ) > fabs( dist ) )
		{
			level = i - 1;
			offset = dist;
		}
	}
}

void FS_TextStyles::kegl2LevelOffset( RealFde kegl, QVector<long>& level, RealFde& offset )
{
	level.clear();

	qint32 value;
	kegl2LevelOffset( kegl, value, offset );

	for( long i = 0; i < m_defKegl.size(); i++ )
		if( fabs( offset ) == fabs( kegl - m_defKegl.at(i) ) )
			level.push_back( i - 1 );
}

///////////////////////////////////////////////////////////////////////////////

long FS_TextStyles::getStyleNo( const FS_LogFont_Color& lfs, long style_2_search, bool forward )
{
	QVector<long> stt;
	if( forward )
	{
		for( long i = 0; i < (long) m_logFonts.size(); i++ )
		{
			if( m_logFonts[ i ] == lfs )
			{
				if( style_2_search == -1 || style_2_search == i )
					return i;
				else
					stt.push_back( i );
			}
		}
	}
	else
	{
		for( long i = (long) m_logFonts.size() - 1; i >= 0; i-- )
		{
			if( m_logFonts[ i ] == lfs )
			{
				if( style_2_search == -1 || style_2_search == i )
					return i;
				else
					stt.push_back( i );
			}
		}
	}

	return (stt.empty() ? -1 : stt.at(0));
}

long FS_TextStyles::addStyle( const FS_LogFont_Color& lfs, const QString& name )
{
	long ret = -1;
	if( m_logFonts.size() == m_names.size() )
	{
		ret = (long) m_names.size();
		m_names.push_back( name );
		m_logFonts.push_back( lfs );
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const FS_TextStyles& lp )
{
	QVector<FS_LogFont_Color> logFonts( lp.m_logFonts.mid(0, FSTYLE_DEFAULT_FONT_NUMBER) );
	QVector<QString> names( lp.m_names.mid(0, FSTYLE_DEFAULT_FONT_NUMBER) );
	return ar << lp.m_logBrush << lp.m_logPen << lp.m_defKegl << logFonts << names;
}

QDataStream& operator >> ( QDataStream& ar, FS_TextStyles& lp )
{
	QDataStream& ret = ar >> lp.m_logBrush >> lp.m_logPen >> lp.m_defKegl >> lp.m_logFonts >> lp.m_names;
	lp.updateDefaults();	// update font sizes
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Static

long FS_TextStyles::getStyleByChar( QChar nChar )
{
	return (::getCurrentUserChoiceStyle() != FSTYLE_NONE ? ::getCurrentUserChoiceStyle() : getStyleByCharOnly( nChar ));
}

long FS_TextStyles::getStyleByCharOnly( QChar nChar )
{
	static QString operators = _T("`~!@#$%^&*()-_=+\\|[]{};:'\",<.>/?");
	if( nChar.isLetter() )
		return FSTYLE_VARIABLE;
	else if( nChar.isDigit() )
		return FSTYLE_NUMBER;
	else if( nChar.isSpace() )
		return FSTYLE_TEXT;
	else if( operators.indexOf(nChar) != -1 )
		return FSTYLE_OP;
	return FSTYLE_TEXT;
}

long FS_TextStyles::getMathMLTag2Style( QString& body, QString& tagname ) 
{
	if( !xml_strcmp( tagname, FBL_TAG_mi ) )
	{
		if( body.length() == 0 )
			return FSTYLE_VARIABLE;
		else if( body.at(0).isLetter() && (body.length() == 1 || !::isOption_MulticharMINotItalic()) )
			return FSTYLE_VARIABLE;
		else
			return FSTYLE_FUNCTION;
	}
	else if( !xml_strcmp( tagname, FBL_TAG_mn ) )
		return FSTYLE_NUMBER;
	else if( !xml_strcmp( tagname, FBL_TAG_mo ) )
		return FSTYLE_OP;
	else if( !xml_strcmp( tagname, FBL_TAG_mtext ) || !xml_strcmp( tagname, FBL_TAG_ms ) || !xml_strcmp( tagname, FBL_TAG_mspace ) )
		return FSTYLE_TEXT;
	return FSTYLE_DEFAULT;
}

///////////////////////////////////////////////////////////////////////////////
