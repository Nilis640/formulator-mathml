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

#include "message.h"

///////////////////////////////////////////////////////////////////////////////

QString DRMessage::Format()
{
	QString formatedText;
	if( c_arg1.length() )
	{
		formatedText = (n_arg1 != -1) ?
			QString("%1 %2: %3").arg(c_arg1).arg(n_arg1).arg(text) : 
			QString("%1: %3").arg(c_arg1).arg(text);
	}
	else
		formatedText = text;

	if( c_arg2.length() )
		return formatedText.arg(c_arg2);
	else if( n_arg2 != -1 )
		return formatedText.arg(n_arg2);
	return formatedText;
}

///////////////////////////////////////////////////////////////////////////////
