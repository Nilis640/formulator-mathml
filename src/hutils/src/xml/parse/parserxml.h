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

/////////////////////////////////////////////////////////////////////////////

#ifndef __COMMON_XML_PARSER_H__
#define __COMMON_XML_PARSER_H__

#include "HUtils/ibaseparser.h"
#include "../xml_tree.h"

class TXML_Parser : public BaseParser {
protected :

	MIArray<AST_TXML_Node> astn;
	long astn_index;

	void GenCode( unsigned int RuleNumber );
	void NewLex( Source& aSource );
	void BeforeMake( void );
	void AfterMake ( void );
  
public :
	TXML_Storage** the_storage;

	TXML_Parser( Source& src, ActionT *actions, TXML_Storage **st );
	virtual void SetSemanticError( void );
	virtual ~TXML_Parser( void )
	{
		astn_index = 0;
	}
};

inline
TXML_Parser::TXML_Parser( Source& src, ActionT *actions, TXML_Storage** st )
	: BaseParser( src, actions )
	, astn( MIArray<AST_TXML_Node>::LeaveItems, 8, 0, 8 )
{
	the_storage = st;
	astn_index = 0;
	NewLex( src );
}

/////////////////////////////////////////////////////////////////////////////

#endif
