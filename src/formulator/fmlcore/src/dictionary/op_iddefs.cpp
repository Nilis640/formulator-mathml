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

#include "../mathml/mml_defs.h"
#include "op_iddefs.h"

///////////////////////////////////////////////////////////////////////////////

int shouldBracket4Apply( QString calculationCenterID, long childPos )
{
	if( calculationCenterID == HF_BTNID_DEFAULT_DIVISION_SP || calculationCenterID == HF_BTNID_SQRT ||
		calculationCenterID == HF_BTNID_DEFAULT_BRACKET_FLOOR || calculationCenterID == HF_BTNID_DEFAULT_BRACKET_CEIL ||
		calculationCenterID == HF_BTNID_DEFAULT_BRACKET_ABS || calculationCenterID == HF_BTNID_DEFAULT_BRACKET_NORM ||
		calculationCenterID == HF_BTNID_DEFAULT_BRACKET_LRA )
		return 0;

	if( (calculationCenterID == HF_BTNID_SUP || calculationCenterID == HF_BTNID_SUB) && (childPos < 0 || childPos == 1) )
		return 0;

	return 1;
}

int getSupContentByOperatorTagID( enum TAG2MML tag_id, QString& szBody, QString& szBase )
{
	int iRet = 0;
	switch( tag_id )
	{
	case TAG2MML_laplacian:
		szBody = _T("&nabla;");
		szBase = _T("2");
		break;
	case TAG2MML_variance:
		szBody = _T("&sigma;");
		szBase = _T("2");
		break;
	case TAG2MML_arcsin:
		szBody = _T("sin");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arccos:
		szBody = _T("cos");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arctan:
		szBody = _T("tan");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arccot:
		szBody = _T("cot");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arcsinh:
		szBody = _T("sinh");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arccosh:
		szBody = _T("cosh");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arctanh:
		szBody = _T("tanh");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arccoth:
		szBody = _T("coth");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arcsec:
		szBody = _T("sec");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arccsc:
		szBody = _T("csc");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arcsech:
		szBody = _T("sech");
		szBase = _T("&minus;1");
		break;
	case TAG2MML_arccsch:
		szBody = _T("csch");
		szBase = _T("&minus;1");
		break;
	default:
		iRet = -1;
	}
	return iRet;
}

///////////////////////////////////////////////////////////////////////////////
