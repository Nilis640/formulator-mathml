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

#include <QtGui>

#include "HMathML/isettings.h"
#include "configdlg.h"
#include "configpages.h"

/////////////////////////////////////////////////////////////////////////////

CFormulatorConfigDlg::CFormulatorConfigDlg( int page, QWidget *parent )
	: QDialog( parent )
{
	m_activePage = qBound( int(MinPage), page, int(MaxPage) );
	m_applyPreset = FORMULIB_PRESET_NONE;

    contentsWidget = new QListWidget();
    contentsWidget->setViewMode(QListView::IconMode);
	//contentsWidget->setIconSize(QSize(128, 128));
    contentsWidget->setIconSize(QSize(96, 96));
	//contentsWidget->setIconSize(QSize(64, 64));
    contentsWidget->setMovement(QListView::Static);
    //contentsWidget->setMaximumHeight(400);
    //contentsWidget->setMaximumWidth(180);
	contentsWidget->setMaximumWidth(160);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(m_option1 = new CTypesettingPage());
    pagesWidget->addWidget(m_option2 = new CTranslatePage());
    pagesWidget->addWidget(m_option3 = new CRenderingPage());
    pagesWidget->addWidget(m_option4 = new CAppearancePage());
    pagesWidget->addWidget(m_option5 = new CWarningsPage());
    pagesWidget->addWidget(m_option6 = new CPerfomancePage());

    presetWordAct = new QAction(QIcon(":/images/preset-word.png"), tr("&Word Processor"), this);
    presetWordAct->setStatusTip(tr("A set of options similar to Word Processor behavior"));
    connect(presetWordAct, SIGNAL(triggered()), this, SLOT(presetWord()));

    presetPmmlAct = new QAction(QIcon(":/images/preset-pmml.png"), tr("&Presentation MathML Editor"), this);
    presetPmmlAct->setStatusTip(tr("A set of options for editing Presentation MathML"));
    connect(presetPmmlAct, SIGNAL(triggered()), this, SLOT(presetPmml()));

	presetCmmlAct = new QAction(QIcon(":/images/preset-cmml2.png"), tr("&Content MathML Editor"), this);
    presetCmmlAct->setStatusTip(tr("A set of options for editing pure Content MathML"));
    connect(presetCmmlAct, SIGNAL(triggered()), this, SLOT(presetCmml()));

	presetCmmlPmmlAct = new QAction(QIcon(":/images/preset-cmml-p.png"), tr("&Content MathML Editor with Presentation markup fragments"), this);
    presetCmmlPmmlAct->setStatusTip(tr("A set of options for mixing Content MathML Editor with Presentation markup"));
    connect(presetCmmlPmmlAct, SIGNAL(triggered()), this, SLOT(presetCmmlPmml()));

	presetCmmlAutographAct = new QAction(QIcon(":/images/preset-cmml-acsii.png"), tr("&Content MathML Editor with ASCII-like typing rules"), this);
    presetCmmlAutographAct->setStatusTip(tr("A set of options for creating Content MathML with specific autodetecting rules"));
    connect(presetCmmlAutographAct, SIGNAL(triggered()), this, SLOT(presetCmmlA()));

	QMenu *presetMenu = new QMenu( this );
	presetMenu->addAction(presetWordAct);
	presetMenu->addAction(presetPmmlAct);
	presetMenu->addAction(presetCmmlAct);
	presetMenu->addAction(presetCmmlPmmlAct);
	presetMenu->addAction(presetCmmlAutographAct);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	QPushButton *presetBtn = new QPushButton( this );
	presetBtn->setText(tr("&Preset"));
	//presetBtn->setIcon(QIcon(":/images/wizard.png"));
	//presetBtn->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
	//presetBtn->setToolButtonStyle( Qt::ToolButtonIconOnly );
	presetBtn->setMenu( presetMenu );
	//presetBtn->setPopupMode( QToolButton::InstantPopup );
	buttonBox->addButton( presetBtn, QDialogButtonBox::ResetRole );
	connect(buttonBox->addButton(tr("&Default"), QDialogButtonBox::ResetRole), SIGNAL(clicked()), this, SLOT(factorySettings()));

	createIcons();
    contentsWidget->setCurrentRow( m_activePage );

	QVBoxLayout *rightLayout = new QVBoxLayout();
	rightLayout->addWidget(pagesWidget);
    rightLayout->addSpacing(32);
	rightLayout->addWidget(buttonBox);
    rightLayout->addStretch(1);

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addLayout(rightLayout);

    setLayout(horizontalLayout);

	setWindowTitle(tr("Preferences"));
}

void CFormulatorConfigDlg::setActivePage( int page )
{
	m_activePage = page;
}

void CFormulatorConfigDlg::createIcons()
{
    QListWidgetItem *typesettingButton = new QListWidgetItem(contentsWidget);
    typesettingButton->setIcon(QIcon(":/images/rules.png"));
    typesettingButton->setText(tr("Typesetting Rules"));
    typesettingButton->setTextAlignment(Qt::AlignHCenter);
    typesettingButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *translateButton = new QListWidgetItem(contentsWidget);
    translateButton->setIcon(QIcon(":/images/translate.png"));
    translateButton->setText(tr("Translate to MathML"));
    translateButton->setTextAlignment(Qt::AlignHCenter);
    translateButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *renderingButton = new QListWidgetItem(contentsWidget);
    renderingButton->setIcon(QIcon(":/images/mmlrendering.png"));
    renderingButton->setText(tr("MathML Rendering"));
    renderingButton->setTextAlignment(Qt::AlignHCenter);
    renderingButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *appearanceButton = new QListWidgetItem(contentsWidget);
    appearanceButton->setIcon(QIcon(":/images/document.png"));
    appearanceButton->setText(tr("Page Setup"));
    appearanceButton->setTextAlignment(Qt::AlignHCenter);
    appearanceButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *warningsButton = new QListWidgetItem(contentsWidget);
    warningsButton->setIcon(QIcon(":/images/readme.png"));
    warningsButton->setText(tr("Warnings"));
    warningsButton->setTextAlignment(Qt::AlignHCenter);
    warningsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *toolsButton = new QListWidgetItem(contentsWidget);
    toolsButton->setIcon(QIcon(":/images/tools.png"));
    toolsButton->setText(tr("Environment"));
    toolsButton->setTextAlignment(Qt::AlignHCenter);
    toolsButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void CFormulatorConfigDlg::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if( !current ) current = previous;
	m_activePage = contentsWidget->row(current);
    pagesWidget->setCurrentIndex( m_activePage );
}

/////////////////////////////////////////////////////////////////////////////

void CFormulatorConfigDlg::accept()
{
	m_option1->save();
	m_option2->save();
	m_option3->save();
	m_option4->save();
	m_option5->save();
	m_option6->save();
	QDialog::accept();
}

/////////////////////////////////////////////////////////////////////////////

void CFormulatorConfigDlg::factorySettings()
{
	m_option1->defaultSettings();
	m_option2->defaultSettings();
	m_option3->defaultSettings();
	m_option4->defaultSettings();
	m_option5->defaultSettings();
	m_option6->defaultSettings();
	m_applyPreset = FORMULIB_PRESET_DEFAULT;
}

void CFormulatorConfigDlg::presetWord()
{
	m_option1->loadPresetWord();
	m_option2->loadPresetWord();
	m_option3->loadPresetWord();
	m_option4->loadPresetWord();
	m_option5->loadPresetWord();
	m_option6->loadPresetWord();
	m_applyPreset = FORMULIB_PRESET_WORD;
}

void CFormulatorConfigDlg::presetPmml()
{
	m_option1->loadPresetPmml();
	m_option2->loadPresetPmml();
	m_option3->loadPresetPmml();
	m_option4->loadPresetPmml();
	m_option5->loadPresetPmml();
	m_option6->loadPresetPmml();
	m_applyPreset = FORMULIB_PRESET_PMML;
}

void CFormulatorConfigDlg::presetCmml()
{
	m_option1->loadPresetCmml();
	m_option2->loadPresetCmml();
	m_option3->loadPresetCmml();
	m_option4->loadPresetCmml();
	m_option5->loadPresetCmml();
	m_option6->loadPresetCmml();
	m_applyPreset = FORMULIB_PRESET_CMML;
}

void CFormulatorConfigDlg::presetCmmlPmml()
{
	m_option1->loadPresetCmmlPmml();
	m_option2->loadPresetCmmlPmml();
	m_option3->loadPresetCmmlPmml();
	m_option4->loadPresetCmmlPmml();
	m_option5->loadPresetCmmlPmml();
	m_option6->loadPresetCmmlPmml();
	m_applyPreset = FORMULIB_PRESET_CMML_P;
}

void CFormulatorConfigDlg::presetCmmlA()
{
	m_option1->loadPresetCmmlAutograph();
	m_option2->loadPresetCmmlAutograph();
	m_option3->loadPresetCmmlAutograph();
	m_option4->loadPresetCmmlAutograph();
	m_option5->loadPresetCmmlAutograph();
	m_option6->loadPresetCmmlAutograph();
	m_applyPreset = FORMULIB_PRESET_CMML_ASCII;
}

/////////////////////////////////////////////////////////////////////////////
