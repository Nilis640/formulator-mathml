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

#include "fastparam.h"
//#include "HUtils/idfas.h"

void HKFastParameterList::setParameter( const QString& key, HKParameter& param )
{
	if( m_list_name.find( key ) == m_list_name.end() )
	{	// add a new parameter
		m_list_value.push_back( param );
		m_list_name[ key ] = (long) (m_list_value.size()) - 1;
	}
	else
		m_list_value[ m_list_name[ key ] ] = param; // assign a new parameter value
}

int HKFastParameterList::Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page )
{
	long ret = 0;
	AST_TXML_Tag* tag = root->getFirstTag( FBL_TAG_property_list );
	if( !tag )
		return 0;

	std::vector<AST_TXML_Tag*> arrtags;
	tag->getAllTags( FBL_TAG_property, arrtags, 1 );

	long i;
	for( i = 0; i < (long) arrtags.size(); i++ )
	{
		if( arrtags[ i ] )
		{
			HKParameter item;
			QString name;
			if( !item.Read_XML( arrtags[ i ], name, dmm, dmm_page ) && name != _T("") )
			{
				if( getParameter( name ) )
				{
					QString err3 = _T("property already exists");
					dmm.AddLine( dmm_page, err3, arrtags[ i ]->getLine() );
				}
				else
					setParameter( name, item );
			}
		}
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////
