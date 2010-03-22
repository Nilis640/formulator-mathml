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

#if !defined( __GRAMMAR_H__ )
#define __GRAMMAR_H__

#define	LEX_NONE	                0 
#define	Main		                1 
#define	Document	                2 
#define	Prolog		                3 
#define	Item		                4 
#define	Misc		                5 
#define	XMLDecl		                6 
#define	DTD		                    7 
#define	DocTypeDecl	                8 
#define	AttLPI		                9 
#define	PI		                    10
#define	DeclArgL	                11
#define	MiscDeclLst	                12
#define	CSTR		                13
#define	MiscDecl	                14
#define	Body		                15
#define	AttL		                16
#define	AttRPI		                17
#define	AttR		                18
#define	LEX_id		                19
#define	LEX_text	                20
#define	LEX_const_int	            21
#define	LEX_const_real	            22
#define	LEX_const_string	        23
#define	LEX_kw_xml	                24
#define	LEX_kw_doctype	            25
#define	LEX_begintag_q              26
#define	LEX_endtag_q              	27
#define	LEX_endtag_s              	28
#define	LEX_begintag_e              29
#define	LEX_lab		                30
#define	LEX_rab		                31
#define	LEX_endtag	                32
#define	LEX_begintag              	33
#define	LEX_begintag_s              34
#define	LEX_equal	                35
#define	LEX_eof		                36

#endif
