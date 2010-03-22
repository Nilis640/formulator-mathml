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

#if !defined( __FORMULATOR_NODEINFO_PROPERTY_H__ )
#define __FORMULATOR_NODEINFO_PROPERTY_H__

#include "selectnode.h"
#include "../../button/btn_attr.h"

class __HERMITECH_FORMULIB__ CNodeInfo_Property : public FBtnAttr
{
protected:
	CNode *m_markup_addon;

public:
	CNodeInfo_Property();
	virtual ~CNodeInfo_Property();
	const CNodeInfo_Property& operator = ( const CNodeInfo_Property& v );
	void setNodeInfo_Property( const CNodeInfo_Property& v );
	void setNodeInfo_Property( const FBtnAttr& v );

	CNode* getAddonNode( void ) const;
	void setAddonNode( CNode* node );
	virtual int isAble2Glue() const;

	friend QDataStream& operator << ( QDataStream& ar, const CNodeInfo_Property& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CNodeInfo_Property& lp );
};

/////////////////////////////////////////////////////////////////////

inline
CNode* CNodeInfo_Property::getAddonNode( void ) const
{
	return m_markup_addon;
}

inline
void CNodeInfo_Property::setAddonNode( CNode* node )
{
	m_markup_addon = node;
}

inline
int CNodeInfo_Property::isAble2Glue() const
{
	if( FBtnAttr::isAble2Glue() == 0 ) return 0;
	return m_markup_addon ? 0 : 1;
}

inline
void CNodeInfo_Property::setNodeInfo_Property( const CNodeInfo_Property& v )
{
	operator =( v );
}

inline
void CNodeInfo_Property::setNodeInfo_Property( const FBtnAttr& v )
{
	(FBtnAttr&) (*this) = (const FBtnAttr&) v;
}

/////////////////////////////////////////////////////////////////////

#endif
