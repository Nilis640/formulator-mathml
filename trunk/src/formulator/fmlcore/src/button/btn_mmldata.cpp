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

#include "btn_mmldata.h"

/////////////////////////////////////////////////////////////////////

FBtnMMLData::FBtnMMLData()
{
	precedence = assoc = is_content = 0;
}

void FBtnMMLData::reset()
{
	value = name = left = right = type = frame = mathml_template_string = reserved = "";
	precedence = 0;
	assoc = 0;
	is_content = 0;
}

const FBtnMMLData& FBtnMMLData::operator = ( const FBtnMMLData& a )
{
	value = a.value;
	name = a.name;
	left = a.left;
	right = a.right;
	type = a.type;
	frame = a.frame;
	mathml_template_string = a.mathml_template_string;
	reserved = a.reserved;
	precedence = a.precedence;
	assoc = a.assoc;
	is_content = a.is_content;
	return *this;
}

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const FBtnMMLData& lp )
{
	return ar << lp.value << lp.name << lp.left << lp.right << lp.type << lp.frame << lp.mathml_template_string << lp.reserved << lp.precedence << lp.assoc << lp.is_content;
}

QDataStream& operator >> ( QDataStream& ar, FBtnMMLData& lp )
{
	return ar >> lp.value >> lp.name >> lp.left >> lp.right >> lp.type >> lp.frame >> lp.mathml_template_string >> lp.reserved >> lp.precedence >> lp.assoc >> lp.is_content;
}

/////////////////////////////////////////////////////////////////////
