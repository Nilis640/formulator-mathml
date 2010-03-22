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

#if !defined( __FORMULATOR_INCLUDE_FMLAPIDEFS__ )
#define __FORMULATOR_INCLUDE_FMLAPIDEFS__

/////////////////////////////////////////////////////////////////////

#define FML_STYLE_MATH					0
#define FML_STYLE_TEXT					1
#define FML_STYLE_VARIABLE				2
#define FML_STYLE_FUNCTION				3
#define FML_STYLE_GREEK					4
#define FML_STYLE_VECTOR				5
#define FML_STYLE_NUMBER				6
#define FML_STYLE_FIXED					7
#define FML_STYLE_OPERATOR				8
#define FML_STYLE_EXTRAMATH				9
#define FML_STYLE_USER1					10
#define FML_STYLE_USER2					11
#define FML_STYLE_OTHER					12

/////////////////////////////////////////////////////////////////////

#define FML_DOC_STATUS_ISEMPTY			0x00000001
#define FML_DOC_STATUS_LINESCOUNT		0x00000002
#define FML_DOC_STATUS_WIDTH			0x00000004
#define FML_DOC_STATUS_HEIGHT			0x00000008

/////////////////////////////////////////////////////////////////////
//
// The following values are connected with the attribute "__flag" inside the XML-buttons definition
// They must be modified only in concordance with the XML-buttons definition.
//

#define FML_MATHML_FLAG__MROW_KEEP1CHILD			0x80000000
#define FML_MATHML_FLAG__MROW_ARTIFICIAL			0x40000000
#define FML_MATHML_FLAG__MROW_CONTENT_AUTODETECT	0x20000000
#define FML_MATHML_FLAG__MROW_FORCE_RENDER			0x10000000

/////////////////////////////////////////////////////////////////////

#define FML_OPTION_FLAG__INPUT_MODE_CONTENT			0x08000000
#define FML_OPTION_FLAG__UNICODE_AS_NAME			0x04000000
#define FML_OPTION_FLAG__RELATIVE_IMG_PATH			0x02000000
#define FML_OPTION_FLAG__SHOW_NAVIGATE_INFO			0x01000000
#define FML_OPTION_FLAG__SHOW_NODE_TIPS_INFO		0x00800000
#define FML_OPTION_FLAG__XHTML_AS_BLOCK				0x00400000
#define FML_OPTION_FLAG__MULTICHAR_MI_NOT_ITALIC	0x00200000
#define FML_OPTION_FLAG__SHOW_REFRESHMATHML_WARN	0x00100000
#define FML_OPTION_FLAG__SHOULD_REFRESH_MATHML		0x00080000
#define FML_OPTION_FLAG__MATHML_STYLE_HAS_PRIORITY	0x00040000	/* editor settings has a priority over 'mathvariant' - italic, bold, "monospace" */

#define FML_OPTION_FLAG__EDITING_STYLE_AUTOG 		0x00020000
#define FML_OPTION_FLAG__RENDER2_ARCFUN				0x00010000
#define FML_OPTION_FLAG__PURE_CONTENT_OUT			0x00008000
#define FML_OPTION_FLAG__DIFF_AS_YX					0x00004000	/* d2y/dx2 = y'', dy/dx = y' */
#define FML_OPTION_FLAG__EDITING_STYLE_ONECHARID	0x00002000
#define FML_OPTION_FLAG__EDITING_STYLE_MOVEOUT		0x00001000
#define FML_OPTION_FLAG__EDITING_STYLE_DXDT			0x00000800

#define FML_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST	0x00000400

#define FML_OPTION_FLAG__SHOWNESTING				0x00000200
#define FML_OPTION_FLAG__SHOWREADONLY				0x00000100
#define FML_OPTION_FLAG__SHOW_INSERTMATHML_WARN		0x00000080
#define FML_OPTION_FLAG__CALC_TEXTRECT_BY_GLYPH		0x00000040


#define FML_OPTION_FLAG__DEFAULT	\
(FML_OPTION_FLAG__UNICODE_AS_NAME | FML_OPTION_FLAG__RELATIVE_IMG_PATH |\
 FML_OPTION_FLAG__USE_COMA_AS_LOCALE_SLIST | FML_OPTION_FLAG__SHOW_INSERTMATHML_WARN |\
 FML_OPTION_FLAG__SHOW_NODE_TIPS_INFO | FML_OPTION_FLAG__XHTML_AS_BLOCK)

/////////////////////////////////////////////////////////////////////

typedef struct tagFmlDocStatus
{
	bool	empty;
	long	lines;
	double	width;
	double	height;
} FmlDocStatus;

/////////////////////////////////////////////////////////////////////
#endif // __FORMULATOR_INCLUDE_FMLAPIDEFS__
