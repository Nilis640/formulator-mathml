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

#ifndef __COMMON_GUICTRL_TOOLBAR_H__
#define __COMMON_GUICTRL_TOOLBAR_H__

#include "HUtils/ihkstring.h"
#include "HUtils/ihkflag.h"
#include "HUtils/ixml.h"

#include "../hutils_global.h"
#include "acceler.h"

///////////////////////////////////////////////////////////////////////////

class HKToolBarButton
{
protected:
	//
	// storable variables
	// 

	qint32		m_parent_toolbar;
	qint32		m_button_no;
	QString		m_button_id;
	QString		m_tooltip;
	HKAccelVector	m_accelvector;

	//
	// runtime variables
	// 

	bool			m_fRuntimeEnable;	// 
	bool			m_fRuntimeVisible;	// 
	qint32			m_nRuntimeID;		// 

protected:
	HKToolBarButton();
	virtual ~HKToolBarButton();

public:
	const HKToolBarButton& operator = ( const HKToolBarButton& btn );

	int operator == ( const HKToolBarButton& /*a*/ ) { return 0; };

	//
	// storable variables
	// 

	qint32 getParentToolBar( void ) const;
	qint32 getButtonNo( void ) const;
	const QString& getID( void ) const;
	const QString& getToolTip( void ) const;

	void setParentToolBar( qint32 parent_toolbar );
	void setButtonNo( qint32 button_no );
	void setID( const QString& id );
	void setToolTip( const QString& tooltip );

	void addAccel( const HKAccel& hk );
	long getAccelCount( void ) const;
	const HKAccel& getAccel( long i ) const;

	int isID( const QString& id ) const;

	//
	// runtime variables
	// 

	bool getRuntimeVisible( void ) const;
	bool getRuntimeEnable( void ) const;
	qint32 getRuntimeID( void ) const;

	void setRuntimeVisible( bool fRuntimeVisible );
	void setRuntimeEnable( bool fRuntimeEnable );
	void setRuntimeID( qint32 nRuntimeID );

	friend QDataStream& operator << ( QDataStream& ar, const HKToolBarButton& lp );
	friend QDataStream& operator >> ( QDataStream& ar, HKToolBarButton& lp );
};

inline 
QDataStream& operator << ( QDataStream& ar, const HKToolBarButton& lp )
{
	return ar << lp.m_parent_toolbar << lp.m_button_no << lp.m_button_id << lp.m_tooltip << lp.m_accelvector;
}

inline 
QDataStream& operator >> ( QDataStream& ar, HKToolBarButton& lp )
{
	return ar >> lp.m_parent_toolbar >> lp.m_button_no >> lp.m_button_id >> lp.m_tooltip >> lp.m_accelvector;
}

/////////////////////////////////////////////////////////////////////

inline 
HKToolBarButton::HKToolBarButton()
	: m_parent_toolbar( -1 )
	, m_button_no( -1 )
	, m_button_id()
	, m_tooltip()
	, m_accelvector( 0 )
	//
	// runtime variables
	// 
	, m_fRuntimeEnable( true )
	, m_fRuntimeVisible( true )
	, m_nRuntimeID( -1 )
{
}

inline 
HKToolBarButton::~HKToolBarButton()
{
}

/////////////////////////////////////////////////////////////////////

inline 
const HKToolBarButton& HKToolBarButton::operator = ( const HKToolBarButton& btn )
{
	m_parent_toolbar	= btn.m_parent_toolbar;
	m_button_no			= btn.m_button_no;
	m_button_id			= btn.m_button_id;
	m_tooltip			= btn.m_tooltip;

	m_accelvector.resize( btn.m_accelvector.size() );
	size_t i, count = m_accelvector.size();
	for ( i = 0; i < count; i++ )
		m_accelvector[ i ] = btn.m_accelvector[ i ];

	return *this;
}

/////////////////////////////////////////////////////////////////////
//
// storable variables
// 

inline
qint32 HKToolBarButton::getParentToolBar( void ) const
	{ return m_parent_toolbar; }

inline
qint32 HKToolBarButton::getButtonNo( void ) const
	{ return m_button_no; }

inline 
const QString& HKToolBarButton::getID( void ) const
	{ return m_button_id; }

inline 
const QString& HKToolBarButton::getToolTip( void ) const
	{ return m_tooltip; }

inline
void HKToolBarButton::setParentToolBar( qint32 parent_toolbar )
	{ m_parent_toolbar = parent_toolbar; }

inline
void HKToolBarButton::setButtonNo( qint32 button_no )
	{ m_button_no = button_no; }

inline 
void HKToolBarButton::setID( const QString& id )
	{ m_button_id = id; }

inline 
void HKToolBarButton::setToolTip( const QString& tooltip )
	{ m_tooltip = tooltip; }

inline 
void HKToolBarButton::addAccel( const HKAccel& hk )
	{ m_accelvector.push_back( hk ); }

inline 
long HKToolBarButton::getAccelCount( void ) const
	{ return (long) m_accelvector.size(); }

inline 
const HKAccel& HKToolBarButton::getAccel( long i ) const 
	{ return m_accelvector[ i ]; }

inline
int HKToolBarButton::isID( const QString& id ) const
	{ return getID() == id; }

/////////////////////////////////////////////////////////////////////
//
// runtime variables
// 

inline
bool HKToolBarButton::getRuntimeVisible( void ) const
	{ return m_fRuntimeVisible; }

inline
bool HKToolBarButton::getRuntimeEnable( void ) const
	{ return m_fRuntimeEnable; }

inline
qint32 HKToolBarButton::getRuntimeID( void ) const
	{ return m_nRuntimeID; }

inline
void HKToolBarButton::setRuntimeVisible( bool fRuntimeVisible )
	{ m_fRuntimeVisible = fRuntimeVisible; }

inline
void HKToolBarButton::setRuntimeEnable( bool fRuntimeEnable )
	{ m_fRuntimeEnable = fRuntimeEnable; }

inline
void HKToolBarButton::setRuntimeID( qint32 nRuntimeID )
	{ m_nRuntimeID = nRuntimeID; }

///////////////////////////////////////////////////////////////////////////
//
// class HKToolBar
//
///////////////////////////////////////////////////////////////////////////

enum HKToolBarImages
{
	HKToolBarImage_Normal = 0, 
	HKToolBarImage_Hot, 
	HKToolBarImage_Disabled, 

	HKToolBarImage_SizeOfEnum
};

class HKToolBar
{
protected:

	//
	// storable variables
	// 

	qint32	m_toolbar_no;		// toolbar number
	qint32	m_visible;			// visible state
	qint32	m_rows;				// -1 for N/A; 0 for direction determination; >0 for actual hor/ver size
	qint32	m_columns;			// -1 for N/A; 0 for direction determination; >0 for actual hor/ver size
	QString	m_id;				// toolbar id string 
	QString	m_group;			// group name
	QString	m_name;				// toolbar name
	QString	m_caption;			// toolbar caption
	QString	m_image[ HKToolBarImage_SizeOfEnum ];			// buttons images path
	QString	m_image_title[ HKToolBarImage_SizeOfEnum ];		// title images path

	//
	// runtime variables
	// 

	bool		m_fRuntimeEnable;	// 
	bool		m_fRuntimeVisible;	// 
	qint32		m_nRuntimeID;		// 

protected:
	HKToolBar();
	virtual ~HKToolBar();

public:
	virtual long getButtonCount() const { return 0; };
        virtual const HKToolBarButton* getButton( long /*index*/ ) { return NULL; };

	//
	// storable variables
	// 

	qint32 getToolBarNo( void ) const;
	void setToolBarNo( qint32 no );

	qint32 getVisible( void ) const;
	void setVisible( qint32 visible = -1 );

	qint32 getRows( void ) const;
	void setRows( qint32 value );
	qint32 getColumns( void ) const;
	void setColumns( qint32 value );

	const QString& getID() const;
	const QString& getGroup() const;
	const QString& getName() const;
	const QString& getCaption() const;
	const QString& getImage() const;
	const QString& getImage_h() const;
	const QString& getImage_d() const;
	const QString& getImageTitle() const;
	const QString& getImageTitle_h() const;
	const QString& getImageTitle_d() const;

	void setID( const QString& name );
	void setGroup( const QString& name );
	void setName( const QString& name );
	void setCaption( const QString& caption );
	void setImage( const QString& image );
	void setImage_h( const QString& image_h );
	void setImage_d( const QString& image_d );
	void setImageTitle( const QString& image_title );
	void setImageTitle_h( const QString& image_title );
	void setImageTitle_d( const QString& image_title );

	//
	const QString& getImage( int type ) const;
	void setImage( int type, const QString& image );

	const QString& getImageTitle( int type ) const;
	void setImageTitle( int type, const QString& image );

	//
	// runtime variables
	// 

	bool getRuntimeVisible( void ) const;
	bool getRuntimeEnable( void ) const;
	qint32 getRuntimeID( void ) const;

	void setRuntimeVisible( bool fRuntimeVisible );
	void setRuntimeEnable( bool fRuntimeEnable );
	void setRuntimeID( qint32 nRuntimeID );

	friend QDataStream& operator << ( QDataStream& ar, const HKToolBar& lp );
	friend QDataStream& operator >> ( QDataStream& ar, HKToolBar& lp );
};

inline 
QDataStream& operator << ( QDataStream& ar, const HKToolBar& lp )
{
	return ar << lp.m_toolbar_no << lp.m_visible << lp.m_rows << lp.m_columns << lp.m_id << 
				 lp.m_group << lp.m_name << lp.m_caption << 
				 lp.m_image[ HKToolBarImage_Normal ] << lp.m_image[ HKToolBarImage_Hot ] << lp.m_image[ HKToolBarImage_Disabled ] <<
				 lp.m_image_title[ HKToolBarImage_Normal ] << lp.m_image_title[ HKToolBarImage_Hot ] << lp.m_image_title[ HKToolBarImage_Disabled ];
}

inline 
QDataStream& operator >> ( QDataStream& ar, HKToolBar& lp )
{
	return ar >> lp.m_toolbar_no >> lp.m_visible >> lp.m_rows >> lp.m_columns >> lp.m_id >> 
				 lp.m_group >> lp.m_name >> lp.m_caption >> 
				 lp.m_image[ HKToolBarImage_Normal ] >> lp.m_image[ HKToolBarImage_Hot ] >> lp.m_image[ HKToolBarImage_Disabled ] >>
				 lp.m_image_title[ HKToolBarImage_Normal ] >> lp.m_image_title[ HKToolBarImage_Hot ] >> lp.m_image_title[ HKToolBarImage_Disabled ];
}

/////////////////////////////////////////////////////////////////////

inline 
HKToolBar::HKToolBar()
	//
	// storable variables
	// 
	: m_toolbar_no( -1 )
	, m_visible( -1 )
	, m_rows( -1 )
	, m_columns( -1 )
	, m_id( _T("") )
	, m_name( _T("") )
	, m_caption( _T("") )
	//
	// runtime variables
	// 
	, m_fRuntimeEnable( true )
	, m_fRuntimeVisible( true )
	, m_nRuntimeID( (quint32) -1 )
{
	setImage( HKToolBarImage_Normal, QString( _T("") ) );
	setImage( HKToolBarImage_Hot, QString( _T("") ) );
	setImage( HKToolBarImage_Disabled, QString( _T("") ) );

	setImageTitle( HKToolBarImage_Normal, QString( _T("") ) );
	setImageTitle( HKToolBarImage_Hot, QString( _T("") ) );
	setImageTitle( HKToolBarImage_Disabled, QString( _T("") ) );
}

inline 
HKToolBar::~HKToolBar()
{
}

///////////////////////////////////////////////////////////////////////////
//
// staorable variables
// 

inline 
qint32 HKToolBar::getRows( void ) const
	{ return m_rows; }

inline 
void HKToolBar::setRows( qint32 value )
	{ m_rows = value; }

inline 
qint32 HKToolBar::getColumns( void ) const
	{ return m_columns; }

inline 
void HKToolBar::setColumns( qint32 value )
	{ m_columns = value; }

inline 
qint32 HKToolBar::getToolBarNo( void ) const
	{ return m_toolbar_no; }

inline 
void HKToolBar::setToolBarNo( qint32 no )
	{ m_toolbar_no = no; }

inline 
qint32 HKToolBar::getVisible( void ) const
	{ return m_visible; }

inline 
void HKToolBar::setVisible( qint32 visible )
	{ m_visible = visible; }

inline 
const QString& HKToolBar::getID() const
	{ return m_id; }

inline 
const QString& HKToolBar::getGroup() const 
	{ return m_group; }

inline 
const QString& HKToolBar::getName() const 
	{ return m_name; }

inline 
const QString& HKToolBar::getCaption() const 
	{ return m_caption; }

inline 
const QString& HKToolBar::getImage() const 
	{ return getImage( HKToolBarImage_Normal ); }

inline 
const QString& HKToolBar::getImage_h() const 
	{ return getImage( HKToolBarImage_Hot ); }

inline 
const QString& HKToolBar::getImage_d() const 
	{ return getImage( HKToolBarImage_Disabled ); }

inline 
const QString& HKToolBar::getImageTitle() const 
	{ return getImageTitle( HKToolBarImage_Normal ); }

inline 
const QString& HKToolBar::getImageTitle_h() const 
	{ return getImageTitle( HKToolBarImage_Hot ); }

inline 
const QString& HKToolBar::getImageTitle_d() const 
	{ return getImageTitle( HKToolBarImage_Disabled ); }

inline 
void HKToolBar::setID( const QString& id )
	{ m_id = id; }

inline 
void HKToolBar::setGroup( const QString& group )
	{ m_group = group; }

inline 
void HKToolBar::setName( const QString& name )
	{ m_name = name; }

inline 
void HKToolBar::setCaption( const QString& caption )
	{ m_caption = caption; }

inline 
void HKToolBar::setImage( const QString& image )
	{ setImage( HKToolBarImage_Normal, image ); }

inline 
void HKToolBar::setImage_h( const QString& image )
	{ setImage( HKToolBarImage_Hot, image ); }

inline 
void HKToolBar::setImage_d( const QString& image )
	{ setImage( HKToolBarImage_Disabled, image ); }

inline 
void HKToolBar::setImageTitle( const QString& image )
	{ setImageTitle( HKToolBarImage_Normal, image ); }

inline 
void HKToolBar::setImageTitle_h( const QString& image )
	{ setImageTitle( HKToolBarImage_Hot, image ); }

inline 
void HKToolBar::setImageTitle_d( const QString& image )
	{ setImageTitle( HKToolBarImage_Disabled, image ); }

inline 
const QString& HKToolBar::getImage( int type ) const
	{ return m_image[ type ]; }

inline 
void HKToolBar::setImage( int type, const QString& image )
	{ m_image[ type ] = image; }

inline 
const QString& HKToolBar::getImageTitle( int type ) const
	{ return m_image_title[ type ]; }

inline 
void HKToolBar::setImageTitle( int type, const QString& image )
	{ m_image_title[ type ] = image; }

///////////////////////////////////////////////////////////////////////////
//
// runtime variables
// 

inline
bool HKToolBar::getRuntimeVisible( void ) const
	{ return m_fRuntimeVisible; }

inline
bool HKToolBar::getRuntimeEnable( void ) const
	{ return m_fRuntimeEnable; }

inline
qint32 HKToolBar::getRuntimeID( void ) const
	{ return m_nRuntimeID; }
inline
void HKToolBar::setRuntimeVisible( bool fRuntimeVisible )
	{ m_fRuntimeVisible = fRuntimeVisible; }

inline
void HKToolBar::setRuntimeEnable( bool fRuntimeEnable )
	{ m_fRuntimeEnable = fRuntimeEnable; }

inline
void HKToolBar::setRuntimeID( qint32 nRuntimeID )
	{ m_nRuntimeID = nRuntimeID; }

///////////////////////////////////////////////////////////////////////////
//
// class HKToolBarSet
//
///////////////////////////////////////////////////////////////////////////

class HKToolBarSet
{
protected:
	qint32	m_curtbr;
	qint32	m_curbtn;

public:
	HKToolBarSet();
	virtual ~HKToolBarSet();

public:
	virtual long getToolBarCount() const { return 0; };
        virtual const HKToolBar* getToolBar( long /*index*/ ) { return 0; };

	void setCurrentToolBar( qint32 tbr );
	void setCurrentButton( qint32 btn );

	qint32 getCurrentToolBar() const;
	qint32 getCurrentButton() const;

	friend QDataStream& operator << ( QDataStream& ar, const HKToolBarSet& lp );
	friend QDataStream& operator >> ( QDataStream& ar, HKToolBarSet& lp );
};
/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const HKToolBarSet& lp )
{
	return ar << lp.m_curtbr << lp.m_curbtn;
}

inline
QDataStream& operator >> ( QDataStream& ar, HKToolBarSet& lp )
{
	return ar >> lp.m_curtbr >> lp.m_curbtn;
}

/////////////////////////////////////////////////////////////////////

inline 
HKToolBarSet::HKToolBarSet() : m_curtbr( -1 ), m_curbtn( -1 )
{
}

inline 
HKToolBarSet::~HKToolBarSet()
{
}

inline 
void HKToolBarSet::setCurrentToolBar( qint32 tbr )
{
	m_curtbr = tbr;
}

inline 
void HKToolBarSet::setCurrentButton( qint32 btn )
{
	m_curbtn = btn;
}

inline 
qint32 HKToolBarSet::getCurrentToolBar() const
{
	return m_curtbr;
}

inline 
qint32 HKToolBarSet::getCurrentButton() const
{
	return m_curbtn;
}

///////////////////////////////////////////////////////////////////////////
#endif // __COMMON_GUICTRL_TOOLBAR_H__
