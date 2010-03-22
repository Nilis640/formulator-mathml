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

#include <QSettings>
#include <QFile>
#include <QDir>

#include "HUtils/iversion.h"

#include "../version/version.h"
#include "../button/toolbarset.h"
#include "../mathml/mml_tag2mml.h"

#include "options.h"

/////////////////////////////////////////////////////////////////////
// style

FORMULATOR__TheOneStyle::FORMULATOR__TheOneStyle()
	: formula_style()
	, onchar_style( FSTYLE_DEFAULT_ONCHAR )
	, onuser_style( FSTYLE_NONE )
	//, onuser_kegl( FKEGL_FRAME )
	, onuser_kegl_level( FTEXT_HEIGHT_FRAME )
{
	onuser_kegl = formula_style.getKegl( onuser_kegl_level );
	onuser_kegl_offset = 0.0;
}

FORMULATOR__TheOneStyle::~FORMULATOR__TheOneStyle()
{
}

/////////////////////////////////////////////////////////////////////

FORMULATOR__TheOneState::FORMULATOR__TheOneState()
	: translator_pref( TRANSLATE_2_MATHML2_NO_NAMESPACE )
	, contentTimes( TRANSLATE_CONTENT_MATHML2_TIMES )
	, m_flags( OLIMP_OPTION_FLAG__DEFAULT )
{
	currentLocale = QLocale::system();
}

/////////////////////////////////////////////////////////////////////

FORMULATOR__GlobalsStorage::FORMULATOR__GlobalsStorage()
	: m_messageManager()
	, m_fStyle( 0 )
	, m_styles()
	, m_pStyle( NULL )
	, m_states()
	, m_pState( NULL )
{
	m_toolBarSet = new FBtnToolbarSet();
	createProject( _T("default") );
}

FORMULATOR__GlobalsStorage::~FORMULATOR__GlobalsStorage()
{
	if( m_toolBarSet )
	{
		delete m_toolBarSet;
		m_toolBarSet = NULL;
	}
	clearProjects();
}

/////////////////////////////////////////////////////////////////////

long FORMULATOR__GlobalsStorage::getCurrentFormulatorContentMathML_TranslateTimes( void )
	{ return ::mml_GetPreference_ContentTimes(); }

void FORMULATOR__GlobalsStorage::setCurrentFormulatorContentMathML_TranslateTimes( long st )
	{ ::mml_UsePreference_ContentTimes( st ); }

/////////////////////////////////////////////////////////////////////

int FORMULATOR__GlobalsStorage::createProject( const QString& strKey )
{
	int fResult = 1;
	if( !createStyle( strKey ) )
		fResult = 0;
	if( !createState( strKey ) )
		fResult = 0;
	return fResult;
}

int FORMULATOR__GlobalsStorage::deleteProject( const QString& strKey )
{
	int fResult = 1;
	if( !deleteStyle( strKey ) )
		fResult = 0;
	if( !deleteState( strKey ) )
		fResult = 0;
	return fResult;
}

int FORMULATOR__GlobalsStorage::clearProjects( void )
{
	int fResult = 1;
	if( !clearStyles() )
		fResult = 0;
	if( !clearStates() )
		fResult = 0;
	return fResult;
}

int FORMULATOR__GlobalsStorage::setActiveProject( const QString& strKey )
{
	int fResult = 1;
	if( setActiveStyle( strKey ) == NULL )
		fResult = 0;
	if( setActiveState( strKey ) == NULL )
		fResult = 0;
	return fResult;
}

/////////////////////////////////////////////////////////////////////
//

int FORMULATOR__GlobalsStorage::createState( const QString& strKey )
{
	mapPtr2OneState::iterator it = m_states.find( strKey );
	if( it != m_states.end() )
		return 0;

	FORMULATOR__TheOneState* pTheOneState = new FORMULATOR__TheOneState();
	if( pTheOneState == NULL )
		return 0;

	m_states[ strKey ] = pTheOneState;
	setActiveState( strKey );
	return 1;
}

int FORMULATOR__GlobalsStorage::deleteState( const QString& strKey )
{
	mapPtr2OneState::iterator it = m_states.find( strKey );
	if( it == m_states.end() )
		return 0;

	Q_ASSERT( it.value() != NULL );
	delete it.value();
	m_states.erase( it );

	setActiveState( m_states.begin() != m_states.end() ? m_states.begin().key() : _T("") );
	return 1;
}

/////////////////////////////////////////////////////////////////////

int FORMULATOR__GlobalsStorage::clearStates( void )
{
	mapPtr2OneState::iterator it, end = m_states.end();
	for( it = m_states.begin(); it != end; it++ )
	{
		Q_ASSERT( it.value() != NULL );
		delete it.value();
	}
	m_states.clear();

	setActiveState( _T("") );
	return 1;
}

/////////////////////////////////////////////////////////////////////
//

int FORMULATOR__GlobalsStorage::createStyle( const QString& strKey )
{
	mapPtr2OneStyle::iterator it = m_styles.find( strKey );
	if( it != m_styles.end() )
		return 0;

	FORMULATOR__TheOneStyle* pTheOneStyle = new FORMULATOR__TheOneStyle();
	if( pTheOneStyle == NULL )
		return 0;

	m_styles[ strKey ] = pTheOneStyle;
	setActiveStyle( strKey );
	return 1;
}

int FORMULATOR__GlobalsStorage::deleteStyle( const QString& strKey )
{
	mapPtr2OneStyle::iterator it = m_styles.find( strKey );
	if( it == m_styles.end() )
		return 0;

	Q_ASSERT( it.value() != NULL );
	delete it.value();
	m_styles.erase( it );

	setActiveStyle( m_styles.begin() != m_styles.end() ? m_styles.begin().key() : _T("") );
	return 1;
}

/////////////////////////////////////////////////////////////////////

int FORMULATOR__GlobalsStorage::clearStyles( void )
{
	mapPtr2OneStyle::iterator it, end = m_styles.end();
	for( it = m_styles.begin(); it != end; it++ )
	{
		Q_ASSERT( it.value() != NULL );
		delete it.value();
	}
	m_styles.clear();
	m_fStyle = 0;

	setActiveStyle( _T("") );
	return 1;
}

/////////////////////////////////////////////////////////////////////

int FORMULATOR__GlobalsStorage::saveStyles( int isLocalUser )
{
	QString strPath = isLocalUser ? FmlCoreSettings::getLocalUserStyleFile() : FmlCoreSettings::getMainStyleFile();
	QFile file( strPath );
	if( !file.open(QIODevice::WriteOnly) )
	{
		::getCurrentDRMessageManager()->AddLine( 0, _T("Error while saving style file \"%1\""), -1, (const QString&) strPath );
		return -1;
	}
	QDataStream ar( &file );
	ar << QString(__COMPANY_NAME__);
	ar << QString(__FMLCORE_ProductVersion);
	ar.setVersion(QDataStream::Qt_4_5);

	quint32 count = (quint32) m_styles.size();
	ar << count;

	QString strKey;
	mapPtr2OneStyle::iterator it, end = m_styles.end();
	for( it = m_styles.begin(); it != end; it++ )
	{
		Q_ASSERT( it.value() != NULL );
		strKey = it.key();
		ar << strKey;
		ar << it.value()->formula_style;
	}

	file.close();
	return 0;
}

int FORMULATOR__GlobalsStorage::loadStyles( int isLocalUser )
{
	if( m_fStyle ) return 0;

	QString localStylePath = FmlCoreSettings::getLocalUserStyleFile(), globalStylePath = FmlCoreSettings::getMainStyleFile();
	if( isLocalUser && !QFile::exists( localStylePath ) )
		QFile::copy( globalStylePath, localStylePath );

	QFile file( localStylePath );
	if( !file.open( QIODevice::ReadOnly ) )
	{
		file.setFileName( FmlCoreSettings::getMainStyleFile() );
		if( !file.open( QIODevice::ReadOnly ) )
		{
			getCurrentDRMessageManager()->AddLine( 0, _T("Error while loading style file") );
			return -1;
		}
	}

	QDataStream ar( &file );
	QString str;
	ar >> str;
	if( str != QString(__COMPANY_NAME__) )
	{
		file.close();
		getCurrentDRMessageManager()->AddLine( 0, _T("wrong format of the binary style file \"%1\""), -1, (const QString&) str );
		return -1;
	}
	ar >> str;
	if( str == QString(__FMLCORE_ProductVersion) )
	{
		ar.setVersion(QDataStream::Qt_4_5);
	}
	else
	{
		file.close();
		getCurrentDRMessageManager()->AddLine( 0, _T("wrong format of the binary style file \"%1\""), -1, (const QString&) str );
		return -1;
	}

	clearStyles();

	quint32 count = 0;
	ar >> count;

	QString strKey;
	FORMULATOR__TheOneStyle* pTheOneStyle = NULL;
	for( unsigned long i = 0; i < count; i++ )
	{
		ar >> strKey;
		pTheOneStyle = new FORMULATOR__TheOneStyle();
		ar >> pTheOneStyle->formula_style;
		m_styles[ strKey ] = pTheOneStyle;
	}

	if( m_styles.begin() == m_styles.end() )
	{
		file.close();
		return (createStyle( _T("default") ) ? 0 : -1);
	}

	m_fStyle = 1;
	setActiveStyle( m_styles.begin().key() );

	file.close();
	return 0;
}

/////////////////////////////////////////////////////////////////////

void FORMULATOR__GlobalsStorage::saveToolBarSet( const QString& path )
{
	if( !m_toolBarSet ) return;
	QFile file( path );
	if( file.open(QIODevice::WriteOnly) )
	{
		QDataStream ar( &file );
		ar << QString(__COMPANY_NAME__);
		ar << QString(__FMLCORE_XmlbtnVersion);
		ar.setVersion(QDataStream::Qt_4_5);
		ar << *m_toolBarSet;
	}
}

void FORMULATOR__GlobalsStorage::loadToolBarSet( const QString& path )
{
	QFile file( path );
	if( !file.open(QIODevice::ReadOnly) ) return;
	QDataStream ar( &file );

	QString str;
	ar >> str;
	if( str != QString(__COMPANY_NAME__) ) return;
	ar >> str;
	if( str == QString(__FMLCORE_XmlbtnVersion) )
	{
		ar.setVersion(QDataStream::Qt_4_5);
	}
	else
		return;

	if( m_toolBarSet )
	{
		delete m_toolBarSet;
		m_toolBarSet = NULL;
	}
	m_toolBarSet = new FBtnToolbarSet();
	ar >> *m_toolBarSet;
}

/////////////////////////////////////////////////////////////////////

void defaultFormulatorSettings()
{
	::setInputMode_Presentation();
	::clearAutographEditing_MoveOutOfHighPriorityOp();
	::clearAutographEditing_dxdt();
	::clearAutographEditing_OneCharID();

	::setOption_UnicodeAsName();
	::clearOption_XHTMLAsBlock();
	::setCurrentFormulatorStyle_TranslatePref(DEFAULT_OPTION_TRANSLATOR);
	::setCurrentFormulatorContentMathML_TranslateTimes(DEFAULT_OPTION_TIMES2TIMES);
	::clearPureContentMathMLOutput();

	::clearOption_MulticharMINotItalic();
	::clearMathMLStylePriority();
	::setRenderMode_Force( 0 );
	::clearAutographDiff();
	::clearRenderArcFunAsPower();
	::setComaAsSLIST();

	::Formulib_SetLineSpacing( DEFAULT_LINESPACING );
	RectFde rect( PointFde(DEFAULT_INDENTS, DEFAULT_INDENTS), PointFde(DEFAULT_INDENTS, DEFAULT_INDENTS) );
	::Formulib_SetIndents( rect );

	::clearOption_ShowNavInfo();
	::clearOption_ShowWarning_RefreshMathML();
	::setOption_ShowWarning_InsertMathML();

	// do not change by design
	//::setOption_Perfomance_CopyImage();
	//::setOption_UpdateMathMLView();

	//::setOption_Check4Updates(); // do not change by design
}

void presetWordFormulatorSettings()
{
	::defaultFormulatorSettings();
}

void presetPmmlFormulatorSettings()
{
	::defaultFormulatorSettings();

	::setMathMLStylePriority();

	::setOption_ShowNavInfo();
	//::setOption_ShowWarning_RefreshMathML();
}

void presetCmmlFormulatorSettings()
{
	::defaultFormulatorSettings();

	::setInputMode_Content();

	::setPureContentMathMLOutput();

	::setMathMLStylePriority();

	::setOption_ShowNavInfo();
	//::setOption_ShowWarning_RefreshMathML();
}

void presetCmmlPmmlFormulatorSettings()
{
	::defaultFormulatorSettings();

	::setInputMode_Content();

	::setMathMLStylePriority();

	::setOption_ShowNavInfo();
	//::setOption_ShowWarning_RefreshMathML();
}

void presetCmmlAutographFormulatorSettings()
{
	::defaultFormulatorSettings();

	::setInputMode_Content();
	::setAutographEditing_MoveOutOfHighPriorityOp();
	::setAutographEditing_dxdt();
	::setAutographEditing_OneCharID();

	::setPureContentMathMLOutput();

	::setMathMLStylePriority();
	::setAutographDiff();
	::setRenderArcFunAsPower();

	::setOption_ShowNavInfo();
	//::setOption_ShowWarning_RefreshMathML();
}

///////////////////////////////////////////////////////////////////////////////

FORMULATOR__GlobalsStorage& getProjectGlobalsStorage( void )
{
	static FORMULATOR__GlobalsStorage projectStorage;
	return projectStorage;
}

///////////////////////////////////////////////////////////////////////////////
