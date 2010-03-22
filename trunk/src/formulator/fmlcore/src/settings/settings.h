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

#if !defined( __FORMULATOR_SETTINGS_SETTINGS_H__ )
#define __FORMULATOR_SETTINGS_SETTINGS_H__

#include <QtGlobal>
#include <QCoreApplication>
#include <QSettings>
#include <QUuid>
#include <QString>

#include "HUtils/iversion.h"
#include "HUtils/idrawing.h"

#include "../fmlcore_global.h"
#include "../crypto/crypto.h"
#include "setdefs.h"

/////////////////////////////////////////////////////////////////////
#ifdef Q_WS_MAC
        const QString FmlCoreSettings_org = QString(__COMPANY_DOMAIN_);
#else
        const QString FmlCoreSettings_org = QString(__COMPANY_NAME__);
#endif
const QString FmlCoreSettings_app = QCoreApplication::applicationName();
class __HERMITECH_FORMULIB__ FmlCoreSettings
{
public:
	//static QSettings getSettings()
	//	{ return QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ); }

	static long getOption( QString group, QString name, long defaultValue )
	{
		//getSettings().value( QString("%1/%2").arg(group).arg(name), defaultValue ).toLong();
		return QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ).
			value( QString("%1/%2").arg(group).arg(name), (qlonglong) defaultValue ).value<long>();
	}
	static void setOption( QString group, QString name, long value )
	{
		QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ).
			setValue( QString("%1/%2").arg(group).arg(name), (qlonglong) value );
	}

	static double getOption( QString group, QString name, double defaultValue )
	{
		return QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ).
			value( QString("%1/%2").arg(group).arg(name), defaultValue ).toDouble();
	}
	static void setOption( QString group, QString name, double value )
	{
		QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ).
			setValue( QString("%1/%2").arg(group).arg(name), value );
	}

	static QString getOption( QString group, QString name, QString defaultValue )
	{
		return QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ).
			value( QString("%1/%2").arg(group).arg(name), defaultValue ).toString();
	}
	void static setOption( QString group, QString name, QString value )
	{
		QSettings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app ).
			setValue( QString("%1/%2").arg(group).arg(name), value );
	}

public:
	static QString getRootDir()
		{ return QString("%1/-formulib").arg(":"/*QCoreApplication::applicationDirPath()*/); }
	static QString getDataDir()
		{ return QString("%1/bin").arg(getRootDir()); }
	static QString getXmlBtnDir()
		{ return QString("%1/xml-button").arg(getRootDir()); }
	static QString getImageDir()
		{ return QString("%1/images").arg(getRootDir()); }
	static QString getXmlBtnGroupsImageDir()
		{ return QString("%1/images/groups").arg(getRootDir()); }

	static QString getXmlBtnFileName()
		{ return QString("buttonstd.cts"); }
	static QString getXmlBtnFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getXmlBtnFileName()); }

	static QString getKeyFileName()
		{ return QString("xkc.cts"); }
	static QString getKeyFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getKeyFileName()); }

	static QString getUserRegFileName()
		{ return QString("xurdt.cts"); }
	static QString getUserRegFile()
		{ return QString("%1/%2").arg(getDataDir()); }

	static QString getMainStyleFileName()
		{ return QString("style4qt.cts"); }
	static QString getMainStyleFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getMainStyleFileName()); }

	static QString getLocalUserStyleFile()
		{
			QSettings settings(QSettings::IniFormat, QSettings::UserScope, FmlCoreSettings_org, FmlCoreSettings_app );
			return QString("%1.style4qt.cts").arg(settings.fileName());
		}

	static QString getOperatorByDSRFileName()
		{ return QString("op_by_dsr.cts"); }
	static QString getOperatorByDSRFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getOperatorByDSRFileName()); }

	static QString getOperatorByMMLFileName()
		{ return QString("op_by_mml.cts"); }
	static QString getOperatorByMMLFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getOperatorByMMLFileName()); }

	static QString getDFAFileName()
		{ return QString("dfa.cts"); }
	static QString getDFAFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getDFAFileName()); }

	static QString getUDFAFileName()
		{ return QString("udfa.cts"); }
	static QString getUDFAFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getUDFAFileName()); }

	static QString getVal2ENameFileName()
		{ return QString("unicodeval_ename.cts"); }
	static QString getVal2ENameFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getVal2ENameFileName()); }

	static QString getXslFileName()
		{ return QString("pmathml.xsl"); }
	static QString getXslFile()
		{ return QString("%1/%2").arg(getDataDir()).arg(getXslFileName()); }
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// settings
/////////////////////////////////////////////////////////////////////////////

inline
double Formulib_GetScale()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_SCALE, DEFAULT_VIEWSCALE );
}
inline 
void Formulib_SetScale( double scale )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_SCALE, scale );
}

inline 
double Formulib_GetLineSpacing()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_LINESPACING, DEFAULT_LINESPACING );
}
inline 
void Formulib_SetLineSpacing( double lineSpacing )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_LINESPACING, lineSpacing );
}

inline 
long Formulib_GetTimes2Times()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_OPTION_TIMES2TIMES, DEFAULT_OPTION_TIMES2TIMES );
}
inline 
void Formulib_SetTimes2Times( long dw )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_OPTION_TIMES2TIMES, dw );
}

inline 
long Formulib_GetTranslator()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_OPTION_TRANSLATOR, DEFAULT_OPTION_TRANSLATOR );
}
inline 
void Formulib_SetTranslator( long dwTranslator )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_OPTION_TRANSLATOR, dwTranslator );
}

inline 
long Formulib_GetOptionsValue()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_OPTIONS, OLIMP_OPTION_FLAG__DEFAULT );
}
inline 
void Formulib_SetOptionsValue( long dwValue )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_OPTIONS, dwValue );
}

inline 
void Formulib_SetCheck4UpdatesUid( QString str )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_CURRENT_UID, str );
}

inline 
QString Formulib_GetCheck4UpdatesUid()
{
	QString ret = FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_CURRENT_UID, QString() );
	if( ret.isEmpty() )
	{
		ret = QUuid::createUuid().toString();
		::Formulib_SetCheck4UpdatesUid( ret );
	}
	return ret;
}

inline 
QString Formulib_GetCheck4UpdatesBuild()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_CURRENT_BUILD, QCoreApplication::applicationVersion() );
}

inline 
void Formulib_SetCheck4UpdatesBuild( QString str )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_CURRENT_BUILD, str );
}

inline 
long Formulib_GetCheck4UpdatesPeriod()
{
	long period = FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_CHECK4UPDATES_PERIOD, long(FORMULIB_CHECK4UPDATES_PERIOD_DEFAULT) );
	if( period < FORMULIB_CHECK4UPDATES_PERIOD_MIN )
		period = FORMULIB_CHECK4UPDATES_PERIOD_MIN;
	else if( period > FORMULIB_CHECK4UPDATES_PERIOD_MAX )
		period = FORMULIB_CHECK4UPDATES_PERIOD_MAX;
	return period;
}

inline 
void Formulib_SetCheck4UpdatesPeriod( long nTime )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_CHECK4UPDATES_PERIOD, nTime );
}

inline 
long Formulib_GetCheck4UpdatesTime()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_CHECK4UPDATES_TIME, long(0) );
}
inline 
void Formulib_SetCheck4UpdatesTime( long nTime )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_CHECK4UPDATES_TIME, nTime );
}

inline 
void Formulib_GetIndents( RectFde& rcIndents )
{
	rcIndents.setTop( (double) FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_INDENT_TOP, DEFAULT_INDENTS ) );
	rcIndents.setLeft( (double) FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_INDENT_LEFT, DEFAULT_INDENTS ) );
	rcIndents.setBottom( (double) FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_INDENT_BOTTOM, DEFAULT_INDENTS ) );
	rcIndents.setRight( (double) FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_INDENT_RIGHT, DEFAULT_INDENTS ) );
}
inline 
void Formulib_SetIndents( RectFde& rcIndents )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_INDENT_TOP, rcIndents.top() );
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_INDENT_LEFT, rcIndents.left() );
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_INDENT_BOTTOM, rcIndents.bottom() );
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_INDENT_RIGHT, rcIndents.right() );
}

inline 
long Formulib_GetHtmlSplit()
{
	return FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_XHTML_SPLIT, DEFAULT_SPLIT );
}
inline 
void Formulib_SetHtmlSplit( long nSplitter )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_XHTML_SPLIT, nSplitter );
}

inline 
long Formulib_GetRegLastCall( long nTime )
{
	QString strData = FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_REG_LASTCALL, QString("") );
	if( strData.isEmpty() ) return nTime;
	bool ok;
	int v = ::deENDE(strData).toInt(&ok);
	return ok ? v : nTime;
}
inline 
void Formulib_SetRegLastCall( long nTime )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_REG_LASTCALL, ::enENDE(QString("%1").arg(nTime)) );
}

inline 
int Formulib_GetRegTotalDay( long nDay )
{
	QString strData = FmlCoreSettings::getOption( FORMULIB_SETTINGS, FORMULIB_REG_TOTALDAY, QString("") );
	if( strData.isEmpty() ) return nDay;
	bool ok;
	int v = ::deENDE(strData).toInt(&ok);
	return ok ? v : nDay;
}

inline 
void Formulib_SetRegTotalDay( long nDay )
{
	FmlCoreSettings::setOption( FORMULIB_SETTINGS, FORMULIB_REG_TOTALDAY, ::enENDE(QString("%1").arg(nDay)) );
}

/////////////////////////////////////////////////////////////////////////////

#endif
