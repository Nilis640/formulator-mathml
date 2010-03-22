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

#if !defined( __HUTILS_DRAWING_FDEDRAW_QT_H__ )
#define __HUTILS_DRAWING_FDEDRAW_QT_H__

#include "drawengine.h"

#include <QPicture>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QImage>
#include <QTextOption>
#include <QMatrix>
#include <QTransform>
#include <QRectF>
#include <QFontMetrics>
#include <QPainterPath>

#include <map>

/////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT CFmlDrawEngineQt : public CFmlDrawEngine
{
	typedef std::map<FS_LogFont,	QFont*>		HKLogFont2FontMap;
	typedef std::map<FS_LogBrush,	QBrush*>	HKLogBrush2BrushMap;
	typedef std::map<FS_LogPen,		QPen*>		HKLogPen2PenMap;
	typedef std::map<FS_LogImage,	QImage*>	HKLogImage2ImageMap;

public:
	CFmlDrawEngineQt( QPaintDevice *device );
	CFmlDrawEngineQt( QPainter& painter );
	CFmlDrawEngineQt();
	virtual ~CFmlDrawEngineQt( void );

public:
	virtual void GetViewPort( RealFde& width, RealFde& height );
	virtual void SetViewPort( RealFde width, RealFde height );

	// трансформации (относительно текущего значения)
	virtual void Identity( void );
	virtual void Multiply( const FdeStateMatrix& sm );
	virtual void Translate( RealFde tx, RealFde ty );
	virtual void Scale( RealFde sx, RealFde sy );
	virtual void Rotate( RealFde an );

	virtual void GetMatrix( FdeStateMatrix& sm );
	virtual void SetMatrix( const FdeStateMatrix& sm );

	virtual void PushMatrix( void );
	virtual void PopMatrix( void );

	virtual void LPtoDP( RealFde& x, RealFde& y );
	virtual void LPtoDP( PointFde& pt );
	virtual void LPtoDP( RectFde& rc );
	virtual void LPtoDP( SizeFde& sz );
	virtual void DPtoLP( RealFde& x, RealFde& y );
	virtual void DPtoLP( PointFde& pt );
	virtual void DPtoLP( RectFde& rc );
	virtual void DPtoLP( SizeFde& sz );

	virtual void Clear( const FS_LogBrush& lb );
	virtual void Flush( void );

	virtual void ClearClip( void );
	virtual void SetClip( const RectFde& rc );
	virtual bool HasClipping();
	virtual void SetClipping( bool enable );

public:

	virtual void GetUnits( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units );

public:

	//
	// primitive functions
	//

	virtual void FillRectangle( const RectFde& rect, const FS_LogBrush& lb );
	virtual void FillRectangle( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogBrush& lb );
	virtual void FillPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogBrush& lb );

	virtual void DrawRectangle( const RectFde& rect, const FS_LogPen& lp );
	virtual void DrawRectangle( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogPen& lp );
	virtual void DrawEllipse( const RectFde& rect, const FS_LogPen& lp );
	virtual void DrawEllipse( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogPen& lp );
	virtual void DrawLine( RealFde x1, RealFde y1, RealFde x2, RealFde y2, const FS_LogPen& lp );
	virtual void DrawArc( const RectFde& rect, RealFde start, RealFde sweep, const FS_LogPen& lp );
	virtual void DrawArc( RealFde x, RealFde y, RealFde w, RealFde h, RealFde start, RealFde sweep, const FS_LogPen& lp );
	virtual void DrawPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogPen& lp );

	virtual void DrawFillPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogPen& lp, const FS_LogBrush& lb );

	//
	// image function
	//

	virtual long AddImage( const QString& path );
	virtual long DelImage( const QString& path );

	virtual bool DrawImage( const RectFde& rect, const QString& path );
	virtual bool DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QString& path );
	virtual SizeFde MeasureImage( const QString& path );

	virtual bool DrawImage( RealFde x, RealFde y, const QPixmap& hBitmap );
	virtual bool DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QPixmap& hBitmap );

	virtual bool DrawImage( RealFde x, RealFde y, const QPicture& img );
	//virtual bool DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QPicture& img );

	//
	// text functions
	//

	virtual void DrawString( RealFde x, RealFde y, const QString& strText, const FS_LogFont& lf );

	virtual SizeFde MeasureString( const QString& strText, long len, const FS_LogFont& lf );
	virtual SizeFde MeasureString( const QString& strText, long pfr, long pto, const FS_LogFont& lf );

	virtual RealFde GetAscent( const FS_LogFont& lf );
	virtual RealFde GetDescent( const FS_LogFont& lf );

private:
	void CFmlDrawEngineQt_ApplyMatrix();

	static const QFont* GetFont_( const FS_LogFont& lf );
	static const QBrush* GetBrush_( const FS_LogFont& lf );
	static const QBrush* GetBrush_( const FS_LogBrush& lb );
	static const QPen* GetPen_( const FS_LogPen& lp );

	static const QImage* GetImage_( const QString& path );
	static long AddImage_( const QString& path );
	static long DelImage_( const QString& path );
	static SizeFde MeasureImage_( const QString& path );
	static void GetPath_( QVector<PointFde>& points, QVector<qint8>& tp, QPainterPath & path );

	static void ClearMaps_( void );

public:
	static void GetUnitsEx_( SYSINFO_Unit2PX& units, QFont& font, QFontInfo& fi, QFontMetricsF& fm );
	static void GetUnitsEx( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units );
	//static void GetUnitsEx( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units, 
	//						const QVector<RealFde>& keglToMeasure, QVector<SizeFde>& defSize, 
	//						QVector<RealFde>& defAscent, QVector<RealFde>& defDescent );

	static SizeFde MeasureStringEx( const QString& strText, long len, const FS_LogFont& lf );
	static SizeFde MeasureStringEx( const QString& strText, long pfr, long pto, const FS_LogFont& lf );

	static RealFde GetAscentEx( const FS_LogFont& lf );
	static RealFde GetDescentEx( const FS_LogFont& lf );
	static RealFde GetLeftBearingEx( const FS_LogFont& lf, QChar ch );
	static RealFde GetRightBearingEx( const FS_LogFont& lf, QChar ch );
	static RealFde GetTopBearingMin( const FS_LogFont& lf, const QChar* str, long len );
	static RealFde GetBottomBearingMin( const FS_LogFont& lf, const QChar* str, long len );

	static long AddImageEx( const QString& path );
	static long DelImageEx( const QString& path );
	static SizeFde MeasureImageEx( const QString& path );

	static void LPtoDPEx( RealFde& x, RealFde& y, double scaleX, double scaleY );
	static void LPtoDPEx( PointFde& pt, double scaleX, double scaleY );
	static void LPtoDPEx( RectFde& rc, double scaleX, double scaleY );
	static void LPtoDPEx( SizeFde& sz, double scaleX, double scaleY );
	static void DPtoLPEx( RealFde& x, RealFde& y, double scaleX, double scaleY );
	static void DPtoLPEx( PointFde& pt, double scaleX, double scaleY );
	static void DPtoLPEx( RectFde& rc, double scaleX, double scaleY );
	static void DPtoLPEx( SizeFde& sz, double scaleX, double scaleY );

private:
	RealFde			m_width, m_height;
	QPainter		m_painter;
	QPainter&		m_painterRef;
	int				m_textFlags;
	QTransform		m_transform;
	QMatrix			m_MatrixD, m_MatrixI;

	static HKLogFont2FontMap	m_fontmap;
	static HKLogBrush2BrushMap	m_brushmap;
	static HKLogPen2PenMap		m_penmap;
	static HKLogImage2ImageMap	m_imagemap;
};

#endif
