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

#include "HUtils/iustring.h"
#include "la_xmlvar.h"

const static QString kw_xml				= _T("xml");
const static QString kw_doctype			= _T("doctype");

long LXMLVarLeapes[ LXMLVarStates * LXMLVarAlphabet ] =
{
//  l_  d -. :
	2, 0, 0,
	2, 2, 3,
	4, 0, 0,
	4, 4, 0
};

long LXMLVarsEndStates[ LXMLVarStates ] =
{
	0, 1, 1, 1
};

long LXMLVar::What( const QChar S )
//            ~~~~
{
	for(;;)
	{
		if( S.isLetter() || (S == _T('_')) )
			return 1;
		if( S.isDigit() || ( S == _T('-') ) || ( S == _T('.') ) )
			return 2;
		if( S == _T(':') )
			return 3;
		return 0;
	}
}

void LXMLVar::Deeds( void )
//            ~~~~~
{
	switch( CurrentState )
	{
	case 1 :
		m_identifier = _T("");
		m_namespace = _T("");
		break;
	case 2 :
		m_identifier += character;
		break;
	case 3 :
		m_namespace = m_identifier;
		m_identifier = _T("");
		break;
	case 4 :
		m_identifier += character;
		break;
	}
}

LXMLVar::LXMLVar( void )
//    ~~~~
: LAuto( LXMLVarStates, LXMLVarAlphabet, LXMLVarLeapes, LXMLVarsEndStates )
{
	m_identifier = _T("");
	m_namespace = _T("");
}

int LXMLVar::Analisys( const QStringRef& aText )
//            ~~~~~~~~
{
	int ret = LAuto::Analisys( aText );
	if( ret )
	{
		if( m_identifier.length() == 0 )
		{
			m_identifier = m_namespace;
			m_namespace = _T("");
		}
		if( !m_identifier.compare( kw_xml, Qt::CaseInsensitive ) )
			return XML;
		else if( !m_identifier.compare( kw_doctype, Qt::CaseInsensitive ) )
			return DOCTYPE;
	}
	return ret;
}
