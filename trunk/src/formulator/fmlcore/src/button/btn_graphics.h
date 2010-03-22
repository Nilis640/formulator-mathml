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

#if !defined( __FORMULATOR_FMLCORE_BTN_GRAPHICS_H__ )
#define __FORMULATOR_FMLCORE_BTN_GRAPHICS_H__

#include <QVector>
#include <QDataStream>

#include "../fmlcore_global.h"
#include "HUtils/ihkstring.h"
#include "HUtils/idrawstruct.h"

/////////////////////////////////////////////////////////////////////

#define COORD_TYPE_RATIO				0
#define COORD_TYPE_FORMULA				1

#define HF_BINBUTTON_VERTICAL_SCALE		100.0
#define HF_BINBUTTON_HORIZONTAL_SCALE	100.0

#define FIXED_SIZE							1
#define VARIABLE_SIZE						2

#define DEFAULT_ALT_GR_PRIMITIVE_GROUP	0

#define ELLIPSE_MASK_ALIGN					0x1000

#define TO_LEFT								0x1
#define TO_RIGHT							0x2
#define HORISONTAL_CENTER					0x3
#define HORISONTAL_MASK						0x3

#define TO_BASELINE							0x0
#define TO_TOP								0x4
#define TO_BOTTOM							0x8
#define VERTICAL_CENTER						0xC
#define VERTICAL_MASK						0xC

#define ELLIPSE_MASK_ALIGN					0x1000

#define DEFAULT_SCALE						0
#define FULL_SCALE							1
#define CHILD_SCALE							2

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FBtnGraphics
{
public:
	enum FBtnGraphicsType
	{
		GRAPH_PRIM_NONE = 0,
		GRAPH_PRIM_LINE,
		GRAPH_PRIM_RECT,
		GRAPH_PRIM_TEXT,
		GRAPH_PRIM_QUATER_ARC,
		GRAPH_PRIM_SPLINE,
		GRAPH_PRIM_SPLINE_EX,
		GRAPH_PRIM_LANGUAGE
	};

public:
	QString				name, text_reserve;
	qint32				line, grType, style, sizeType, stretchy, altGroup, align, expr_enable, bind_to, coord_type;
	QColor				color;
	RectFde				pos;
	RealFde				height, width;
	QVector<qint8>		point_type;
	QVector<RealFde>	point_x, point_y;
	RealFde				offset_direction_x, offset_direction_y;

	FBtnGraphics( void );
	void createLine( qint32 grTp, qint32 penStyle, RectFde& rc );

	int is_binded_text( void ) 
		{ return grType == GRAPH_PRIM_TEXT && bind_to > 0; }

	int operator < ( const FBtnGraphics& a ) const
		{ return altGroup < a.altGroup; }
};

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const FBtnGraphics& lp );
QDataStream& operator >> ( QDataStream& ar, FBtnGraphics& lp );

/////////////////////////////////////////////////////////////////////

#endif
