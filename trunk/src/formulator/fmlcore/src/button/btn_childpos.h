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

#if !defined( __FORMULATOR_FMLCORE_BTN_CHILDPOS_H__ )
#define __FORMULATOR_FMLCORE_BTN_CHILDPOS_H__

#include <QVector>
#include <QDataStream>

#include "../fmlcore_global.h"
#include "HUtils/idrawstruct.h"

/////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FBtnChildPos
{
public:
	enum AlignmentType
	{
		Default = 0,
		Child,
		Child_Average,
		Frame,
		Frame_Blank_Average,
		Half,
		MSPACE_Exact_HeightDepth,
		TableAxis,
		TableCenter2Baseline,
		TableTop2Baseline,
		TableBottom2Baseline
	};

public:
	RectFde pos;
	qint32 align;
	qint32 level;

public:
	FBtnChildPos();
	FBtnChildPos( RectFde apos, long aalign, long alevel );
};

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const FBtnChildPos& lp );
QDataStream& operator >> ( QDataStream& ar, FBtnChildPos& lp );

/////////////////////////////////////////////////////////////////////

#endif
