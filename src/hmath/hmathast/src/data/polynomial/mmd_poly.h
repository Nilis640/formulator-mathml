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

#ifndef __PARSEDSR_DATA_MMD_POLY_H__
#define __PARSEDSR_DATA_MMD_POLY_H__

#include "../object/obj_vec.h"

///////////////////////////////////////////////////////////////////////////////

#define MMD_Polynomial_MAX_POW_VALUE 500
class __HMATHLNGDLL__ MMD_Polynomial :  public MMD_Object
{
protected:
	long			max_pow;
	QString			var_name;
	MArray<UniWord>	term_pow;
	MArray<UniWord>	term_coef;
	int				is_normal;

protected:
	void _getNormalCoef_integer( void );
	void _getNormalCoef_real( void );
	void _getNormalCoef_complex( void );

public :
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, enum CDsrDataTypes nested_type = DSRDATA_TYPE_REAL, long term_c = 0 );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const QString& x_name, enum CDsrDataTypes nested_type = DSRDATA_TYPE_REAL );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, long is );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, CDSRIntegerStorage is );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, CDSRRealStorage is );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, CDSRComplexStorage is );
	MMD_Polynomial( const MMD_Polynomial& ms );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const CDSRArray<CDSRInteger>& src );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const CDSRArray<CDSRReal>& src );
	MMD_Polynomial( CParseDsrSymbolTable& _smbtable, const CDSRArray<CDSRComplex>& src );
	virtual MMD_Object* copy( const MMD_Polynomial *src );
	MMD_Polynomial* clone( void );
	virtual ~MMD_Polynomial( void );

	long getTermCount( void );
	long getMaxPow( void );
	void setMaxPow( void );
	long getNewMaxPow( void );

	void setVarName( QString& n );
	QString getVarName();
	QString getMmlVarName();
	UniWord& getPow( long term_no );
	UniWord& getCoef( long term_no );
	MArray<UniWord>& getNormalCoef( void );
	int isNormal( void );
	void setNormal( int v = 1 );
	void Normalize( void );
	void getValue( UniWord& ret, const UniWord& x );

	QString getTextView_Term4Complex( long i, UniWord coef, int ishead );
	QString getTextView_Term( long i, UniWord coef, int ishead );
	virtual QString getTextView( void );
	QString getFormulatorView_Term4Complex( long i, UniWord coef, int ishead );
	QString getFormulatorView_Term( long i, UniWord coef, int ishead );
	virtual QString getFormulatorView( void );
	QString getMmlView_Term( long i, UniWord coef, int ishead, QString& s_sign );
	virtual QString getMmlView( void );

	void getCoef( CDSRArray<CDSRInteger>& dest );
	void getCoef( CDSRArray<CDSRReal>& dest );
	void getCoef( CDSRArray<CDSRComplex>& dest );

	QString getMmlValue_One( CParseDsrSymbolTable& smbtable );
};

///////////////////////////////////////////////////////////////////////////////

typedef MMD_Polynomial* CDSRPolyStorage;

#endif
