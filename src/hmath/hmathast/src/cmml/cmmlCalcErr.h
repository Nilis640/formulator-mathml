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

#if !defined( __HMATH_CMMCALC_CMMCALCERR__ )
#define __HMATH_CMMCALC_CMMCALCERR__

///////////////////////////////////////////////////////////////////////////////

enum CMMLErrorID
{
	CMMLError_NONE = 0,

	CMMLError_NoAST,
	CMMLError_NoVM,
	CMMLError_NotSupportedCurrently,
	CMMLError_IgnoringNode_Deprecated,
	CMMLError_IgnoringNode_WrongContainer,
	CMMLError_UnexpectedApplyOp,
	CMMLError_IgnoreExcessArgs,
	CMMLError_NotAFunction,
	CMMLError_ArgNumberMismatch,
	CMMLError_ArgNumberMismatch_No,
	CMMLError_UnknownSymbol,
	CMMLError_UnknownXmlTag,
	CMMLError_CNSyntax,
	CMMLError_UnknownConstant,
	CMMLError_CNSyntax_Type,
	CMMLError_EmptyCI,
	CMMLError_ConstructorItemsExpected,
	CMMLError_MatrixItemsExpected,
	CMMLError_MatrixSyntax,
	CMMLError_Internal_NoRecord,
	CMMLError_FunctionBodyExpected,
	CMMLError_SymbolAlreadyDefined,
	CMMLError_SymbolInitError,
	CMMLError_PiecewiseSyntax,
	CMMLError_PiecewiseSyntax_Piece2Arg,
	CMMLError_PiecewiseSyntax_Piece2Apply,
	CMMLError_Semantic,

	CMMLError_LAST
};

///////////////////////////////////////////////////////////////////////////////

#endif
