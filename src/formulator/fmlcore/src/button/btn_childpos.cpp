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

#include "btn_childpos.h"
#include "btn_graphics.h"
#include "../style/style.h"

/////////////////////////////////////////////////////////////////////

FBtnChildPos::FBtnChildPos()
	: pos( RectFde( PointFde(0.0, 0.0), PointFde(HF_BINBUTTON_HORIZONTAL_SCALE, HF_BINBUTTON_VERTICAL_SCALE) ) )
	, align( HORISONTAL_CENTER | VERTICAL_CENTER )
	, level( 0 )
{
}

FBtnChildPos::FBtnChildPos( RectFde apos, long aalign, long alevel )
	: pos( apos )
	, align( aalign )
	, level( alevel )
{
}

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const FBtnChildPos& lp )
{
	return	ar << lp.pos << lp.align << lp.level;
}

QDataStream& operator >> ( QDataStream& ar, FBtnChildPos& lp )
{
	return	ar >> lp.pos >> lp.align >> lp.level;
}

/////////////////////////////////////////////////////////////////////
