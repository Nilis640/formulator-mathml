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

#include "../settings/options.h"
#include "../style/style.h"
#include "../button/toolbarset.h"
#include "../mathml/mml_uchar.h"
#include "../mathml/mml_char.h"
#include "op_iddefs.h"
#include "entity_map.h"

///////////////////////////////////////////////////////////////////////////////

void FMmlMap::InitDFA( void )
{
	QString dfapath = FmlCoreSettings::getDFAFile();
	if( dfapath.length() )
	{
		m_char2no = new FMmlCharMap();
		if( m_char2no && m_char2no->ReadFile( dfapath ) == -1 && ::shouldCreateCTSOnMissing() )
		{
			if( m_char2no )
			{
				delete m_char2no;
				m_char2no = 0;
			}
			m_char2no = new FMmlCharMap( __mml_char__name, MML_CHAR__NUMBER );
			m_char2no->WriteFile( dfapath );
		}
	}
	else
		m_char2no = ::shouldCreateCTSOnMissing() ? new FMmlCharMap( __mml_char__name, MML_CHAR__NUMBER ) : new FMmlCharMap();
}

void FMmlMap::InitUDFA( void )
{
	QString udfapath = FmlCoreSettings::getUDFAFile();
	if( udfapath.length() )
	{
		m_uchar2no = new FMmlCharMap();
		if( m_uchar2no && m_uchar2no->ReadFile( udfapath ) == -1 && ::shouldCreateCTSOnMissing() )
		{
			if( m_uchar2no )
			{
				delete m_uchar2no;
				m_uchar2no = 0;
			}
			m_uchar2no = new FMmlCharMap( __mml_unicode_char__name, MML_UNICODE_CHAR );
			m_uchar2no->WriteFile( udfapath );
		}
	}
	else
		m_uchar2no = ::shouldCreateCTSOnMissing() ? new FMmlCharMap( __mml_unicode_char__name, MML_UNICODE_CHAR ) : new FMmlCharMap();
}

void FMmlMap::InitValue2Name( void )
{
	QString path = FmlCoreSettings::getVal2ENameFile();
	if( path.length() )
	{
		m_u2ent = new FMmlEntityMap();
		if( ReadU2Ent( path ) == -1 )
		{
			for( long i = 0; i < MML_UNICODE_CHAR; i++ )
				(*m_u2ent)[ __mml_unicode_char__value[ i ] ] = __mml_unicode_char__name[ i ];
			WriteU2Ent( path );
		}
	}
	else if( m_u2ent )
	{
		for( long i = 0; i < MML_UNICODE_CHAR; i++ )
			(*m_u2ent)[ __mml_unicode_char__value[ i ] ] = __mml_unicode_char__name[ i ];
	}
}

///////////////////////////////////////////////////////////////////////////////

unsigned short mapString2UnicodeValue( QString& str, FMmlMap& mc2s, LCEntity& la_ent )
{
	if( str.isEmpty() || !mc2s.getKeywordUnicodeMAPKeyword() || !mc2s.getKeywordMAPKeyword() )
		return 0;
	else if( str.length() == 1 )
		return str.at(0).unicode();

	unsigned short rval = 0;
	int ret = la_ent.Analisys( str.midRef(0) );
	if( ret )
	{
		if( ret == LCEntity::Number )
			rval = la_ent.getVal();
		else
		{
			ret = mc2s.getKeywordUnicodeMAPKeyword()->Analisys( la_ent.getID() );
			if( ret >= 1 && ret <= MML_UNICODE_CHAR )
				rval = __mml_unicode_char__value[ ret - 1 ];
		}
	}

	mc2s.getKeywordMAPKeyword()->Reset();
	mc2s.getKeywordUnicodeMAPKeyword()->Reset();
	la_ent.Reset();

	return rval;
}

void mapString2UnicodeValue( QString& dest, const QString& src, FMmlMap& mc2s, LCEntity& la_ent )
{
	dest = "";
	for( long i = 0; i < src.length(); )
	{
		if( src.at(i) != QChar('&') )
		{
			dest.append( src.at(i++) );
			continue;
		}
		if( la_ent.Analisys( src.midRef(i) ) == LCEntity::Number )
		{
			QString entityName = mc2s.unicode2name( la_ent.getVal() );
			dest.append( entityName.length() ? entityName : src.mid(i, la_ent.GetRead()) );
			i += la_ent.GetRead();
		}
		else
			dest.append( src.at(i++) );
		la_ent.Reset();
	}
}

unsigned short mapString2UnicodeValue( QString& src_str )
{
	MArray<QString> dest_str(1);
	MArray<long> dest_style(1);
	QString tagname;
	long mode = __GLOBAL_USER_MODE1;
	unsigned short unicode_ret_val;
	::getMathMLChar2Style( dest_str, dest_style, src_str, tagname, mode, &unicode_ret_val );
	return unicode_ret_val;
}

void mapUnicodeValue2EntityName( QString& dest_str, QString& src_str )
{
	MArray<QString> arr1(1);
	MArray<long> arr2(1);
	long mode = __GLOBAL_USER_MODE2;
	::getMathMLChar2Style( arr1, arr2, dest_str, src_str, mode );
}

///////////////////////////////////////////////////////////////////////////////

void getMathMLChar2Style( std::vector<QString>& dest_str, std::vector<long>& dest_style, 
							QString& src_str, QString& tagname, long mode, unsigned short *unicode_ret_val )
{
	static FMmlMap mc2s;
	static LCEntity la_ent;

	if( mode == __GLOBAL_INIT_MODE ) // just to init static variable
		return;
	else if( mode == __GLOBAL_USER_MODE1 ) // return unicode value
	{
		if( unicode_ret_val )
			*unicode_ret_val = ::mapString2UnicodeValue( src_str, mc2s, la_ent );
		return;
	}
	else if( mode == __GLOBAL_USER_MODE2 ) // entity no -> entity name
	{
		::mapString2UnicodeValue( src_str, tagname, mc2s, la_ent );
		return;
	}
	else if( mode == __GLOBAL_USER_MODE3 ) // entity no -> entity name
	{
		int ret = la_ent.Analisys( src_str.midRef(0) );
		if( ret == LCEntity::Number )
		{
			QString entity_name = mc2s.unicode2name( la_ent.getVal() );
			if( entity_name.length() )
				src_str = entity_name;
		}
		la_ent.Reset();
		return;
	}

	int ret;
	long to_add_style;
	QString to_add_str;

	dest_str.clear();
	dest_style.clear();

	if( src_str == _T( "" ) )
	{
		to_add_style = FS_TextStyles::getMathMLTag2Style( src_str, tagname );
		dest_str.push_back( src_str );
		dest_style.push_back( to_add_style );
		return;
	}

	if( !mc2s.getKeywordUnicodeMAPKeyword() || !mc2s.getKeywordMAPKeyword() )
	{
		to_add_style = FS_TextStyles::getMathMLTag2Style( src_str, tagname );
		dest_str.push_back( src_str );
		dest_style.push_back( to_add_style );
		return;
	}

	long i = 0, idx, cb;
	long _toolbar_no = -1, _button_no = -1;
	for(;;)
	{
		to_add_str = "";
		to_add_style = FSTYLE_NONE;
		for( ; i < src_str.length(); )
		{
			if( src_str.at(i).isSpace() )
			{
				to_add_str.append(" ");
				while( i < src_str.length() && src_str.at(i).isSpace() )
					i++;
				continue;
			}

			if( src_str.at(i) == MML_CHAR_AMP ) // all keyword are starting with & ALWAYS!!!
			{
				if( to_add_str.length() ) break;

				ret = la_ent.Analisys( src_str.midRef(i) );
				cb = la_ent.GetRead();
				if( !ret )
				{
					to_add_str.append( src_str.at(i++) );
					mc2s.getKeywordMAPKeyword()->Reset();
					mc2s.getKeywordUnicodeMAPKeyword()->Reset();
					la_ent.Reset();
					continue;
				}
				else if( ret == LCEntity::Number )
				{
					QString entity_name = mc2s.unicode2name( la_ent.getVal() );
					int f = 1;
					if( entity_name.length() )
					{
						if( ::getCurrentToolBarSet()->getButtonCoordByMathMLValue( entity_name, _toolbar_no, _button_no ) != -1 )
						{
							FXmlBtn *ptrFXmlBtn = 0;
							ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
							if( ptrFXmlBtn && ptrFXmlBtn->getNodeType() == NODE_PLANETEXT )
							{
								to_add_str = ptrFXmlBtn->getID();
								to_add_style = FSTYLE_TEXT_BUTTON;
								f = 0;
							}
						}
					}

					if( f )
					{
						to_add_str = QString(QChar(la_ent.getVal()));
						to_add_style = FSTYLE_UNICODE;
					}
					// cb = la_ent.GetRead();
				}
				else
				{
					ret = mc2s.getKeywordUnicodeMAPKeyword()->Analisys( la_ent.getID() );

					if( ret && ::getCurrentToolBarSet()->getButtonCoordByMathMLValue( la_ent.getID(), _toolbar_no, _button_no ) != -1 )
					{
						FXmlBtn *ptrFXmlBtn = 0;
						ptrFXmlBtn = ::getCurrentToolBarSet()->getButtonCalc( _toolbar_no, _button_no );
						if( ptrFXmlBtn && ptrFXmlBtn->getNodeType() == NODE_PLANETEXT )
						{
							to_add_str = ptrFXmlBtn->getID();
							to_add_style = FSTYLE_TEXT_BUTTON;

							i += cb;
							mc2s.getKeywordMAPKeyword()->Reset();
							mc2s.getKeywordUnicodeMAPKeyword()->Reset();
							la_ent.Reset();
							if( cb ) break;
						}
					}
					else
					{
						int ret_alt = mc2s.getKeywordMAPKeyword()->Analisys( la_ent.getID() );
						if( ret_alt && ret_alt >= 1 && ret_alt <= MML_CHAR__NUMBER )
						{
							to_add_str = __mml_char__value[ ret_alt - 1 ];
							switch( __mml_char__font[ ret_alt - 1 ] )
							{
							case MML_STYLE_GREEK:
								to_add_style = FSTYLE_GREEK;
								break;          
							case MML_STYLE_OP:
								to_add_style = FS_TextStyles::getMathMLTag2Style( to_add_str, tagname );
								if( to_add_style == FSTYLE_DEFAULT )
									to_add_style = FSTYLE_OP;
								break;
							case MML_STYLE_TEXT:
								to_add_style = FSTYLE_TEXT;
								break;          
							default:
								break;
							}
							cb = mc2s.getKeywordMAPKeyword()->GetRead();
						}
						else if( ret && ret >= 1 && ret <= MML_UNICODE_CHAR )
						{
							to_add_str = QString(QChar(__mml_unicode_char__value[ ret - 1 ]));
							to_add_style = FSTYLE_UNICODE;
							cb = mc2s.getKeywordUnicodeMAPKeyword()->GetRead();
						}
						else
						{
							to_add_str.append( src_str.at(i++) );
							mc2s.getKeywordMAPKeyword()->Reset();
							mc2s.getKeywordUnicodeMAPKeyword()->Reset();
							la_ent.Reset();
							continue;
						}
					}
				}

				i += cb;
				mc2s.getKeywordMAPKeyword()->Reset();
				mc2s.getKeywordUnicodeMAPKeyword()->Reset();
				la_ent.Reset();
				if( cb ) break;
			}

			to_add_str.append( src_str.at(i++) );
		}

		if( !to_add_str.isEmpty() )
		{
			if( to_add_style == FSTYLE_NONE )
				to_add_style = FS_TextStyles::getMathMLTag2Style( to_add_str, tagname );
			idx = (long) dest_style.size();
			if( idx > 0 && dest_style[ idx - 1 ] == to_add_style && to_add_style != FSTYLE_TEXT_BUTTON && to_add_style != FSTYLE_UNICODE )
				dest_str[ idx - 1 ] += to_add_str;
			else
			{
				dest_str.push_back( to_add_str );
				dest_style.push_back( to_add_style );
			}
		}
		else
			return;
	}
}

///////////////////////////////////////////////////////////////////////////////
