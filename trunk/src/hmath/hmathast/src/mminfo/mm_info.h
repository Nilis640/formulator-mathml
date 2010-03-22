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

#if !defined( __PARSEDSR_OPERATOR_MM_INFO_H__ )
#define __PARSEDSR_OPERATOR_MM_INFO_H__

#include "../hmathast_global.h"
#include "../type/typedsr.h"
#include "mm_domain.h"

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__ extern enum CDsrDataTypes CDsrMMDefaultTypeTable[ DSRMM_LAST ];
__HMATHLNGDLL__ enum CDsrDataTypes getCDsrMMDefaultType( enum CDsrMathModel mm_id, long mm_sub_task = -1 );
__HMATHLNGDLL__ extern int CDsrMMPropertyTable[ DSRMM_LAST ][ DSR_HAVE_LAST ];
__HMATHLNGDLL__ long getCDsrMMProperty( enum CDsrMathModel mm_id, enum CDsrMMProperty mmprop_id );
__HMATHLNGDLL__ int isCDsrAGWCongruence( int agw );

///////////////////////////////////////////////////////////////////////////////

#endif
