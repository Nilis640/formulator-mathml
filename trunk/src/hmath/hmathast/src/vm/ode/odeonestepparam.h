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

#if !defined(AFX_ODEONESTEPPARAM_H__801102A0_6EC8_11D4_8A72_4854E82A9A73__INCLUDED_)
#define AFX_ODEONESTEPPARAM_H__801102A0_6EC8_11D4_8A72_4854E82A9A73__INCLUDED_

#include "HMathBS/inumber.h"

class COdeOneStepParam  
{
public:
	COdeOneStepParam(CDSRReal betta, CDSRReal gamma);
	virtual ~COdeOneStepParam();

	void setStep(CDSRReal step);

public:
	CDSRReal b21h;
	CDSRReal p22h;
	CDSRReal p21h;
	CDSRReal b32h;
	CDSRReal b31h;
	CDSRReal p33h;
	CDSRReal p32h;
	CDSRReal p31h;

	CDSRReal pe31h;
	CDSRReal pe32h;

protected:
	CDSRReal b21;
	CDSRReal p22;
	CDSRReal p21;
	CDSRReal b32;
	CDSRReal b31;
	CDSRReal p33;
	CDSRReal p32;
	CDSRReal p31;

	CDSRReal pe31;
	CDSRReal pe32;
};

#endif // !defined(AFX_ODEONESTEPPARAM_H__801102A0_6EC8_11D4_8A72_4854E82A9A73__INCLUDED_)
