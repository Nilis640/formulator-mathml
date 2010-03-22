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

#include "btn_graphics.h"
#include "../style/style.h"

/////////////////////////////////////////////////////////////////////

FBtnGraphics::FBtnGraphics( void ) 
	: pos( 0.0, 0.0, 100.0, 100.0 )
{
	line = -1;
	grType = FBtnGraphics::GRAPH_PRIM_NONE;
	color = QColor(0, 0, 0);
	style = Qt::SolidLine;
	height = FTEXT_HEIGHT_FRAME;
	width = LINETHICKNESS_DEFAULT;
	sizeType = VARIABLE_SIZE;
	coord_type = COORD_TYPE_RATIO;
	stretchy = 0;
	altGroup = DEFAULT_ALT_GR_PRIMITIVE_GROUP;
	align = HORISONTAL_CENTER | VERTICAL_CENTER;
	expr_enable = 0;
	bind_to = -1;
	offset_direction_x = 0.0;
	offset_direction_y = 0.0;
}

void FBtnGraphics::createLine( int grTp, int penStyle, RectFde& rc )
{
	line = -1;
	name = text_reserve = "";
	coord_type = COORD_TYPE_RATIO;
	pos = RectFde( PointFde(0.0, 0.0), PointFde(HF_BINBUTTON_HORIZONTAL_SCALE, HF_BINBUTTON_VERTICAL_SCALE) );
	color = QColor( 0, 0, 0 );
	height = FTEXT_HEIGHT_FRAME;
	width = LINETHICKNESS_DEFAULT;
	sizeType = VARIABLE_SIZE;
	altGroup = DEFAULT_ALT_GR_PRIMITIVE_GROUP;
	stretchy = 0;
	align = HORISONTAL_CENTER | VERTICAL_CENTER;
	expr_enable = 0;
	bind_to = -1;
	grType = grTp;
	style = penStyle;
	pos = rc;
}

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const FBtnGraphics& lp )
{
	return	ar << lp.name << lp.text_reserve << lp.line << lp.grType << lp.style << lp.sizeType << 
			lp.stretchy << lp.altGroup << lp.align << lp.expr_enable << lp.bind_to << lp.coord_type << 
			lp.color << lp.pos << lp.height << lp.width << lp.point_type << lp.point_x << lp.point_y << 
			lp.offset_direction_x << lp.offset_direction_y;
}

QDataStream& operator >> ( QDataStream& ar, FBtnGraphics& lp )
{
	return	ar >> lp.name >> lp.text_reserve >> lp.line >> lp.grType >> lp.style >> lp.sizeType >> 
			lp.stretchy >> lp.altGroup >> lp.align >> lp.expr_enable >> lp.bind_to >> lp.coord_type >> 
			lp.color >> lp.pos >> lp.height >> lp.width >> lp.point_type >> lp.point_x >> lp.point_y >> 
			lp.offset_direction_x >> lp.offset_direction_y;
}

/////////////////////////////////////////////////////////////////////

