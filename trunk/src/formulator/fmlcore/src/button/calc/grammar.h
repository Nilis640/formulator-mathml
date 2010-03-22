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

#if !defined( __BULON_GRAMMAR_H__ )
#define __BULON_GRAMMAR_H__

#define	LEX_NONE		0
#define	MAIN			1
#define	STMT_LIST		2
#define	STMT			3
#define	EXPR			4
#define	TERM			5
#define	FACTOR			6
#define	OPERAND			7
#define	LEX_ident		8
#define	LEX_const_real	9
#define	LEX_kw_abs		10
#define	LEX_kw_av		11
#define	LEX_kw_min		12
#define	LEX_kw_max		13
#define	LEX_assign		14
#define	LEX_addsub		15
#define	LEX_muldiv		16
#define	LEX_coma		17
#define	LEX_dot			18
#define	LEX_lb			19
#define	LEX_rb			20
#define	LEX_eol			21
#define	LEX_eof			22

#endif
