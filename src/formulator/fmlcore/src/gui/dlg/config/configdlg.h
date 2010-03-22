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

#ifndef __FMLCORE_DLG_CONFIGDLG_H__
#define __FMLCORE_DLG_CONFIGDLG_H__

#include <QDialog>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QAction;
QT_END_NAMESPACE

class CTypesettingPage;
class CTranslatePage;
class CRenderingPage;
class CAppearancePage;
class CWarningsPage;
class CPerfomancePage;

/////////////////////////////////////////////////////////////////////////////

class CFormulatorConfigDlg : public QDialog
{
    Q_OBJECT

protected:
	CTypesettingPage	*m_option1;
	CTranslatePage		*m_option2;
	CRenderingPage		*m_option3;
	CAppearancePage		*m_option4;
	CWarningsPage		*m_option5;
	CPerfomancePage		*m_option6;

	QAction				*presetWordAct;
	QAction				*presetPmmlAct;
	QAction				*presetCmmlAct;
	QAction				*presetCmmlPmmlAct;
	QAction				*presetCmmlAutographAct;
	int					m_applyPreset;
	int					m_activePage;

	enum
	{ MinPage = 0, MaxPage = 5 };

public:
    CFormulatorConfigDlg( int page, QWidget *parent );

	int getPreset() { return m_applyPreset; }

	void setActivePage( int page );
	int activePage() { return m_activePage; }

public slots:
	virtual void accept();
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

	void factorySettings();
	void presetWord();
	void presetPmml();
	void presetCmml();
	void presetCmmlPmml();
	void presetCmmlA();

private:
    void createIcons();

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
};

/////////////////////////////////////////////////////////////////////////////

#endif
