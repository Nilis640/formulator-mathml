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

#if !defined( __COMMON_CTRL_TYPE_H__ )
#define __COMMON_CTRL_TYPE_H__

#include "../basexxx.h"

#define GUI_CTRL_TEMPLATE_NONE							-1
#define GUI_CTRL_TEMPLATE_STATIC_TEXT					0
#define GUI_CTRL_TEMPLATE_EDIT_BOX						1
#define GUI_CTRL_TEMPLATE_DROPDOWN_COMBO_BOX			2
#define GUI_CTRL_TEMPLATE_OPEN_FILE_EDIT_BOX			3
#define GUI_CTRL_TEMPLATE_FORMULATOR_EDIT_BOX			4
#define GUI_CTRL_TEMPLATE_COLOR_COMBO_BOX				5

#define GUI_CTRL_TEMPLATE_NAME_NONE						_T("none")
#define GUI_CTRL_TEMPLATE_NAME_STATIC_TEXT				_T("static")
#define GUI_CTRL_TEMPLATE_NAME_EDIT_BOX					_T("edit box")
#define GUI_CTRL_TEMPLATE_NAME_DROPDOWN_COMBO_BOX		_T("dropdown combo box")
#define GUI_CTRL_TEMPLATE_NAME_OPEN_FILE_EDIT_BOX		_T("open file edit box")
#define GUI_CTRL_TEMPLATE_NAME_FORMULATOR_EDIT_BOX		_T("formulator edit box")
#define GUI_CTRL_TEMPLATE_NAME_COLOR_COMBO_BOX			_T("color combo box")

#define GUI_CTRL_TEMPLATE_PARAM_NAME_PREDEFINED			_T("predefined")
#define GUI_CTRL_TEMPLATE_PARAM_NAME_ALLFILES			_T("all files")

#endif //__COMMON_CTRL_TYPE_H__
