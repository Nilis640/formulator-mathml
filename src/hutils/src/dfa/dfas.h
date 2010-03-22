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

#if !defined( __MATHLANG_LIMAUTO_DFAS_H__ )
#define __MATHLANG_LIMAUTO_DFAS_H__

#include "digit/la_digit.h"
#include "digit/la_sdigit.h"
#include "digit/la_sdigit_base.h"
#include "digit/la_xmlattr_sdigit.h"
#include "dsr2_var/la_dsr2var.h"
#include "entity/la_entity.h"
#include "keyword/la_keyword.h"
#include "string/la_str.h"
#include "string/la_xmlstr.h"
#include "var/la_cvar.h"
#include "var/la_xmlvar.h"

#define ELH_NO_ERROR		0
#define ELH_SOFT_ERROR		-2
#define ELH_CRITICAL_ERROR	-1

HUTILS_EXPORT int _hatol( const QStringRef& str, long& value );
HUTILS_EXPORT int _hatof( const QStringRef& str, double& value );
HUTILS_EXPORT int _hatol_base( const QStringRef& str, const QString& base, long& value );
HUTILS_EXPORT int _hatof_base( const QStringRef& str, const QString& base, double& value );

#endif
