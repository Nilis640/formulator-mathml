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

#ifndef __FMLCORE_DLG_CONFIGPAGES_H__
#define __FMLCORE_DLG_CONFIGPAGES_H__

/////////////////////////////////////////////////////////////////////////////

#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>

class CTypesettingPage : public QWidget
{
	Q_OBJECT

protected:
	QCheckBox *m_isContent, *m_isMoveOut, *m_isDxDt, *m_isOneCharID;

public:
    CTypesettingPage(QWidget *parent = 0);
	void load();
	void save();
	void loadPresetWord();
	void loadPresetPmml();
	void loadPresetCmml();
	void loadPresetCmmlPmml();
	void loadPresetCmmlAutograph();
public slots:
	void defaultSettings();
};

/////////////////////////////////////////////////////////////////////////////

class CTranslatePage : public QWidget
{
	Q_OBJECT

protected:
	QCheckBox *m_isSymbolUnicode, *m_isPureContent;
	QRadioButton *m_displayBlockBtn, *m_displayInlineBtn;
	QRadioButton *m_namespaceNoBtn, *m_namespaceMBtn, *m_namespaceXmlnsBtn;
	QRadioButton *m_timesTimesBtn, *m_timesSdotBtn, *m_timesInvisibleBtn;

public:
    CTranslatePage(QWidget *parent = 0);
	void load();
	void save();
	void loadPresetWord();
	void loadPresetPmml();
	void loadPresetCmml();
	void loadPresetCmmlPmml();
	void loadPresetCmmlAutograph();
public slots:
	void defaultSettings();
};

/////////////////////////////////////////////////////////////////////////////

class CRenderingPage : public QWidget
{
	Q_OBJECT

protected:
	QCheckBox *m_isMultipleItalic, *m_isForceMathML, *m_isShowHidden;
	QCheckBox *m_isDiff, *m_isArc, *m_isCalcTextByGlyph;
	QRadioButton *m_listComaBtn, *m_listLocaleBtn;

public:
    CRenderingPage(QWidget *parent = 0);
	void load();
	void save();
	void loadPresetWord();
	void loadPresetPmml();
	void loadPresetCmml();
	void loadPresetCmmlPmml();
	void loadPresetCmmlAutograph();
public slots:
	void defaultSettings();
};

/////////////////////////////////////////////////////////////////////////////

class CAppearancePage : public QWidget
{
	Q_OBJECT

protected:
	QSpinBox *leftEdit, *topEdit, *rightEdit, *bottomEdit, *linespacingEdit;

protected slots:
	void resetSettings();

public:
    CAppearancePage(QWidget *parent = 0);
	void load();
	void save();
	void loadPresetWord();
	void loadPresetPmml();
	void loadPresetCmml();
	void loadPresetCmmlPmml();
	void loadPresetCmmlAutograph();
public slots:
	void defaultSettings();
};

/////////////////////////////////////////////////////////////////////////////

class CWarningsPage : public QWidget
{
	Q_OBJECT

protected:
	QCheckBox *m_isShowNavigation,/* *m_isShowRefresh, */*m_isShowClipboard;

public:
    CWarningsPage(QWidget *parent = 0);
	void load();
	void save();
	void loadPresetWord();
	void loadPresetPmml();
	void loadPresetCmml();
	void loadPresetCmmlPmml();
	void loadPresetCmmlAutograph();
public slots:
	void defaultSettings();
};

/////////////////////////////////////////////////////////////////////////////

class CPerfomancePage : public QWidget
{
	Q_OBJECT

protected:
	QCheckBox *m_isCopyImage, *m_isUpdateMmlEditor, *m_isUpdate;

public:
    CPerfomancePage(QWidget *parent = 0);
	void load();
	void save();
	void loadPresetWord();
	void loadPresetPmml();
	void loadPresetCmml();
	void loadPresetCmmlPmml();
	void loadPresetCmmlAutograph();
public slots:
	void defaultSettings();
};

/////////////////////////////////////////////////////////////////////////////

#endif
