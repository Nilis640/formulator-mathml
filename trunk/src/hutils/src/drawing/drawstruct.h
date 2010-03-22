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

#if !defined( __HUTILS_DRAWING_DRAWSTRUCT_H__ )
#define __HUTILS_DRAWING_DRAWSTRUCT_H__

#include "../hutils_global.h"

#include <QMap>
#include <QVector>
#include <QString>
#include <QDataStream>
#include <QByteArray>
#include <QColor>
#include <QPen>
#include <QFont>

#include <cstdlib>
#include <algorithm>

/////////////////////////////////////////////////////////////////////
// fde types

typedef qreal RealFde;
typedef QPointF PointFde;
typedef QSizeF SizeFde;
typedef QRectF RectFde;

/////////////////////////////////////////////////////////////////////

#define spln_point_type_clsfig			0x01
#define spln_point_type_lineto			0x02
#define spln_point_type_splnto			0x04
#define spln_point_type_moveto			0x06

/////////////////////////////////////////////////////////////////////
//
//
// font structures
//
//
/////////////////////////////////////////////////////////////////////

inline
bool operator < ( const QColor& a, const QColor& b )
{
	if( a.alpha() > b.alpha() ) return false;
	else if( a.alpha() < b.alpha() ) return true;
	if( a.red() > b.red() ) return false;
	else if( a.red() < b.red() ) return true;
	if( a.green() > b.green() ) return false;
	else if( a.green() < b.green() ) return true;
	if( a.blue() > b.blue() ) return false;
	else if( a.blue() < b.blue() ) return true;
	return false;
}

class FS_LogFont_Color
{
public:
	FS_LogFont_Color( const QString& name = "", bool pitch = false, bool bold = false, bool italic = false, QColor color = QColor(0, 0, 0), enum QFont::StyleHint hint = QFont::AnyStyle )
	{
		m_isFixed = pitch;
		m_isBold = bold;
		m_isItalic = italic;
		m_color = color;
		m_name = name;
		m_hint = hint;
	}
	FS_LogFont_Color( const FS_LogFont_Color& lf )
	{
		m_isFixed = lf.m_isFixed;
		m_isBold = lf.m_isBold;
		m_isItalic = lf.m_isItalic;
		m_color = lf.m_color;
		m_name = lf.m_name;
		m_hint = lf.m_hint;
	}

	const FS_LogFont_Color& operator = ( const FS_LogFont_Color& lf )
	{
		m_isFixed = lf.m_isFixed;
		m_isBold = lf.m_isBold;
		m_isItalic = lf.m_isItalic;
		m_color = lf.m_color;
		m_name = lf.m_name;
		m_hint = lf.m_hint;
		return *this;
	}

	bool operator == ( const FS_LogFont_Color& lf ) const
	{
		return m_isFixed == lf.m_isFixed && m_isBold == lf.m_isBold && m_isItalic == lf.m_isItalic &&
				m_color == lf.m_color && m_name == lf.m_name /*&& m_hint == lf.m_hint*/;
	}

	bool operator < ( const FS_LogFont_Color& lf ) const
	{
		if( m_isFixed && !lf.m_isFixed ) return false;
		else if( !m_isFixed && lf.m_isFixed ) return true;

		if( m_isBold && !lf.m_isBold ) return false;
		else if( !m_isBold && lf.m_isBold ) return true;

		if( m_isItalic && !lf.m_isItalic ) return false;
		else if( !m_isItalic && lf.m_isItalic ) return true;

		if( m_name > lf.m_name ) return false;
		else if( m_name < lf.m_name ) return true;

		//if( m_hint > lf.m_hint ) return false;
		//else if( m_hint < lf.m_hint ) return true;

		return m_color < lf.m_color;
	}

	QColor	getColor() const { return m_color; }

public:
	QString	m_name;
	bool	m_isFixed, m_isBold, m_isItalic;
	enum QFont::StyleHint m_hint;
	QColor	m_color;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FS_LogFont_Color& lf )
{
	qint32 v = lf.m_hint;
	return ar << lf.m_name << lf.m_isFixed << lf.m_isBold << lf.m_isItalic << lf.m_color << v;
}

inline
QDataStream& operator >> ( QDataStream& ar, FS_LogFont_Color& lf )
{
	qint32 v;
	QDataStream& ret = ar >> lf.m_name >> lf.m_isFixed >> lf.m_isBold >> lf.m_isItalic >> lf.m_color >> v;
	lf.m_hint = (QFont::StyleHint) v;
	return ret;
}

/////////////////////////////////////////////////////////////////////

class FS_LogFont : public FS_LogFont_Color
{
public:
	FS_LogFont( void )
		: FS_LogFont_Color()
	{
		m_size = 0;
	}

	FS_LogFont( const FS_LogFont_Color& lf, RealFde size = 0 )
		: FS_LogFont_Color( lf )
	{
		m_size = size;
	}

	const FS_LogFont& operator = ( const FS_LogFont& lf )
		{ FS_LogFont_Color::operator = ( lf ); m_size = lf.m_size; return *this; }

	bool operator == ( const FS_LogFont& lf ) const 
		{ return FS_LogFont_Color::operator == ( lf ) && m_size == lf.m_size; }

	bool operator < ( const FS_LogFont& lf ) const
	{
		if( m_size < lf.m_size ) return true;
		else if( m_size > lf.m_size ) return false;

		return (FS_LogFont_Color)*this < (FS_LogFont_Color)lf;
	}

	RealFde getPixelSize() const { return m_size; }

public:
	RealFde		m_size;		// em font size
};

inline
QDataStream& operator << ( QDataStream& ar, const FS_LogFont& lf )
	{ return ar << lf.m_name << lf.m_isFixed << lf.m_isBold << lf.m_isItalic << lf.m_color << lf.m_size; }
inline
QDataStream& operator >> ( QDataStream& ar, FS_LogFont& lf )
	{ return ar >> lf.m_name >> lf.m_isFixed >> lf.m_isBold >> lf.m_isItalic >> lf.m_color >> lf.m_size; }

/////////////////////////////////////////////////////////////////////	
//
//
// pen drawing structures
//
//
/////////////////////////////////////////////////////////////////////

struct FS_LogPen
{
public:
	FS_LogPen( QColor color = QColor(0, 0, 0), Qt::PenStyle style = Qt::SolidLine, RealFde width = 1.0f ) 
	{
		m_color = color;
		m_style = style;
		m_width = width;
	}

	RealFde getWidth() const { return m_width; }
	Qt::PenStyle getStyle() const { return m_style; }
	QColor	getColor() const { return m_color; }

	bool operator == ( const FS_LogPen& lb ) const 
        { return /*std::*/memcmp( this, &lb, sizeof( FS_LogPen ) ) == 0; }
	bool operator < ( const FS_LogPen& lb ) const 
        { return /*std::*/memcmp( this, &lb, sizeof( FS_LogPen ) ) < 0; }

public:
	QColor			m_color;
	Qt::PenStyle	m_style;
	RealFde			m_width;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FS_LogPen& lp )
{
	qint32 v = lp.m_style;
	return ar << v << lp.m_width << lp.m_color;
}

inline
QDataStream& operator >> ( QDataStream& ar, FS_LogPen& lp )
{
	qint32 v;
	ar >> v;
	lp.m_style = (Qt::PenStyle) v;
	return ar >> lp.m_width >> lp.m_color;
}

/////////////////////////////////////////////////////////////////////
//
//
// brush drawing structures
//
//
/////////////////////////////////////////////////////////////////////

struct FS_LogBrush
{
public:
	FS_LogBrush( QColor color = QColor(0, 0, 0), Qt::BrushStyle style = Qt::NoBrush/*Qt::SolidPattern*/ ) 
	{
		m_color = color;
		m_style = style;
	}

	Qt::BrushStyle getStyle() const { return m_style; }
	QColor getColor() const { return m_color; }

	bool operator == ( const FS_LogBrush& lb ) const 
        { return /*std::*/memcmp( this, &lb, sizeof( FS_LogBrush ) ) == 0; }
	bool operator < ( const FS_LogBrush& lb ) const 
        { return /*std::*/memcmp( this, &lb, sizeof( FS_LogBrush ) ) < 0; }

public:
    QColor			m_color;
	Qt::BrushStyle	m_style;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FS_LogBrush& lb )
{
	qint32 v = lb.m_style;
	return ar << lb.m_color << v;
}

inline
QDataStream& operator >> ( QDataStream& ar, FS_LogBrush& lb )
{
	qint32 v;
	QDataStream& ret = ar >> lb.m_color >> v;
	lb.m_style = (Qt::BrushStyle) v;
	return ret;
}

/////////////////////////////////////////////////////////////////////
//
//
// image drawing structures
//
//
/////////////////////////////////////////////////////////////////////

struct FS_LogImage
{
public:
	FS_LogImage( const QString& path = QString("") )
	{
		m_ref = 1;
		m_path = path;
	}

	FS_LogImage( const FS_LogImage& lb )
	{
		m_ref = lb.m_ref;
		m_path = lb.m_path;
	}

	const FS_LogImage& operator = ( const FS_LogImage& lb )
		{ m_ref = lb.m_ref; m_path = lb.m_path; return *this; }

	bool operator == ( const FS_LogImage& lb ) const 
		{ return m_path == lb.m_path; }
	bool operator < ( const FS_LogImage& lb ) const 
		{ return m_path < lb.m_path; }

public:
	QString		m_path;
	qint32		m_ref;
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FS_LogImage& lb )
{
	return ar << lb.m_path;
}

inline
QDataStream& operator >> ( QDataStream& ar, FS_LogImage& lb )
{
	return ar >> lb.m_path;
}

/////////////////////////////////////////////////////////////////////

struct SYSINFO_Unit2PX
{
	RealFde em2px, ex2px, px2px;
	RealFde in2px_x, cm2px_x, mm2px_x, pt2px_x, pc2px_x;
	RealFde in2px_y, cm2px_y, mm2px_y, pt2px_y, pc2px_y;
};

/////////////////////////////////////////////////////////////////////

inline
const RectFde& assign( RectFde& rect, const PointFde& pt ) 
{
	rect.setLeft( pt.x() );
	rect.setRight( pt.x() );
	rect.setTop( pt.y() );
	rect.setBottom( pt.y() );
	return rect;
}

inline
const RectFde& assign( RectFde& rect, const SizeFde& sz ) 
{
	rect.setRight( rect.left() + sz.width() );
	rect.setBottom( rect.top() + sz.height() );
	return rect;
}

inline
const RectFde& operator += ( RectFde& rect, const PointFde& pt ) 
{
	rect.translate( pt );
	return rect;
}

inline
const RectFde& operator -= ( RectFde& rect, const PointFde& pt ) 
{
	rect.translate( - pt );
	return rect;
}

inline
const RectFde& operator += ( RectFde& rect, const SizeFde& sz ) 
{
	rect.setRight( rect.right() + sz.width() );
	rect.setBottom( rect.bottom() + sz.height() );
	return rect;
}

inline
const RectFde& operator -= ( RectFde& rect, const SizeFde& sz ) 
{
	rect.setRight( rect.right() - sz.width() );
	rect.setBottom( rect.bottom() - sz.height() );
	return rect;
}

/////////////////////////////////////////////////////////////////////

#endif
