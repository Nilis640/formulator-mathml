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

#include "extc_test.h"

/////////////////////////////////////////////////////////////////////////////

CExtNodeContent_Test::CExtNodeContent_Test( void ) :
	CExtNodeContent_Base( EXTNODECONTENT_ID_TEST )
{
	setWidth( 50.0 );
	setHeight( 50.0 );
}

CExtNodeContent_Test::CExtNodeContent_Test( const CExtNodeContent_Test& _content ) :
	CExtNodeContent_Base( _content )
{
}

CExtNodeContent_Base* CExtNodeContent_Test::Clone( void )
{
	return new CExtNodeContent_Test( *this );
}

void CExtNodeContent_Test::Init( void * /*pData*/ )
{
}

void CExtNodeContent_Test::Store( QDataStream& ar ) const
{
	CExtNodeContent_Base::Store( ar );
}

void CExtNodeContent_Test::Load( QDataStream& ar )
{
	CExtNodeContent_Base::Load( ar );
}

void CExtNodeContent_Test::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& /*rc*/ )
{
	RectFde rcf = getDrawSize();
	rcf.translate( tl.x(), tl.y() );

	FS_LogPen lp( QColor( 0, 0, 127 ) );
	fde.DrawRectangle( rcf.left(), rcf.top(), rcf.width(), rcf.height(), lp );
	fde.DrawLine( rcf.left(), rcf.top(), rcf.right(), rcf.bottom(), lp );
	fde.DrawLine( rcf.left(), rcf.bottom(), rcf.right(), rcf.top(), lp );
}

/////////////////////////////////////////////////////////////////////////////
