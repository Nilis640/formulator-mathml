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

#include <QtGlobal>
#include <QtGui>

#include "HUtils/iversion.h"
#include "HMathML/isettings.h"
#include "HMathML/itoolbarset.h"
#include "HMathML/idictionary.h"
#include "HMathML/itoolbarset.h"

#include "fmlmainwindow.h"
#include "../widget/fmlwidget.h"
#include "../widget/fmleditwidget.h"
#include "../document/fmldocument.h"

#include "../dlg/buildtoolbarset.h"
#include "../textedit/mmltexteditwidget.h"
#include "../viewer/mmlfileviewerwidget.h"
#include "../dlg/config/configdlg.h"
#include "../../version/version.h"
#include "../../utils/updaterequest.h"

///////////////////////////////////////////////////////////////////////////////

QFormulatorMainWindow::QFormulatorMainWindow(bool isEditMode, QWidget *parent, Qt::WFlags flags)
	: QMainWindow( parent, flags )
{
	QCoreApplication::setOrganizationName(__COMPANY_NAME__);
	QCoreApplication::setOrganizationDomain(__COMPANY_DOMAIN_);
	QCoreApplication::setApplicationVersion( __FMLCORE_ProductVersion );

	m_mmlEditor = 0;
	m_mmlViewer = 0;
	m_presetBtn = 0;
	m_inputStatusLabel = 0;
	m_hiddenStatusLabel = 0;
	m_lastAppliedPreset = FORMULIB_PRESET_NONE;
	m_updater = 0;
	m_configLastPage = 0;
	m_currentPath = "";

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    mdiArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    setCentralWidget( mdiArea );
	
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(updateOnActiveWindow(QMdiSubWindow *)));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActiveSubWindow(QWidget *)));

	if( QApplication::clipboard() )
		connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(updateUI_OnClipboardChanged()));

	readCoreFmlSettings();

	m_mmlEditor = new QMathMLTextEdit( !isEditMode, QString(), this );
	m_mmlEditor->setObjectName("__qt__fmlide_widget_QMathMLTextEdit");
	addDockWidget(Qt::RightDockWidgetArea, m_mmlEditor);
    connect(m_mmlEditor, SIGNAL(visibilityChanged(bool)), this, SLOT(updateMmlEditor(bool)));
    connect(m_mmlEditor, SIGNAL(uploadRequested()), this, SLOT(uploadMathML()));
    connect(m_mmlEditor, SIGNAL(loadRequested()), this, SLOT(loadMathML()));
	m_mmlEditorUpdateTime = 0;

	m_mmlViewer = new QMathMLFileViewer( this );
	m_mmlViewer->setObjectName("__qt__fmlide_widget_QMathMLFileViewer");
	addDockWidget(Qt::RightDockWidgetArea, m_mmlViewer);
    //connect(m_mmlViewer, SIGNAL(visibilityChanged(bool)), this, SLOT(showFileViewerSelectedMml(bool)));
    connect(m_mmlViewer, SIGNAL(openFile(QString, QWidget*)), this, SLOT(openByFileViewer(QString, QWidget*)));
    connect(m_mmlViewer, SIGNAL(loadFile(QString, QWidget*)), this, SLOT(loadByFileViewer(QString, QWidget*)));

	initGui( m_isEditMode = isEditMode );
    readSettings();

	setWindowTitle(__FMLCORE_Product_Title);
	setUnifiedTitleAndToolBarOnMac(false);
	setWindowIcon(QIcon(":/images/fmlideico.ico"));

	newFile();
	automaticCheckUpdate();
}

void QFormulatorMainWindow::initGui( bool isEditMode )
{
	QTime sleep;
	sleep.start();

	if( isEditMode )
	{
		QFormulatorEditWindow::showFormulatorSplash( this );
		QFormulatorEditWindow::initFormulatorCore();
		QFormulatorEditWindow::initGui( this );
	}
	else
	{
		QFormulatorViewWindow::showFormulatorSplash( this );
		QFormulatorViewWindow::initFormulatorCore();
		QFormulatorViewWindow::initGui( this );
	}

	createActions();
    createMenus();
    createToolBars();
    createStatusBar();

	::StartUpFDE( FmlDrawEngine_Qt );
        while( sleep.elapsed() < 1500 ) ;

	if( isEditMode )
		QFormulatorEditWindow::hideFormulatorSplash( this );
	else
		QFormulatorViewWindow::hideFormulatorSplash( this );
}

QFormulatorMainWindow::~QFormulatorMainWindow()
{
	if( m_updater )
	{
		delete m_updater;
		m_updater = 0;
	}
}

void QFormulatorMainWindow::readSettings()
{
#ifdef Q_WS_MAC
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_DOMAIN_, QCoreApplication::applicationName() );
#else
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_NAME__, QCoreApplication::applicationName() );
#endif
	restoreGeometry(settings.value("FmlIde/geometry").toByteArray());
	restoreState(settings.value("FmlIde/guistate").toByteArray());
	
	m_lastAppliedPreset = settings.value("FmlIde/options/preset", FORMULIB_PRESET_NONE).toInt();
	setPresetBtnIcon( m_lastAppliedPreset );

	m_configLastPage = settings.value("FmlIde/usability/configLastPage", 0).toInt();
	m_currentPath = settings.value("FmlIde/usability/currentDir", "").toString();

	// load Formulator Core settings
	//::setCurrentFormulatorContentMathML_TranslateTimes( ::Formulib_GetTimes2Times() );
	//::setCurrentFormulatorStyle_TranslatePref( ::Formulib_GetTranslator() );
	//::setCurrentFormulatorOptionsValue( ::Formulib_GetOptionsValue() );

	if( m_mmlEditor ) m_mmlEditor->readSettings( settings );
	if( m_mmlViewer ) m_mmlViewer->readSettings( settings );

	QString currentVersion = ::Formulib_GetCheck4UpdatesBuild();
	if( currentVersion.isEmpty() )
	{
		currentVersion = __FMLCORE_ProductVersion;
		QCoreApplication::setApplicationVersion( currentVersion );
		::Formulib_SetCheck4UpdatesBuild( currentVersion );
	}
	else
		QCoreApplication::setApplicationVersion( currentVersion );
}

void QFormulatorMainWindow::readCoreFmlSettings()
{
	// load Formulator Core settings
	::setCurrentFormulatorContentMathML_TranslateTimes( ::Formulib_GetTimes2Times() );
	::setCurrentFormulatorStyle_TranslatePref( ::Formulib_GetTranslator() );
	::setCurrentFormulatorOptionsValue( ::Formulib_GetOptionsValue() );
}

void QFormulatorMainWindow::writeSettings()
{
#ifdef Q_WS_MAC
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_DOMAIN_, QCoreApplication::applicationName() );
#else
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_NAME__, QCoreApplication::applicationName() );
#endif
	settings.setValue("FmlIde/geometry", saveGeometry());
	settings.setValue("FmlIde/guistate", saveState());
	settings.setValue("FmlIde/options/preset", m_lastAppliedPreset);
	settings.setValue("FmlIde/usability/configLastPage", m_configLastPage);
	settings.setValue("FmlIde/usability/currentDir", m_currentPath);

	// save Formulator Core settings
	::Formulib_SetOptionsValue( ::getCurrentFormulatorOptionsValue() );
	::Formulib_SetTimes2Times( ::getCurrentFormulatorContentMathML_TranslateTimes() );
	::Formulib_SetTranslator( ::getCurrentFormulatorStyle_TranslatePref() );
	// current UI view scale
	//double value = double(zoomAny->currentText().toUInt()) / 100.0;
	//if( value > FMLIDE_MAX_VIEWSCALE ) value = FMLIDE_MAX_VIEWSCALE;
	//else if( value < FMLIDE_MIN_VIEWSCALE ) value = FMLIDE_MIN_VIEWSCALE;
	//::Formulib_SetScale( value );

	if( m_mmlEditor ) m_mmlEditor->writeSettings( settings );
	if( m_mmlViewer ) m_mmlViewer->writeSettings( settings );
}

///////////////////////////////////////////////////////////////////////////////

void QFormulatorMainWindow::createActions()
{
	for( int i = 0; i < QFormulatorViewWindow::MaxRecentFiles; i++ )
	{
        recentFileActs[ i ] = new QAction( this );
        recentFileActs[ i ]->setVisible( false );
        connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

	exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    toolsBuildToolbarsetAct = new QAction(tr("&Build Math Toolbars..."), this);
	//toolsBuildToolbarsetAct->setShortcut(tr("F7"));
    toolsBuildToolbarsetAct->setStatusTip(tr("Build mathematical toolbars from text description"));
    connect(toolsBuildToolbarsetAct, SIGNAL(triggered()), this, SLOT(buildToolbarset()));

	configDlgAct = new QAction(QIcon(":/images/tools.png"), tr("&Preferences..."), this);
    configDlgAct->setStatusTip(tr("Change current preferences"));
    connect(configDlgAct, SIGNAL(triggered()), this, SLOT(configDlg()));

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Ctrl+F4"));
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));
	connect(this, SIGNAL(hasChildSignal(bool)), closeAct, SLOT(setEnabled(bool)));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));
	connect(this, SIGNAL(hasChildSignal(bool)), closeAllAct, SLOT(setEnabled(bool)));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));
	connect(this, SIGNAL(hasChildSignal(bool)), tileAct, SLOT(setEnabled(bool)));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));
	connect(this, SIGNAL(hasChildSignal(bool)), cascadeAct, SLOT(setEnabled(bool)));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));
	connect(this, SIGNAL(hasChildSignal(bool)), nextAct, SLOT(setEnabled(bool)));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous window"));
    connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
	connect(this, SIGNAL(hasChildSignal(bool)), previousAct, SLOT(setEnabled(bool)));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

	frmWebDonateAct = new QAction(tr("&Support Formulator project"), this);
    frmWebDonateAct->setStatusTip(tr("Open a link to a page where you can support further development of this free Formulator software with monetary donation"));
    connect(frmWebDonateAct, SIGNAL(triggered()), this, SLOT(mmlsoftDonateWeb()));

	frmWebAct = new QAction(tr("&Formulator on the Web"), this);
    frmWebAct->setStatusTip(tr("Open a link to the Formulator site"));
    connect(frmWebAct, SIGNAL(triggered()), this, SLOT(mmlsoftWeb()));

	manualCheckUpdateAct = new QAction(tr("&Check for Updates"), this);
    manualCheckUpdateAct->setStatusTip(tr("Check if a new version of the product is available"));
    connect(manualCheckUpdateAct, SIGNAL(triggered()), this, SLOT(manualCheckUpdate()));

	aboutAct = new QAction(QIcon(":/images/about.png"), tr("&About..."), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void QFormulatorMainWindow::createMenus()
{
	QString name = QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("File");
	QMenu *fileMenu = menuBar()->findChild<QMenu *>(name);
	if( fileMenu )
	{
		fileMenu->addSeparator();
		for( int i = 0; i < QFormulatorViewWindow::MaxRecentFiles; i++ )
			fileMenu->addAction(recentFileActs[i]);
    	fileMenu->addSeparator();
    	fileMenu->addAction(exitAct);
		updateRecentFileActions();
	}

	QMenu *editMenu = menuBar()->findChild<QMenu *>(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("Edit"));
	if( editMenu )
	{
		editMenu->addSeparator();
		editMenu->addAction(configDlgAct);
	}

	QMenu *viewMenu = menuBar()->findChild<QMenu *>(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("View"));
	if( viewMenu )
	{
    	viewMenu->addSeparator();
    	connect(viewMenu->addAction(tr("Switch layout direction")), SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    	viewMenu->addSeparator();
		viewMenu->addAction( m_mmlEditor->toggleViewAction() );
		viewMenu->addAction( m_mmlViewer->toggleViewAction() );
	}

	//toolsMenu = menuBar()->addMenu(tr("&Tools"));
	//if( m_mmlEditor )
	//	toolsMenu->addAction( m_mmlEditor->toggleViewAction() );
    //toolsMenu->addAction(toolsBuildToolbarsetAct);
	//toolsMenu->addSeparator();
	//toolsMenu->addAction(configDlgAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
	//helpMenu->addAction(toolsBuildToolbarsetAct);
	//helpMenu->addSeparator();
	helpMenu->addAction(frmWebDonateAct);
	helpMenu->addAction(frmWebAct);
	helpMenu->addAction(manualCheckUpdateAct);
	helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
}

void QFormulatorMainWindow::createToolBars()
{
}

void QFormulatorMainWindow::createStatusBar()
{
	m_inputStatusLabel = new QLabel();
	m_hiddenStatusLabel = new QLabel();

    QAction* presetWordAct = new QAction(QIcon(":/images/preset-word.png"), tr("&Word Processor"), this);
    presetWordAct->setStatusTip(tr("A set of options similar to Word Processor behavior"));
    connect(presetWordAct, SIGNAL(triggered()), this, SLOT(presetWord()));
    QAction* presetPmmlAct = new QAction(QIcon(":/images/preset-pmml.png"), tr("&Presentation MathML Editor"), this);
    presetPmmlAct->setStatusTip(tr("A set of options for editing Presentation MathML"));
    connect(presetPmmlAct, SIGNAL(triggered()), this, SLOT(presetPmml()));
	QAction* presetCmmlAct = new QAction(QIcon(":/images/preset-cmml2.png"), tr("&Content MathML Editor"), this);
    presetCmmlAct->setStatusTip(tr("A set of options for editing pure Content MathML"));
    connect(presetCmmlAct, SIGNAL(triggered()), this, SLOT(presetCmml()));
	QAction* presetCmmlPmmlAct = new QAction(QIcon(":/images/preset-cmml-p.png"), tr("&Content MathML Editor with Presentation markup fragments"), this);
    presetCmmlPmmlAct->setStatusTip(tr("A set of options for mixing Content MathML Editor with Presentation markup"));
    connect(presetCmmlPmmlAct, SIGNAL(triggered()), this, SLOT(presetCmmlPmml()));
	QAction* presetCmmlAutographAct = new QAction(QIcon(":/images/preset-cmml-acsii.png"), tr("&Content MathML Editor with ASCII-like typing rules"), this);
    presetCmmlAutographAct->setStatusTip(tr("A set of options for creating Content MathML with specific autodetecting rules"));
    connect(presetCmmlAutographAct, SIGNAL(triggered()), this, SLOT(presetCmmlA()));
	QMenu *presetMenu = new QMenu( this );
	presetMenu->addAction(presetWordAct);
	presetMenu->addAction(presetPmmlAct);
	presetMenu->addAction(presetCmmlAct);
	presetMenu->addAction(presetCmmlPmmlAct);
	presetMenu->addAction(presetCmmlAutographAct);

	m_presetBtn = new QToolButton();
	setPresetBtnIcon( m_lastAppliedPreset );
	m_presetBtn->setPopupMode( QToolButton::InstantPopup );
	m_presetBtn->setToolButtonStyle( Qt::ToolButtonIconOnly );
	m_presetBtn->setMenu( presetMenu );

	QStatusBar *status = statusBar();
	status->addPermanentWidget( m_presetBtn );
	status->addPermanentWidget( m_inputStatusLabel );
	status->addPermanentWidget( m_hiddenStatusLabel );
	updateStatusBar();
    status->showMessage(tr("Ready"), 2000);
}

void QFormulatorMainWindow::updateStatusBar()
{
	static const QString textBold("normal");
	static const QColor textColor(0, 0, 0);
	if( m_inputStatusLabel )
	{
		m_inputStatusLabel->setText(
			QString(tr("<p style=\"font-weight: %1; color: %2;\">&nbsp;%3 mode&nbsp;</p>"))
			.arg(textBold)
			.arg(textColor.name())
			.arg(tr(::isInputMode_Content() ? "Content" : "Presentation"))
			);
	}
	if( m_hiddenStatusLabel )
	{
		m_hiddenStatusLabel->setText(
			QString(tr("<p style=\"font-weight: %1; color: %2;\">&nbsp;%3 non-visual content&nbsp;</p>"))
			.arg(textBold)
			.arg(textColor.name())
			.arg(::isRenderMode_Force() ? tr("Show") : tr("Hide"))
			);
	}
	if( ::isOption_ShowNavInfo() )
	{
		QFormulatorEditWindow *child = activeEditSubWindow();
		statusBar()->showMessage(child ? child->caretInfo() : QString());
	}
	statusBar()->update();
}

///////////////////////////////////////////////////////////////////////////////

void QFormulatorMainWindow::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(closeAct);
	windowMenu->addAction(closeAllAct);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAct);
	windowMenu->addAction(cascadeAct);
	windowMenu->addSeparator();
	windowMenu->addAction(nextAct);
	windowMenu->addAction(previousAct);
	windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible( !windows.isEmpty() );

    for( int i = 0; i < windows.size(); i++ )
	{
        QFormulatorViewWindow *child = qobject_cast<QFormulatorViewWindow *>(windows.at(i)->widget());
		if( !child ) continue;
        QString text = (i < 9 ? QString("&%1 %2") : QString("%1 %2")).arg(i + 1).arg(child->userFriendlyCurrentFile());
        QAction *action  = windowMenu->addAction( text );
        action->setCheckable( true );
        action->setChecked( child == activeViewSubWindow() );
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping( action, windows.at(i) );
    }
}

void QFormulatorMainWindow::updateRecentFileActions()
{
#ifdef Q_WS_MAC
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_DOMAIN_, QCoreApplication::applicationName() );
#else
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_NAME__, QCoreApplication::applicationName() );
#endif
    QStringList files = settings.value("FmlIde/recentFileList").toStringList();
	int numRecentFiles = qMin(files.size(), (int) QFormulatorViewWindow::MaxRecentFiles);
    for (int i = 0; i < numRecentFiles; ++i)
	{
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
	for (int j = numRecentFiles; j < QFormulatorViewWindow::MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);
    separatorAct->setVisible(numRecentFiles > 0);
}

void QFormulatorMainWindow::updateOnActiveWindow(QMdiSubWindow *window)
{
	if( window && window->widget() )
	{
		window->widget()->setFocus();
		QFormulatorViewWindow *currentChild = activeViewSubWindow();
		if( currentChild )
		{
			QFormulatorViewWindow::updateZoomComboText( currentChild );
			if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
				updateMmlEditor( true );
		}
	}
	emit hasChildSignal(true);
	emit hasEditChildSignal( activeEditSubWindow() ? true : false );
	updateUI_OnClipboardChanged();
}

void QFormulatorMainWindow::updateUI_OnChildScaleChanged()
{
	QFormulatorViewWindow *currentChild = activeViewSubWindow();
	if( !currentChild || ((void*) currentChild->fmlWidget() != (void*) sender()) )
		return;
	QFormulatorViewWindow::updateZoomComboText( currentChild );
}

void QFormulatorMainWindow::updateUI_OnCloseChild()
{
	if( !activeViewSubWindow() )
	{
		emit hasChildSignal( false );
		emit hasEditChildSignal( false );
		emit hasClipboardForChildSignal( false );
		emit undoAvailableSignal( false );
		emit redoAvailableSignal( false );
		emit copyAvailableSignal( false );
	}
}

void QFormulatorMainWindow::updateUI_OnClipboardChanged()
{
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard ? clipboard->mimeData() : 0;
	emit hasClipboardForChildSignal( mimeData && activeEditSubWindow() && CFormulatorDoc::isAcceptedMimeFormat( mimeData ) );
}

void QFormulatorMainWindow::updateUI_OnChildDocModified( bool enable )
{
	if( m_mmlEditor && m_mmlEditor->isVisible() && enable && ::isOption_UpdateMathMLView() )
	{
		time_t now = time(0);
		if( now < m_mmlEditorUpdateTime || now - m_mmlEditorUpdateTime < MinPeriodMmlEditorUpdate )
		{
			if( !m_mmlEditor->delayedUpdate() )
			{
				time_t delta = MinPeriodMmlEditorUpdate - qMax(time_t(0), now - m_mmlEditorUpdateTime);
				QTimer::singleShot(delta * 1000, this, SLOT(updateMmlEditor()));
				m_mmlEditor->setDelayedUpdate( true );
			}
		}
		else
		{
			updateMmlEditor( true );
		}
	}
}

void QFormulatorMainWindow::updateMmlEditor()
{
	if( m_mmlEditor->delayedUpdate() )
		updateMmlEditor( true );
}

void QFormulatorMainWindow::updateMmlEditor( bool visibility )
{
	if( visibility && m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		QFormulatorViewWindow *currentChild = activeViewSubWindow();
		m_mmlEditor->setText( currentChild ? currentChild->text() : QString() );
		m_mmlEditorUpdateTime = time(0);
		QApplication::restoreOverrideCursor();
	}
}

void QFormulatorMainWindow::updateUI_OnWidgetUpdate()
{
	updateStatusBar();
}

void QFormulatorMainWindow::undoAvailableSlot( bool enable )
{
	QFormulatorViewWindow *currentChild = activeViewSubWindow();
	emit undoAvailableSignal( currentChild && enable );
}

void QFormulatorMainWindow::redoAvailableSlot( bool enable )
{
	QFormulatorViewWindow *currentChild = activeViewSubWindow();
	emit redoAvailableSignal( currentChild && enable );
}

void QFormulatorMainWindow::copyAvailableSlot( bool enable )
{
	QFormulatorViewWindow *currentChild = activeViewSubWindow();
	emit copyAvailableSignal( currentChild && enable );
}

/////////////////////////////////////////////////////////////////////////////

//QFormulatorViewWindow* QFormulatorMainWindow::createOrReuseSubViewWindow()
//{
//	QFormulatorViewWindow *currentChild = activeViewSubWindow();
//	if( currentChild &&
//		QString::compare(currentChild->metaObject()->className(), "QFormulatorViewWindow", Qt::CaseSensitive) == 0 )
//	{
//		setActiveSubWindow( currentChild );
//		return currentChild;
//	}
//	else
//	{
//		foreach( QMdiSubWindow *window, mdiArea->subWindowList() )
//		{
//			currentChild = qobject_cast<QFormulatorViewWindow *>(window->widget());
//			if( currentChild &&
//				QString::compare(currentChild->metaObject()->className(), "QFormulatorViewWindow", Qt::CaseSensitive) == 0 )
//			{
//				setActiveSubWindow( currentChild );
//				return currentChild;
//			}
//		}
//	}
//	return createSubViewWindow();
//}

QFormulatorViewWindow* QFormulatorMainWindow::reuseSubViewWindow()
{
	QFormulatorViewWindow *currentChild = activeViewSubWindow();
	if( currentChild &&
		QString::compare(currentChild->metaObject()->className(), "QFormulatorViewWindow", Qt::CaseSensitive) == 0 )
	{
		return currentChild;
	}
	else
	{
		foreach( QMdiSubWindow *window, mdiArea->subWindowList() )
		{
			currentChild = qobject_cast<QFormulatorViewWindow *>(window->widget());
			if( currentChild &&
				QString::compare(currentChild->metaObject()->className(), "QFormulatorViewWindow", Qt::CaseSensitive) == 0 )
			{
				return currentChild;
			}
		}
	}
	return 0;
}

QFormulatorViewWindow* QFormulatorMainWindow::createSubViewWindow()
{
    QFormulatorViewWindow *child = new QFormulatorViewWindow( this, this );
    /*QMdiSubWindow *activeSubW = */mdiArea->addSubWindow( child );
    return child;
}

QFormulatorViewWindow* QFormulatorMainWindow::createSubWindow()
{
    QFormulatorViewWindow *child = m_isEditMode ? new QFormulatorEditWindow( this, this ) : new QFormulatorViewWindow( this, this );
    /*QMdiSubWindow *activeSubW = */mdiArea->addSubWindow( child );
    return child;
}

QFormulatorViewWindow* QFormulatorMainWindow::activeViewSubWindow()
{
	QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow();
    return !activeSubWindow ? 0 : qobject_cast<QFormulatorViewWindow *>(activeSubWindow->widget());
}

QFormulatorEditWindow* QFormulatorMainWindow::activeEditSubWindow()
{
	QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow();
    return !activeSubWindow ? 0 : qobject_cast<QFormulatorEditWindow *>(activeSubWindow->widget());
}

QFormulatorWidget* QFormulatorMainWindow::activeViewWidget()
{
	QFormulatorViewWindow *child = activeViewSubWindow();
	return child ? child->fmlWidget() : 0;
}


QFormulatorEditWidget* QFormulatorMainWindow::activeEditWidget()
{
	QFormulatorEditWindow *child = activeEditSubWindow();
	return child ? child->fmlEditWidget() : 0;
}

QMdiSubWindow *QFormulatorMainWindow::findSubWindow( const QString &fileName )
{
    QString canonicalFilePath = QFileInfo( fileName ).canonicalFilePath();
    foreach( QMdiSubWindow *window, mdiArea->subWindowList() )
	{
        QFormulatorViewWindow *mdiChild = qobject_cast<QFormulatorViewWindow *>(window->widget());
        if( mdiChild && mdiChild->currentFile() == canonicalFilePath )
            return window;
    }
    return 0;
}

QMdiSubWindow *QFormulatorMainWindow::findSubWindow( QFormulatorViewWindow *child2find )
{
    foreach( QMdiSubWindow *window, mdiArea->subWindowList() )
	{
        QFormulatorViewWindow *mdiChild = qobject_cast<QFormulatorViewWindow *>(window->widget());
        if( mdiChild && mdiChild == child2find )
            return window;
    }
    return 0;
}

void QFormulatorMainWindow::setActiveSubWindow( QWidget *window )
{
    if( !window ) return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
	if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
		updateMmlEditor( true );
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorMainWindow::newFile()
{
    QFormulatorViewWindow *child = createSubWindow();
    child->newFile();
    child->showMaximized();
	emit hasChildSignal( true );
	emit hasEditChildSignal( activeEditSubWindow() ? true : false );
	updateUI_OnClipboardChanged();
	if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
		updateMmlEditor( true );
}

void QFormulatorMainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, __FMLCORE_Product_Title, m_currentPath, tr("MathML Files (*.mml);;XML Files (*.xml);;All Files (*.*)"));
	openFile(fileName);
}

void QFormulatorMainWindow::openFile(QString fileName)
{
    if (!fileName.isEmpty())
	{
        QMdiSubWindow *existing = findSubWindow(fileName);
        if( existing )
		{
			QFormulatorEditWindow *existingChild = qobject_cast<QFormulatorEditWindow *>(existing->widget());
			if( existingChild )
			{
				mdiArea->setActiveSubWindow(existing);
				return;
			}
        }
		QFormulatorViewWindow *child = createSubWindow();
		if( child == 0 ) return;
		if( child->loadFile( fileName ) )
		{
			m_currentPath = QFileInfo( child->currentFile() ).canonicalPath();
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->showMaximized();
			foreach( QWidget *widget, QApplication::topLevelWidgets() )
			{
				QFormulatorMainWindow *mainWin = qobject_cast<QFormulatorMainWindow *>(widget);
				if (mainWin) mainWin->updateRecentFileActions();
			}
			emit hasChildSignal( true );
			emit hasEditChildSignal( activeEditSubWindow() ? true : false );
			updateUI_OnClipboardChanged();
			if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
				updateMmlEditor( true );
		}
		else
		{
			child->close();
			mdiArea->closeActiveSubWindow();
		}
	}
}

void QFormulatorMainWindow::openByFileViewer(QString fileName, QWidget *focusBack)
{
	openFile(fileName);
	if( focusBack )
		focusBack->setFocus();
}

void QFormulatorMainWindow::loadByFileViewer(QString fileName, QWidget *focusBack)
{
	openFileForView(fileName);
	if( focusBack )
		focusBack->setFocus();
}

//void QFormulatorMainWindow::openFileForView(QString fileName)
//{
//    if (!fileName.isEmpty())
//	{
//		QFormulatorViewWindow *oldChild = reuseSubViewWindow();
//		QFormulatorViewWindow *newChild = createSubViewWindow();
//		if( newChild == 0 ) return;
//		if( newChild->loadFileSilent(fileName) )
//		{
//			if( oldChild )
//			{
//				//setActiveSubWindow( oldChild );
//				oldChild->close();
//				//mdiArea->closeActiveSubWindow();
//			}
//			m_currentPath = QFileInfo( fileName ).canonicalPath();
//			statusBar()->showMessage(tr("File loaded"), 2000);
//
//			setActiveSubWindow( newChild );
//			newChild->showMaximized();
//			foreach( QWidget *widget, QApplication::topLevelWidgets() )
//			{
//				QFormulatorMainWindow *mainWin = qobject_cast<QFormulatorMainWindow *>(widget);
//				if (mainWin) mainWin->updateRecentFileActions();
//			}
//
//			emit hasChildSignal( true );
//			emit hasEditChildSignal( activeEditSubWindow() ? true : false );
//			updateUI_OnClipboardChanged();
//			if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
//				updateMmlEditor( true );
//		}
//		else
//		{
//			newChild->close();
//			//mdiArea->closeActiveSubWindow();
//		}
//	}
//}

void QFormulatorMainWindow::openFileForView(QString fileName)
{
    if( !fileName.isEmpty() )
	{
		bool isNew = false;
		QFormulatorViewWindow *child = reuseSubViewWindow();
		if( child == 0 )
		{
			child = createSubViewWindow();
			isNew = true;
		}
		if( child == 0 ) return;
		if( child->reloadFileSilent( fileName ) )
		{
			if( !isNew )
			{
				QMdiSubWindow *existing = findSubWindow( child );
				if( existing ) mdiArea->setActiveSubWindow( existing );
			}
			m_currentPath = QFileInfo( fileName ).canonicalPath();
			statusBar()->showMessage(tr("File loaded"), 2000);
			child->show();
			foreach( QWidget *widget, QApplication::topLevelWidgets() )
			{
				QFormulatorMainWindow *mainWin = qobject_cast<QFormulatorMainWindow *>(widget);
				if (mainWin) mainWin->updateRecentFileActions();
			}

			emit hasChildSignal( true );
			emit hasEditChildSignal( activeEditSubWindow() ? true : false );
			updateUI_OnClipboardChanged();
			if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
				updateMmlEditor( true );
		}
		else if( isNew )
		{
			QMdiSubWindow *existing = findSubWindow( child );
			child->close();
			if( existing )
			{
				mdiArea->removeSubWindow( existing );
				delete existing;
			}
		}
	}
}

void QFormulatorMainWindow::uploadMathML()
{
	QFormulatorViewWindow *child = activeViewSubWindow();
	if( child && m_mmlEditor )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		child->upload( m_mmlEditor->text() );
		QApplication::restoreOverrideCursor();
	}
}

void QFormulatorMainWindow::loadMathML()
{
	QFormulatorViewWindow *currentChild = activeViewSubWindow();
	if( m_mmlEditor )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		m_mmlEditor->setText( currentChild ? currentChild->text() : QString() );
		m_mmlEditorUpdateTime = time(0);
		QApplication::restoreOverrideCursor();
	}
}

void QFormulatorMainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
	{
		QString fileName = action->data().toString();
		if (!fileName.isEmpty())
		{
			QMdiSubWindow *existing = findSubWindow(fileName);
			if (existing)
			{
				mdiArea->setActiveSubWindow(existing);
				return;
			}

			QFormulatorViewWindow *child = createSubWindow();
			if( child && child->loadFile( fileName ) )
			{
				statusBar()->showMessage(tr("File loaded"), 2000);
				child->showMaximized();
				emit hasChildSignal( true );
				emit hasEditChildSignal( activeEditSubWindow() ? true : false );
				updateUI_OnClipboardChanged();
				if( m_mmlEditor && m_mmlEditor->isVisible() && ::isOption_UpdateMathMLView() )
					updateMmlEditor( true );
			}
			else
			{
#ifdef Q_WS_MAC
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_DOMAIN_, QCoreApplication::applicationName() );
#else
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_NAME__, QCoreApplication::applicationName() );
#endif
				QStringList files = settings.value("FmlIde/recentFileList").toStringList();
				files.removeAll(fileName);
				settings.setValue("FmlIde/recentFileList", files);
				child->close();
				mdiArea->closeActiveSubWindow();
			}
			foreach (QWidget *widget, QApplication::topLevelWidgets())
			{
				QFormulatorMainWindow *mainWin = qobject_cast<QFormulatorMainWindow *>(widget);
				if( mainWin ) mainWin->updateRecentFileActions();
			}
		}
	}
}

void QFormulatorMainWindow::save()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child && child->save(m_currentPath) )
	{
		m_currentPath = QFileInfo( child->currentFile() ).canonicalPath();
        statusBar()->showMessage(tr("File saved"), 2000);
		foreach( QWidget *widget, QApplication::topLevelWidgets() )
		{
			QFormulatorMainWindow *mainWin = qobject_cast<QFormulatorMainWindow *>(widget);
			if( mainWin ) mainWin->updateRecentFileActions();
		}
	}
}

void QFormulatorMainWindow::saveAs()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child && child->saveAs(m_currentPath) )
	{
		m_currentPath = QFileInfo( child->currentFile() ).canonicalPath();
		statusBar()->showMessage(tr("File saved"), 2000);
	}
}

void QFormulatorMainWindow::exportPdf()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
	QString strFilter = "PDF Files (*.pdf);;All Files (*.*)";
	QString dest = QFileDialog::getSaveFileName( this, "Save Document as PDF", "./", strFilter );
	if( dest.isNull() ) return;
    if( child ) child->exportPdf( dest );
}

//void QFormulatorMainWindow::exportEmf()
//{
//    QFormulatorViewWindow *child = activeViewSubWindow();
//    if( child ) child->exportEmf();
//}

void QFormulatorMainWindow::exportSvg()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
	QString strFilter = "SVG Files (*.svg);;All Files (*.*)";
	QString dest = QFileDialog::getSaveFileName( this, "Save Document as SVG", "./", strFilter );
	if( dest.isNull() ) return;
    if( child ) child->exportSvg( dest );
}

void QFormulatorMainWindow::exportImage()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
	QString strFilter = "Portable Network Graphics Format (*.png);;JPEG File Interchange Format (*.jpg *.jpeg);;"
						"Windows Bitmap Format (*.bmp);;X11 Bitmap (*.xbm);;"
						"Tagged Image File Format (*.tiff);;Portable Pixmap (*.ppm);;"
						"Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.ppm *.tiff *.xbm *.xpm);;"
						"All Files (*.*)";
	QString dest = QFileDialog::getSaveFileName( this, "Save Document as Image", "./", strFilter);
	if( dest.isNull() ) return;
    if( child ) child->exportImage( dest );
}

void QFormulatorMainWindow::exportWeb()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
	QString strFilter = "Web Pages (*.html *.htm);; MathML Text (*.mml *.mathml);;"
						"All Files (*.*)";
	QString dest = QFileDialog::getSaveFileName( this, "Save Document as SVG", "./", strFilter );
	if( dest.isNull() ) return;
    if( child ) child->exportWeb( dest );
}

void QFormulatorMainWindow::print()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->print();
}

void QFormulatorMainWindow::printPreview()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->printPreview();
}

void QFormulatorMainWindow::pageSetup()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->pageSetup();
}

void QFormulatorMainWindow::undo()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->undo();
}

void QFormulatorMainWindow::redo()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->redo();
}

void QFormulatorMainWindow::cut()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->cut();
}

void QFormulatorMainWindow::copy()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->copy();
}

void QFormulatorMainWindow::copyAll()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->copyAll();
}

void QFormulatorMainWindow::paste()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->paste();
}

//void QFormulatorMainWindow::delete()
//{
//	QFormulatorEditWindow *child = activeEditSubWindow();
//	if( child ) child->delete();
//}

void QFormulatorMainWindow::selectAll()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->selectAll();
}

void QFormulatorMainWindow::properties()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->properties();
}

void QFormulatorMainWindow::evaluate()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->evaluate();
}

void QFormulatorMainWindow::zoomValue( int idx )
{
    QFormulatorViewWindow *child = activeViewSubWindow();
	if( !child ) return;

    QComboBox *action = qobject_cast<QComboBox *>(sender());
    if( !action ) return;

    double value = double(action->itemText( idx ).toUInt()) / 100.0;
	if( value > DEFAULT_VIEWSCALE_MAX ) value = DEFAULT_VIEWSCALE_MAX;
	else if( value < DEFAULT_VIEWSCALE_MIN ) value = DEFAULT_VIEWSCALE_MIN;

	if( child->fmlWidget()->scale() != value )
	{
		child->setFocus();
		child->fmlWidget()->setScale( value );
	}
}

void QFormulatorMainWindow::setStyleByCombo( int idx )
{
	QFormulatorEditWindow *child = activeEditSubWindow();
	if( !child ) return;

	QComboBox *action = qobject_cast<QComboBox *>(sender());
	if( !action ) return;

	int styleNo = action->itemData( idx ).toInt();
	if( ::isOtherStyle( styleNo ) )
	{
		child->setStyleOther();
	}
	else
	{
		child->setStyle( styleNo );
		child->setFocus();
	}
}

void QFormulatorMainWindow::setSizeByCombo( int idx )
{
	QFormulatorEditWindow *child = activeEditSubWindow();
	if( !child ) return;

	QComboBox *action = qobject_cast<QComboBox *>(sender());
	if( !action ) return;

	int styleNo = action->itemData( idx ).toInt();
	if( ::isOtherSizeIdx( styleNo ) )
		setSizeOther();
	else
	{
		child->setSize( styleNo );
		child->setFocus();
	}
}

void QFormulatorMainWindow::zoomIn()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomIn();
}

void QFormulatorMainWindow::zoomOut()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomOut();
}

void QFormulatorMainWindow::zoom100()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomValueQuick( 1.0 );
}

void QFormulatorMainWindow::zoom200()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomValueQuick( 2.0 );
}

void QFormulatorMainWindow::zoom300()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomValueQuick( 3.0 );
}

void QFormulatorMainWindow::zoom500()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomValueQuick( 5.0 );
}

void QFormulatorMainWindow::zoom1000()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->zoomValueQuick( 10.0 );
}

void QFormulatorMainWindow::showNesting( bool state )
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->showNesting( state );
}

void QFormulatorMainWindow::showReadonly( bool state )
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->showReadonly( state );
}

void QFormulatorMainWindow::viewMathMLText( bool state )
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child ) child->viewMathMLText( state );
}

void QFormulatorMainWindow::switchLayoutDirection()
{
    if( layoutDirection() == Qt::LeftToRight )
        qApp->setLayoutDirection( Qt::RightToLeft );
    else
        qApp->setLayoutDirection( Qt::LeftToRight );
}

void QFormulatorMainWindow::insertSymbol()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->insertSymbol();
}

void QFormulatorMainWindow::insertButton()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
	if( !child ) return;
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
	{
		QString buttonID = action->data().toString();
		if (!buttonID.isEmpty())
			child->insertButton( buttonID );
	}
}

void QFormulatorMainWindow::insertAssign()
{
	QFormulatorEditWindow *child = activeEditSubWindow();
	if( child )
		child->insertButton( HF_BTNID_CMASSIGN2_ID );
}

void QFormulatorMainWindow::showSenderStatusText()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
	if( !child ) return;
    QAction *action = qobject_cast<QAction *>(sender());
    if( action )
	{
		action->showStatusText();
		QToolTip::showText( QCursor::pos(), action->toolTip() );
	}
}

void QFormulatorMainWindow::showSenderStatusText(QAction *action)
{
    QFormulatorEditWindow *child = activeEditSubWindow();
	if( !child ) return;
    if( action )
	{
		action->showStatusText();
		QToolTip::showText( QCursor::pos(), action->toolTip() );
	}
}

void QFormulatorMainWindow::insertButtonViaToolbar()
{
	QFormulatorEditWindow *child = activeEditSubWindow();
	if( !child ) return;
    QAbstractButton *action = qobject_cast<QAbstractButton *>(sender());
    if(action)
	{
		QString buttonID = action->text();
		if (!buttonID.isEmpty())
			child->insertButton( buttonID );
	}
}

void QFormulatorMainWindow::insertImage()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->insertImage();
}

void QFormulatorMainWindow::setStyleSelected(QAction *action)
{
	if( !action )
		action = qobject_cast<QAction *>(sender());
	if( action /*&& action->isChecked() */)
	{
		QFormulatorEditWindow *child = activeEditSubWindow();
		if( child )
		{
			int no = action->data().toInt();
			if( ::isOtherStyle( no ) )
				child->setStyleOther();
			else
				child->setStyle( no );
		}
	}
}

void QFormulatorMainWindow::setSizeLargeSymbol()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeLargeSymbol();
}

void QFormulatorMainWindow::setSizeRegular()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeRegular();
}

void QFormulatorMainWindow::setSizeSubscript()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeSubscript();
}

void QFormulatorMainWindow::setSizeSubSubscript()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeSubSubscript();
}

void QFormulatorMainWindow::setSizeOther()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeOther();
}

void QFormulatorMainWindow::setSizeSmaller()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeSmaller();
}

void QFormulatorMainWindow::setSizeLarger()
{
    QFormulatorEditWindow *child = activeEditSubWindow();
    if( child ) child->setSizeLarger();
}

void QFormulatorMainWindow::formatDefineStyle()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child && child->formatDefineStyle() )
	{
		foreach( QMdiSubWindow *window, mdiArea->subWindowList() )
		{
			QFormulatorViewWindow *mdiChild = window ? (qobject_cast<QFormulatorViewWindow *>(window->widget())) : 0;
			if( mdiChild && mdiChild->fmlWidget() )
				mdiChild->fmlWidget()->recalculateSize();
		}
	}
}

void QFormulatorMainWindow::formatDefineSize()
{
    QFormulatorViewWindow *child = activeViewSubWindow();
    if( child && child->formatDefineSize() )
	{
		foreach( QMdiSubWindow *window, mdiArea->subWindowList() )
		{
			QFormulatorViewWindow *mdiChild = window ? (qobject_cast<QFormulatorViewWindow *>(window->widget())) : 0;
			if( mdiChild && mdiChild->fmlWidget() )
				mdiChild->fmlWidget()->recalculateSize();
		}
	}
}

void QFormulatorMainWindow::buildToolbarset()
{
    CBuildToolbarsetDlg dlg;
    if( dlg.exec() == QDialog::Accepted )
		m_isEditMode ? QFormulatorEditWindow::initFormulatorCore() : QFormulatorViewWindow::initFormulatorCore();
}

void QFormulatorMainWindow::setLineSpacing( RealFde value )
{
	m_ls = value;
	QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for( int i = 0; i < windows.size(); i++ )
	{
        QFormulatorViewWindow *child = qobject_cast<QFormulatorViewWindow *>(windows.at(i)->widget());
		if( child ) child->fmlWidget()->setLineSpacing( value );
	}
}

RealFde QFormulatorMainWindow::lineSpacing()
{
	return m_ls;
}

void QFormulatorMainWindow::setIndents( RectFde rect )
{
	m_indents = rect;
	QList<QMdiSubWindow*> windows = mdiArea->subWindowList();
    for( int i = 0; i < windows.size(); i++ )
	{
        QFormulatorViewWindow *child = qobject_cast<QFormulatorViewWindow *>(windows.at(i)->widget());
		if( child ) child->fmlWidget()->setIndents( rect );
	}
}

void QFormulatorMainWindow::indents( RectFde& rect )
{
	rect = m_indents;
}

void QFormulatorMainWindow::closeEvent( QCloseEvent *event )
{
    mdiArea->closeAllSubWindows();
    if( activeViewSubWindow() )
        event->ignore();
    else
	{
        writeSettings();
        event->accept();
    }
}

void QFormulatorMainWindow::about()
{
	QPixmap aboutPix(":/images/fmlideico.ico");
	QLabel *logoLabel = new QLabel();
	logoLabel->setPixmap( aboutPix );
	QVBoxLayout *logoLayout = new QVBoxLayout();
	logoLayout->addWidget( logoLabel, 0, Qt::AlignRight );
	logoLayout->addStretch();

	QColor foreColor(0, 0, 0);
#ifdef Q_WS_MAC
	QFont currentFont("Verdana", 12);
#else
	QFont currentFont("Verdana", 8);
#endif
	QString version = QString( "<p style=\"color: %1; font-size: %2pt; font-family: %3; \">"
		"<b>%4 Info:</b><br/><br/>Version %5<br/>(c) 2003-2009 Hermitech Labs<br/>All rights reserved."
		"<table><tr><td>E-mail:</td><td><a href=\"mailto:info@mmlsoft.com\">info@mmlsoft.com</a></td></tr>"
		"<tr><td>Web:</td><td><a href=\"http://www.mmlsoft.com\">http://www.mmlsoft.com</a></td></tr><table></p>"
		"<p style=\"color: %6; font-size: %7pt; font-family: %8; \">"
		"This software uses the following libraries with thanks:<br/>- Qt (LGPL, http://qtsoftware.com),"
		"<br/>- Nuvola icon theme for kde 3.x (LGPL, http://www.icon-king.com) <br/>and Crystal Project Icons (LGPL, http://www.everaldo.com)</p>" )		
						.arg(foreColor.name())
						.arg(currentFont.pointSize())
						.arg(currentFont.family())
						.arg(__FMLCORE_Product_Title)
						.arg(__FMLCORE_ProductVersion)
						.arg(foreColor.name())
						.arg(currentFont.pointSize())
						.arg(currentFont.family());
	QLabel *swLabel = new QLabel();
	swLabel->setText( version );
	//swLabel->setFrameShape( QFrame::Box );
	QHBoxLayout *nwLayout = new QHBoxLayout();
	nwLayout->addWidget( swLabel, 1 );
	nwLayout->addSpacing( 32 );
	nwLayout->addLayout( logoLayout );

	QFile licenseFile(":/dox/license.html");
	if( !licenseFile.open(QFile::ReadOnly | QFile::Text) ) return;
	QTextStream in(&licenseFile);
	QString licenseText(in.readAll());
	licenseFile.close();
	QString licenseBody = QString( "<body style=\"background-color: white; color: %2; font-size: %3pt; "
		"font-family: %4; font-weight: %5; font-style: %6;\">%7</body>" )
						.arg(foreColor.name())
						.arg(currentFont.pointSize())
						.arg(currentFont.family())
						.arg(currentFont.bold() ? "bold" : "normal")
						.arg(currentFont.italic() ? "italic" : "normal")
						.arg(licenseText);

	QVBoxLayout *neLayout = new QVBoxLayout();
	QTextEdit *lcText = new QTextEdit();
	lcText->setHtml(licenseBody);
	lcText->setReadOnly( true );
	lcText->setMinimumWidth( 400 );
	lcText->setMaximumWidth( 700 );
	//lcText->setMinimumHeight( 200 );
	neLayout->addLayout( nwLayout );
	neLayout->addSpacing( 16 );
	neLayout->addWidget( lcText );
	neLayout->addSpacing( 16 );

	QDialog dlg( this );
	QPushButton *closeBtn = new QPushButton(tr("Close"));
	connect(closeBtn, SIGNAL(clicked()), &dlg, SLOT(reject()));
	neLayout->addWidget( closeBtn, 0, Qt::AlignHCenter );

	dlg.setLayout( neLayout );
	dlg.setWindowTitle(QString(tr("About %1")).arg(__FMLCORE_Product_Title));
	dlg.exec();
}

void QFormulatorMainWindow::automaticCheckUpdate()
{
	if( !::isOption_Check4Updates() ) return;
	time_t lastTime = ::Formulib_GetCheck4UpdatesTime(), nowTime = time(0);
	if( double(nowTime - lastTime) / 86400.0 < ::Formulib_GetCheck4UpdatesPeriod() ) return;
	checkUpdate();
}

void QFormulatorMainWindow::manualCheckUpdate()
{
	checkUpdate();
	QMessageBox::information( this, __FMLCORE_Product_Title,
		QString(tr("<p>Thank you.</p><p>Your request is sent to http://www.mmlsoft.com. </p>")), QMessageBox::Ok );
}

void QFormulatorMainWindow::checkUpdate()
{
	::Formulib_SetCheck4UpdatesTime( time(0) );
	if( m_updater ) delete m_updater;
	m_updater = new CUpdateRequest( this );
}

void QFormulatorMainWindow::updateOk()
{
	if( !m_updater ) return;

	QStringList currentVersion = ::Formulib_GetCheck4UpdatesBuild().split(".");
	QStringList newVersion = m_updater->text().split(".");
	bool newVersionAvailable = false;
	unsigned int curNo, newNo;
	for( int i = 0; i < newVersion.size() && i < currentVersion.size(); i++ )
	{
		curNo = currentVersion.at(i).toUInt();
		newNo = newVersion.at(i).toUInt();
		if( curNo > newNo )
			break;
		else if( curNo < newNo )
		{
			newVersionAvailable = true;
			break;
		}
	}

	if( newVersionAvailable )
	{
		QMessageBox::information( this, __FMLCORE_Product_Title,
			QString(tr("<p>Update to a new version of %1 is available.<br/><br/>"
			"Please visit our download page "
			"<a href=\"http://www.mmlsoft.com/hdata/fml_downloads.php\">http://www.mmlsoft.com/hdata/fml_downloads.php</a> "
			"to get a new version.</p>")).arg( __FMLCORE_Product_Title ), QMessageBox::Ok );
		statusBar()->showMessage(tr("Updates are available..."), 2000);
	}
	else
		statusBar()->showMessage(tr("http://www.mmlsoft.com has responded: There are no updates available..."), 5000);

	delete m_updater;
	m_updater = 0;
}

void QFormulatorMainWindow::mmlsoftWeb()
{
	QDesktopServices::openUrl(QUrl(__COMPANY_URL_));
}

void QFormulatorMainWindow::mmlsoftDonateWeb()
{
	QDesktopServices::openUrl(QUrl(__COMPANY_URL_FORMULATOR_HOME__));
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorMainWindow::configDlg()
{
	long nRenderMode_Force = ::isRenderMode_Force();
	long nTranslateTimes = ::getCurrentFormulatorContentMathML_TranslateTimes();
	CFormulatorConfigDlg dlg( m_configLastPage, this );
	if( dlg.exec() == QDialog::Accepted )
	{
		// last used options page
		m_configLastPage = dlg.activePage();
		// last used preset
		if( dlg.getPreset() != FORMULIB_PRESET_NONE )
		{
			m_lastAppliedPreset = dlg.getPreset();
			setPresetBtnIcon( m_lastAppliedPreset );
		}
		updateUI_OnOptionsChanged();

		// should we refresh the document?
		if( ::isOption_ShowWarning_RefreshMathML() )
		{
			if( (nRenderMode_Force != ::isRenderMode_Force()) ||
				(nTranslateTimes != ::getCurrentFormulatorContentMathML_TranslateTimes()) )
			{
				QMessageBox::StandardButton ret = QMessageBox::question( 0, __FMLCORE_Product_Title,
					QObject::tr("MathML rendering settings are changed. Do you want to reload current document with new rendering settings?"),
					QMessageBox::Yes | QMessageBox::No );
				if( ret == QMessageBox::Yes )
				{
					QFormulatorViewWindow *child = activeViewSubWindow();
					if( child ) child->refreshViaMathML();
				}
			}
		}

		RectFde rcIndentsNew;
		::Formulib_GetIndents( rcIndentsNew );
		RectFde rcIndentsCurrent;
		indents( rcIndentsCurrent );
		if( rcIndentsNew != rcIndentsCurrent )
			setIndents( rcIndentsNew );

		RealFde lsNew = ::Formulib_GetLineSpacing();
		if( lineSpacing() != lsNew )
			setLineSpacing( lsNew );
	}
}

void QFormulatorMainWindow::presetWord()
{
	::presetWordFormulatorSettings();
	m_lastAppliedPreset = FORMULIB_PRESET_WORD;
	setPresetBtnIcon( m_lastAppliedPreset );
	updateUI_OnOptionsChanged();
}

void QFormulatorMainWindow::presetPmml()
{
	::presetPmmlFormulatorSettings();
	m_lastAppliedPreset = FORMULIB_PRESET_PMML;
	setPresetBtnIcon( m_lastAppliedPreset );
	updateUI_OnOptionsChanged();
}

void QFormulatorMainWindow::presetCmml()
{
	::presetCmmlFormulatorSettings();
	m_lastAppliedPreset = FORMULIB_PRESET_CMML;
	setPresetBtnIcon( m_lastAppliedPreset );
	updateUI_OnOptionsChanged();
}

void QFormulatorMainWindow::presetCmmlPmml()
{
	::presetCmmlPmmlFormulatorSettings();
	m_lastAppliedPreset = FORMULIB_PRESET_CMML_P;
	setPresetBtnIcon( m_lastAppliedPreset );
	updateUI_OnOptionsChanged();
}

void QFormulatorMainWindow::presetCmmlA()
{
	::presetCmmlAutographFormulatorSettings();
	m_lastAppliedPreset = FORMULIB_PRESET_CMML_ASCII;
	setPresetBtnIcon( m_lastAppliedPreset );
	updateUI_OnOptionsChanged();
}

void QFormulatorMainWindow::setPresetBtnIcon( int presetNo )
{
	if( !m_presetBtn ) return;
	QString toolTip, iconPath;
	QSize iconSize( 24, 24 );
	switch( presetNo )
	{
	case FORMULIB_PRESET_WORD:
		toolTip = tr("Current preset: Word Processor");
		iconPath = ":/images/preset-word.png";
		break;
	case FORMULIB_PRESET_PMML:
		toolTip = tr("Current preset: Presentation MathML");
		iconPath = ":/images/preset-pmml.png";
		break;
	case FORMULIB_PRESET_CMML:
		toolTip = tr("Current preset: Content MathML");
		iconPath = ":/images/preset-cmml2.png";
		break;
	case FORMULIB_PRESET_CMML_P:
		toolTip = tr("Current preset: Content MathML with Presentation markup");
		iconPath = ":/images/preset-cmml-p.png";
		break;
	case FORMULIB_PRESET_CMML_ASCII:
		toolTip = tr("Current preset: Content MathML with ASCII-like typing");
		iconPath = ":/images/preset-cmml-acsii.png";
		break;
	case FORMULIB_PRESET_USER:
	default:
		toolTip = tr("Current options preset: User Defined");
		iconPath = ":/images/users.png";
		break;
	}
	m_presetBtn->setToolTip( toolTip );
	m_presetBtn->setIcon( QIcon(QPixmap(iconPath).scaled(iconSize)) );
}

void QFormulatorMainWindow::updateUI_OnOptionsChanged()
{
	updateStatusBar();
	QFormulatorWidget *child = activeViewWidget();
	if( child ) child->setMmlChanged( true );
	updateMmlEditor( true );
}

/////////////////////////////////////////////////////////////////////////////
