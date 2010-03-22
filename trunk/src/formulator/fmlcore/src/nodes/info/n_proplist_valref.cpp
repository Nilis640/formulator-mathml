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

#include "../../mathml/mml_defs.h"
#include "n_proplist.h"

///////////////////////////////////////////////////////////////////////////

#define CNode_TextProperty_MathArrLen 2
static QString CNode_TextProperty_MathArr[ CNode_TextProperty_MathArrLen ] = 
{
	_T("inline"),
	_T("block")
};

#define CNode_TextProperty_MathOverflowArrLen 4
static QString CNode_TextProperty_MathOverflowArr[ CNode_TextProperty_MathOverflowArrLen ] = 
{
	_T("scroll"),
	_T("elide"),
	_T("truncate"),
	_T("scale")
};

#define CNode_TextProperty_MathvariantArrLen 14
static QString CNode_TextProperty_MathvariantArr[ CNode_TextProperty_MathvariantArrLen ] = 
{
	_T("normal"),
	_T("bold"),
	_T("italic"),
	_T("bold-italic"),
	_T("double-struck"),
	_T("bold-fraktur"),
	_T("script"),
	_T("bold-script"),
	_T("fraktur"),
	_T("sans-serif"),
	_T("bold-sans-serif"),
	_T("sans-serif-italic"),
	_T("sans-serif-bold-italic"),
	_T("monospace")
};

#define CNode_TextProperty_MathsizeArrLen 3
static QString CNode_TextProperty_MathsizeArr[ CNode_TextProperty_MathsizeArrLen ] = 
{
	_T("normal"),
	_T("small"),
	_T("big")
};

#define CNode_TextProperty_MathcolorArrLen 17
static QString CNode_TextProperty_MathcolorArr[ CNode_TextProperty_MathcolorArrLen ] = 
{
	_T("black"),
	_T("green"),
	_T("silver"),
	_T("lime"),
	_T("gray"),
	_T("olive"),
	_T("white"),
	_T("yellow"),
	_T("maroon"),
	_T("navy"),
	_T("red"),
	_T("blue"),
	_T("purple"),
	_T("teal"),
	_T("fuchsia"),
	_T("aqua"),
	_T("orange")
};

#define CNode_TextProperty_MathwidthArrLen 7
static QString CNode_TextProperty_MathwidthArr[ CNode_TextProperty_MathwidthArrLen ] = 
{
	_T("veryverythinmathspace"),
	_T("verythinmathspace"),
	_T("thinmathspace"),
	_T("mediummathspace"),
	_T("thickmathspace"),
	_T("verythickmathspace"),
	_T("veryverythickmathspace")
};

#define CNode_TextProperty_ThicknessArrLen 3
static QString CNode_TextProperty_ThicknessArr[ CNode_TextProperty_ThicknessArrLen ] = 
{
	_T("thin"),
	_T("medium"),
	_T("thick")
};

#define CNode_TextProperty_HoralignArrLen 3
static QString CNode_TextProperty_HoralignArr[ CNode_TextProperty_HoralignArrLen ] = 
{
	_T("left"),
	_T("center"),
	_T("right")
};

#define CNode_TextProperty_MOFormArrLen 3
static QString CNode_TextProperty_MOFormArr[ CNode_TextProperty_MOFormArrLen ] = 
{
	_T("prefix"),
	_T("infix"),
	_T("postfix")
};

#define CNode_TextProperty_CNTypeArrLen 6
static QString CNode_TextProperty_CNTypeArr[ CNode_TextProperty_CNTypeArrLen ] = 
{
	_T("real"),
	_T("integer"),
	_T("rational"),
	_T("complex-cartesian"),
	_T("complex-polar"),
	_T("constant")
};

#define CNode_TextProperty_CITypeArrLen 13
static QString CNode_TextProperty_CITypeArr[ CNode_TextProperty_CITypeArrLen ] = 
{
	_T("integer"),
	_T("real"),
	_T("rational"),
	_T("complex-cartesian"),
	_T("complex-polar"),
	_T("complex"),
	_T("constant"),
	_T("function"),
	_T("interval"),
	_T("list"),
	_T("matrix"),
	_T("set"),
	_T("vector")
};

#define CNode_TextProperty_SetTypeArrLen 2
static QString CNode_TextProperty_SetTypeArr[ CNode_TextProperty_SetTypeArrLen ] = 
{
	_T("normal"),
	_T("multiset")
};

#define CNode_TextProperty_TendstoArrLen 3
static QString CNode_TextProperty_TendstoArr[ CNode_TextProperty_TendstoArrLen ] = 
{
	_T("above"),
	_T("below"),
	_T("two-sided")
};

#define CNode_TextProperty_IntervalTypeArrLen 4
static QString CNode_TextProperty_IntervalTypeArr[ CNode_TextProperty_IntervalTypeArrLen ] = 
{
	_T("closed"),
	_T("open"),
	_T("open-closed"),
	_T("closed-open")
};

#define CNode_TextProperty_ListOrderingArrLen 2
static QString CNode_TextProperty_ListOrderingArr[ CNode_TextProperty_ListOrderingArrLen ] = 
{
	_T("lexicographic"),
	_T("numeric")
};


#define CNode_TextProperty_DeclareOccurenceArrLen 3
static QString CNode_TextProperty_DeclareOccurenceArr[ CNode_TextProperty_DeclareOccurenceArrLen ] = 
{
	_T("function-model"),
	_T("prefix"),
	_T("infix")
};


#define CNode_TextProperty_DeclareScopeArrLen 2
static QString CNode_TextProperty_DeclareScopeArr[ CNode_TextProperty_DeclareScopeArrLen ] = 
{
	_T("local"),
	_T("global")
};

#define CNode_TextProperty_EncodingArrLen 4
static QString CNode_TextProperty_EncodingArr[ CNode_TextProperty_EncodingArrLen ] = 
{
	_T("MathML-Presentation"),
	_T("MathML-Content"),
	_T("TeX"),
	_T("OpenMath")
};

#define CNode_TextProperty_MaxsizeArrLen 8
static QString CNode_TextProperty_MaxsizeArr[ CNode_TextProperty_MaxsizeArrLen ] = 
{
	_T("infinity"),
	_T("veryverythinmathspace"),
	_T("verythinmathspace"),
	_T("thinmathspace"),
	_T("mediummathspace"),
	_T("thickmathspace"),
	_T("verythickmathspace"),
	_T("veryverythickmathspace")
};

#define CNode_TextProperty_NotationLen 20
static QString CNode_TextProperty_Notation[ CNode_TextProperty_NotationLen ] = 
{
	_T("longdiv"),
	_T("actuarial"),
	_T("radical"),
	_T("box"),
	_T("roundedbox"),
	_T("circle"),
	_T("left"),
	_T("right"),
	_T("top"),
	_T("bottom"),
	_T("updiagonalstrike"),
	_T("downdiagonalstrike"),
	_T("verticalstrike"),
	_T("horizontalstrike"),
	_T("joint-status"),
	_T("top-left"),
	_T("top-right"),
	_T("bottom-left"),
	_T("bottom-right"),
	_T("box-dashed")
};

#define CNode_TextProperty_AlignArrLen 5
static QString CNode_TextProperty_AlignArr[ CNode_TextProperty_AlignArrLen ] = 
{
	_T("top"),
	_T("bottom"),
	_T("center"),
	_T("baseline"),
	_T("axis")
};

#define CNode_TextProperty_RowalignArrLen 5
static QString CNode_TextProperty_RowalignArr[ CNode_TextProperty_RowalignArrLen ] = 
{
	_T("top"),
	_T("bottom"),
	_T("center"),
	_T("baseline"),
	_T("axis")
};

#define CNode_TextProperty_ColumnalignArrLen 3
static QString CNode_TextProperty_ColumnalignArr[ CNode_TextProperty_ColumnalignArrLen ] = 
{
	_T("left"),
	_T("center"),
	_T("right")
};

#define CNode_TextProperty_ColumnlinesArrLen 3
static QString CNode_TextProperty_ColumnlinesArr[ CNode_TextProperty_ColumnlinesArrLen ] = 
{
	_T("none"),
	_T("solid"),
	_T("dashed")
};

#define CNode_TextProperty_LinebreakArrLen 6
static QString CNode_TextProperty_LinebreakArr[ CNode_TextProperty_LinebreakArrLen ] = 
{
	FBL_VAL_auto,
	FBL_VAL_newline,
	FBL_VAL_indentingnewline,
	FBL_VAL_nobreak,
	FBL_VAL_goodbreak,
	FBL_VAL_badbreak
};

//#define CNode_TextProperty_ContentOpArrLen 83
QString CNode_TextProperty_ContentOpArr[ CNode_TextProperty_ContentOpArrLen ] = 
{
	_T("factorial"),
	_T("minus"),
	_T("abs"),
	_T("conjugate"),
	_T("arg"),
	_T("real"),
	_T("imaginary"),
	_T("floor"),
	_T("ceiling"),
	_T("not"),
	_T("ident"),
	_T("domain"),
	_T("codomain"),
	_T("image"),
	_T("sin"),
	_T("cos"),
	_T("tan"),
	_T("sec"),
	_T("csc"),
	_T("cot"),
	_T("sinh"),
	_T("cosh"),
	_T("tanh"),
	_T("sech"),
	_T("csch"),
	_T("coth"),
	_T("arcsin"),
	_T("arccos"),
	_T("arctan"),
	_T("arccosh"),
	_T("arccot"),
	_T("arccoth"),
	_T("arccsc"),
	_T("arccsch"),
	_T("arcsec"),
	_T("arcsech"),
	_T("arcsinh"),
	_T("arctanh"),
	_T("exp"),
	_T("ln"),
	_T("log"),
	_T("determinant"),
	_T("divergence"),
	_T("grad"),
	_T("curl"),
	_T("laplacian"),
	_T("card"),
	_T("quotient"),
	_T("divide"),
	_T("minus"),
	_T("power"),
	_T("rem"),
	_T("implies"),
	_T("equivalent"),
	_T("approx"),
	_T("setdiff"),
	_T("vectorproduct"),
	_T("scalarproduct"),
	_T("outerproduct"),
	_T("plus"),
	_T("times"),
	_T("max"),
	_T("min"),
	_T("gcd"),
	_T("lcm"),
	_T("mean"),
	_T("sdev"),
	_T("variance"),
	_T("median"),
	_T("mode"),
	_T("and"),
	_T("or"),
	_T("xor"),
	_T("union"),
	_T("intersect"),
	_T("cartesianproduct"),
	_T("int"),
	_T("sum"),
	_T("product"),
	_T("diff"),
	_T("partialdiff"),
	_T("forall"),
	_T("exists")
};

///////////////////////////////////////////////////////////////////////////

#define textPropertyDataMathML_Length	102

static CNode_TextPropertyData textPropertyDataMathML[ textPropertyDataMathML_Length ] =
{
	CNode_TextPropertyData( _T("class"),		_T("class"),		_T("common attribute"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("style"),		_T("style"),		_T("common attribute"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("id"),			_T("id"),			_T("common attribute"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("xref"),			_T("xref"),			_T("common attribute"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("xlink:href"),	_T("xlink:href"),	_T("common attribute"),	CNode_TextProperty_TEXT, -1 ),

	CNode_TextPropertyData( _T("macros"),		_T("macros"),		_T("a way of pointing to external macro definition files"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("mode"),			_T("mode"),			_T("defines rendering in a display style or an in-line style (deprecated)"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("display"),		_T("display"),		_T("defines rendering in a display style or an in-line style"), CNode_TextProperty_LIST, CNode_TextPropertyEnum_Math ),
	CNode_TextPropertyData( _T("overflow"),		_T("overflow"),		_T("suggestion of an alternative processing method"), CNode_TextProperty_LIST, CNode_TextPropertyEnum_MathOverflow ),
	CNode_TextPropertyData( _T("altimg"),		_T("altimg"),		_T("alternative image (for browsers that do not support embedded elements)"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("alttext"),		_T("alttext"),		_T("alternative text (for browsers that do not support embedded elements)"), CNode_TextProperty_TEXT, -1 ),

	CNode_TextPropertyData( _T("mathvariant"),	_T("mathvariant"),	_T("font style"),		CNode_TextProperty_LIST, 		CNode_TextPropertyEnum_Mathvariant ),
	CNode_TextPropertyData( _T("mathsize"),		_T("mathsize"),		_T("font size"),		CNode_TextProperty_COMBO_UNIT,	CNode_TextPropertyEnum_Mathsize ),
	CNode_TextPropertyData( _T("mathcolor"),	_T("mathcolor"),	_T("text color"),		CNode_TextProperty_COMBO_COLOR,	CNode_TextPropertyEnum_Mathcolor ),
	CNode_TextPropertyData( _T("mathbackground"),	_T("mathbackground"),	_T("background color"),	CNode_TextProperty_COMBO_COLOR,	CNode_TextPropertyEnum_Mathbackcolor ),

	CNode_TextPropertyData( _T("fontsize"),		_T("fontsize"),		_T("font size (deprecated)"),		CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("fontweight"),	_T("fontweight"),	_T("font weight (deprecated)"),		CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("fontstyle"),	_T("fontstyle"),	_T("font style (deprecated)"),		CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("fontfamily"),	_T("fontfamily"),	_T("font name (deprecated except for mglyph)"),	CNode_TextProperty_COMBO_FONT,	-1 ),
	CNode_TextPropertyData( _T("color"),		_T("color"),		_T("text/graphics color (deprecated)"),	CNode_TextProperty_COMBO_COLOR,	CNode_TextPropertyEnum_Mathcolor ),

	CNode_TextPropertyData( _T("form"), 		_T("form"), 		_T("operator form"), 				CNode_TextProperty_LIST, CNode_TextPropertyEnum_MOForm ),
	CNode_TextPropertyData( _T("fence"), 		_T("fence"), 		_T("operator is a fence"), 			CNode_TextProperty_BOOL, -1 ),
	CNode_TextPropertyData( _T("separator"), 	_T("separator"), 	_T("operator is a separator"), 		CNode_TextProperty_BOOL, -1 ),
	CNode_TextPropertyData( _T("stretchy"), 	_T("stretchy"), 	_T("operator is stretchy"), 		CNode_TextProperty_BOOL, -1 ),
	CNode_TextPropertyData( _T("symmetric"), 	_T("symmetric"), 	_T("operator is symmetric"), 		CNode_TextProperty_BOOL, -1 ),
	CNode_TextPropertyData( _T("largeop"), 		_T("largeop"), 		_T("operator is a large symbol"), 	CNode_TextProperty_BOOL, -1 ),
	CNode_TextPropertyData( _T("movablelimits"),_T("movablelimits"),_T("operator has movable limits"), 	CNode_TextProperty_BOOL, -1 ),
	CNode_TextPropertyData( _T("accent"), 		_T("accent"), 		_T("operator is an accent"), 		CNode_TextProperty_BOOL, -1 ),
	                                        
	CNode_TextPropertyData( _T("lspace"), 		_T("lspace"), 		_T("left spacing"), 	CNode_TextProperty_COMBO_UNIT, CNode_TextPropertyEnum_Mathwidth ),
	CNode_TextPropertyData( _T("rspace"), 		_T("rspace"), 		_T("right spacing"), 	CNode_TextProperty_COMBO_UNIT, CNode_TextPropertyEnum_Mathwidth ),
	CNode_TextPropertyData( _T("maxsize"), 		_T("maxsize"), 		_T("maximum size"), 	CNode_TextProperty_COMBO_UNIT, CNode_TextPropertyEnum_Maxsize ),
	CNode_TextPropertyData( _T("minsize"), 		_T("minsize"), 		_T("minimum size"), 	CNode_TextProperty_COMBO_UNIT, CNode_TextPropertyEnum_Mathwidth ),
	                                        
	CNode_TextPropertyData( _T("width"), 		_T("width"), 		_T("width of a whitespace"),  CNode_TextProperty_COMBO_UNIT, CNode_TextPropertyEnum_Mathwidth ),
	CNode_TextPropertyData( _T("height"), 		_T("height"), 		_T("height of a whitespace"), CNode_TextProperty_TEXT_NUM_UNIT, -1 ),
	CNode_TextPropertyData( _T("depth"), 		_T("depth"), 		_T("depth of a whitespace"),  CNode_TextProperty_TEXT_NUM_UNIT, -1 ),
	CNode_TextPropertyData( _T("linebreak"),	_T("linebreak"), 	_T("a linebreaking hint for a visual renderer"),  CNode_TextProperty_LIST, CNode_TextPropertyEnum_Linebreak ),

	CNode_TextPropertyData( _T("lquote"), 		_T("lquote"), 		_T("left quote"), 		CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("rquote"), 		_T("rquote"), 		_T("right quote"), 		CNode_TextProperty_TEXT, -1 ),

	CNode_TextPropertyData( _T("alt"), 			_T("alt"), 			_T("an alternate name for the glyph"), CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("index"), 		_T("index"), 		_T("the fontfamily and index  uniquely identify the mglyph"), CNode_TextProperty_TEXT_NUM, -1 ),

	CNode_TextPropertyData( _T("linethickness"),_T("linethickness"),_T("fraction bar thickness"),	CNode_TextProperty_LIST, CNode_TextPropertyEnum_Thickness ),
	CNode_TextPropertyData( _T("numalign"), 	_T("numalign"), 	_T("numerator alignment"), 		CNode_TextProperty_LIST, CNode_TextPropertyEnum_Horalign ),
	CNode_TextPropertyData( _T("denumalign"),	_T("denumalign"),	_T("denumerator alignment"), 	CNode_TextProperty_LIST, CNode_TextPropertyEnum_Horalign ),
	CNode_TextPropertyData( _T("bevelled"),		_T("bevelled"),		_T("change the way how fraction is displayed"), CNode_TextProperty_BOOL, -1 ),

	CNode_TextPropertyData( _T("scriptlevel"),	_T("scriptlevel"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("displaystyle"),	_T("displaystyle"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("scriptsizemultiplier"),	_T("scriptsizemultiplier"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("scriptminsize"),_T("scriptminsize"),_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("background"),	_T("background"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("veryverythinmathspace"),	_T("veryverythinmathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("verythinmathspace"),	_T("verythinmathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("thinmathspace"),	_T("thinmathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("mediummathspace"),	_T("mediummathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("thickmathspace"),	_T("thickmathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("verythickmathspace"),	_T("verythickmathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("verythickmathspace"),	_T("verythickmathspace"),	_T(""), CNode_TextProperty_NONE,	-1 ),

	CNode_TextPropertyData( _T("open"),			_T("open"),			_T("opening fence"), CNode_TextProperty_TEXT,	-1 ),
	CNode_TextPropertyData( _T("close"),		_T("close"),		_T("closing fence"), CNode_TextProperty_TEXT,	-1 ),
	CNode_TextPropertyData( _T("separators"),	_T("separators"),	_T("separators list"), CNode_TextProperty_TEXT,	-1 ),

	CNode_TextPropertyData( _T("notation"),		_T("notation"),		_T("a kind of rendering of the menclose element"),		CNode_TextProperty_COMBO, 	CNode_TextPropertyEnum_Notation ),

	CNode_TextPropertyData( _T("subscriptshift"),	_T("subscriptshift"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("superscriptshift"),	_T("superscriptshift"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("accentunder"),	_T("accentunder"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("accent"),		_T("accent"),		_T(""), CNode_TextProperty_NONE,	-1 ),

	CNode_TextPropertyData( _T("align"),		_T("align"),		_T("where to align the table with respect to its environment"),		CNode_TextProperty_COMBO, 	CNode_TextPropertyEnum_Align ),
	CNode_TextPropertyData( _T("rowalign"),		_T("rowalign"),		_T("how the entries in each row should be aligned"),		CNode_TextProperty_COMBO, 	CNode_TextPropertyEnum_Rowalign ),
	CNode_TextPropertyData( _T("columnalign"),	_T("columnalign"),	_T("how wide a column should be"),CNode_TextProperty_COMBO, 	CNode_TextPropertyEnum_Columnalign ),
	CNode_TextPropertyData( _T("groupalign"),	_T("groupalign"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("alignmentscope"),	_T("alignmentscope"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("columnwidth"),	_T("columnwidth"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("mtable.width"),	_T("width"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("rowspacing"),	_T("rowspacing"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("columnspacing"),	_T("columnspacing"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("rowlines"),		_T("rowlines"),		_T(""), CNode_TextProperty_COMBO,	CNode_TextPropertyEnum_Columnlines ),
	CNode_TextPropertyData( _T("columnlines"),	_T("columnlines"),	_T(""), CNode_TextProperty_COMBO,	CNode_TextPropertyEnum_Columnlines ),
	CNode_TextPropertyData( _T("frame"),		_T("frame"),		_T(""), CNode_TextProperty_LIST,	CNode_TextPropertyEnum_Columnlines ),
	CNode_TextPropertyData( _T("framespacing"),	_T("framespacing"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("equalrows"),	_T("equalrows"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("equalcolumns"),	_T("equalcolumns"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("mtable.displaystyle"),	_T("displaystyle"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("side"),	_T("side"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("minlabelspacing"),	_T("minlabelspacing"),	_T(""), CNode_TextProperty_NONE,	-1 ),

	CNode_TextPropertyData( _T("rowspan"),	_T("rowspan"),	_T(""), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("columnspan"),	_T("columnspan"),	_T(""), CNode_TextProperty_NONE,	-1 ),

	CNode_TextPropertyData( _T("edge"),	_T("edge"),	_T(""), CNode_TextProperty_NONE,	-1 ),

	CNode_TextPropertyData( _T("actiontype"),	_T("actiontype"),	_T("a deprecated attribute"), CNode_TextProperty_NONE,	-1 ),
	CNode_TextPropertyData( _T("selection"),	_T("selection"),	_T("a deprecated attribute"), CNode_TextProperty_NONE,	-1 ),

	CNode_TextPropertyData( _T("definitionURL"),	_T("definitionURL"),	_T("external definition of the semantics"), 	CNode_TextProperty_TEXT, -1 ),
	CNode_TextPropertyData( _T("encoding"), 	_T("encoding"),		_T("encoding of the annotation"), 			 	CNode_TextProperty_COMBO, CNode_TextPropertyEnum_Encoding ),

	CNode_TextPropertyData( _T("cn.type"), 		_T("type"), 		_T("type of the number"), 							CNode_TextProperty_LIST, CNode_TextPropertyEnum_CNType ),
	CNode_TextPropertyData( _T("declare.type"),	_T("type"), 		_T("type of the first child of a 'declare' element"),CNode_TextProperty_COMBO, CNode_TextPropertyEnum_CIType ),
	CNode_TextPropertyData( _T("ci.type"), 		_T("type"), 		_T("type of the identifier"), 						CNode_TextProperty_COMBO, CNode_TextPropertyEnum_CIType ),
	CNode_TextPropertyData( _T("set.type"), 	_T("type"), 		_T("type of the set"), 								CNode_TextProperty_LIST, CNode_TextPropertyEnum_SetType ),
	CNode_TextPropertyData( _T("tendsto.type"),	_T("type"), 		_T("type of the 'tendsto' element"), 				CNode_TextProperty_LIST, CNode_TextPropertyEnum_Tendsto ),

	CNode_TextPropertyData( _T("base"), 		_T("base"), 		_T("numerical base of the number"), 				CNode_TextProperty_TEXT_NUM, -1 ),
	CNode_TextPropertyData( _T("nargs"), 		_T("nargs"),		_T("number of arguments"), 							CNode_TextProperty_TEXT_NUM, -1 ),
	CNode_TextPropertyData( _T("occurrence"),	_T("occurrence"),	_T("occurrence for operator declarations"),			CNode_TextProperty_LIST, CNode_TextPropertyEnum_DeclareOccurence ),
	CNode_TextPropertyData( _T("scope"),		_T("scope"),		_T("scope of applicability"), 						CNode_TextProperty_LIST, CNode_TextPropertyEnum_DeclareScope ),
	CNode_TextPropertyData( _T("closure"),		_T("closure"),		_T("closure of the interval"), 						CNode_TextProperty_LIST, CNode_TextPropertyEnum_IntervalType ),
	CNode_TextPropertyData( _T("order"),		_T("order"),		_T("ordering on the list"), 						CNode_TextProperty_LIST, CNode_TextPropertyEnum_ListOrdering ),

	CNode_TextPropertyData( TXTPROP_IKEY__OP,	TXTPROP_IKEY__OP,	_T("internal property: the name of the operator defined by MathML"),CNode_TextProperty_COMBO, CNode_TextPropertyEnum_ContentOp ),
	CNode_TextPropertyData( TXTPROP_IKEY__BVAR,	TXTPROP_IKEY__BVAR,	_T("internal property: the number of child 'bvar' elements"),		CNode_TextProperty_TEXT_NUM, -1 )
};

///////////////////////////////////////////////////////////////////////////

void getNodeTextPropertyCtrlInstanceData( char typeCtrlInstance, QString*& head, long& number )
{
	head = 0;
	number = 0;
	switch( typeCtrlInstance )
	{
	case CNode_TextPropertyEnum_Math:
		head = CNode_TextProperty_MathArr;
		number = CNode_TextProperty_MathArrLen;
		break;

	case CNode_TextPropertyEnum_MathOverflow:
		head = CNode_TextProperty_MathOverflowArr;
		number = CNode_TextProperty_MathOverflowArrLen;
		break;

	case CNode_TextPropertyEnum_Mathvariant:
		head = CNode_TextProperty_MathvariantArr;
		number = CNode_TextProperty_MathvariantArrLen;
		break;

	case CNode_TextPropertyEnum_Mathsize:
		head = CNode_TextProperty_MathsizeArr;
		number = CNode_TextProperty_MathsizeArrLen;
		break;

	case CNode_TextPropertyEnum_Mathcolor:
		head = CNode_TextProperty_MathcolorArr;
		number = CNode_TextProperty_MathcolorArrLen;
		break;

	case CNode_TextPropertyEnum_Mathwidth:
		head = CNode_TextProperty_MathwidthArr;
		number = CNode_TextProperty_MathwidthArrLen;
		break;

	case CNode_TextPropertyEnum_Thickness:
		head = CNode_TextProperty_ThicknessArr;
		number = CNode_TextProperty_ThicknessArrLen;
		break;

	case CNode_TextPropertyEnum_Horalign:
		head = CNode_TextProperty_HoralignArr;
		number = CNode_TextProperty_HoralignArrLen;
		break;

	case CNode_TextPropertyEnum_MOForm:
		head = CNode_TextProperty_MOFormArr;
		number = CNode_TextProperty_MOFormArrLen;
		break;

	case CNode_TextPropertyEnum_CNType:
		head = CNode_TextProperty_CNTypeArr;
		number = CNode_TextProperty_CNTypeArrLen;
		break;

	case CNode_TextPropertyEnum_CIType:
		head = CNode_TextProperty_CITypeArr;
		number = CNode_TextProperty_CITypeArrLen;
		break;

	case CNode_TextPropertyEnum_SetType:
		head = CNode_TextProperty_SetTypeArr;
		number = CNode_TextProperty_SetTypeArrLen;
		break;

	case CNode_TextPropertyEnum_Tendsto:
		head = CNode_TextProperty_TendstoArr;
		number = CNode_TextProperty_TendstoArrLen;
		break;

	case CNode_TextPropertyEnum_IntervalType:
		head = CNode_TextProperty_IntervalTypeArr;
		number = CNode_TextProperty_IntervalTypeArrLen;
		break;

	case CNode_TextPropertyEnum_ListOrdering:
		head = CNode_TextProperty_ListOrderingArr;
		number = CNode_TextProperty_ListOrderingArrLen;
		break;

	case CNode_TextPropertyEnum_DeclareOccurence:
		head = CNode_TextProperty_DeclareOccurenceArr;
		number = CNode_TextProperty_DeclareOccurenceArrLen;
		break;

	case CNode_TextPropertyEnum_DeclareScope:
		head = CNode_TextProperty_DeclareScopeArr;
		number = CNode_TextProperty_DeclareScopeArrLen;
		break;

	case CNode_TextPropertyEnum_Encoding:
		head = CNode_TextProperty_EncodingArr;
		number = CNode_TextProperty_EncodingArrLen;
		break;

	case CNode_TextPropertyEnum_Maxsize:
		head = CNode_TextProperty_MaxsizeArr;
		number = CNode_TextProperty_MaxsizeArrLen;
		break;

	case CNode_TextPropertyEnum_ContentOp:
		head = CNode_TextProperty_ContentOpArr;
		number = CNode_TextProperty_ContentOpArrLen;
		break;

	case CNode_TextPropertyEnum_Notation:
		head = CNode_TextProperty_Notation;
		number = CNode_TextProperty_NotationLen;
		break;

	case CNode_TextPropertyEnum_Align:
		head = CNode_TextProperty_AlignArr;
		number = CNode_TextProperty_AlignArrLen;
		break;

	case CNode_TextPropertyEnum_Rowalign:
		head = CNode_TextProperty_RowalignArr;
		number = CNode_TextProperty_RowalignArrLen;
		break;

	case CNode_TextPropertyEnum_Columnalign:
		head = CNode_TextProperty_ColumnalignArr;
		number = CNode_TextProperty_ColumnalignArrLen;
		break;

	case CNode_TextPropertyEnum_Columnlines:
		head = CNode_TextProperty_ColumnlinesArr;
		number = CNode_TextProperty_ColumnlinesArrLen;
		break;

	case CNode_TextPropertyEnum_Linebreak:
		head = CNode_TextProperty_LinebreakArr;
		number = CNode_TextProperty_LinebreakArrLen;
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////

CMMLNodeAttrList::CMMLNodeAttrList() :
	m_tagname2opid( CNode_TextProperty_ContentOpArr, CNode_TextProperty_ContentOpArrLen )
{
	for( long i = 0; i < textPropertyDataMathML_Length; i++ )
		setParameter( textPropertyDataMathML[ i ] );
	setMapTag2Attr();
}

long CMMLNodeAttrList::getContentMMLOperatorProperty( QString name )
{
	long *p = m_tagname2opid.isFind( name );
	if( p && *p >= 0 && *p < CNode_TextProperty_ContentOpArrLen )
		return *p;
	return -1;
}

///////////////////////////////////////////////////////////////////////////
