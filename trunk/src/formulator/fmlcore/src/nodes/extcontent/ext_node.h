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

#if !defined( __FORMULATOR_EXTNODE_H__ )
#define __FORMULATOR_EXTNODE_H__

#include "HUtils/idrawing.h"
#include "HUtils/ihkstring.h"
#include "ext_nodemngr.h"
#include "../node.h"

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CExtNodeContent_Base
{
protected:
	qint32	m_classid;
	SizeFde	m_size;

public:
	CExtNodeContent_Base( long classid );
	CExtNodeContent_Base( const CExtNodeContent_Base& _content );
	virtual ~CExtNodeContent_Base( void );

	long getClassID( void ) const;

	SizeFde getSize( void ) const;
	void setSize( SizeFde sz );
	RealFde getWidth( void ) const;
	RealFde getHeight( void ) const;
	void setWidth( RealFde value );
	void setHeight( RealFde value );

public:
	virtual void Init( void *pData );
	virtual CExtNodeContent_Base* Clone( void );

	virtual QString to_mathml( long nLevel, long& nodesCount );

	virtual RealFde Recalculate_VerticalCenter( SizeFde sz, RealFde ascent );
	virtual RectFde getDrawSize();
	virtual void Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	virtual void Store( QDataStream& ar ) const;
	virtual void Load( QDataStream& ar );
};

/////////////////////////////////////////////////////////////////////

inline
CExtNodeContent_Base::CExtNodeContent_Base( long classid )
{
	m_classid = classid;
	m_size.rwidth() = m_size.rheight() = 0;
}

inline
CExtNodeContent_Base::CExtNodeContent_Base( const CExtNodeContent_Base& _content )
{
	m_classid = _content.m_classid;
	m_size = _content.m_size;
}

inline
CExtNodeContent_Base::~CExtNodeContent_Base( void )
{
}

/////////////////////////////////////////////////////////////////////

inline
long CExtNodeContent_Base::getClassID( void ) const
	{ return m_classid; }

inline
SizeFde CExtNodeContent_Base::getSize( void ) const
	{ return m_size; }

inline
void CExtNodeContent_Base::setSize( SizeFde sz )
	{ m_size = sz; }

inline
RealFde CExtNodeContent_Base::getWidth( void ) const
	{ return m_size.width(); }

inline
RealFde CExtNodeContent_Base::getHeight( void ) const
	{ return m_size.height(); }

inline
void CExtNodeContent_Base::setWidth( RealFde value )
	{ m_size.rwidth() = value; }

inline
void CExtNodeContent_Base::setHeight( RealFde value )
	{ m_size.rheight() = value; }

/////////////////////////////////////////////////////////////////////

inline
CExtNodeContent_Base* CExtNodeContent_Base::Clone( void )
	{ return new CExtNodeContent_Base( *this ); }

inline
void CExtNodeContent_Base::Init( void * /*pData*/ )
	{}

inline
QString CExtNodeContent_Base::to_mathml( long /*nLevel*/, long& /*nodesCount*/ )
	{ return QString( _T("") ); }

inline
RectFde CExtNodeContent_Base::getDrawSize()
	{ return RectFde( PointFde(0.0, 0.0), getSize() ); }

inline
RealFde CExtNodeContent_Base::Recalculate_VerticalCenter( SizeFde nSize, RealFde /*ascent*/ )
	{ return nSize.height() / 2.0; }

inline
void CExtNodeContent_Base::Draw( CFmlDrawEngine& /*fde*/, const PointFde& /*tl*/, const RectFde& /*rc*/ )
	{}

/////////////////////////////////////////////////////////////////////
//
// class CExtNode
//
/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CExtNode : public CPlaneText
{
public:
	CExtNode();
	CExtNode( QString strText, long nStyle, CNodeInfo &ni, CNodeExInfo &ex_ni );

	CExtNode( CExtNode* pExtNode );
	virtual ~CExtNode();

public:
	CExtNodeContent_Base* getContent( void ) const;
	void releaseContent( void );
	void setContent( CExtNodeContent_Base* newcontent);

public:
	virtual void Add( char ch );
	virtual void Insert( long npos, char ch );
	virtual void Add( QString &str );
	virtual void Insert( long npos, QString &str );
	virtual void Delete( long npos  );
	virtual void DeleteLast();

	virtual int isSingle( void );
	virtual void Glue( CPlaneText *pPlaneText );

	virtual int CopyText( const QString& lpszString, long nFStyle, const FS_LogFont_Color& lfs, int fUnicode );

public:
	virtual void Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const;
	virtual void Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad );
	virtual int CopyNodeInfo( CNode* pNode );

	// draw
	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc );

	RectFde getDrawSize( long nfr, long nto, RealFde *pascent = NULL );
	RectFde getDrawSize( long pos = -1, RealFde *pascent = NULL );
	long getPosFromPoint( const PointFde &pt );

	virtual void RecalculateSize( int bRecalc );
	virtual void RecalculateSizeFull();

	virtual QString to_mathml( long nLevel, long& nodesCount );

protected:
	CExtNodeContent_Base *m_pContent;
};

/////////////////////////////////////////////////////////////////////

inline
void CExtNode::Add( char /*ch*/ )
	{}

inline
void CExtNode::Insert( long /*npos*/, char /*ch*/ )
	{}

inline
void CExtNode::Add( QString& /*str*/ )
	{}

inline
void CExtNode::Insert( long /*npos*/, QString& /*str*/ )
	{}

inline
void CExtNode::Delete( long /*npos*/ )
	{}

inline
void CExtNode::DeleteLast()
	{}

inline
int CExtNode::isSingle()
	{ return 1; }	// the node can't be divided into several parts

inline
void CExtNode::Glue( CPlaneText * /*pPlaneText*/ )
	{ Q_ASSERT( 0 ); }

/////////////////////////////////////////////////////////////////////

inline
CExtNodeContent_Base* CExtNode::getContent( void ) const
	{ return m_pContent; }

inline
void CExtNode::releaseContent( void )
{
	if( m_pContent )
	{
		delete m_pContent;
		m_pContent = NULL;
	}
}

inline
void CExtNode::setContent( CExtNodeContent_Base *newcontent )
{
	releaseContent();
	m_pContent = newcontent;
}

/////////////////////////////////////////////////////////////////////

inline
QString CExtNode::to_mathml( long nLevel, long& nodesCount )
{
	if( getContent() )
		return getContent()->to_mathml( nLevel, nodesCount );
	return QString( _T("") );
}

/////////////////////////////////////////////////////////////////////
#endif//__FORMULATOR_EXTNODE_H__
