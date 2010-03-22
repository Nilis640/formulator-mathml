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

#if !defined( __HUTILS_DRAWING_DRAWENGINE_H__ )
#define __HUTILS_DRAWING_DRAWENGINE_H__

#include "../hutils_global.h"
#include "drawstruct.h"
//#include "fdedraw_qt.h"

#include <QPainter>
#include <stack>

/////////////////////////////////////////////////////////////////////

struct FdeStateMatrix
{
	RealFde m_a11, m_a12, m_a21, m_a22, m_a31, m_a32;
};

/////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT CFmlDrawEngine
{

protected:
	CFmlDrawEngine( QPaintDevice *device );
	CFmlDrawEngine();

public:
	virtual ~CFmlDrawEngine( void );

public:

	bool IsPrinting( void );
	void SetPrinting( bool printing );

	virtual void GetViewPort( RealFde& width, RealFde& height ) = 0;
	virtual void SetViewPort( RealFde width, RealFde height ) = 0;

	// трансформации (относительно текущего значения)
	virtual void Identity( void ) = 0;
	virtual void Multiply( const FdeStateMatrix& sm ) = 0;
	virtual void Translate( RealFde tx, RealFde ty ) = 0;
	virtual void Scale( RealFde sx, RealFde sy ) = 0;
	virtual void Rotate( RealFde an ) = 0;

	virtual void GetMatrix( FdeStateMatrix& sm ) = 0;
	virtual void SetMatrix( const FdeStateMatrix& sm ) = 0;

	virtual void PushMatrix( void ) = 0;
	virtual void PopMatrix( void ) = 0;

	virtual void LPtoDP( PointFde& pt ) = 0;
	virtual void LPtoDP( RectFde& rc ) = 0;
	virtual void LPtoDP( SizeFde& sz ) = 0;
	virtual void DPtoLP( PointFde& pt ) = 0;
	virtual void DPtoLP( RectFde& rc ) = 0;
	virtual void DPtoLP( SizeFde& sz ) = 0;

	//
	virtual void Clear( const FS_LogBrush& lb ) = 0;
	virtual void Flush( void ) = 0;

	virtual void ClearClip( void ) = 0;
	virtual void SetClip( const RectFde& rc ) = 0;
	virtual bool HasClipping() = 0;
	virtual void SetClipping( bool enable ) = 0;

public:

	virtual void GetUnits( const FS_LogFont_Color& lfc, RealFde points, SYSINFO_Unit2PX& units ) = 0;

public:

	//
	// primitive functions
	//

	virtual void FillRectangle( const RectFde& rect, const FS_LogBrush& lb ) = 0;
	virtual void FillRectangle( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogBrush& lb ) = 0;
	virtual void FillPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogBrush& lb ) = 0;

	virtual void DrawRectangle( const RectFde& rect, const FS_LogPen& lp ) = 0;
	virtual void DrawRectangle( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogPen& lp ) = 0;
	virtual void DrawEllipse( const RectFde& rect, const FS_LogPen& lp ) = 0;
	virtual void DrawEllipse( RealFde x, RealFde y, RealFde w, RealFde h, const FS_LogPen& lp ) = 0;
	virtual void DrawLine( RealFde x1, RealFde y1, RealFde x2, RealFde y2, const FS_LogPen& lp ) = 0;
	virtual void DrawArc( const RectFde& rect, RealFde start, RealFde sweep, const FS_LogPen& lp ) = 0;
	virtual void DrawArc( RealFde x, RealFde y, RealFde w, RealFde h, RealFde start, RealFde sweep, const FS_LogPen& lp ) = 0;
	virtual void DrawPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogPen& lp ) = 0;

	virtual void DrawFillPath( QVector<PointFde>& points, QVector<qint8>& tp, const FS_LogPen& lp, const FS_LogBrush& lb ) = 0;

	//
	// image function
	//

	virtual long AddImage( const QString& path ) = 0;
	virtual long DelImage( const QString& path ) = 0;

	virtual bool DrawImage( const RectFde& rect, const QString& path ) = 0;
	virtual bool DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QString& path ) = 0;
	virtual SizeFde MeasureImage( const QString& path ) = 0;

	virtual bool DrawImage( RealFde x, RealFde y, const QPixmap& hBitmap ) = 0;
	virtual bool DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QPixmap& hBitmap ) = 0;

	virtual bool DrawImage( RealFde x, RealFde y, const QPicture& img ) = 0;
	//virtual bool DrawImage( RealFde x, RealFde y, RealFde w, RealFde h, const QPicture& img ) = 0;

	//
	// text functions
	//

	virtual void DrawString( RealFde x, RealFde y, const QString& strText, const FS_LogFont& lf ) = 0;

	virtual SizeFde MeasureString( const QString& strText, long len, const FS_LogFont& lf ) = 0;
	virtual SizeFde MeasureString( const QString& strText, long pfr, long pto, const FS_LogFont& lf ) = 0;

	virtual RealFde GetAscent( const FS_LogFont& lf ) = 0;
	virtual RealFde GetDescent( const FS_LogFont& lf ) = 0;

protected:
	bool	m_printing;			// printing flag
	std::stack<FdeStateMatrix>	m_states;
};

/////////////////////////////////////////////////////////////////////
//
// 
//
/////////////////////////////////////////////////////////////////////

inline
bool CFmlDrawEngine::IsPrinting( void )
	{ return m_printing; }

inline
void CFmlDrawEngine::SetPrinting( bool printing )
	{ m_printing = printing; }

/////////////////////////////////////////////////////////////////////
//
//
//
//
//
/////////////////////////////////////////////////////////////////////

enum FmlDrawEngineTypes
{
	FmlDrawEngine_None = 0,
	FmlDrawEngine_Qt, 
	FmlDrawEngine_Gdi, 
	FmlDrawEngine_GgiPlus, 
	FmlDrawEngine_OpenLG, 
	FmlDrawEngine_DirectDraw, 
	FmlDrawEngine_Direct3D9, 
	FmlDrawEngine_Direct3D10 
};

HUTILS_EXPORT quint32 StartUpFDE( enum FmlDrawEngineTypes type = FmlDrawEngine_Qt );
HUTILS_EXPORT quint32 ShutDownFDE( void );

HUTILS_EXPORT CFmlDrawEngine* CreateFDE( QPaintDevice *device );
HUTILS_EXPORT CFmlDrawEngine* CreateFDE( QPainter& painter );
HUTILS_EXPORT void DeleteFDE( CFmlDrawEngine*& pFde );

/////////////////////////////////////////////////////////////////////

#endif
