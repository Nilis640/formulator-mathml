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

#ifndef __COMMON_VISUAL_PARAM_H__
#define __COMMON_VISUAL_PARAM_H__

#include "ctrl_template.h"

#define HKParameterState_Visible	0x00000001
#define HKParameterState_Enable		0x00000002

class HUTILS_EXPORT HKVisualParameter : public HKParameter
{
protected:
	int					multi;			// not for storing and comparing
	HKLongFlag			state;
	GUICtrlTemplate		gui_ctrl;

	long property_page_no;
	QString property_page_title;

public:
	HKVisualParameter( void );
	HKVisualParameter( const HKVisualParameter& value );
	~HKVisualParameter();

	virtual int isVisual( void ) const;

	int isMulti( void ) const;
	void setMulti( int _multi = 1 );

	long getPropertyPageNo( void ) const;
	void setPropertyPageNo( long ppn );
	QString& getPropertyPageTitle( void );
	void setPropertyPageTitle( QString& ppn );

	void setGUICtrl( GUICtrlTemplate& gc );
	GUICtrlTemplate& getGUICtrl( void );

	HKLongFlag getState( void ) const;
	bool isVisible( void );
	void setVisible( bool visible = true );

	bool isEnable( void );
	void setEnable( bool enable = true );

	HKVisualParameter& operator = ( const HKVisualParameter& param );

	int Read_XML( AST_TXML_Tag *root, QString& name, DRMessageManager& dmm, long dmm_page = 0 );
	int Read_XML_ControlTag( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page );
	int Read_XML_VisualFeatures( AST_TXML_Tag *root, AST_TXML_Attribute *page_title,  
				AST_TXML_Attribute *control, DRMessageManager& dmm, long dmm_page );

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );

	//friend CArchive& operator << ( CArchive& ar, HKVisualParameter& param );
	//friend CArchive& operator >> ( CArchive& ar, HKVisualParameter& param );
};

inline
HKVisualParameter::HKVisualParameter( void )
	: HKParameter(),
	gui_ctrl()
{
	multi = 0;
	property_page_no = -1;
	property_page_title = "";
	setVisible( true );
	setEnable( true );
}

inline
HKVisualParameter::~HKVisualParameter()
{
}

inline
HKVisualParameter::HKVisualParameter( const HKVisualParameter& value )
	: HKParameter( value ), 
	gui_ctrl( value.gui_ctrl )
{
	multi = value.multi;
	property_page_no = value.property_page_no;
	property_page_title = value.property_page_title;
	state = value.state;
}

inline
HKVisualParameter& HKVisualParameter::operator = ( const HKVisualParameter& param )
{
	HKParameter::operator = ( param );
	property_page_no = param.property_page_no;
	property_page_title = param.property_page_title;
	gui_ctrl = param.gui_ctrl;
	state = param.state;
	return *this;
}

inline
int HKVisualParameter::isVisual( void ) const
{
	return 1;
}

inline
int HKVisualParameter::isMulti( void ) const
{
	return multi;
}

inline
void HKVisualParameter::setMulti( int _multi )
{
	multi = _multi;
}

inline
long HKVisualParameter::getPropertyPageNo( void ) const
{
	return property_page_no;
}

inline
void HKVisualParameter::setPropertyPageNo( long ppn )
{
	property_page_no = ppn;
}

inline
QString& HKVisualParameter::getPropertyPageTitle( void )
{
	return property_page_title;
}

inline
void HKVisualParameter::setPropertyPageTitle( QString& ppn )
{
	property_page_title = ppn;
}

inline
void HKVisualParameter::setGUICtrl( GUICtrlTemplate& gc )
{
	gui_ctrl = gc;
}

inline
GUICtrlTemplate& HKVisualParameter::getGUICtrl( void )
{
	return gui_ctrl;
}

inline
HKLongFlag HKVisualParameter::getState( void ) const
{
	return state;
}

inline
bool HKVisualParameter::isVisible( void )
{
	return state.isSetFlag( HKParameterState_Visible ) != 0;
}

inline
void HKVisualParameter::setVisible( bool visible )
{ 
	if( visible )
		state.setFlag( HKParameterState_Visible );
	else
		state.clearFlag( HKParameterState_Visible );
}

inline
bool HKVisualParameter::isEnable( void )
{
	return state.isSetFlag( HKParameterState_Enable ) != 0;
}

inline
void HKVisualParameter::setEnable( bool enable )
{
	if( enable )
		state.setFlag( HKParameterState_Enable );
	else
		state.clearFlag( HKParameterState_Enable );
}

/*
inline
void HKVisualParameter::store( CArchive& ar )
{
	HKParameter::store( ar );
	long st = state.getValue();
	ar << st;
	ar << gui_ctrl;
	ar << property_page_no;
	ar << property_page_title;
}

inline
void HKVisualParameter::load( CArchive& ar )
{
	HKParameter::load( ar );
	long st;
	ar >> st;
	state.setValue( st );
	ar >> gui_ctrl;
	ar >> property_page_no;
	ar >> property_page_title;
}

inline
CArchive& operator << ( CArchive& ar, HKVisualParameter& param )
{
	param.store( ar );
	return ar;
}

inline
CArchive& operator >> ( CArchive& ar, HKVisualParameter& param )
{
	param.load( ar );
	return ar;
}
*/

///////////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT HKVisualParameterList
{
protected:
	HKMap<QString, HKVisualParameter> m_visual_list;

public:
	HKVisualParameterList( void );
	HKVisualParameterList( const HKVisualParameterList& p );
	~HKVisualParameterList();

	HKVisualParameterList& operator = ( const HKVisualParameterList& p );

	unsigned long getParameterNumber();
	void Flush( void );
	                         
	void setParameter( const QString& key, HKVisualParameter& param );
	void delParameter( const QString& key );

	HKVisualParameter* isParameter( const QString& key );
	HKVisualParameter& getParameter( const QString& key );

	bool tofirst();
	bool tonext();
	void get( QString& key, HKVisualParameter& param );

	int Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page = 0 );

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );
};

inline
HKVisualParameterList& HKVisualParameterList::operator = ( const HKVisualParameterList& p ) 
{
	m_visual_list = p.m_visual_list;
	return *this;
}

inline
unsigned long HKVisualParameterList::getParameterNumber()
{
	return (unsigned long) m_visual_list.size();
}

inline
void HKVisualParameterList::Flush( void )
{
	m_visual_list.clear();
}

inline
void HKVisualParameterList::setParameter( const QString& key, HKVisualParameter& param )
{
	m_visual_list[ key ] = param;
}

inline
void HKVisualParameterList::delParameter( const QString& key )
{
	m_visual_list.erase( key );
}

inline
HKVisualParameter* HKVisualParameterList::isParameter( const QString& key )
{
	QString keyval( key );
	return m_visual_list.is_find( keyval );
}

inline
HKVisualParameter& HKVisualParameterList::getParameter( const QString& key )
{
	return m_visual_list[ key ];
}

inline
bool HKVisualParameterList::tofirst()
{
	return m_visual_list.tofirst();
}

inline
bool HKVisualParameterList::tonext()
{
	return m_visual_list.tonext();
}

inline
void HKVisualParameterList::get( QString& key, HKVisualParameter& param )
{
	m_visual_list.get( key, param );
}

/*
inline
void HKVisualParameterList::store( CArchive& ar )
{
	ar << m_visual_list;
}

inline
void HKVisualParameterList::load( CArchive& ar )
{
	ar >> m_visual_list;
}
*/

///////////////////////////////////////////////////////////////////////////

#define HKP_VISUAL_PARAMETER_PAGE__REGULAR		0
#define HKP_VISUAL_PARAMETER_PAGE__ALL			1

struct HUTILS_EXPORT HKVisualParameterPage
{
	QString title;
	long type;	//HKP_VISUAL_PARAMETER_PAGE__REGULAR or ...__ALL

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );

	//friend CArchive& operator << ( CArchive& ar, HKVisualParameterPage& param );
	//friend CArchive& operator >> ( CArchive& ar, HKVisualParameterPage& param );
};

/*
inline 
void HKVisualParameterPage::store( CArchive& ar )
{
	ar << title;
	ar << type;
}

inline 
void HKVisualParameterPage::load( CArchive& ar )
{
	ar >> title;
	ar >> type;
}

inline
CArchive& operator << ( CArchive& ar, HKVisualParameterPage& page )
{
	page.store( ar );
	return ar;
}

inline
CArchive& operator >> ( CArchive& ar, HKVisualParameterPage& page )
{
	page.load( ar );
	return ar;
}
*/

///////////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT HKVisualParameterBar: public HKVisualParameterList
{
protected:
	std::vector<HKVisualParameterPage> pages;
	QString name, title, fast_navigate_control;

public:
	HKVisualParameterBar( void );
	HKVisualParameterBar( HKVisualParameterBar& p );
	~HKVisualParameterBar();

	HKVisualParameterBar& operator = ( HKVisualParameterBar& p );

	void Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page = 0 );
	void AdjustPropertyPages( void );

	//int LoadTemplate( const QString& sect, const QString& key, DRMessageManager& dmm = DRMessageManager() );
        int LoadTemplateFile( const QString& path, DRMessageManager& dmm/* = DRMessageManager()*/ );

	long getPropertyPageNumber();
	HKVisualParameterPage& getPropertyPage( long i );
	int isPropertyPage_All( long i );

	QString& getName();
	QString& getTitle();
	QString& getFastNavigateControl();

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );

	//friend CArchive& operator << ( CArchive& ar, HKVisualParameterBar& param );
	//friend CArchive& operator >> ( CArchive& ar, HKVisualParameterBar& param );
};

inline
HKVisualParameterBar& HKVisualParameterBar::operator = ( HKVisualParameterBar& p ) 
{
	HKVisualParameterList::operator = ( p );
	pages = p.pages;
	name = p.name;
	title = p.title;
	fast_navigate_control = p.fast_navigate_control;

	return *this;
}

inline
long HKVisualParameterBar::getPropertyPageNumber()
{
	return (long) pages.size();
}

inline
HKVisualParameterPage& HKVisualParameterBar::getPropertyPage( long i ) 
{
	return pages[ i ];
}

inline
int HKVisualParameterBar::isPropertyPage_All( long i ) 
{
	return pages[ i ].type == HKP_VISUAL_PARAMETER_PAGE__ALL;
}

inline
QString& HKVisualParameterBar::getName()
{
	return name;
}

inline
QString& HKVisualParameterBar::getTitle()
{
	return title;
}

inline
QString& HKVisualParameterBar::getFastNavigateControl()
{
	return fast_navigate_control;
}

/*
inline
void HKVisualParameterBar::store( CArchive& ar )
{
	HKVisualParameterList::store( ar );

	ar << pages;
	ar << name;
	ar << title;
	ar << fast_navigate_control;
}

inline
void HKVisualParameterBar::load( CArchive& ar )
{
	HKVisualParameterList::load( ar );

	ar >> pages;
	ar >> name;
	ar >> title;
	ar >> fast_navigate_control;
}

inline
CArchive& operator << ( CArchive& ar, HKVisualParameterBar& param )
{
	param.store( ar );
	return ar;
}

inline
CArchive& operator >> ( CArchive& ar, HKVisualParameterBar& param )
{
	param.load( ar );
	return ar;
}
*/

#endif //__COMMON_VISUAL_PARAM_H__
