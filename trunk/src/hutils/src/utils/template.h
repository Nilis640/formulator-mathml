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

#if !defined( __COMMON_UTILS_TEMPLATE_H__ )
#define __COMMON_UTILS_TEMPLATE_H__

#include <QVector>

#include "HUtils/ihkstring.h"
#include "../hutils_global.h"

#define __HERMITECH_TEMPLATE_MARK	_T("///$$$")

// class HKMatchTemplate
// ~~~~~ ~~~~~~~~~~
//
class HUTILS_EXPORT HKMatchTemplate
{
protected :
	QString buffer;

public :
	HKMatchTemplate() {}
	QString& getBuffer() { return buffer; }
	int Treate( const QString& src, const QVector<QString>& args );
};

#endif
