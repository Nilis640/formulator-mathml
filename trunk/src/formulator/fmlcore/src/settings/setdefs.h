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

#ifndef __FMLCORE_SETTINGS_SETDEFS_H__
#define __FMLCORE_SETTINGS_SETDEFS_H__

/////////////////////////////////////////////////////////////////////

#include "HUtils/ihkstring.h"

/////////////////////////////////////////////////////////////////////

// profile name
#define FORMULIB_PROFILE_NAME			_T("Formulib")

// sections name
#define FORMULIB_DATA					_T("Data")
#define FORMULIB_DIRECTORIES			_T("Directories")
#define FORMULIB_SETTINGS				_T("Settings")

// Data (lm)
#define FORMULIB_LIBCOUNT				_T( "count" )
#define FORMULIB_LIBFORMAT				_T( "lib-%d" )
#define FORMULIB_KEY_CODE				_T( "kc" )
#define FORMULIB_USERREGDATA			_T( "user-registration-data" )
#define FORMULIB_FORMAT_STYLE			_T( "style2-lib" )
#define FORMULIB_FORMAT_OPERATORBYDSR	_T( "op_by_dsr-lib" )
#define FORMULIB_FORMAT_OPERATORBYMML	_T( "op_by_mml-lib" )
#define FORMULIB_UDFAFORMAT				_T( "unicode-dfa-lib" )
#define FORMULIB_DFAFORMAT				_T( "dfa-lib" )
#define FORMULIB_VAL2ENAMEFORMAT		_T( "unicodeval-ename-lib" )
#define FORMULIB_XSL					_T( "xsl" )

// Directories
#define FORMULIB_IMAGES					_T("images")
#define FORMULIB_TEMPLATE				_T("template")

// Settings (cu)
#define FORMULIB_OPTIONS				_T("options-value")
#define FORMULIB_SCALE					_T("scale-r")
#define FORMULIB_LINESPACING			_T("line-spacing-r")

#define FORMULIB_OPTION_TIMES2TIMES		_T("times2times")
#define FORMULIB_OPTION_TRANSLATOR		_T("translator")
#define FORMULIB_OPTION_MATH2BLOCK		_T("math2block")

#define FORMULIB_CHECK4UPDATES_TIME		_T("up-time")
#define FORMULIB_CHECK4UPDATES_PERIOD	_T("up-period")
#define FORMULIB_CURRENT_BUILD			_T("up-build")
#define FORMULIB_CURRENT_UID			_T("up-uid")

#define FORMULIB_CHECK4UPDATES_PERIOD_DEFAULT	10
#define FORMULIB_CHECK4UPDATES_PERIOD_MIN		1
#define FORMULIB_CHECK4UPDATES_PERIOD_MAX		60

#define FORMULIB_INDENT_TOP				_T("indent-top")
#define FORMULIB_INDENT_LEFT			_T("indent-left")
#define FORMULIB_INDENT_BOTTOM			_T("indent-bottom")
#define FORMULIB_INDENT_RIGHT			_T("indent-right")
#define FORMULIB_XHTML_SPLIT			_T("xhtml-split")

#define FORMULIB_REG_LASTCALL			_T("math-std-lib")
#define FORMULIB_REG_TOTALDAY			_T("math-ext-lib")

/////////////////////////////////////////////////////////////////////

#define DEFAULT_SYMBOLSIZE_MIN				5
#define DEFAULT_SYMBOLSIZE_MAX				127

/////////////////////////////////////////////////////////////////////

#define NODE_PLANETEXT__POINT_SIZE_DEFAULT	0.0
#define FORMULATOR_PLANETEXT_CMPVALUE		0.45

#define DEFAULT_FONTSIZE					double(12.0)

#define DEFAULT_NESTING						long(0)
#define DEFAULT_READONLY					long(0)
#define DEFAULT_VIEWSCALE					double(2.0)
#define DEFAULT_VIEWSCALE_STEP				double(0.5)
#define DEFAULT_VIEWSCALE_MAX				double(10.0)
#define DEFAULT_VIEWSCALE_MIN				double(0.5)
#define DEFAULT_LINESPACING					double(1.0)

#define DEFAULT_OPTION_INPUTC				long(0)
#define DEFAULT_OPTION_TIMES2TIMES			long(0)
#define DEFAULT_OPTION_FORCERENDER			long(0)
#define DEFAULT_OPTION_TRANSLATOR			long(0)
#define DEFAULT_OPTION_MATH2BLOCK			long(1)
#define DEFAULT_OPTION_ENUMBER2NAME			long(1)
#define DEFAULT_OPTION_SHOWNAVINFO			long(1)
#define DEFAULT_OPTION_SHOWREFRESHWARN		long(1)

#define DEFAULT_INDENTS						double(20.0)
#define DEFAULT_SPLIT						long(270)

/////////////////////////////////////////////////////////////////////

#define FORMULIB_PRESET_NONE			0
#define FORMULIB_PRESET_WORD			1
#define FORMULIB_PRESET_PMML			2
#define FORMULIB_PRESET_CMML			3
#define FORMULIB_PRESET_CMML_P			4
#define FORMULIB_PRESET_CMML_ASCII		5
#define FORMULIB_PRESET_DEFAULT			FORMULIB_PRESET_WORD
#define FORMULIB_PRESET_USER			-1

/////////////////////////////////////////////////////////////////////

#define TRANSLATE_2_MATHML2_NO_NAMESPACE			0
#define TRANSLATE_2_MATHML2_NAMESPACE_M				1
#define TRANSLATE_2_MATHML2_NAMESPACE_ATTR			2

#define TRANSLATE_2_MATHML2_NAMESPACE_M_PREFIXNAME	_T("m:")

/////////////////////////////////////////////////////////////////////

#define OLIMP_MATHML_FLAG__MROW_KEEP1CHILD				0x80000000
#define OLIMP_MATHML_FLAG__MROW_ARTIFICIAL				0x40000000
#define OLIMP_MATHML_FLAG__MROW_CONTENT_AUTODETECT		0x20000000
#define OLIMP_MATHML_FLAG__MROW_FORCE_RENDER			0x10000000

/////////////////////////////////////////////////////////////////////

#define OLIMP_OPTION_FLAG__INPUT_MODE_CONTENT			0x08000000
#define OLIMP_OPTION_FLAG__UNICODE_AS_NAME				0x04000000
#define OLIMP_OPTION_FLAG__RELATIVE_IMG_PATH			0x02000000
#define OLIMP_OPTION_FLAG__SHOW_NAVIGATE_INFO			0x01000000
#define OLIMP_OPTION_FLAG__SHOW_NODE_TIPS_INFO			0x00800000
#define OLIMP_OPTION_FLAG__XHTML_AS_BLOCK				0x00400000
#define OLIMP_OPTION_FLAG__MULTICHAR_MI_NOT_ITALIC		0x00200000
#define OLIMP_OPTION_FLAG__SHOW_REFRESHMATHML_WARN		0x00100000
#define OLIMP_OPTION_FLAG__SHOULD_REFRESH_MATHML		0x00080000
#define OLIMP_OPTION_FLAG__MATHML_STYLE_HAS_PRIORITY	0x00040000

#define OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE				0x00020000
#define OLIMP_OPTION_FLAG__AUTOGRAPH_RENDER2_ARCFUN				0x00010000
#define OLIMP_OPTION_FLAG__AUTOGRAPH_PURE_CONTENT_OUT			0x00008000
#define OLIMP_OPTION_FLAG__AUTOGRAPH_DIFF_AS_YX					0x00004000
#define OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_ONECHARID	0x00002000
#define OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_MOVEOUT		0x00001000
#define OLIMP_OPTION_FLAG__AUTOGRAPH_EDITING_STYLE_DXDT			0x00000800

#define OLIMP_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST		0x00000400

#define OLIMP_OPTION_FLAG__SHOWNESTING					0x00000200
#define OLIMP_OPTION_FLAG__SHOWREADONLY					0x00000100
#define OLIMP_OPTION_FLAG__SHOW_INSERTMATHML_WARN		0x00000080

#define OLIMP_OPTION_FLAG__PERFOMANCE_COPYIMAGE			0x00000040
#define OLIMP_OPTION_FLAG__PERFOMANCE_UPDATEMMLTEXT		0x00000020

#define OLIMP_OPTION_FLAG__CHECK4UPDATES				0x00000008
#define OLIMP_OPTION_FLAG__CALC_TEXTRECT_BY_GLYPH		0x00000004

#define OLIMP_OPTION_FLAG__DEFAULT			long(OLIMP_OPTION_FLAG__UNICODE_AS_NAME | OLIMP_OPTION_FLAG__RELATIVE_IMG_PATH | OLIMP_OPTION_FLAG__SHOW_NAVIGATE_INFO | OLIMP_OPTION_FLAG__SHOW_INSERTMATHML_WARN | OLIMP_OPTION_FLAG__SHOW_NODE_TIPS_INFO | OLIMP_OPTION_FLAG__XHTML_AS_BLOCK | OLIMP_OPTION_FLAG__CHECK4UPDATES | OLIMP_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST | OLIMP_OPTION_FLAG__PERFOMANCE_COPYIMAGE)

/////////////////////////////////////////////////////////////////////

#endif
