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

#ifndef __PARSEDSR_DATA_MMD_SET_OP_H__
#define __PARSEDSR_DATA_MMD_SET_OP_H__

#include "../uniword/uniword_op.h"
#include "mmd_set.h"

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
MMD_Set* _dsrop_SetAdd( MMD_Set *a, MMD_Set *b );
__HMATHLNGDLL__
MMD_Set* _dsrop_SetMul( MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
MMD_Set* _dsrop_SetSub( MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
MMD_Set* _dsrop_SetDecartMul( MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
UniWord _dsrop_SetRelIn( UniWord& ret, MMD_Set *vset, UniWord& word );
__HMATHLNGDLL__
UniWord _dsrop_SetRelNotIn( UniWord& ret, MMD_Set *vset, UniWord& word );
__HMATHLNGDLL__
UniWord _dsrop_SetRelSetIn( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
UniWord _dsrop_SetRelSetNotIn( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
UniWord _dsrop_SetRelSetInInv( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
UniWord _dsrop_SetRelSetInEq( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
UniWord _dsrop_SetRelSetInEqInv( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 );
__HMATHLNGDLL__
UniWord _dsrop_SetEqual_anytype( MMD_Set *a, MMD_Set *b );
__HMATHLNGDLL__
UniWord _dsrop_SetNotEqual_anytype( MMD_Set *a, MMD_Set *b );

///////////////////////////////////////////////////////////////////////////////

#endif
