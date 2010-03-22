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
//	StmtList	:	Node*;
/////////////////////////////////////////
void CDsrAST_StmtList::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
	{
		if( stmt[ i ] )
		{
			stmt[ i ]->getInstruction( vm, dest );
			if( stmt[ i ]->getRTStackOverhead() > 0 )
			{
				UniWord uw( _create_lparam( 0, 0, stmt[ i ]->getRTStackOverhead() ) );
				CDSR_VMEval_Cmd item( 
					VMC_DSR_DEC_RUNSTACK_PTR, 
					uw,
					getLine() );
				dest.Add( item );
			}
		}
	}
}

/////////////////////////////////////////
//	Compound	:	Keyword, StmtList, Keyword;
/////////////////////////////////////////
void CDsrAST_Compound::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	if( stmt ) stmt->getInstruction( vm, dest );
}

/////////////////////////////////////////
//	NullStmt	:	Separator;
/////////////////////////////////////////
void CDsrAST_NullStmt::getInstruction( CDSR_VMEval& /*vm*/, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	CDSR_VMEval_Cmd item( VMC_DSR_NOP, getLine() );
	dest.Add( item );
}

/////////////////////////////////////////
//	If			:	Keyword, Expr, Keyword, Node, Keyword, Node;
/////////////////////////////////////////
void CDsrAST_If::getInstruction( CDSR_VMEval& vm, CDSR_VMEval_Line& dest )
{
	// put instruction
	//////////////////
	if( n_expr )
		n_expr->getInstruction( vm, dest );
	CDSR_VMEval_Cmd skipCmd( VMC_DSR_SKIP_ON_FALSE, n_then ? n_then->getLine() : getLine() );
	long skip_f = dest.Add( skipCmd );
	if( n_then ) n_then->getInstruction( vm, dest );
	if( n_else )
	{
		CDSR_VMEval_Cmd item1( VMC_DSR_SKIP, n_else ? n_else->getLine() : getLine() );
		long skip = dest.Add( item1 );
		dest[ skip_f ].setLParam( (long) dest.ArrayDim() - skip_f );
		n_else->getInstruction( vm, dest );
		dest[ skip ].setLParam( (long) dest.ArrayDim() - skip );
		CDSR_VMEval_Cmd item2( VMC_DSR_NOP, n_else ? n_else->getLine() : getLine() );
		dest.Add( item2 );
	}
	else
		dest[ skip_f ].setLParam( (long) dest.ArrayDim() - skip_f );
}

///////////////////////////////////////////////////////////////////////////////
