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

#include "baseratio.h"
#include "../style/style.h"
#include "../settings/options.h"
#include "../nodes/dump/dumptags.h"

//////////////////////////////////////////////////////////////////////

CGraphLine::CGraphLine()
	: CGraphPrim() 
{
}

CGraphLine::CGraphLine( const CGraphLine& a ) 
	: CGraphPrim( a ) 
{
}

CGraphLine::~CGraphLine()
{
}

/////////////////////////////////////////////////////////////////////

int CGraphLine::Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode* )
{
	if( GetWidth() == 0 ) return 0;
	RectFde objPosition = GetPositionRect();
	objPosition.translate( tl.x(), tl.y() );

	QColor color = (GetColor() == DEFAULT_GRAPH_COLOR ? ::getCurrentFormulatorStyle().getLogPen().m_color : GetColor());
	fde.DrawLine( objPosition.left(), objPosition.top(), objPosition.right(), objPosition.bottom(), 
		FS_LogPen( color, (Qt::PenStyle) GetStyle(), GetWidth() ) );

	return 1;
}

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const CGraphLine& lp )
{
	return ar << (const CGraphPrim&) lp;
}

QDataStream& operator >> ( QDataStream& ar, CGraphLine& lp )
{
	return ar >> (CGraphPrim&) lp;
}

//////////////////////////////////////////////////////////////////////
