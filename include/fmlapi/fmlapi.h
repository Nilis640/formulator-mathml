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

#if !defined( __FORMULATOR_INCLUDE_FMLAPI_H__ )
#define __FORMULATOR_INCLUDE_FMLAPI_H__

#include "fmlexport.h"
#include "fmlapidefs.h"

extern "C" FMLAPI_API ulong fmlapiCreate( const ushort* key );
extern "C" FMLAPI_API bool fmlapiDelete( ulong id );

extern "C" FMLAPI_API bool fmlapiSetMathML( ulong id, const ushort* bstrMathML );
extern "C" FMLAPI_API const ushort* fmlapiGetMathML( ulong id );
extern "C" FMLAPI_API const ushort* fmlapiGetMathMLStrContent( ulong id );

extern "C" FMLAPI_API bool fmlapiSetBackgroundColor( ulong id, ulong value );
extern "C" FMLAPI_API ulong fmlapiGetBackgroundColor( ulong id );

extern "C" FMLAPI_API bool fmlapiSetShowReadOnly( ulong id, bool value );
extern "C" FMLAPI_API bool fmlapiGetShowReadOnly( ulong id );

extern "C" FMLAPI_API bool fmlapiSetLineSpacing( ulong id, double value );
extern "C" FMLAPI_API double fmlapiGetLineSpacing( ulong id );

extern "C" FMLAPI_API bool fmlapiSetScale( ulong id, double value );
extern "C" FMLAPI_API double fmlapiGetScale( ulong id );

extern "C" FMLAPI_API bool fmlapiSetTopIndent( ulong id, double value );
extern "C" FMLAPI_API double fmlapiGetTopIndent( ulong id );
extern "C" FMLAPI_API bool fmlapiSetBottomIndent( ulong id, double value );
extern "C" FMLAPI_API double fmlapiGetBottomIndent( ulong id );
extern "C" FMLAPI_API bool fmlapiSetLeftIndent( ulong id, double value );
extern "C" FMLAPI_API double fmlapiGetLeftIndent( ulong id );
extern "C" FMLAPI_API bool fmlapiSetRightIndent( ulong id, double value );
extern "C" FMLAPI_API double fmlapiGetRightIndent( ulong id );

extern "C" FMLAPI_API bool fmlapiSetShowInvisibleElements( ulong id, bool value );
extern "C" FMLAPI_API bool fmlapiGetShowInvisibleElements( ulong id );

extern "C" FMLAPI_API bool fmlapiSetStyleFaceName( ulong id, long style, const ushort* value );
extern "C" FMLAPI_API const ushort* fmlapiGetStyleFaceName( ulong id, long style );
extern "C" FMLAPI_API bool fmlapiSetStyleBold( ulong id, long style, bool value );
extern "C" FMLAPI_API bool fmlapiGetStyleBold( ulong id, long style );
extern "C" FMLAPI_API bool fmlapiSetStyleItalic( ulong id, long style, bool value );
extern "C" FMLAPI_API bool fmlapiGetStyleItalic( ulong id, long style );
extern "C" FMLAPI_API bool fmlapiSetStyleColor( ulong id, long style, ulong value );
extern "C" FMLAPI_API ulong fmlapiGetStyleColor( ulong id, long style );

extern "C" FMLAPI_API bool fmlapiSetSymbolSize( ulong id, long type, double value );
extern "C" FMLAPI_API double fmlapiGetSymbolSize( ulong id, long type );

extern "C" FMLAPI_API bool fmlapiSetLocale( ulong id, ulong language, ulong country );
extern "C" FMLAPI_API bool fmlapiGetLocale( ulong id, ulong& language, ulong& country );

extern "C" FMLAPI_API bool fmlapiSetExpressionColor( ulong id, ulong value );
extern "C" FMLAPI_API bool fmlapiSetExpressionSize( ulong id, double value, bool one4all );

extern "C" FMLAPI_API bool fmlapiExportImage( ulong id, const ushort* pathName );
extern "C" FMLAPI_API const char* fmlapiExportImage2ByteArray( ulong id, const ushort* strFormat, ulong& len );
extern "C" FMLAPI_API void* fmlapiExport2PlatformImage( ulong id );

extern "C" FMLAPI_API FmlDocStatus fmlapiGetDocStatus( ulong id, long status );

extern "C" FMLAPI_API bool fmlapiSetOptionsValue( ulong id, ulong value );
extern "C" FMLAPI_API ulong fmlapiGetOptionsValue( ulong id );

#endif // __FORMULATOR_INCLUDE_FMLAPI_H__
