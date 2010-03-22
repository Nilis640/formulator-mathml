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

#if !defined( __FORMULATOR_BASERATIO_H__ )
#define __FORMULATOR_BASERATIO_H__

#include <QVector>
#include <QDataStream>

#include "HUtils/ihkstring.h"
#include "HUtils/idrawing.h"

#include "../button/btn.h"
#include "../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////

class CNode;
class CParentNode;

class __HERMITECH_FORMULIB__ CGraphPrimFormula
{
protected:
	RectFde				m_pecents;
	QColor				m_color;
	QString				m_str;
	RealFde				m_width, m_height, m_offset_direction_x, m_offset_direction_y;
	qint32				m_style, m_bind_no, m_is_formula, m_type, m_align, m_size_type, m_stretchy;
	QVector<qint8>		m_point_type;
	QVector<PointFde>	m_point_xy;
	qint32				m_expr_enable;	// is expr enabled BY MEANS OF the primitive (default = 0)

public:
	CGraphPrimFormula( void );
	CGraphPrimFormula( const CGraphPrimFormula& a );
	virtual ~CGraphPrimFormula();

	RectFde GetPeasentPosition( void ) const	{ return m_pecents; }
	long GetSizeType( void ) const				{ return m_size_type; }

	QColor GetColor( void ) const				{ return m_color; }
	void SetColor( QColor c )					{ m_color = c; }

	RealFde GetWidth( void ) const				{ return m_width; }
	void SetWidth( RealFde width )				{ m_width = width; }

	long GetStyle( void ) const					{ return m_style; }
	long GetType( void ) const					{ return m_type; }
	const QString& GetString( void ) const		{ return m_str; }

	RealFde GetHeight( void )					{ return m_height; }

	long isCalcEnabled( void )					{ return m_expr_enable; }
	long isFormulaType( void )					{ return m_is_formula; }
	long GetAlign( void )						{ return m_align; }
	long GetBindToFrame( void )					{ return m_bind_no; }

	int Create( const FBtnGraphics& a );
	RectFde GetRealPosition( CNode* pNode );

	friend QDataStream& operator << ( QDataStream& ar, const CGraphPrimFormula& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphPrimFormula& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }

	friend class CGraphPrim;
	friend class CGraphBSpline;
	friend class CGraphBSplineEx;
};

///////////////////////////////////////////////////////////////////////////////
// Graph Primitives
///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphPrim  
{
protected:
	RealFde			m_aRealPosition[ 4 ], m_height, m_width;
	qint32			m_is_formula, m_bind_no, m_align, m_style, m_type, m_level, m_stretchy;
	QColor			m_color;
	qint32			m_expr_enable;	// is expr enabled BY MEANS OF the primitive (default = 0)

public:
	CGraphPrim();
	CGraphPrim( const CGraphPrim& a );
	virtual ~CGraphPrim();

	virtual int Create( const CGraphPrimFormula& templatePrim );
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent = NULL ) = 0;

	void SetPosition( const RectFde &point );
	RectFde GetPositionRect( void ) const;

	long GetType( void ) const		{ return m_type; }

	RealFde GetWidth( void ) const	{ return m_width; }
	void SetWidth( RealFde width )	{ m_width = width; }

	long GetStyle( void ) const		{ return m_style; }

	QColor GetColor( void ) const	{ return m_color; }
	void SetColor( QColor color )	{ m_color = color; }

	long GetLevel( void ) const		{ return m_level; }
	void SetLevel( long level )		{ m_level = level; }

	void getKernelPositionPtr( RealFde*& a )	{ a = (RealFde*) m_aRealPosition; }
	RealFde& getPosition( long i )	{ return m_aRealPosition[ i ]; }

	RealFde GetHeight( void )		{ return m_height; }

	long isCalcEnabled( void )		{ return m_expr_enable; }
	long isFormulaType( void )		{ return m_is_formula; }
	long GetAlign( void )			{ return m_align; }
	long GetBindToFrame( void )		{ return m_bind_no; }
	long isBound( void )			{ return m_bind_no > 0; }
	long getBound( void )			{ return m_bind_no; }

	CNode* getBoundFrame( CParentNode *parent );

	virtual void setTxtSize() {}
        virtual int correctParentNodeSize( CParentNode * /*parent*/ ) { return 0; }
        virtual int canDraw( CParentNode* /*node*/ ) { return 1; }

	friend QDataStream& operator << ( QDataStream& ar, const CGraphPrim& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphPrim& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }
};

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphLine : public CGraphPrim  
{
public:
	CGraphLine();
	CGraphLine( const CGraphLine& a );
	virtual ~CGraphLine();

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent = NULL );

	friend QDataStream& operator << ( QDataStream& ar, const CGraphLine& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphLine& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }
};

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphRect : public CGraphPrim  
{
public:
	CGraphRect();
	CGraphRect( const CGraphRect& a );
	virtual ~CGraphRect();

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent = NULL );

	virtual void Store( QDataStream& ar ) const			{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }

	friend QDataStream& operator << ( QDataStream& ar, const CGraphRect& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphRect& lp );
};

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphText : public CGraphPrim 
{
protected:
	QString		m_Str;
	SizeFde		m_txtSize;

public:
	CGraphText();
	CGraphText( const CGraphText& a );
	virtual ~CGraphText();

	QString GetString( void ) const { return m_Str; }

	RealFde getFontHeight( void );
	RealFde GetVerticalCenter();

	virtual int Create( const CGraphPrimFormula& templatePrim );
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent = NULL );

	virtual void setTxtSize();
	virtual int correctParentNodeSize( CParentNode *parent );
	virtual int canDraw( CParentNode* node );

	friend QDataStream& operator << ( QDataStream& ar, const CGraphText& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphText& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }

protected:
	void GetObjPosition( CNode *parentnode, RectFde& objPosition );
};

///////////////////////////////////////////////////////////////////////////////

#define DRAWENGINE_BSPLINE_SCALE_X	100.0f
#define DRAWENGINE_BSPLINE_SCALE_Y	100.0f

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphBSpline : public CGraphPrim
{
protected:
	QVector<qint8>		m_point_type;
	QVector<PointFde>	m_point_xy;

public:
	CGraphBSpline();
	CGraphBSpline( const CGraphBSpline& a );
	virtual ~CGraphBSpline();

	virtual int Create( const CGraphPrimFormula& templatePrim );
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent = NULL );

protected:
	virtual int GetPoints( const RectFde &rc, QVector<PointFde> &xy, QVector<qint8> &tp );

public:
	friend QDataStream& operator << ( QDataStream& ar, const CGraphBSpline& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphBSpline& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }
};

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphBSplineEx : public CGraphBSpline
{
protected:
	RealFde			m_offset_direction_x, m_offset_direction_y;

public:
	CGraphBSplineEx();
	CGraphBSplineEx( const CGraphBSplineEx& a );
	virtual ~CGraphBSplineEx();

	virtual int Create( const CGraphPrimFormula& templatePrim );

protected:
	virtual int GetPoints( const RectFde &rc, QVector<PointFde> &xy, QVector<qint8> &tp );

public:
	friend QDataStream& operator << ( QDataStream& ar, const CGraphBSplineEx& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphBSplineEx& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }
};

///////////////////////////////////////////////////////////////////////////////
// to be compatible with previous versions of Formulator
///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CGraphQuaterArc : public CGraphPrim  
{
protected:
	QString			orientation;

public:
	CGraphQuaterArc();
	CGraphQuaterArc( const CGraphQuaterArc& a );
	virtual ~CGraphQuaterArc();

	virtual int Create( const CGraphPrimFormula& templatePrim );
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent = NULL );

protected:
	void GetObjPosition( RectFde& objPosition, RealFde& start, RealFde& sweep );

public:
	friend QDataStream& operator << ( QDataStream& ar, const CGraphQuaterArc& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CGraphQuaterArc& lp );

	virtual void Store( QDataStream& ar ) const		{ ar << *this; }
	virtual void Load( QDataStream& ar )			{ ar >> *this; }
};

///////////////////////////////////////////////////////////////////////////////

#endif
