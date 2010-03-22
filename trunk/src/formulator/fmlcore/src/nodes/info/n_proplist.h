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

#if !defined( __FORMULATOR_NODE_MATHML_N_PROPLIST_H__ )
#define __FORMULATOR_NODE_MATHML_N_PROPLIST_H__

#include <vector>

#include "HUtils/ixml.h"
#include "HUtils/ihkstring.h"
#include "HUtils/ihkmap.h"

#include "nodeinfo_property.h"

///////////////////////////////////////////////////////////////////////////

#define CNode_TextProperty_NONE				-1
#define CNode_TextProperty_BOOL				0
#define CNode_TextProperty_TEXT				1
#define CNode_TextProperty_TEXT_NUM			2
#define CNode_TextProperty_TEXT_UNIT		3
#define CNode_TextProperty_TEXT_NUM_UNIT	4
#define CNode_TextProperty_LIST				5
#define CNode_TextProperty_COMBO			6
#define CNode_TextProperty_COMBO_UNIT		7
#define CNode_TextProperty_COMBO_COLOR		8
#define CNode_TextProperty_COMBO_FONT		9

#define CNode_TextPropertyEnum_None				-1
#define CNode_TextPropertyEnum_Mathvariant		100
#define CNode_TextPropertyEnum_Mathsize			101
#define CNode_TextPropertyEnum_Mathcolor		102
#define CNode_TextPropertyEnum_Mathbackcolor	CNode_TextPropertyEnum_Mathcolor
#define CNode_TextPropertyEnum_Mathwidth		103
#define CNode_TextPropertyEnum_Thickness		104
#define CNode_TextPropertyEnum_Horalign			105
#define CNode_TextPropertyEnum_MOForm			106
#define CNode_TextPropertyEnum_CNType           107
#define CNode_TextPropertyEnum_CIType           108
#define CNode_TextPropertyEnum_SetType          109
#define CNode_TextPropertyEnum_Tendsto          110
#define CNode_TextPropertyEnum_IntervalType     111
#define CNode_TextPropertyEnum_ListOrdering     112
#define CNode_TextPropertyEnum_DeclareOccurence 113
#define CNode_TextPropertyEnum_DeclareScope     114
#define CNode_TextPropertyEnum_Encoding         115
#define CNode_TextPropertyEnum_Maxsize			116
#define CNode_TextPropertyEnum_ContentOp		117
#define CNode_TextPropertyEnum_Notation			118
#define CNode_TextPropertyEnum_Align			119
#define CNode_TextPropertyEnum_Rowalign			120
#define CNode_TextPropertyEnum_Columnalign		121
#define CNode_TextPropertyEnum_Columnlines		122
#define CNode_TextPropertyEnum_Linebreak		123
#define CNode_TextPropertyEnum_Math				124
#define CNode_TextPropertyEnum_MathOverflow		125

///////////////////////////////////////////////////////////////////////////

struct __HERMITECH_FORMULIB__ CNode_TextPropertyData
{
public:
	char	m_typeCtrl, m_typeCtrlInstance;
	QString	m_key, m_name, m_value, m_hint;

public:
	CNode_TextPropertyData()
	{
		m_typeCtrl = CNode_TextProperty_NONE;
		m_typeCtrlInstance = -1;
		m_key = m_name = m_value = m_hint = _T( "" );
	}
	CNode_TextPropertyData( const CNode_TextPropertyData& a )
	{
		m_typeCtrl = a.m_typeCtrl;
		m_typeCtrlInstance = a.m_typeCtrlInstance;
		m_key = a.m_key;
		m_name = a.m_name;
		m_value = a.m_value;
		m_hint = a.m_hint;
	}
	CNode_TextPropertyData( QString key, QString name, char type, char instance )
	{
		m_typeCtrl = type;
		m_typeCtrlInstance = instance;
		m_key = key;
		m_name = name;
		m_value = _T( "" );
	}
	CNode_TextPropertyData( QString key, QString name, QString hint, char type, char instance = CNode_TextPropertyEnum_None )
	{
		m_typeCtrl = type;
		m_typeCtrlInstance = instance;
		m_key = key;
		m_name = name;
		m_hint = hint;
		m_value = _T( "" );
	}
	QString getKey()
	{
		return m_key;
	}
	char getCtrlInstance()
	{
		return m_typeCtrlInstance;
	}
};

///////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CNode_TextPropertyList
{
private:
	QVector<CNode_TextPropertyData>	m_list_value;
	HKMap<QString, long>			m_list_name;

	AST_TXML_Compound*	m_nodeXMLTree;
	AST_TXML_Tag*		m_rootAttrFocus;

public:
	CNode_TextPropertyList( void );
	CNode_TextPropertyList( CNode_TextPropertyList& p );
	~CNode_TextPropertyList( void );
	CNode_TextPropertyList& operator = ( CNode_TextPropertyList& p );

	unsigned long getParameterNumber();
	void reset( void );
	void setParameter( CNode_TextPropertyData& param );
	long getParameterIndex( QString key );
	CNode_TextPropertyData* getParameter( unsigned long index );
	CNode_TextPropertyData* getParameter( QString key );

	void setXMLPtrs( AST_TXML_Compound*& bms, AST_TXML_Tag*& root );
	void resetXMLPtrs();
	AST_TXML_Compound* getXMLTree2Read();
	AST_TXML_Tag* getXMLNode2ChangeAttr();
	void OnOKPropsEditing( std::vector<char>& supports );
};

///////////////////////////////////////////////////////////////////////////

inline
CNode_TextPropertyList::~CNode_TextPropertyList( void )
{
	resetXMLPtrs();
}

inline
void CNode_TextPropertyList::resetXMLPtrs()
{
	if( m_nodeXMLTree != 0 )
	{
		delete m_nodeXMLTree;
		m_nodeXMLTree = 0;
	}
	m_rootAttrFocus = 0;
}

inline
void CNode_TextPropertyList::setXMLPtrs( AST_TXML_Compound*& bms, AST_TXML_Tag*& root )
{
	resetXMLPtrs();
	m_nodeXMLTree = bms;
	m_rootAttrFocus = root;
}

inline
AST_TXML_Compound* CNode_TextPropertyList::getXMLTree2Read()
{
	return m_nodeXMLTree;
}

inline
AST_TXML_Tag* CNode_TextPropertyList::getXMLNode2ChangeAttr()
{
	return m_rootAttrFocus;
}

///////////////////////////////////////////////////////////////////////////

inline
unsigned long CNode_TextPropertyList::getParameterNumber()
{
	return (unsigned long) m_list_value.size();
}

///////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CMMLNodeAttrList : public CNode_TextPropertyList
{
protected:
	HKMapString2Long 				m_tagname2opid;
	std::multimap<QString, long>	m_tag2attrno;
public:
	CMMLNodeAttrList();
	void setMapTag2Attr();
	long getContentMMLOperatorProperty( QString name );
	void getTag2AttrList( QString tagname, std::vector<long>& attrIdx );
	int isSupportedTag2Attr( QString tagName, QString attrName );
	const std::multimap<QString, long>& getTag2AttrMap() { return m_tag2attrno; }
};

///////////////////////////////////////////////////////////////////////////

#define __Hint4UserAttr							_T("user-defined attribute")
#define TXTPROP_IKEY__OP						_T("node:operator")
#define TXTPROP_IKEY__BVAR						_T("node:bvar")

///////////////////////////////////////////////////////////////////////////

#define CNode_TextProperty_ContentOpArrLen 83
__HERMITECH_FORMULIB__ extern QString CNode_TextProperty_ContentOpArr[ CNode_TextProperty_ContentOpArrLen ];
__HERMITECH_FORMULIB__ void getNodeTextPropertyCtrlInstanceData( char typeCtrlInstance, QString*& head, long& number );
__HERMITECH_FORMULIB__ CMMLNodeAttrList& getFullMathMLAttrList();
__HERMITECH_FORMULIB__ void parseNodeAttr2PropList( CNode_TextPropertyList& props, std::vector<char>& supports, CNode *frmNode );
__HERMITECH_FORMULIB__ void parseNodeAttr2PropList( CNode_TextPropertyList& props, std::vector<char>& supports, QString mathmlRes );
__HERMITECH_FORMULIB__ QString convertAttrName2Key( QString tagName, QString attrName );

///////////////////////////////////////////////////////////////////////////

#endif
