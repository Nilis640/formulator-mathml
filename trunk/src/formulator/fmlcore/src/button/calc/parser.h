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

#ifndef __BULAN_PARSER_H__
#define __BULAN_PARSER_H__

#include "HUtils/ihkarray.h"
#include "HUtils/ibaseparser.h"

#include "thelex.h"
#include "ast.h"

class __HERMITECH_FORMULIB__ BulanParser : public BaseParser
{
protected :
	MIArray<FBAST_Node> astn;
	long astn_index;

	virtual void GenCode( unsigned int RuleNumber );
	virtual void NewLex( Source& aSource );
	virtual void BeforeMake();
	virtual void AfterMake ();
	virtual void SetSemanticError();
  
public :
	const QString& GetLexError();
	PiCodeASTStorage** the_storage;
	BulanParser( Source& src, ActionT *aActionTable, PiCodeASTStorage** st );
};

inline
BulanParser::BulanParser( Source& src, ActionT *aActionTable, PiCodeASTStorage** st ) :
  	BaseParser( src, aActionTable ),
	astn( MIArray<FBAST_Node>::LeaveItems, 8, 0, 8 )
{
	the_storage = st;
	astn_index = 0;
	SemanticErrorHappened = false;
	NewLex( src );
}

#endif
