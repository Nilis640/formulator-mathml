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

#if !defined( __FORMULATOR_FMLCORE_BTN_H__ )
#define __FORMULATOR_FMLCORE_BTN_H__

#include "HUtils/igui.h"
#include "HUtils/idrawstruct.h"

#include "calc/vmi_calc.h"
#include "btn_mmldata.h"
#include "btn_graphics.h"
#include "btn_childpos.h"
#include "btn_attr.h"

///////////////////////////////////////////////////////////////////////////////

enum FNodeType
{
	NODE_MATHML = 0,
	NODE_FRAME,
	NODE_LINE,
	NODE_PLANETEXT,
	NODE_FORMULA,
	NODE_NONE,
};

class __HERMITECH_FORMULIB__ FBtn : public HKToolBarButton
{
protected:
	FBtnMMLData	to_mathml;
	FBtnAttr proplist;
	FBtnCalc coord_calculation;
	QVector<FBtnChildPos> ar_child;
	QVector<FBtnGraphics> ar_graphics;
	QVector<QString> input_properties;
	QString	formula_type_name, mathml_name, node_mathml_string, text_value, m_cannot_draw_width, m_cannot_draw_height;
	QColor text_color;
	qint32 node_type, vertical_center_type, vertical_center_value, rows, columns, text_style;

public:
	FBtn( void );
	const FBtn& operator = ( const FBtn& a );
        bool operator == ( const FBtn& /*a*/ ) const			{ return false; }

	void				setName( const QString& str )	{ formula_type_name = str; }
	const QString&		getName() const					{ return formula_type_name; }

	const QString&		getMathMLName( void ) const		{ return to_mathml.name; }
	const QString&		getMathMLValue( void ) const	{ return to_mathml.value; }
	const FBtnMMLData&	getMathMLData() const			{ return to_mathml; }
	const QString&		getNodeMathMLText( void ) const	{ return node_mathml_string; }

	void				setRows( long arg )				{ rows = arg; }
	long 				getRows() const					{ return rows; }
	void				setColumns( long arg )			{ columns = arg; }
	long				getColumns() const				{ return columns; }

	long getAlignmentType( void ) const					{ return vertical_center_type; }
	long getAlignmentValue( void ) const				{ return vertical_center_value; }


	int					isContentEatLeftOp() const		{ return proplist.isContentEatLeftOp(); }
	int					isFrameHidden() const			{ return proplist.isFrameHidden(); }
	const FBtnAttr&		getPropList() const 			{ return proplist; }

	const QString&		getCannotDraw_Width() const		{ return m_cannot_draw_width; }
	const QString&		getCannotDraw_Height() const	{ return m_cannot_draw_height; }

	void				SetToMathML( FBtnMMLData& arg_to_mathml )
														{ to_mathml = arg_to_mathml; }

	long				getNodeType() const				{ return node_type; }
	const QString&		getTextValue() const			{ return text_value; }
	long				getTextStyle() const			{ return text_style; }
	int					isFormulaTypeGraphics( long i )	const
														{ return ar_graphics.at(i).coord_type == COORD_TYPE_FORMULA; }
	int					isTextType() const				{ return getNodeType() == NODE_PLANETEXT; }

	FBtnCalc*			getCalc() { return &coord_calculation; }
	int					isPositionRunExist() const		{ return coord_calculation.size() > 0; }
	int					PositionRun( QVector<RealFde>& frame, QVector<RealFde>& frameMargin, QVector<RealFde>& primitive )
														{ return coord_calculation.PositionRun( frame, frameMargin, primitive ); }

	long				getChildCount() const			{ return ar_child.size();	}
	const FBtnChildPos&	getChildItem( long i ) const	{ return ar_child.at(i);	}
	long				getGraphicsCount() const		{ return ar_graphics.size();}
	const FBtnGraphics&	getGraphicsItem( long i ) const	{ return ar_graphics.at(i);	}

	int					isPropertyInput( QString str = "" ) const
														{ return str.length() > 0 ?	(input_properties.indexOf( str ) != -1) : 
																		(input_properties.size() > 0); }

protected:
	void				AddChildData( FBtnChildPos& fp )
														{ ar_child.push_back( fp ); }
	void				AddGraphicsData( FBtnGraphics& gp )
														{ ar_graphics.push_back( gp ); }

	__HERMITECH_FORMULIB__ friend QDataStream& operator << ( QDataStream& ar, const FBtn& lp );
	__HERMITECH_FORMULIB__ friend QDataStream& operator >> ( QDataStream& ar, FBtn& lp );
};

/////////////////////////////////////////////////////////////////////

#endif
