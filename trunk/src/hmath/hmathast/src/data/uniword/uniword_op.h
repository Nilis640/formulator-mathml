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

#ifndef __PARSEDSR_DATA_UNIWORD_UNIWORD_OP_H__
#define __PARSEDSR_DATA_UNIWORD_UNIWORD_OP_H__

#include "uniword.h"

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
UniWord _create_lparam( long p1 = 0, long p2 = 0, long p3 = 0, long p4 = 0 );
__HMATHLNGDLL__
void _set_null( UniWord& ret );
__HMATHLNGDLL__
int _is_null( const UniWord& ret );
__HMATHLNGDLL__
int _is_negative_number( const UniWord& ret );
__HMATHLNGDLL__
int _is_number_one( const UniWord& ret );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, const MMD_Address& v_addr );
__HMATHLNGDLL__
QString getFormulatorView( CParseDsrSymbolTable& smbtable, const MMD_Address& v_addr );
__HMATHLNGDLL__
QString getMmlView( CParseDsrSymbolTable& smbtable, const MMD_Address& v_addr );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSRNaturalStorage v );
__HMATHLNGDLL__
QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRNaturalStorage v );
__HMATHLNGDLL__
QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRNaturalStorage v );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSRIntegerStorage v );
__HMATHLNGDLL__
QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRIntegerStorage v );
__HMATHLNGDLL__
QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRIntegerStorage v );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSRRationalStorage v );
__HMATHLNGDLL__
QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRRationalStorage v );
__HMATHLNGDLL__
QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRRationalStorage v );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSRRealStorage v );
__HMATHLNGDLL__
QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRRealStorage v );
__HMATHLNGDLL__
QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRRealStorage v );
__HMATHLNGDLL__
QString getTextView_Complex2StrPart( CParseDsrSymbolTable& smbtable, CDSRComplex v, QString& r, QString& i, QString& s );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSRComplex v );
__HMATHLNGDLL__
QString getFormulatorView_Complex2StrPart( CParseDsrSymbolTable& smbtable, CDSRComplex v, QString& r, QString& i, QString& s );
__HMATHLNGDLL__
QString getFormulatorView( CParseDsrSymbolTable& smbtable, CDSRComplex v );
__HMATHLNGDLL__
QString getMmlView_Complex2StrPart( CParseDsrSymbolTable& smbtable, CDSRComplex v, QString& r, QString& i, QString& s );
__HMATHLNGDLL__
QString getMmlView( CParseDsrSymbolTable& smbtable, CDSRComplex v );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
int _dsrop_Add_scalar( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
int _dsrop_Sub_scalar( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
int _dsrop_Plus_scalar( UniWord& ret, UniWord a );
__HMATHLNGDLL__
int _dsrop_Minus_scalar( UniWord& ret, UniWord a );
__HMATHLNGDLL__
int _dsrop_Mul_scalar( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
int _dsrop_Div_scalar( UniWord& ret, UniWord a, UniWord b );


__HMATHLNGDLL__
UniWord _dsrop_Add_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Add_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Add_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Add_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Add_complex( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_Sub_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Sub_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Sub_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Sub_complex( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_Plus_natural( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Plus_integer( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Plus_rational( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Plus_real( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Plus_complex( UniWord& ret, UniWord a );

__HMATHLNGDLL__
UniWord _dsrop_Minus_integer( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Minus_rational( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Minus_real( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Minus_complex( UniWord& ret, UniWord a );

__HMATHLNGDLL__
UniWord _dsrop_Mul_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Mul_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Mul_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Mul_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Mul_complex( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_Div_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Div_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Div_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Div_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Div_complex( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_Mod_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Mod_integer( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_IntPow_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_IntPow_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_IntPow_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_IntPow_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_IntPow_complex( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_IntPow_group_string( UniWord& ret, UniWord a, UniWord b );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
UniWord _dsrop_Less_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Less_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Less_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Less_real( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_LessEq_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_LessEq_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_LessEq_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_LessEq_real( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_Greater_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Greater_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Greater_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Greater_real( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_GreaterEq_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_GreaterEq_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_GreaterEq_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_GreaterEq_real( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_Equal_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Equal_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Equal_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Equal_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Equal_complex( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_Equal_bool( UniWord& ret, UniWord a, UniWord b );

__HMATHLNGDLL__
UniWord _dsrop_NotEqual_natural( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_NotEqual_integer( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_NotEqual_rational( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_NotEqual_real( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_NotEqual_complex( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_NotEqual_bool( UniWord& ret, UniWord a, UniWord b );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
UniWord _dsrop_Convert_natural2integer( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_natural2rational( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_natural2real( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_natural2complex( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_integer2rational( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_integer2real( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_integer2complex( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_rational2real( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_rational2complex( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_real2complex( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_natural2bits( UniWord& ret, UniWord a );
__HMATHLNGDLL__
UniWord _dsrop_Convert_integer2bits( UniWord& ret, UniWord a );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
UniWord _dsrop_BoolAnd_bool( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_BoolOr_bool( UniWord& ret, UniWord a, UniWord b );
__HMATHLNGDLL__
UniWord _dsrop_BoolNot_bool( UniWord& ret, UniWord a );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
void _dsrop_abs_natural( UniWord *arg );
__HMATHLNGDLL__
void _dsrop_abs_integer( UniWord *arg );
__HMATHLNGDLL__
void _dsrop_abs_rational( UniWord *arg );
__HMATHLNGDLL__
void _dsrop_abs_real( UniWord *arg );
__HMATHLNGDLL__
void _dsrop_abs_complex( UniWord *arg );

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
CDSRReal arch( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arch( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arccos( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arccos( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arccosec( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arccosec( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arcsec( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arcsec( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arcsin( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arcsin( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arctg( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arctg( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arcth( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arcth( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal arsh( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex arsh( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal ch( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex ch( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal cosec( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex cosec( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal cosech( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex cosech( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal ctg( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex ctg( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal cth( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex cth( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal sec( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex sec( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal sech( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex sech( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal sh( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex sh( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal tg( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex tg( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal th( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex th( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal lg( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex lg( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal ln( CDSRReal a );
__HMATHLNGDLL__
CDSRComplex ln( CDSRComplex a );
__HMATHLNGDLL__
CDSRReal pow10( CDSRReal a );

///////////////////////////////////////////////////////////////////////////////

#endif
