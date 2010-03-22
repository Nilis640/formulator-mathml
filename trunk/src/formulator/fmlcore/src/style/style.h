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

#if !defined( __FORMULATOR_STYLE_STYLE_H__ )
#define __FORMULATOR_STYLE_STYLE_H__

/////////////////////////////////////////////////////////////////////

#include <QVector>
#include <algorithm>

#include "HUtils/ihkstring.h"
#include "HUtils/idrawstruct.h"

#include "../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////

#define LINETHICKNESS_MEDIUM			1.0
#define LINETHICKNESS_THIN				1.0
#define LINETHICKNESS_THICK				2.0
#define LINETHICKNESS_DEFAULT			LINETHICKNESS_MEDIUM

#define FSTYLE_OTHER_STYLE				-4
#define FSTYLE_TEXT_BUTTON				-3
#define FSTYLE_UNICODE					-2
#define FSTYLE_NONE						-1
#define FSTYLE_GREEK					0
#define FSTYLE_FIXED					1
#define FSTYLE_VARIABLE					2
#define FSTYLE_VECTOR					3
#define FSTYLE_NUMBER					4
#define FSTYLE_KEYWORD					5
#define FSTYLE_REMARK					6
#define FSTYLE_FUNCTION					7
#define FSTYLE_OP						8
#define FSTYLE_TEXT						9
#define FSTYLE_MTEXTRA					10
#define FSTYLE_USER1					11
#define FSTYLE_USER2					12
#define FSTYLE_DEFAULT					FSTYLE_NUMBER

#define FSTYLE_DEFAULT_ONCHAR			FSTYLE_DEFAULT
#define FSTYLE_UNICODE_DEFAULT			FSTYLE_MTEXTRA

#define FSTYLE_DEFAULT_FONT_NUMBER				13
#define FSTYLE_DEFAULT_MAX_LEVEL_PLAINTEXT		4
#define FSTYLE_DEFAULT_MAX_LEVEL_FORMULATEXT	4

#define FTEXT_HEIGHT_AUTO				-2
#define FTEXT_HEIGHT_LARGEOP			-1
#define FTEXT_HEIGHT_FRAME				0
#define FTEXT_HEIGHT_CHILD1				1
#define FTEXT_HEIGHT_CHILD2				2

#define FTEXT_HEIGHT_LAST				FTEXT_HEIGHT_CHILD2

#define FKEGL_LARGEOP					17.0
#define FKEGL_FRAME						12.0
#define FKEGL_CHILD1					9.0
#define FKEGL_CHILD2					6.0

#define FTEXT_STYLE_DIVIDER				'$'

#define DEFAULT_GRAPH_WIDTH				LINETHICKNESS_DEFAULT
#define DEFAULT_GRAPH_COLOR				QColor( 0, 0, 0 )
#define DEFAULT_GRAPH_STYLE				Qt::SolidLine

/////////////////////////////////////////////////////////////////////

inline
int isFStyleHasReservedRecord( long fstyle )
	{ return fstyle > FSTYLE_NONE && fstyle < FSTYLE_DEFAULT_FONT_NUMBER; }

/////////////////////////////////////////////////////////////////////
//
// class FS_TextStyles
//
/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FS_TextStyles
{
protected:
	QVector<QString>			m_names;		// storable
	QVector<FS_LogFont_Color>	m_logFonts;		// storable
	QVector<qreal>				m_defKegl;		// storable

	FS_LogPen					m_logPen;		// storable
	FS_LogBrush					m_logBrush;		// storable

	QVector<SizeFde>			m_defSize;		// recalc
	QVector<RealFde>			m_defAscent;	// recalc
	QVector<RealFde>			m_defDescent;	// recalc
	SYSINFO_Unit2PX				m_unit2px;		// recalc

public:
	FS_TextStyles( void );
	FS_TextStyles( const FS_TextStyles& fs );
	virtual ~FS_TextStyles( void );

	const FS_TextStyles& operator = ( const FS_TextStyles& fs );

	void InitDefaults( void );
	void updateDefaults( void );

public:
	static long getStyleByChar( QChar nChar );
	static long getStyleByCharOnly( QChar nChar );
	static long getMathMLTag2Style( QString& body, QString& tagname );

public:

	//
	// styles function
	//

	long getRegStyleNumber( void );
	long getStyleByName( const QString& name );
	QString getStyleName( long style );

	int isFaceNameCriticalDrawing( long st );

	// font styles
	QVector<FS_LogFont_Color>& getAllFontStyles( void );

	FS_LogFont_Color& getFontStyle( long style );
	FS_LogFont_Color& getFontStyle( const QString& name );

	// other font function
	long getStyleNo( const FS_LogFont_Color& lf, long style_2_search = -1, bool forward = true );
	long addStyle( const FS_LogFont_Color& lf, const QString& name = _T("") );

	//
	// size 
	//

	RealFde getKegl( long level );
	void setKegl( long level, RealFde kegl );
	void kegl2LevelOffset( RealFde kegl, qint32& level, RealFde& offset );
	void kegl2LevelOffset( RealFde kegl, QVector<long>& level, RealFde& offset );

	const SYSINFO_Unit2PX& getUnit2PX( void );

	RealFde kegl2pixel( RealFde kegl, bool vert = true );
	RealFde getEM( void );
	RealFde getEX( void );

	//
	// line settings
	//

	const FS_LogPen& getLogPen( void );
	void setLogPen( const FS_LogPen& lp );

	const FS_LogBrush& getLogBrush( void );
	void setLogBrush( const FS_LogBrush& lb );

	//
	// default settings
	//

	const SizeFde& getDefaultSize( long level );
	RealFde getDefaultAscent( long level );
	RealFde getDefaultDescent( long level );

	friend QDataStream& operator << ( QDataStream& ar, const FS_TextStyles& lp );
	friend QDataStream& operator >> ( QDataStream& ar, FS_TextStyles& lp );
};

/////////////////////////////////////////////////////////////////////

inline
long FS_TextStyles::getRegStyleNumber( void )
	{ return (m_logFonts.size() == m_names.size() ? (long) m_logFonts.size() : 0); }

inline
QString FS_TextStyles::getStyleName( long style )
	{ return(style >= 0 && style < (long) m_names.size() ? m_names[ style ] : QString()); }

inline
long FS_TextStyles::getStyleByName( const QString& name )
{
	int idx = m_names.indexOf(name);
	return (idx == -1) ? FSTYLE_NONE : idx;
}

inline
QVector<FS_LogFont_Color>& FS_TextStyles::getAllFontStyles( void )
	{ return m_logFonts; }

///////////////////////////////////////////////////////////////////////////////

inline
FS_LogFont_Color& FS_TextStyles::getFontStyle( long style )
	{ return (style >= 0 && style < getRegStyleNumber() ? m_logFonts[ style ] : m_logFonts[ FSTYLE_DEFAULT ]); }

inline
FS_LogFont_Color& FS_TextStyles::getFontStyle( const QString& name )
{
	long i = getStyleByName( name );
	return getFontStyle( i == FSTYLE_NONE ? FSTYLE_DEFAULT : i );
}

///////////////////////////////////////////////////////////////////////////////

inline
const SYSINFO_Unit2PX& FS_TextStyles::getUnit2PX( void )
	{ return m_unit2px; }

inline
RealFde FS_TextStyles::getEM( void )
	{ return getUnit2PX().em2px; }

inline
RealFde FS_TextStyles::getEX( void )
	{ return getUnit2PX().ex2px; }

inline
RealFde FS_TextStyles::kegl2pixel( RealFde kegl, bool vert )
{
	Q_ASSERT( getUnit2PX().pt2px_x != 0.0 && getUnit2PX().pt2px_y != 0.0 );
	return vert ? kegl * getUnit2PX().pt2px_y : kegl * getUnit2PX().pt2px_x;
}

///////////////////////////////////////////////////////////////////////////////

inline
RealFde FS_TextStyles::getKegl( long level )
{
	unsigned long levelAdjusted = level + 1;
	return m_defKegl[ std::min( levelAdjusted, (unsigned long)(FSTYLE_DEFAULT_MAX_LEVEL_PLAINTEXT - 1)) ];
}

inline
void FS_TextStyles::setKegl( long level, RealFde kegl )
{
	unsigned long levelAdjusted = level + 1;
	m_defKegl[ std::min( levelAdjusted, (unsigned long)(FSTYLE_DEFAULT_MAX_LEVEL_PLAINTEXT - 1)) ] = kegl;
}

///////////////////////////////////////////////////////////////////////////////
// line settings

inline
const FS_LogPen& FS_TextStyles::getLogPen( void )
	{ return m_logPen; }
inline
void FS_TextStyles::setLogPen( const FS_LogPen& lp )
	{ m_logPen = lp; }

inline
const FS_LogBrush& FS_TextStyles::getLogBrush( void )
	{ return m_logBrush; }
inline
void FS_TextStyles::setLogBrush( const FS_LogBrush& lb )
	{ m_logBrush = lb; }

///////////////////////////////////////////////////////////////////////////////

inline
const SizeFde& FS_TextStyles::getDefaultSize( long level )
	{ return m_defSize[ std::min( long(level + 1), long(FSTYLE_DEFAULT_MAX_LEVEL_PLAINTEXT - 1)) ]; }

inline
RealFde FS_TextStyles::getDefaultAscent( long level )
	{ return m_defAscent[ std::min( long(level + 1), long(FSTYLE_DEFAULT_MAX_LEVEL_PLAINTEXT - 1)) ]; }

inline
int FS_TextStyles::isFaceNameCriticalDrawing( long st )
	{ return /*st == FSTYLE_GREEK || */st == FSTYLE_MTEXTRA; }

///////////////////////////////////////////////////////////////////////////////

#endif //__FORMULATOR_STYLE_STYLE_H__
