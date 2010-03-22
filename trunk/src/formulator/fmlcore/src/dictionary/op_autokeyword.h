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

#if !defined( __FORMULATOR_DICTIONARY_OP_AUTOKEYWORD_H__ )
#define __FORMULATOR_DICTIONARY_OP_AUTOKEYWORD_H__

#include <QMap>

#include "HUtils/ihkstring.h"

#include "../mathml/mml_tag2mml.h"
#include "../fmlcore_global.h"

///////////////////////////////////////////////////////////////////////////////

__HERMITECH_FORMULIB__ const HKMap<QString, QString>& getText2ButtonIDMap();

__HERMITECH_FORMULIB__ int detectContentTag( QString& name, QString& body, QString& sattr, QString& parsedBody, long parsedStyle );
__HERMITECH_FORMULIB__ long getCMKeyword2ButtonID( QString keyword, QString& btnID );
__HERMITECH_FORMULIB__ enum TAG2MML getCMKeyword2TagID( QString keyword, QString& opBody );
__HERMITECH_FORMULIB__ int isCMKeywordPrefix( QString keyword );
__HERMITECH_FORMULIB__ int shouldBracket4Apply( QString calculationCenterID, long childPos );
__HERMITECH_FORMULIB__ int getSupContentByOperatorTagID( enum TAG2MML tag_id, QString& szBody, QString& szBase );

///////////////////////////////////////////////////////////////////////////////

#endif
