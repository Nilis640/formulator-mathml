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

#if !defined( __FORMULATOR_FBTN_TAGS_H__ )
#define __FORMULATOR_FBTN_TAGS_H__

#include "HUtils/ihkstring.h"

#define FBL_TAG_math					"math"
#define FBL_TAG_toolbar					_T("toolbar")
#define FBL_TAG_button					_T("button")
#define FBL_TAG_frames					_T("frames")
#define FBL_TAG_frame					_T("frame")
#define FBL_TAG_objects					_T("objects")
#define FBL_TAG_object					_T("object")
#define FBL_TAG_point					_T("point")
#define FBL_TAG_expr					_T("expr")
#define FBL_TAG_align					_T("align")
#define FBL_TAG_mathml					_T("mathml")
#define FBL_TAG_mathml_template			_T("mathml-template")
#define FBL_TAG_dsr						_T("dsr")
#define FBL_TAG_nodes					_T("nodes")
#define FBL_TAG_line					_T("line")
#define FBL_TAG_formula					_T("formula")
#define FBL_TAG_hotkeys					_T("hot-keys")
#define FBL_TAG_hotkey					_T("hot-key")
#define FBL_TAG_sequence				_T("sequence")

#define FBL_ATTR_mathml_template		_T("mathml-template")
#define FBL_ATTR_id						_T("id")
#define FBL_ATTR_visible				_T("visible")
#define FBL_ATTR_name					_T("name")
#define FBL_ATTR_caption				_T("caption")
#define FBL_ATTR_image					_T("image")
#define FBL_ATTR_image_hot				_T("image-hot")
#define FBL_ATTR_image_disabled			_T("image-disabled")
#define FBL_ATTR_image_title			_T("image-title")
#define FBL_ATTR_image_title_hot		_T("image-title-hot")
#define FBL_ATTR_image_title_disabled	_T("image-title-disabled")
#define FBL_ATTR_tooltip				_T("tooltip")
#define FBL_ATTR_left					_T("left")		
#define FBL_ATTR_top					_T("top")		
#define FBL_ATTR_right					_T("right")		
#define FBL_ATTR_bottom					_T("bottom")	 
#define FBL_ATTR_valign					_T("valign")	 
#define FBL_ATTR_halign					_T("halign")
#define FBL_ATTR_level					_T("level")
#define FBL_ATTR_shape					_T("shape")
#define FBL_ATTR_type					_T("type")		
#define FBL_ATTR_color					_T("color")		
#define FBL_ATTR_color_red				_T("color_red")	
#define FBL_ATTR_color_green			_T("color_green")
#define FBL_ATTR_color_blue				_T("color_blue") 
#define FBL_ATTR_height					_T("height")
#define FBL_ATTR_width					_T("width")	
#define FBL_ATTR_style					_T("style")		
#define FBL_ATTR_pen_style				_T("pen-style")
#define FBL_ATTR_size					_T("size")		
#define FBL_ATTR_confidence				_T("confidence") 
#define FBL_ATTR_frame					_T("frame")		
#define FBL_ATTR_side					_T("side")
#define FBL_ATTR_etalon					_T("etalon")
#define FBL_ATTR_head					_T("head")	
#define FBL_ATTR_tail					_T("tail")
#define FBL_ATTR_separator				_T("separator")	
#define FBL_ATTR_param					_T("param")		
#define FBL_ATTR_bracket				_T("bracket")		
#define FBL_ATTR_value					_T("value")		
#define FBL_ATTR_rows					_T("rows")
#define FBL_ATTR_columns				_T("columns")
#define FBL_ATTR_plane					_T("plane")
#define FBL_ATTR_alt_group				_T("alt-group")
#define FBL_ATTR_name					_T("name")
#define FBL_ATTR_type					_T("type")
#define FBL_ATTR_mathml_support			_T("mathml-support")
#define FBL_ATTR_mathml_form			_T("mathml-form")
#define FBL_ATTR_frame					_T("frame")
#define FBL_ATTR_vertical_center_type	_T("vertical-center-type")
#define FBL_ATTR_vertical_center_value	_T("vertical-center-value")
#define FBL_ATTR_enable					_T("enable")
#define FBL_ATTR_bind_to				_T("bind-to")
#define FBL_ATTR_bevelled				_T("bevelled")
#define FBL_ATTR_ctrl					_T("ctrl")
#define FBL_ATTR_alt					_T("alt")
#define FBL_ATTR_shift					_T("shift")
#define FBL_ATTR_key					_T("key")
#define FBL_ATTR_type					_T("type")
#define FBL_ATTR_x						_T("x")
#define FBL_ATTR_y						_T("y")
#define FBL_ATTR_offset_direction_x		_T("offset-direction-x")
#define FBL_ATTR_offset_direction_y		_T("offset-direction-y")
#define FBL_ATTR_group					_T("group")
#define FBL_ATTR___flag					_T("__flag")
#define FBL_ATTR_eatleftop				_T("eatleftop")
#define FBL_ATTR_disabledFrame			_T("disabled-frame")
#define FBL_ATTR_cannot_draw_width		_T("cannot-draw-width")
#define FBL_ATTR_cannot_draw_height		_T("cannot-draw-height")
#define FBL_ATTR_encoding				"encoding"
#define FBL_ATTR_stretchy				"stretchy"
#define FBL_ATTR_symmetric				"symmetric"
#define FBL_ATTR_largeop				"largeop"
#define FBL_ATTR_lspace					"lspace"
#define FBL_ATTR_rspace					"rspace"

#define FBL_VAL_postfix					_T("postfix")		
#define FBL_VAL_prefix					_T("prefix")		
#define FBL_VAL_infix					_T("infix")		
#define FBL_VAL_line					_T("line")		
#define FBL_VAL_rectangle				_T("rectangle")		
#define FBL_VAL_ellipse					_T("ellipse")		
#define FBL_VAL_quater_arc				_T("quater arc")
#define FBL_VAL_text					_T("text")
#define FBL_VAL_spline					_T("spline")
#define FBL_VAL_spline_ex				_T("spline-ex")
#define FBL_VAL_moveto					_T("moveto")
#define FBL_VAL_lineto					_T("lineto")
#define FBL_VAL_splnto					_T("splnto")
#define FBL_VAL_percent					_T("percent")
#define FBL_VAL_formula					_T("formula")
#define FBL_VAL_fixed					_T("fixed")
#define FBL_VAL_variable				_T("variable")
#define FBL_VAL_black					_T("black")
#define FBL_VAL_red						_T("red")
#define FBL_VAL_green					_T("green")
#define FBL_VAL_blue					_T("blue")
#define FBL_VAL_white					_T("white")
#define FBL_VAL_light_red				_T("light red")
#define FBL_VAL_light_green				_T("light green")
#define FBL_VAL_light_blue				_T("light blue")
#define FBL_VAL_grey					_T("grey")
#define FBL_VAL_parent					_T("parent")
#define FBL_VAL_frame					_T("frame")
#define FBL_VAL_child					_T("child")
#define FBL_VAL_child1					_T("child1")
#define FBL_VAL_child2					_T("child2")
#define FBL_VAL_default					_T("default")
#define FBL_VAL_formula					_T("formula")
#define FBL_VAL_table					_T("table")
#define FBL_VAL_greek					_T("greek")
#define FBL_VAL_number					_T("number")
#define FBL_VAL_vector					_T("vector")
#define FBL_VAL_keyword					_T("keyword")
#define FBL_VAL_ask						_T("ask")
#define FBL_VAL_NW						_T("NW")
#define FBL_VAL_NE						_T("NE")
#define FBL_VAL_SW						_T("SW")
#define FBL_VAL_SE						_T("SE")
#define FBL_VAL_on						_T("on")
#define FBL_VAL_off						_T("off")
#define FBL_VAL_compound				_T("compound")
#define FBL_VAL_yes						_T("yes")
#define FBL_VAL_no						_T("no")
#define FBL_VAL_bracket					_T("bracket")
#define FBL_VAL_table					_T("table")
#define FBL_VAL_object					_T("object")
#define FBL_VAL_or						_T("or")
#define FBL_VAL_and						_T("and")
#define FBL_VAL_true					_T("true")
#define FBL_VAL_false					_T("false")
#define FBL_VAL_medium					_T("medium")
#define FBL_VAL_thin					_T("thin")
#define FBL_VAL_thick					_T("thick")
#define FBL_VAL_center					_T("center")		
#define FBL_VAL_left					_T("left")		
#define FBL_VAL_top						_T("top")		
#define FBL_VAL_right					_T("right")		
#define FBL_VAL_bottom					_T("bottom")		
#define FBL_VAL_mathml					_T("mathml")
#define FBL_VAL_average					_T("average")
#define FBL_VAL_blank_average			_T("blank-average")
#define FBL_VAL_half					_T("half")
#define FBL_VAL_up						_T("up")
#define FBL_VAL_down					_T("down")
#define FBL_VAL_tab						_T("tab")
#define FBL_VAL_enter					_T("enter")
#define FBL_VAL_baseline				_T("baseline")
#define FBL_VAL_solid					_T("solid")
#define FBL_VAL_dash					_T("dash")
#define FBL_VAL_dot						_T("dot")
#define FBL_VAL_dash_dot				_T("dash-dot")
#define FBL_VAL_dash_dot_dot			_T("dash-dot-dot")
#define FBL_VAL_dashed					"dashed"

#endif
