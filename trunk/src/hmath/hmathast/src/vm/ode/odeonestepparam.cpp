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

#include "HUtils/ihkstring.h"
#include "odeonestepparam.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COdeOneStepParam::COdeOneStepParam(CDSRReal betta, CDSRReal gamma)
{
	if ( fabs( gamma ) < 1e-14 || fabs( betta ) < 1e-14 )
		throw _T("Internal error");

	b21 = betta;
	p22 = 1.0 / (2.0 * b21);
	p21 = 1.0 - p22;
	b32 = b21 * (gamma + 2.0 - 3.0 * b21) / (gamma * gamma);
	b31 = (gamma - 1.0) * b32;

	if ( fabs(6.0 - b32 * b21) < 1e-14 )
		throw _T("Internal error");
	p33 = 1.0 / (6.0 - b32 * b21);

	p32 = (1.0 - 2.0 * p33 * gamma * b32) / (2.0 * b21);
	p31 = 1.0 - p32 - p33;
	pe31 = p31 - p21;
	pe32 = p32 - p22;

	//
	b21h = 0.0;
	p22h = 0.0;
	p21h = 0.0;
	b32h = 0.0;
	b31h = 0.0;
	p33h = 0.0;
	p32h = 0.0;
	p31h = 0.0;
	pe31h = 0.0;
	pe32h = 0.0;
}

COdeOneStepParam::~COdeOneStepParam()
{
}

//////////////////////////////////////////////////////////////////////

void COdeOneStepParam::setStep(CDSRReal step)
{
	b21h = b21 * step;
	p22h = p22 * step;
	p21h = p21 * step;
	b32h = b32 * step;
	b31h = b31 * step;
	p33h = p33 * step;
	p32h = p32 * step;
	p31h = p31 * step;
	pe31h = pe31h * step;
	pe32h = pe32h * step;
};

//////////////////////////////////////////////////////////////////////
