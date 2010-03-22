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

#if !defined( __COMMON_LANGUAGE_MATHML_MML_FORMAT_H__ )
#define __COMMON_LANGUAGE_MATHML_MML_FORMAT_H__

#include <vector>
#include "../fmlcore_global.h"
#include "HUtils/ihkstring.h"
#include "HUtils/ixml.h"
#include "mml_char.h"

///////////////////////////////////////////////////////////////////////////////

#define MML_FORMAT_INDENT_SPACE			2
#define MML_ERGO_WIDTH					65
#define MML_4HTML_AMP					"amp;"
#define MAKE_MATHML_ATTR(l, r)			(QString(" %1=\'%2\'").arg(l).arg(r))

///////////////////////////////////////////////////////////////////////////////

__HERMITECH_FORMULIB__ QString mml_tag_self_nl( const QString& name, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_self_no_nl( const QString& name, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_no_nl( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_no_nl_part1( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_no_nl_part2( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_nl( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_nl_part1( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_nl_part2( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag_auto_nl( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tag( const QString& name, const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_root_tag_xmlns( const QString& body, long level = 0 );
__HERMITECH_FORMULIB__ QString mml_root_tag( const QString& body, long level = 0 );
__HERMITECH_FORMULIB__ QString mml_root_tag_4_html_mathml( int is_block, const QString& body, long level = 0 );
__HERMITECH_FORMULIB__ QString mml_table_tag( const QString& body, const QString& attr, long level );
__HERMITECH_FORMULIB__ QString mml_tablerow_tag( const QString& body, long level );
__HERMITECH_FORMULIB__ QString mml_tablecell_tag( const QString& body, long level, QString attr = _T("") );
__HERMITECH_FORMULIB__ QString mml_fence( const QString& body, const QString& left, const QString& right, long level );
__HERMITECH_FORMULIB__ void mml_validate4html( QString& src );
__HERMITECH_FORMULIB__ QString mml_row_tag_nl( const QString& body, long level );
__HERMITECH_FORMULIB__ QString mml_row_tag_no_nl( const QString& body, long level );
__HERMITECH_FORMULIB__ int mml_is_table_tag_correct( AST_TXML_Compound *_math_tags, std::vector<long>& line_length );
__HERMITECH_FORMULIB__ int mml_is_table_tag_correct( AST_TXML_Tag *_mtable, std::vector<long>& line_length );
__HERMITECH_FORMULIB__ AST_TXML_Compound* mml_get_table_tag_cell( AST_TXML_Tag *_mtable, long row_no, long col_no );
__HERMITECH_FORMULIB__ AST_TXML_Tag* mml_get_table_tag_mtr( AST_TXML_Tag *_mtable, long row_no );
__HERMITECH_FORMULIB__ AST_TXML_Tag* mml_get_table_tag_mtd( AST_TXML_Tag *_mtable, long row_no, long col_no );
__HERMITECH_FORMULIB__ int mml_is_table_tag_box_solid( AST_TXML_Tag* node );
__HERMITECH_FORMULIB__ int mml_is_token_tag_correct( AST_TXML_Tag *node );
__HERMITECH_FORMULIB__ int mml_is_italic_token_mi( const QString& _tag_name, const QString& _tag_body );
__HERMITECH_FORMULIB__ int mml_has_equivalent_attr( const AST_TXML_Compound* _new_attr, const QString attrname );
__HERMITECH_FORMULIB__ int mml_isAttrTakingNamedspace( QString attrName );

///////////////////////////////////////////////////////////////////////////////

int mml_is_root_tag( const QString& tag );
int mml_is_table_tag( const QString& tag );
int mml_is_fenced_tag( const QString& tag );
int mml_is_under_tag( const QString& tag );
int mml_is_over_tag( const QString& tag );
int mml_is_underover_tag( const QString& tag );
int mml_is_under_n_over_tag( const QString& tag );
int mml_is_frac_tag( const QString& tag );
int mml_is_sup_tag( const QString& tag );
int mml_is_sub_tag( const QString& tag );
int mml_is_mmultiscripts_tag( const QString& tag );
int mml_is_style_tag( const QString& tag );
int mml_is_mphantom_tag( const QString& tag );
int mml_is_maction_tag( const QString& tag );
int mml_is_mglyph_tag( const QString& tag );
int mml_is_tablerow_tag( const QString& tag );
int mml_is_tablecell_tag( const QString& tag );
int mml_is_row_tag( const QString& tag );
int mml_is_token_string_tag( const QString& tag );
int mml_is_token_space_tag( const QString& tag );
int mml_is_token_identifier_tag( const QString& tag );
int mml_is_token_number_tag( const QString& tag );
int mml_is_token_op_tag( const QString& tag );
int mml_is_token_text_tag( const QString& tag );
int mml_is_token_tag( const QString& tag );
int mml_is_default_cn_base( const QString& src );
int mml_is_cm_token( const QString& str );
int mml_is_cm_apply( const QString& str );

///////////////////////////////////////////////////////////////////////////////

#endif
