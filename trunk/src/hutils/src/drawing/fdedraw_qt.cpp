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

#include <QPrinter>
#include "fdedraw_qt.h"

/////////////////////////////////////////////////////////////////////
//
// constructor / destructor
//
/////////////////////////////////////////////////////////////////////

#define CFmlDrawEngineQt_TextFlags	(Qt::TextIncludeTrailingSpaces | Qt::TextSingleLine | Qt::TextDontClip)

CFmlDrawEngineQt::CFmlDrawEngineQt( QPaintDevice *device )
	: CFmlDrawEngine( device )
	, m_painter( device )
	, m_painterRef( m_painter )
{
	m_width = 0.0;
	m_height = 0.0;
	ClearMaps_();
	m_painterRef.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing, true );
	m_textFlags = CFmlDrawEngineQt_TextFlags;
	m_MatrixD = m_painter.deviceMatrix();
	m_MatrixI = m_MatrixD.inverted();
	m_transform.setMatrix( 
		m_MatrixD.m11(), m_MatrixD.m12(), 0, 
		m_MatrixD.m21(), m_MatrixD.m22(), 0, 
		m_MatrixD.dx(), m_MatrixD.dy(), 1 );
}

CFmlDrawEngineQt::CFmlDrawEngineQt()
	: CFmlDrawEngine()
	, m_painter()
	, m_painterRef( m_painter )
{
	m_width = 0.0;
	m_height = 0.0;
	ClearMaps_();
	m_textFlags = CFmlDrawEngineQt_TextFlags;
	m_MatrixD = m_painter.deviceMatrix();
	m_MatrixI = m_MatrixD.inverted();
}

CFmlDrawEngineQt::CFmlDrawEngineQt( QPainter& painter )
	: CFmlDrawEngine()
	, m_painter()	// dummy instance
	, m_painterRef( painter )
{
	m_width = 0.0;
	m_height = 0.0;
	ClearMaps_();
	m_painterRef.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing, true );
	m_textFlags = CFmlDrawEngineQt_TextFlags;
	m_MatrixD = m_painterRef.deviceMatrix();
	m_MatrixI = m_MatrixD.inverted();
	m_transform.setMatrix( 
		m_MatrixD.m11(), m_MatrixD.m12(), 0, 
		m_MatrixD.m21(), m_MatrixD.m22(), 0, 
		m_MatrixD.dx(), m_MatrixD.dy(), 1 );
}

/////////////////////////////////////////////////////////////////////

CFmlDrawEngineQt::~CFmlDrawEngineQt( void )
{
	ClearMaps_();
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// 
// primitive functions
//
/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::FillRectangle( const RectFde& rect, const FS_LogBrush& lb )
{
	const QBrush *pBrush = GetBrush_( lb );
	if( pBrush ) m_painterRef.fillRect( rect, *pBrush );
}

void CFmlDrawEngineQt::FillRectangle( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogBrush& lb )
{
	const QBrush *pBrush = GetBrush_( lb );
	if( pBrush ) m_painterRef.fillRect( RectFde( x, y, w, h ), *pBrush );
}

void CFmlDrawEngineQt::DrawEllipse( const RectFde& rect, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		m_painterRef.setPen( *pPen );
		m_painterRef.drawEllipse( rect );
	}
}

void CFmlDrawEngineQt::DrawEllipse( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		m_painterRef.setPen( *pPen );
		m_painterRef.drawEllipse( RectFde( x, y, w, h ) );
	}
}

void CFmlDrawEngineQt::DrawLine( RealFde x1, RealFde y1, RealFde x2, RealFde y2, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		QPointF a( x1, y1 ), b( x2, y2 );
		m_painterRef.setPen( *pPen );
		m_painterRef.drawLine( a, b );
	}
}

void CFmlDrawEngineQt::DrawArc( const RectFde& rect, RealFde start, RealFde sweep, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		int startAngle = int(16 * start), spanAngle = int(16 * (start + sweep));
		m_painterRef.setPen( *pPen );
		m_painterRef.drawArc( rect, startAngle, spanAngle );
	}
}

void CFmlDrawEngineQt::DrawArc( RealFde x, RealFde y, RealFde w, RealFde h, RealFde start, RealFde sweep, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		int startAngle = int(16 * start);
		int spanAngle = int(16 * (start + sweep));
		m_painterRef.setPen( *pPen );
		m_painterRef.drawArc( RectFde( x, y, w, h ), startAngle, spanAngle );
	}
}

void CFmlDrawEngineQt::DrawString( RealFde x, RealFde y, const QString& text, const FS_LogFont& lf )
{
	const QBrush *pBrush = GetBrush_( lf );
	const QFont *pFont = GetFont_( lf );
	if( pBrush && pFont )
	{
		QPointF pos( x, y );
		m_painterRef.setBrush( *pBrush );
		m_painterRef.setPen( QPen( lf.m_color ) );
		m_painterRef.setFont( *pFont );
		m_painterRef.drawText( QRectF( pos, QSizeF(0, 0) ), m_textFlags, text );
	}
}

void CFmlDrawEngineQt::DrawRectangle( const RectFde& rect, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		m_painterRef.setPen( *pPen );
		m_painterRef.drawRect( rect );
	}
}

void CFmlDrawEngineQt::DrawRectangle( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogPen& lp )
{
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		m_painterRef.setPen( *pPen );
		m_painterRef.drawRect( RectFde( x, y, w, h ) );
	}
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::GetViewPort( RealFde& width, RealFde& height )
{
	width = m_width;
	height = m_height;
}

void CFmlDrawEngineQt::SetViewPort( RealFde width, RealFde height )
{
	m_width = width;
	m_height = height;
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::CFmlDrawEngineQt_ApplyMatrix()
{
	m_MatrixI = m_MatrixD.inverted();
	m_transform.setMatrix( 
		m_MatrixD.m11(), m_MatrixD.m12(), 0, 
		m_MatrixD.m21(), m_MatrixD.m22(), 0, 
		m_MatrixD.dx(), m_MatrixD.dy(), 1 );
	m_painterRef.setWorldTransform( m_transform );
}

void CFmlDrawEngineQt::Identity( void )
{
	m_MatrixD.reset();
	CFmlDrawEngineQt_ApplyMatrix();
}

void CFmlDrawEngineQt::Multiply( const FdeStateMatrix& sm )
{
	m_MatrixD *= QMatrix( sm.m_a11, sm.m_a12, sm.m_a21, sm.m_a22, sm.m_a31, sm.m_a32 );
	CFmlDrawEngineQt_ApplyMatrix();
}

void CFmlDrawEngineQt::Translate( RealFde tx, RealFde ty )
{
	m_MatrixD.translate( tx, ty );
	CFmlDrawEngineQt_ApplyMatrix();
}

void CFmlDrawEngineQt::Scale( RealFde sx, RealFde sy )
{
	m_MatrixD.scale( sx, sy );
	CFmlDrawEngineQt_ApplyMatrix();
}

void CFmlDrawEngineQt::Rotate( RealFde an )
{
	m_MatrixD.rotate( an );
	CFmlDrawEngineQt_ApplyMatrix();
}

void CFmlDrawEngineQt::GetMatrix( FdeStateMatrix& sm )
{
	sm.m_a11 = m_MatrixD.m11();
	sm.m_a12 = m_MatrixD.m12();
	sm.m_a21 = m_MatrixD.m21();
	sm.m_a22 = m_MatrixD.m22();
	sm.m_a31 = m_MatrixD.dx();
	sm.m_a32 = m_MatrixD.dy();
}

void CFmlDrawEngineQt::SetMatrix( const FdeStateMatrix& sm )
{
	m_MatrixD.setMatrix( sm.m_a11, sm.m_a12, sm.m_a21, sm.m_a22, sm.m_a31, sm.m_a32 );
	CFmlDrawEngineQt_ApplyMatrix();
}

void CFmlDrawEngineQt::PushMatrix( void )
{
	FdeStateMatrix sm;
	GetMatrix( sm );
	m_states.push( sm );
}

void CFmlDrawEngineQt::PopMatrix( void )
{
	SetMatrix( m_states.top() );
	m_states.pop();
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::LPtoDP( RealFde& x, RealFde& y )
{
	RealFde rx, ry;
	m_MatrixD.map( x, y, &rx, &ry );
	x = rx;
	y = ry;
}

void CFmlDrawEngineQt::LPtoDP( PointFde& pt )
{
	LPtoDP( pt.rx(), pt.ry() );
}

void CFmlDrawEngineQt::LPtoDP( RectFde& rc )
{
	RealFde left = rc.left(), top = rc.top(), right = rc.right(), bottom = rc.bottom();
	LPtoDP( left, top );
	LPtoDP( right, bottom );
	rc.setCoords( left, top, right, bottom );
}

void CFmlDrawEngineQt::LPtoDP( SizeFde& sz )
{
	LPtoDP( sz.rwidth(), sz.rheight() );
}

void CFmlDrawEngineQt::DPtoLP( RealFde& x, RealFde& y )
{
	RealFde rx, ry;
	m_MatrixI.map( x, y, &rx, &ry );
	x = rx;
	y = ry;
}

void CFmlDrawEngineQt::DPtoLP( PointFde& pt )
{
	DPtoLP( pt.rx(), pt.ry() );
}

void CFmlDrawEngineQt::DPtoLP( RectFde& rc )
{
	RealFde left = rc.left(), top = rc.top(), right = rc.right(), bottom = rc.bottom();
	DPtoLP( left, top );
	DPtoLP( right, bottom );
	rc.setCoords( left, top, right, bottom );
}

void CFmlDrawEngineQt::DPtoLP( SizeFde& sz )
{
	DPtoLP( sz.rwidth(), sz.rheight() );
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::Clear( const FS_LogBrush& lb )
{
	QRectF rect( 0.0, 0.0, m_width, m_height );
	const QBrush *pBrush = GetBrush_( lb );
	if( pBrush ) m_painterRef.fillRect ( rect, *pBrush );
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::Flush( void )
{
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::ClearClip( void )
{
	m_painterRef.setClipping( false );
}

void CFmlDrawEngineQt::SetClip( const RectFde& rc )
{
	m_painterRef.setClipRect( rc );
}

bool CFmlDrawEngineQt::HasClipping()
{
	return m_painterRef.hasClipping();
}

void CFmlDrawEngineQt::SetClipping( bool enable )
{
	m_painterRef.setClipping( enable );
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::GetUnits( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units )
{
	CFmlDrawEngineQt::GetUnitsEx(lfc, points, units);
}

/////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::FillPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogBrush& lb )
{
	QPainterPath path;
	GetPath_( points, tp, path );
	const QBrush *pBrush = GetBrush_( lb );
	if( pBrush ) m_painterRef.fillPath( path, *pBrush );
}

void CFmlDrawEngineQt::DrawPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogPen& lp )
{
	QPainterPath path;
	GetPath_( points, tp, path );
	const QPen *pPen = GetPen_( lp );
	if( pPen )
	{
		m_painterRef.setPen( *pPen );
		m_painterRef.drawPath( path );
	}
}

void CFmlDrawEngineQt::DrawFillPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogPen& lp, const FS_LogBrush& lb )
{
	QPainterPath path;
	GetPath_( points, tp, path );
	const QBrush *pBrush = GetBrush_( lb );
	const QPen *pPen = GetPen_( lp );
	if( pBrush && pPen )
	{
		m_painterRef.fillPath( path, *pBrush );
		m_painterRef.setPen( *pPen );
		m_painterRef.drawPath( path );
	}
}

/////////////////////////////////////////////////////////////////////
//
// image functions
//
/////////////////////////////////////////////////////////////////////

long CFmlDrawEngineQt::AddImage( const QString& path )
{
	return AddImage_( path );
}

long CFmlDrawEngineQt::DelImage( const QString& path )
{
	return DelImage_( path );
}

bool CFmlDrawEngineQt::DrawImage( const RectFde& rect, const QString& path )
{
	const QImage *pImage = (QImage*) GetImage_( path );
	if( !pImage ) return false;

	m_painterRef.drawImage( rect, *pImage );
	return true;
}

bool CFmlDrawEngineQt::DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QString& path )
{
	const QImage *pImage = (QImage*) GetImage_( path );
	if( !pImage ) return false;

	m_painterRef.drawImage( RectFde( x, y, w, h ), *pImage );
	return true;
}

SizeFde CFmlDrawEngineQt::MeasureImage( const QString& path )
{
	return MeasureImage_( path );
}

/////////////////////////////////////////////////////////////////////

bool CFmlDrawEngineQt::DrawImage( RealFde x, RealFde y, const QPixmap& bitmap )
{
	QPointF point( x, y );
	m_painterRef.drawPixmap( point, bitmap );
	return true;
}

bool CFmlDrawEngineQt::DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QPixmap& bitmap )
{
	m_painterRef.drawPixmap( (int) x, (int) y, (int) w, (int) h, bitmap );
	return true;
}

bool CFmlDrawEngineQt::DrawImage( RealFde x, RealFde y, const QPicture& img )
{
	QPointF point( x, y );
	m_painterRef.drawPicture( point, img );
	return true;
}

/////////////////////////////////////////////////////////////////////
//
// text functions
//
/////////////////////////////////////////////////////////////////////

RealFde CFmlDrawEngineQt::GetAscent( const FS_LogFont& lf )
{
	const QFont* pFont = GetFont_( lf );
	if( !pFont ) return 0.0;
	QFontMetricsF fm( *pFont );
	return fm.ascent();
}

RealFde CFmlDrawEngineQt::GetDescent( const FS_LogFont& lf )
{
	const QFont* pFont = GetFont_( lf );
	if( !pFont ) return 0.0;
	QFontMetricsF fm( *pFont );
	return fm.descent();
}

/////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////

CFmlDrawEngineQt::HKLogFont2FontMap CFmlDrawEngineQt::m_fontmap;

/////////////////////////////////////////////////////////////////////

const QBrush* CFmlDrawEngineQt::GetBrush_( const FS_LogFont& lf )
	{ return GetBrush_( FS_LogBrush( lf.getColor() ) ); }

const QFont* CFmlDrawEngineQt::GetFont_( const FS_LogFont& lfc )
{
	HKLogFont2FontMap::iterator it = m_fontmap.find( lfc );
	if( it != m_fontmap.end() ) 
		return it->second;

	QFont *pFont = new QFont( lfc.m_name, -1, lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
	pFont->setFixedPitch( lfc.m_isFixed );
	pFont->setPixelSize( (int) lfc.getPixelSize() );
	pFont->setStyleHint( lfc.m_hint );
	m_fontmap[ lfc ] = pFont;
	return pFont;
}

/////////////////////////////////////////////////////////////////////

CFmlDrawEngineQt::HKLogBrush2BrushMap CFmlDrawEngineQt::m_brushmap;

/////////////////////////////////////////////////////////////////////

const QBrush* CFmlDrawEngineQt::GetBrush_( const FS_LogBrush& lb )
{
	HKLogBrush2BrushMap::iterator it = m_brushmap.find( lb );
	if( it != m_brushmap.end() )
		return it->second;

	QBrush *pBrush = new QBrush( lb.getColor(), lb.getStyle() );
	m_brushmap[ lb ] = pBrush;
	return pBrush;
}

/////////////////////////////////////////////////////////////////////

CFmlDrawEngineQt::HKLogPen2PenMap CFmlDrawEngineQt::m_penmap;

/////////////////////////////////////////////////////////////////////

const QPen* CFmlDrawEngineQt::GetPen_( const FS_LogPen& lp )
{
	HKLogPen2PenMap::iterator it = m_penmap.find( lp );
	if( it != m_penmap.end() )
		return it->second;

	QPen *pPen = new QPen( lp.getStyle() );
	pPen->setWidthF( lp.getWidth() );
	pPen->setColor( lp.getColor() );
	m_penmap[ lp ] = pPen;
	return pPen;
}

/////////////////////////////////////////////////////////////////////

CFmlDrawEngineQt::HKLogImage2ImageMap CFmlDrawEngineQt::m_imagemap;

/////////////////////////////////////////////////////////////////////

const QImage* CFmlDrawEngineQt::GetImage_( const QString& path )
{
	HKLogImage2ImageMap::iterator it = m_imagemap.find( FS_LogImage( path ) );
	return (it != m_imagemap.end() ? it->second : NULL);
}

long CFmlDrawEngineQt::AddImage_( const QString& path )
{
	if( path.isEmpty() ) return 0;

	FS_LogImage lb( path );
	HKLogImage2ImageMap::iterator it = m_imagemap.find( lb );
	if( it != m_imagemap.end() )
	{
		((FS_LogImage&) it->first).m_ref++;
		return it->first.m_ref;
	}

	QImage* pImage = new QImage( lb.m_path );
	if( pImage->isNull() )
	{
		delete pImage;
		pImage = NULL;
		return 0;
	}

	m_imagemap[ lb ] = pImage;
	return lb.m_ref;
}

long CFmlDrawEngineQt::DelImage_( const QString& path )
{
	if( path.isEmpty() )
		return 0;

	FS_LogImage lb( path );
	HKLogImage2ImageMap::iterator it = m_imagemap.find( lb );
	if( it != m_imagemap.end() )
	{
		((FS_LogImage&) it->first).m_ref--;
		if( it->first.m_ref )
			return it->first.m_ref;

		if( it->second != NULL )
			delete it->second;
		m_imagemap.erase( it );
	}
	return 0;
}

SizeFde CFmlDrawEngineQt::MeasureImage_( const QString& path )
{
	const QImage* pImage = (QImage*) GetImage_( path );
	return (pImage != NULL ? SizeFde( pImage->width(), pImage->height() ) : SizeFde(0, 0));
}

/////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::GetPath_( QVector<PointFde>& points, QVector<qint8>& tp, QPainterPath & path )
{
	if( points.size() != tp.size() || points.size() == 0 ) return;

	qint8 type;
	long i, j, lastIdx, cb;
	for( i = 0; i < (long) tp.size(); )
	{
		type = tp[ i ] & ~spln_point_type_clsfig;
		switch( type )
		{
		case spln_point_type_moveto:
			path.moveTo( points[ lastIdx = i ] );
			i++;
			break;
		case spln_point_type_lineto:
			j = i++;
			while(i < (long) tp.size() && (tp[ i ] & ~spln_point_type_clsfig) == spln_point_type_lineto) i++;
			lastIdx = i - 1;
			{
				QVector<QPointF> polyPoints;
				polyPoints.push_back( path.currentPosition() );
				polyPoints += points.mid(j, i - j);
				if( polyPoints.size() > 2 )
					path.addPolygon( QPolygonF(polyPoints) );
				else
				{
					for(long pi = 1; pi < (long) polyPoints.size(); pi++)
						path.lineTo( polyPoints[ pi ] );
				}
			}
			if( tp[ lastIdx ] & spln_point_type_clsfig )
			{
				path.closeSubpath();
				//path.moveTo( points[ lastIdx ] );
			}
			break;
		case spln_point_type_splnto:
			j = i++;
			for( cb = 1; cb < 3 && i < (long) tp.size() && (tp[ i ] & ~spln_point_type_clsfig) == spln_point_type_splnto; cb++, i++ ) ;
			if( cb == 3 )
			{
				lastIdx = j + 2;
				Q_ASSERT( lastIdx == i - 1 );
				path.cubicTo( points[ j ], points[ j + 1 ], points[ j + 2 ] );
				//path.moveTo( points[ j + 2 ] );
				if( tp[ lastIdx ] & spln_point_type_clsfig )
				{
					path.closeSubpath();
					//path.moveTo( points[ lastIdx ] );
				}
			}
			break;
		default:
			i++;
		}
	}
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::ClearMaps_( void )
{
	{
		HKLogFont2FontMap::iterator it, end = m_fontmap.end();
		for( it = m_fontmap.begin(); it != end; it++ ) 
			if( it->second != NULL )
				delete it->second;
		m_fontmap.clear();
	}

	{
		HKLogBrush2BrushMap::iterator it, end = m_brushmap.end();
		for( it = m_brushmap.begin(); it != end; it++ ) 
			if( it->second != NULL )
				delete it->second;
		m_brushmap.clear();
	}

	{
		HKLogPen2PenMap::iterator it, end = m_penmap.end();
		for( it = m_penmap.begin(); it != end; it++ ) 
			if( it->second != NULL )
				delete it->second;
		m_penmap.clear();
	}
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::GetUnitsEx_( SYSINFO_Unit2PX& units, QFont& /*font*/, QFontInfo& fi, QFontMetricsF& fm )
{
	//QPrinter pd;
	//units.mm2px_x = RealFde(pd.widthMM()) / pd.width();
	//units.mm2px_y = RealFde(pd.heightMM()) / pd.height();
	//units.cm2px_x = 10.0 * units.mm2px_x;
	//units.cm2px_y = 10.0 * units.mm2px_y;
	//units.in2px_x = 25.4 * units.mm2px_x;
	//units.in2px_y = 25.4 * units.mm2px_y;

	//units.em2px = fm.width( QChar('x') );
	//units.ex2px = fm.xHeight();

	//units.pt2px_x = RealFde(fi.pointSizeF()) / fi.pixelSize();
	//units.pt2px_y = units.pt2px_x * (units.mm2px_y / units.mm2px_x);

	//units.pc2px_x = 12.0 * units.pt2px_x;
	//units.pc2px_y = 12.0 * units.pt2px_y;

	//units.px2px = 1.0;
	QPrinter pd;
	units.in2px_x = pd.logicalDpiX();
	units.in2px_y = pd.logicalDpiY();
	units.mm2px_x = units.in2px_x / 25.4;
	units.mm2px_y = units.in2px_y / 25.4;
	units.cm2px_x = 10.0 * units.mm2px_x;
	units.cm2px_y = 10.0 * units.mm2px_y;

	units.pt2px_x = RealFde(fi.pixelSize()) / RealFde(fi.pointSizeF());
	units.pt2px_y = units.pt2px_x * (units.mm2px_y / units.mm2px_x);
	//units.em2px = RealFde(fm.width( QChar('x') )) / units.pt2px_x;
	//units.ex2px = RealFde(fm.xHeight()) / units.pt2px_y;
	units.em2px = RealFde(fm.width( QChar('x') ));
	units.ex2px = RealFde(fm.height());

	units.pc2px_x = 12.0 * units.pt2px_x;
	units.pc2px_y = 12.0 * units.pt2px_y;

	units.px2px = 1.0;
}

void CFmlDrawEngineQt::GetUnitsEx( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units )
{
	QFont font( lfc.m_name, -1, lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
	font.setPointSizeF( points );
	font.setFixedPitch( lfc.m_isFixed );
	font.setStyleHint( lfc.m_hint );
	QFontInfo fi( font );
	QFontMetricsF fm( font );

	CFmlDrawEngineQt::GetUnitsEx_( units, font, fi, fm );
}

//void CFmlDrawEngineQt::GetUnitsEx( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units, 
//									const QVector<RealFde>& keglToMeasure, QVector<SizeFde>& defSize, 
//									QVector<RealFde>& defAscent, QVector<RealFde>& defDescent )
//{
//	QFont font( lfc.m_name, -1, lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
//	font.setPointSizeF( points );
//	font.setFixedPitch( lfc.m_isFixed );
//	font.setStyleHint( lfc.m_hint );
//	QFontInfo fi( font );
//	QFontMetricsF fm( font );
//
//	CFmlDrawEngineQt::GetUnitsEx_( units, font, fi, fm );
//
//	defSize.clear();
//	defAscent.clear();
//	defDescent.clear();
//
//	for( long i = 0; i < keglToMeasure.size(); i++ )
//	{
//		font.setPixelSize( (int) keglToMeasure[ i ] );
//		QFontMetricsF fmex( font );
//		defSize.push_back( SizeFde( fmex.width(QChar('x')), fmex.height() ) );
//		defAscent.push_back( fmex.ascent() );
//		defDescent.push_back( fmex.descent() );
//	}
//}

/////////////////////////////////////////////////////////////////////

SizeFde CFmlDrawEngineQt::MeasureImageEx( const QString& path )
{
	return MeasureImage_( path );
}

SizeFde CFmlDrawEngineQt::MeasureString( const QString& text, long pfr, long pto, const FS_LogFont& lf )
{
	return MeasureString( text.mid(pfr, pto - pfr), -1, lf );
}

SizeFde CFmlDrawEngineQt::MeasureString( const QString& text, long len, const FS_LogFont& lf )
{
	if( len < 0 || len > text.length() )
		len = text.length();
	//const QBrush *pBrush = GetBrush_( lf );
	const QFont *pFont = GetFont_( lf );
	QRectF rect(0, 0, 0, 0);
	if( /*pBrush && */pFont )
	{
		//m_painterRef.setBrush( *pBrush );
		//m_painterRef.setFont( *pFont );
		//rect = m_painterRef.boundingRect( rect, m_textFlags, text.left(len) );

		QFontMetricsF fm( *pFont );
		rect = fm.boundingRect( rect, m_textFlags, text.left(len) );
	}
	return rect.size();
}

SizeFde CFmlDrawEngineQt::MeasureStringEx( const QString& text, long pfr, long pto, const FS_LogFont& lf )
{
	return CFmlDrawEngineQt::MeasureStringEx( text.mid(pfr, pto - pfr), -1, lf );
}

SizeFde CFmlDrawEngineQt::MeasureStringEx( const QString& text, long len, const FS_LogFont& lf )
{
	if( len < 0 || len > text.length() )
		len = text.length();
	const QFont *pFont = GetFont_( lf );
	QRectF rect(0, 0, 0, 0);
	if( pFont )
	{
		QFontMetricsF fm( *pFont );
		rect = fm.boundingRect( rect, CFmlDrawEngineQt_TextFlags, text.left(len) );
	}
	return rect.size();
}

RealFde CFmlDrawEngineQt::GetAscentEx( const FS_LogFont& lf )
{
	const QFont* pFont = GetFont_( lf );
	if( !pFont ) return 0.0;
	QFontMetricsF fm( *pFont );
	return fm.ascent();
}

RealFde CFmlDrawEngineQt::GetDescentEx( const FS_LogFont& lf )
{
	const QFont* pFont = GetFont_( lf );
	if( !pFont ) return 0.0;
	QFontMetricsF fm( *pFont );
	return fm.descent();
}

RealFde CFmlDrawEngineQt::GetLeftBearingEx( const FS_LogFont& lf, QChar ch )
{
	const QFont* pFont = GetFont_( lf );
	if( !pFont ) return 0.0;
	QFontMetricsF fm( *pFont );
	return fm.leftBearing( ch );
}

RealFde CFmlDrawEngineQt::GetRightBearingEx( const FS_LogFont& lf, QChar ch )
{
	const QFont* pFont = GetFont_( lf );
	if( !pFont ) return 0.0;
	QFontMetricsF fm( *pFont );
	return fm.rightBearing( ch );
}

RealFde CFmlDrawEngineQt::GetTopBearingMin( const FS_LogFont& lf, const QChar* str, long len )
{
	const QFont* pFont = GetFont_( lf );
	if( pFont == NULL ) return RealFde( 0 );

	QFontMetricsF fm( *pFont );
	QString strUpper = QString( str, len ).toUpper();

	long i, origin = 0;
	for( i = 0; i < len; i++ )
	{
		if( str[ i ] == strUpper[ i ] )
			break;

		QRectF gm1 = fm.boundingRect( str[ i ] );
		QRectF gm2 = fm.boundingRect( strUpper[ i ] );

		if( gm1.y() <= gm2.y() )
			break;

		if( origin > gm2.y() - gm1.y() )
			origin = gm2.y() - gm1.y();
	}

	return RealFde( i == len ? origin : 0 );
}

RealFde GetBottomBearingMin( const FS_LogFont& lf, const QChar* str, long len )
{
	return RealFde( 0 );
}

/////////////////////////////////////////////////////////////////////

void CFmlDrawEngineQt::LPtoDPEx( RealFde& x, RealFde& y, double scaleX, double scaleY )
{
	x *= scaleX;
	y *= scaleY;
}

void CFmlDrawEngineQt::LPtoDPEx( PointFde& pt, double scaleX, double scaleY )
{
	LPtoDPEx( pt.rx(), pt.ry(), scaleX, scaleY );
}

void CFmlDrawEngineQt::LPtoDPEx( RectFde& rc, double scaleX, double scaleY )
{
	RealFde left = rc.left(), top = rc.top(), right = rc.right(), bottom = rc.bottom();
	LPtoDPEx( left, top, scaleX, scaleY );
	LPtoDPEx( right, bottom, scaleX, scaleY );
	rc.setCoords( left, top, right, bottom );
}

void CFmlDrawEngineQt::LPtoDPEx( SizeFde& sz, double scaleX, double scaleY )
{
	LPtoDPEx( sz.rwidth(), sz.rheight(), scaleX, scaleY );
}

void CFmlDrawEngineQt::DPtoLPEx( RealFde& x, RealFde& y, double scaleX, double scaleY )
{
	x /= scaleX;
	y /= scaleY;
}

void CFmlDrawEngineQt::DPtoLPEx( PointFde& pt, double scaleX, double scaleY )
{
	DPtoLPEx( pt.rx(), pt.ry(), scaleX, scaleY );
}

void CFmlDrawEngineQt::DPtoLPEx( RectFde& rc, double scaleX, double scaleY )
{
	RealFde left = rc.left(), top = rc.top(), right = rc.right(), bottom = rc.bottom();
	DPtoLPEx( left, top, scaleX, scaleY );
	DPtoLPEx( right, bottom, scaleX, scaleY );
	rc.setCoords( left, top, right, bottom );
}

void CFmlDrawEngineQt::DPtoLPEx( SizeFde& sz, double scaleX, double scaleY )
{
	DPtoLPEx( sz.rwidth(), sz.rheight(), scaleX, scaleY );
}

/////////////////////////////////////////////////////////////////////

long CFmlDrawEngineQt::AddImageEx( const QString& path )
{
	return AddImage_( path );
}

long CFmlDrawEngineQt::DelImageEx( const QString& path )
{
	return DelImage_( path );
}

/////////////////////////////////////////////////////////////////////
