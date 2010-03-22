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

#if !defined( __HUTILS_HTML_COLOR_H__ )
#define __HUTILS_HTML_COLOR_H__

#include "../basexxx.h"

/////////////////////////////////////////////////////////////////////////////

#define HTML_COLORS_NUMBER 17
#define HK_HTML_COLOUR_ROW				5
#define HK_HTML_COLOUR_COL				8

HUTILS_EXPORT int html_color_name2rgb( const QString& name, unsigned short& r, unsigned short& g, unsigned short& b );
HUTILS_EXPORT QString html_rgb2color_name( unsigned short& r, unsigned short& g, unsigned short& b );
HUTILS_EXPORT unsigned long* get_htmlColorValue_Ex();
HUTILS_EXPORT const QString* get_hk_htmlColorName_Ex();

/////////////////////////////////////////////////////////////////////////////

#endif
