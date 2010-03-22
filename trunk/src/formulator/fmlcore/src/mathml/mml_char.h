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

#if !defined( __COMMON_LANGUAGE_MATHML_FORMAT_MML_CHAR_H__ )
#define __COMMON_LANGUAGE_MATHML_FORMAT_MML_CHAR_H__

#include "../fmlcore_global.h"
#include "HUtils/ihkstring.h"

#define MML_CHAR__ASCII_NUMBER		17
#define MML_CHAR__GREEK_NUMBER		26
#define MML_CHAR__GREEK_OP_NUMBER	18
#define MML_CHAR__NUMBER			(2 * MML_CHAR__GREEK_NUMBER + MML_CHAR__ASCII_NUMBER + MML_CHAR__GREEK_OP_NUMBER)

#define MML_CHAR_AMP				_T('&')

#define MML_STYLE_GREEK				0
#define MML_STYLE_OP				1
#define MML_STYLE_TEXT				2

__HERMITECH_FORMULIB__ extern const QString __mml_char__name[ MML_CHAR__NUMBER ];
__HERMITECH_FORMULIB__ extern const QString __mml_char__value[ MML_CHAR__NUMBER ];
__HERMITECH_FORMULIB__ extern const long __mml_char__font[ MML_CHAR__NUMBER ];
__HERMITECH_FORMULIB__ QString mml_getGreek( QChar a );

#endif
