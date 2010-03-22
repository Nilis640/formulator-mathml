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

#if !defined( __FORMULATOR_FMLCORE_BTN_FROMXML_H__ )
#define __FORMULATOR_FMLCORE_BTN_FROMXML_H__

#include "HUtils/ixml.h"
#include "HUtils/imessage.h"

#include "btn.h"

///////////////////////////////////////////////////////////////////////////////

class FBtnVM;

class __HERMITECH_FORMULIB__ FXmlBtn : public FBtn
{
public:
	FXmlBtn() : FBtn() {}
	const FXmlBtn& operator = ( const FXmlBtn& a ) { (FBtn&) (*this) = (const FBtn&)a; return *this; }
	int Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page = 0 );

protected:
	void validateVerticalCenter( FBtnVM& vm );
	void AddButtonData( AST_TXML_Tag *root, FBtnVM& vm );
	void AddAccels( AST_TXML_Tag *root, FBtnVM& vm );
	int AddChildData( AST_TXML_Tag *root, FBtnVM& vm );
	int AddChildData_read_align( AST_TXML_Tag *node, FBtnChildPos& child, FBtnVM& vm );
	int AddGraphicsData( AST_TXML_Tag *root, FBtnVM& vm );
	int AddGraphicsData_read_width( AST_TXML_Tag *node, FBtnGraphics& child, FBtnVM& vm );
	int AddGraphicsData_spline( AST_TXML_Tag *tag, FBtnGraphics &child, FBtnVM& vm );
	int AddNodeType_NODE_MATHML(AST_TXML_Tag *root, FBtnVM& vm);
	int CompileCalculationScheme( AST_TXML_Tag *root, FBtnVM& vm );
	void SetFormulaName( AST_TXML_Tag *root );
	void SetToMathML( AST_TXML_Tag *root, FBtnVM& vm );
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FXmlBtn& lp )
{
	return ar << (const FBtn&) lp;
}

inline
QDataStream& operator >> ( QDataStream& ar, FXmlBtn& lp )
{
	return ar >> (FBtn&) lp;
}

/////////////////////////////////////////////////////////////////////

int bb_read_attr_linethickness( AST_TXML_Tag *node, const QString& attr_name, RealFde& child_width );
int bb_read_attr_align_vertical( AST_TXML_Tag *node, const QString& attr_name, long& child_align );
int bb_read_attr_align_vertical( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& child_align, long defaultAlign );
int bb_read_attr_align_horizontal( AST_TXML_Tag *node, const QString& attr_name, long& child_align );
int bb_read_attr_align_horizontal( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& child_align, long defaultAlign );
int bb_read_attr_table_lines( AST_TXML_Tag *node, const QString& attr_name, std::vector<long>& penStyle );

/////////////////////////////////////////////////////////////////////

#endif
