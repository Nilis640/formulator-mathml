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

#include "../vmevallink.h"
#include "../astdsr.h"

/////////////////////////////////////////
// 	Node		:	;
/////////////////////////////////////////
void CDsrAST_Node::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	IdxListItem	:	Separator, ArgList, Separator;
/////////////////////////////////////////
void CDsrAST_IdxListItem::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	Keyword		:	long;
/////////////////////////////////////////
void CDsrAST_Keyword::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	Separator	:	long;
/////////////////////////////////////////
void CDsrAST_Separator::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	Remark		:	QString;
/////////////////////////////////////////
void CDsrAST_Remark::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	Blank		:	QString;
/////////////////////////////////////////
void CDsrAST_Blank::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	0d0a		:	;
/////////////////////////////////////////
void CDsrAST_0d0a::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

/////////////////////////////////////////
//	unknown		:	QString;
/////////////////////////////////////////
void CDsrAST_unknown::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& /*dest*/ )
{}

///////////////////////////////////////////////////////////////////////////////
