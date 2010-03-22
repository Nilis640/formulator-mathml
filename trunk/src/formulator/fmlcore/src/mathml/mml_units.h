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

#if !defined( __COMMON_LANGUAGE_MATHML_FORMAT_MML_UNITS_H__ )
#define __COMMON_LANGUAGE_MATHML_FORMAT_MML_UNITS_H__

#include "../fmlcore_global.h"
#include "HUtils/ihkstring.h"
#include "HUtils/idrawstruct.h"

#define MML_UNIT_NAME__EM		_T("em")
#define MML_UNIT_NAME__EX		_T("ex")
#define MML_UNIT_NAME__PX		_T("px")
#define MML_UNIT_NAME__IN		_T("in")
#define MML_UNIT_NAME__CM		_T("cm")
#define MML_UNIT_NAME__MM		_T("mm")
#define MML_UNIT_NAME__PT		_T("pt")
#define MML_UNIT_NAME__PC		_T("pc")
#define MML_UNIT_NAME__PERCENT	_T("%")

__HERMITECH_FORMULIB__ int mml_attr_unit_2_value( int is_y, RealFde& old_value, const QString& body, const SYSINFO_Unit2PX& unit2px );
__HERMITECH_FORMULIB__ int mml_convertSpaceName2FloatValue( int is_y, const QString& spacename, RealFde& value, const SYSINFO_Unit2PX& unit2px );
__HERMITECH_FORMULIB__ int mml_convertFloatValue2SpaceName( QString& destname, RealFde value, const SYSINFO_Unit2PX& unit2px );
__HERMITECH_FORMULIB__ int mml_attr_paddedunit_2_value( long selfIdx, const QString& body, RealFde* newValues, RealFde* oldValues, const SYSINFO_Unit2PX& unit2px );
__HERMITECH_FORMULIB__ int mml_findUnit( QString& destUnitName, double& destVal, int& isNeedRound, const QString& body, int is_y, const SYSINFO_Unit2PX& unit2px );

#endif
