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

#include "vmi_picode.h"

///////////////////////////////////////////////////////////////////////////////
// Button Language VM defines

QString FBtnPiCode_Command_Str[ LPC_LAST ] =
{
	_T("LPC_NONE"),
	_T("LPC_ASIGN"),
	_T("LPC_POP"),
	_T("LPC_PUSH"),
	_T("LPC_PUSHVAL"),
	_T("LPC_MINUS"),
	_T("LPC_ADD"),
	_T("LPC_SUB"),
	_T("LPC_MUL"),
	_T("LPC_DIV"),
	_T("LPC_ABS"),
	_T("LPC_AVERAGE"),
	_T("LPC_MIN"),
	_T("LPC_MAX")
};

void FBtnPiCode::operator=( const FBtnPiCode& ls )
{
	line = ls.line;
	val = ls.val;
	frame_var = ls.frame_var;
	command = ls.command;
}

///////////////////////////////////////////////////////////////////////////////
