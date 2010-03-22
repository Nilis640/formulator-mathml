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

#include "HUtils/idrawing.h"
#include "HMathML/isettings.h"
#include "HMathML/itoolbarset.h"
#include "HMathML/idictionary.h"

#include "fmlviewwindow.h"
#include "../widget/fmlwidget.h"
#include "../../version/version.h"

//////////////////////////////////////////////////////////////////////////////

QFormulatorViewWindow::QFormulatorViewWindow( QFormulatorWidget *fml, QWidget *parent, QObject *uiUpdater )
	: QScrollArea( parent )
{
	if( fml )
	{
		init( fml, uiUpdater );
	}
}

QFormulatorViewWindow::QFormulatorViewWindow( QWidget *parent, QObject *uiUpdater )
	: QScrollArea( parent )
{
	init( new QFormulatorWidget( this, uiUpdater ), uiUpdater );
}

void QFormulatorViewWindow::init( QFormulatorWidget *fml, QObject* /*uiUpdater*/ )
{
	setCursor(Qt::IBeamCursor);
	if( horizontalScrollBar() ) horizontalScrollBar()->setCursor(Qt::ArrowCursor);
	if( verticalScrollBar() ) verticalScrollBar()->setCursor(Qt::ArrowCursor);

	setAttribute(Qt::WA_DeleteOnClose);
	setBackgroundRole(QPalette::Light);
	setAlignment(Qt::AlignLeft);

	m_fmlWidget = fml;
	setWidget( m_fmlWidget );
	m_fmlWidget->initialUpdate();
	m_fmlWidget->setVerbose( true );

    m_isUntitled = true;
}

QFormulatorViewWindow::~QFormulatorViewWindow()
{
	if( m_fmlWidget ) delete m_fmlWidget;
}

//void QFormulatorViewWindow::setBackColor( QColor color )
//{
//	QFormulatorWidget::setBackColor();
//	setBackgroundRole(QPalette::Light);
//}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::newFile()
{
    static int sequenceNumber = 1;
    m_isUntitled = true;
    m_curFile = tr("document%1.mml").arg(sequenceNumber++);
    setWindowTitle( m_curFile + "[*]" );
	fmlWidget()->fileNew();
}

bool QFormulatorViewWindow::loadFile(const QString &fileName)
{
	bool bRet = true;
    QApplication::setOverrideCursor(Qt::WaitCursor);
	if( fmlWidget()->fileOpen( fileName ) )
	{
		setCurrentFile( fileName );
		QApplication::restoreOverrideCursor();
	}
	else if( fmlWidget()->isVerbose() )
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(this, __FMLCORE_Product_Title, tr("Cannot read file %1").arg(fileName));
		bRet = false;
	}
    return bRet;
}

bool QFormulatorViewWindow::loadFileSilent(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
	int iRet = fmlWidget()->fileOpenSilent( fileName );
	if( iRet == 1 )
		setCurrentFileName( fileName );
	QApplication::restoreOverrideCursor();
	return iRet == 1 ? true : false;
}

bool QFormulatorViewWindow::reloadFileSilent(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
	int iRet = fmlWidget()->fileReloadSilent( fileName );
	if( iRet == 1 )
		setCurrentFileName( fileName );
	QApplication::restoreOverrideCursor();
	return iRet == 1 ? true : false;
}

bool QFormulatorViewWindow::save( QString currentPath )
{
	return m_isUntitled ? saveAs( currentPath ) : saveFile( m_curFile );
}

bool QFormulatorViewWindow::saveAs( QString currentPath )
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), m_isUntitled ? currentPath : m_curFile, "MathML Files (*.mml);;XML Files (*.xml);;All Files (*.*)");
    return fileName.isEmpty() ? false : saveFile( fileName );
}

bool QFormulatorViewWindow::saveFile( const QString &fileName )
{
	bool bRet = true;
    QApplication::setOverrideCursor(Qt::WaitCursor);
	if( fmlWidget()->fileSave( fileName ) )
		setCurrentFile( fileName );
	else if( fmlWidget()->isVerbose() )
	{
		QMessageBox::warning(this, __FMLCORE_Product_Title, tr("Cannot write file %1").arg(fileName));
		bRet = false;
	}
    QApplication::restoreOverrideCursor();
    return bRet;
}

void QFormulatorViewWindow::closeEvent(QCloseEvent *event)
{
	fmlWidget()->fileClose();
	if( maybeSave() )
        event->accept();
	else
        event->ignore();
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::exportPdf(const QString &fileName)
{
	fmlWidget()->fileExportPdf( fileName );
}

//void QFormulatorViewWindow::exportEmf(const QString &fileName)
//{
//}

void QFormulatorViewWindow::exportSvg(const QString &fileName)
{
	fmlWidget()->fileExportSvg( fileName, windowTitle() );
}

void QFormulatorViewWindow::exportImage(const QString &fileName)
{
	fmlWidget()->fileExportImage( fileName );
}

void QFormulatorViewWindow::exportWeb(const QString &fileName)
{
	fmlWidget()->fileExportWeb( fileName, windowTitle() );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::print()
{
	fmlWidget()->filePrint();
}

void QFormulatorViewWindow::printPreview()
{
	fmlWidget()->filePrintPreview();
}

void QFormulatorViewWindow::pageSetup()
{
	fmlWidget()->filePageSetup();
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::copyAll()
{
	fmlWidget()->copyAll();
}

void QFormulatorViewWindow::upload( const QString& newText )
{
	fmlWidget()->upload( newText );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::zoomIn()
{
	fmlWidget()->setScale( qMin( fmlWidget()->scale() + DEFAULT_VIEWSCALE_STEP, DEFAULT_VIEWSCALE_MAX ) );
}

void QFormulatorViewWindow::zoomOut()
{
	fmlWidget()->setScale( qMax( fmlWidget()->scale() - DEFAULT_VIEWSCALE_STEP, DEFAULT_VIEWSCALE_MIN ) );
}

void QFormulatorViewWindow::zoomValue( double value )
{
	fmlWidget()->setScale( qMax( qMin( value, DEFAULT_VIEWSCALE_MAX ), DEFAULT_VIEWSCALE_MIN ) );
}

void QFormulatorViewWindow::zoomValueQuick( double value )
{
	fmlWidget()->setScale( value );
}

void QFormulatorViewWindow::showNesting( bool state )
{
	fmlWidget()->viewShowNesting( state );
}

void QFormulatorViewWindow::showReadonly( bool state )
{
	fmlWidget()->viewShowReadonly( state );
}

void QFormulatorViewWindow::refreshViaMathML()
{
	fmlWidget()->refreshViaMathML();
}

void QFormulatorViewWindow::viewMathMLText( bool /*state*/ )
{
}

void QFormulatorViewWindow::properties()
{
}

//////////////////////////////////////////////////////////////////////////////

QString QFormulatorViewWindow::m_styleTitle[ styleNumber ] = 
{
	tr("Auto Math"), tr("Text"), tr("Function"), tr("Variable"),
	tr("Number"), tr("Greek"), tr("Vector"), tr("Fixed"),
	tr("Operator"), tr("Extra Math"), tr("User 1"), tr("User 2"), tr("Other...")
};

int QFormulatorViewWindow::m_styleID[ styleNumber ] = 
{
	FSTYLE_NONE, FSTYLE_TEXT, FSTYLE_FUNCTION, FSTYLE_VARIABLE,
	FSTYLE_NUMBER, FSTYLE_GREEK, FSTYLE_VECTOR, FSTYLE_FIXED,
	FSTYLE_OP, FSTYLE_MTEXTRA, FSTYLE_USER1, FSTYLE_USER2, FSTYLE_DEFAULT_FONT_NUMBER
};

bool QFormulatorViewWindow::formatDefineStyle()
{
	return fmlWidget()->formatDefineStyle( styleNumber, m_styleTitle, m_styleID ) ? true : false;
}

bool QFormulatorViewWindow::formatDefineSize()
{
	return fmlWidget()->formatDefineSize() ? true : false;
}

//////////////////////////////////////////////////////////////////////////////

int QFormulatorViewWindow::setText( QString mmlText )
{
	if( m_fmlWidget )
		return m_fmlWidget->setText( mmlText );
	return 0;
}

QString QFormulatorViewWindow::text()
{
	return m_fmlWidget ? m_fmlWidget->text() : QString();
}

//////////////////////////////////////////////////////////////////////////////

QString QFormulatorViewWindow::userFriendlyCurrentFile()
{
    return strippedName( m_curFile );
}

bool QFormulatorViewWindow::maybeSave()
{
    if( fmlWidget()->isModified() && fmlWidget()->isVerbose() )
	{
		QMessageBox::StandardButton ret = QMessageBox::warning(this, __FMLCORE_Product_Title,
			tr("'%1' has been modified.\nDo you want to save your changes?").arg( userFriendlyCurrentFile() ),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if( ret == QMessageBox::Save )
			return save();
		else if( ret == QMessageBox::Cancel )
			return false;
    }
    return true;
}

void QFormulatorViewWindow::setCurrentFile( const QString &fileName )
{
	setCurrentFileName( fileName );
#ifdef Q_WS_MAC
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_DOMAIN_, QCoreApplication::applicationName() );
#else
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, __COMPANY_NAME__, QCoreApplication::applicationName() );
#endif
    QStringList files = settings.value("FmlIde/recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
        files.removeLast();
    settings.setValue("FmlIde/recentFileList", files);
}

QString QFormulatorViewWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void QFormulatorViewWindow::setCurrentFileName( const QString &fileName )
{
    m_curFile = QFileInfo( fileName ).canonicalFilePath();
    m_isUntitled = false;
    setWindowModified( false );
    setWindowTitle( userFriendlyCurrentFile() + "[*]" );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::initGui( QMainWindow *uiUpdater )
{
	QFormulatorViewWindow::initActions( uiUpdater );
	QFormulatorViewWindow::initCommonGui( uiUpdater );
	//!!! QFormulatorEditWindow::initXmlBtnGui( uiUpdater );
	QFormulatorViewWindow::adjustViewMenuToggleToolbars();
}

void QFormulatorViewWindow::initCommonGui( QMainWindow *uiUpdater )
{
	QFormulatorViewWindow::initFileMenu( uiUpdater );
	QFormulatorViewWindow::initViewMenu( uiUpdater );
	QFormulatorViewWindow::initFileToolbar( uiUpdater );
	QFormulatorViewWindow::initExportToolbar( uiUpdater );
	QFormulatorViewWindow::initViewToolbar( uiUpdater );
}

void QFormulatorViewWindow::initActions( QMainWindow *uiUpdater )
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), uiUpdater);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), uiUpdater, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), uiUpdater);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), uiUpdater, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), uiUpdater);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), uiUpdater, SLOT(save()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), saveAct, SLOT(setEnabled(bool)));

    saveAsAct = new QAction(tr("Save &As..."), uiUpdater);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), uiUpdater, SLOT(saveAs()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), saveAsAct, SLOT(setEnabled(bool)));

    exportPdfAct = new QAction(QIcon(":/images/exportpdf.png"), tr("Export to PDF..."), uiUpdater);
    exportPdfAct->setStatusTip(tr("Export the document to a PDF file"));
    connect(exportPdfAct, SIGNAL(triggered()), uiUpdater, SLOT(exportPdf()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), exportPdfAct, SLOT(setEnabled(bool)));

    //exportEmfAct = new QAction(QIcon(":/images/exportemf.png"), tr("Export to EMF..."), uiUpdater);
    //exportEmfAct->setStatusTip(tr("Export the document to an EMF file"));
    //connect(exportEmfAct, SIGNAL(triggered()), uiUpdater, SLOT(exportEmf()));
	//connect(uiUpdater, SIGNAL(hasChildSignal(bool)), exportEmfAct, SLOT(setEnabled(bool)));

    exportSvgAct = new QAction(QIcon(":/images/exportsvg.png"), tr("Export to SVG..."), uiUpdater);
    exportSvgAct->setStatusTip(tr("Export the document to an SVG file"));
    connect(exportSvgAct, SIGNAL(triggered()), uiUpdater, SLOT(exportSvg()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), exportSvgAct, SLOT(setEnabled(bool)));

	exportImageAct = new QAction(QIcon(":/images/exportimg.png"), tr("Export to Image..."), uiUpdater);
    exportImageAct->setStatusTip(tr("Export the document to a raster image file"));
    connect(exportImageAct, SIGNAL(triggered()), uiUpdater, SLOT(exportImage()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), exportImageAct, SLOT(setEnabled(bool)));

    exportWebAct = new QAction(QIcon(":/images/exportweb.png"), tr("Export to IE..."), uiUpdater);
    exportWebAct->setStatusTip(tr("Export the document to a Web page for later use with IE"));
    connect(exportWebAct, SIGNAL(triggered()), uiUpdater, SLOT(exportWeb()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), exportWebAct, SLOT(setEnabled(bool)));

    copyAllAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy All to Clipboard"), uiUpdater);
    //copyAllAct->setShortcuts(QKeySequence::Copy);
    copyAllAct->setStatusTip(tr("Copy the current document's contents to the clipboard"));
    connect(copyAllAct, SIGNAL(triggered()), uiUpdater, SLOT(copyAll()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), copyAllAct, SLOT(setEnabled(bool)));

	pageSetupAct = new QAction(tr("Page Setup..."), uiUpdater);
    pageSetupAct->setStatusTip(tr("Setup page margins and line spacing for the current document"));
    connect(pageSetupAct, SIGNAL(triggered()), uiUpdater, SLOT(pageSetup()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), pageSetupAct, SLOT(setEnabled(bool)));

	printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), uiUpdater);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Prints the current document"));
    connect(printAct, SIGNAL(triggered()), uiUpdater, SLOT(print()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), printAct, SLOT(setEnabled(bool)));
	
    printPreviewAct = new QAction(QIcon(":/images/printpreview.png"), tr("Print Pre&view"), uiUpdater);
    printPreviewAct->setStatusTip(tr("Print preview for the current document"));
    connect(printPreviewAct, SIGNAL(triggered()), uiUpdater, SLOT(printPreview()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), printPreviewAct, SLOT(setEnabled(bool)));

	zoomInAct = new QAction(QIcon(":/images/zoomin.png"), QString(tr("Zoom &In (%1%)")).arg(int(DEFAULT_VIEWSCALE_STEP * 100)), uiUpdater);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setStatusTip(tr("Zoom in the current document"));
    connect(zoomInAct, SIGNAL(triggered()), uiUpdater, SLOT(zoomIn()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoomInAct, SLOT(setEnabled(bool)));

    zoomOutAct = new QAction(QIcon(":/images/zoomout.png"), QString(tr("Zoom &Out (%1%)")).arg(int(DEFAULT_VIEWSCALE_STEP * 100)), uiUpdater);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setStatusTip(tr("Zoom out the current document"));
    connect(zoomOutAct, SIGNAL(triggered()), uiUpdater, SLOT(zoomOut()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoomOutAct, SLOT(setEnabled(bool)));

    zoom100Act = new QAction(tr("100%"), uiUpdater);
    zoom100Act->setCheckable(true);
    zoom100Act->setChecked( ::Formulib_GetScale() == 1.0 );
    zoom100Act->setShortcut(tr("Ctrl+1"));
    zoom100Act->setStatusTip(tr("Scale the editing view to 100% size"));
    connect(zoom100Act, SIGNAL(triggered()), uiUpdater, SLOT(zoom100()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoom100Act, SLOT(setEnabled(bool)));

    zoom200Act = new QAction(tr("200%"), uiUpdater);
    zoom200Act->setCheckable(true);
    zoom200Act->setChecked( ::Formulib_GetScale() == 2.0 );
    zoom200Act->setShortcut(tr("Ctrl+2"));
    zoom200Act->setStatusTip(tr("Scale the editing view to 200% size"));
    connect(zoom200Act, SIGNAL(triggered()), uiUpdater, SLOT(zoom200()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoom200Act, SLOT(setEnabled(bool)));

    zoom300Act = new QAction(tr("300%"), uiUpdater);
    zoom300Act->setCheckable(true);
    zoom300Act->setChecked( ::Formulib_GetScale() == 3.0 );
    zoom300Act->setShortcut(tr("Ctrl+3"));
    zoom300Act->setStatusTip(tr("Scale the editing view to 300% size"));
    connect(zoom300Act, SIGNAL(triggered()), uiUpdater, SLOT(zoom300()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoom300Act, SLOT(setEnabled(bool)));

    zoom500Act = new QAction(tr("500%"), uiUpdater);
    zoom500Act->setCheckable(true);
    zoom500Act->setChecked( ::Formulib_GetScale() == 5.0 );
    zoom500Act->setShortcut(tr("Ctrl+5"));
    zoom500Act->setStatusTip(tr("Scale the editing view to 500% size"));
    connect(zoom500Act, SIGNAL(triggered()), uiUpdater, SLOT(zoom500()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoom500Act, SLOT(setEnabled(bool)));

    zoom1000Act = new QAction(tr("1000%"), uiUpdater);
    zoom1000Act->setCheckable(true);
    zoom1000Act->setChecked( ::Formulib_GetScale() == 10.0 );
    zoom1000Act->setShortcut(tr("Ctrl+0"));
    zoom1000Act->setStatusTip(tr("Scale the editing view to 1000% size"));
    connect(zoom1000Act, SIGNAL(triggered()), uiUpdater, SLOT(zoom1000()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoom1000Act, SLOT(setEnabled(bool)));

	zoomActionGroup = new QActionGroup(uiUpdater);
	zoomActionGroup->addAction(zoom100Act);
	zoomActionGroup->addAction(zoom200Act);
	zoomActionGroup->addAction(zoom300Act);
	zoomActionGroup->addAction(zoom500Act);
	zoomActionGroup->addAction(zoom1000Act);

    showNestingAct = new QAction(tr("Show Nesting"), uiUpdater);
    showNestingAct->setShortcut(tr("Ctrl+Shift+T"));
    showNestingAct->setCheckable(true);
	showNestingAct->setChecked( ::getCurrentFormulatorNestingState() ? true : false );
    showNestingAct->setStatusTip(tr("Show or hide nesting"));
    connect(showNestingAct, SIGNAL(toggled(bool)), uiUpdater, SLOT(showNesting(bool)));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), showNestingAct, SLOT(setEnabled(bool)));

    showReadonlyAct = new QAction(tr("Show Read-Only"), uiUpdater);
    showReadonlyAct->setShortcut(tr("Ctrl+Shift+R"));
    showReadonlyAct->setCheckable(true);
    showReadonlyAct->setChecked( ::getCurrentFormulatorReadOnlyState() ? true : false );
    showReadonlyAct->setStatusTip(tr("Show or hide read-only highlighting"));
    connect(showReadonlyAct, SIGNAL(toggled(bool)), uiUpdater, SLOT(showReadonly(bool)));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), showReadonlyAct, SLOT(setEnabled(bool)));

    propertiesAct = new QAction(tr("Properties"), uiUpdater);
    propertiesAct->setStatusTip(tr("Edit properties of the current item of an expression"));
    connect(propertiesAct, SIGNAL(triggered()), uiUpdater, SLOT(properties()));
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), propertiesAct, SLOT(setEnabled(bool)));
}

void QFormulatorViewWindow::initFileMenu( QMainWindow *uiUpdater )
{
    fileMenu = uiUpdater->menuBar()->addMenu(tr("&File"));
    fileMenu->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("File"));

    fileMenu->addAction( newAct );
    fileMenu->addAction( openAct );
    fileMenu->addAction( saveAct );
    fileMenu->addAction( saveAsAct );
    //fileMenu->addSeparator();
    //fileMenu->addAction( copyAllAct );

	exportMenu = fileMenu->addMenu(tr("Export"));
	exportMenu->addAction( exportPdfAct );
	//exportMenu->addAction( exportEmfAct );
	exportMenu->addAction( exportSvgAct );
	exportMenu->addAction( exportImageAct );
	exportMenu->addAction( exportWebAct );
	exportMenu->addSeparator();
	exportMenu->addAction( copyAllAct );

    fileMenu->addSeparator();
	//fileMenu->addAction(pageSetupAct);
    fileMenu->addAction(printAct);
    fileMenu->addAction(printPreviewAct);
}

void QFormulatorViewWindow::initViewMenu( QMainWindow *uiUpdater )
{
    viewMenu = uiUpdater->menuBar()->addMenu(tr("&View"));
    viewMenu->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_MENU).arg("View"));

	viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);

	zoomMenu = viewMenu->addMenu(tr("&Zoom"));
    zoomMenu->addAction(zoom100Act);
    zoomMenu->addAction(zoom200Act);
    zoomMenu->addAction(zoom300Act);
    zoomMenu->addAction(zoom500Act);
    zoomMenu->addAction(zoom1000Act);

    viewMenu->addSeparator();
    viewMenu->addAction(showNestingAct);
    viewMenu->addAction(showReadonlyAct);
}

void QFormulatorViewWindow::initFileToolbar( QMainWindow *uiUpdater )
{
	fileToolBar = uiUpdater->addToolBar(tr("File"));
    fileToolBar->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg("File"));
	fileToolBar->setIconSize( QSize(IconSizeX, IconSizeY) );

    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printAct);
    fileToolBar->addAction(printPreviewAct);
}

void QFormulatorViewWindow::initExportToolbar( QMainWindow *uiUpdater )
{
	exportToolBar = uiUpdater->addToolBar(tr("Export"));
    exportToolBar->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg("Export"));
	exportToolBar->setIconSize( QSize(IconSizeX, IconSizeY) );

    exportToolBar->addAction(exportPdfAct);
    exportToolBar->addAction(exportSvgAct);
    exportToolBar->addAction(exportImageAct);
    exportToolBar->addAction(exportWebAct);
}

void QFormulatorViewWindow::initViewToolbar( QMainWindow *uiUpdater )
{
	viewToolBar = uiUpdater->addToolBar(tr("View"));
    viewToolBar->setObjectName(QString(FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR).arg("View"));
	viewToolBar->setIconSize( QSize(IconSizeX, IconSizeY) );

    viewToolBar->addAction(zoomInAct);
    viewToolBar->addAction(zoomOutAct);

	zoomAny = new QComboBox();
	zoomAny->setEditable(true);
	zoomAny->setDuplicatesEnabled(false);
	zoomAny->addItem( "100", QVariant(uint(100)) );
	zoomAny->addItem( "200", QVariant(uint(200)) );
	zoomAny->addItem( "300", QVariant(uint(300)) );
	zoomAny->addItem( "500", QVariant(uint(400)) );
	zoomAny->addItem( "1000", QVariant(uint(500)) );
	zoomAny->setValidator( new QIntValidator(int(100*DEFAULT_VIEWSCALE_MIN), int(100*DEFAULT_VIEWSCALE_MAX), zoomAny) );
	connect(uiUpdater, SIGNAL(hasChildSignal(bool)), zoomAny, SLOT(setEnabled(bool)));

	QFormulatorViewWindow::updateZoomComboText( ::Formulib_GetScale() );

	connect(zoomAny, SIGNAL(currentIndexChanged(int)), uiUpdater, SLOT(zoomValue(int)));
    viewToolBar->addWidget( zoomAny );
}

void QFormulatorViewWindow::adjustViewMenuToggleToolbars()
{
	if( viewMenu )
	{
		QAction *separatorAct = viewMenu->insertSeparator( zoomInAct );
		viewMenu->insertMenu(separatorAct, toggleToolbarMenu = new QMenu(tr("View Toolbar")));
		if( fileToolBar ) toggleToolbarMenu->addAction(fileToolBar->toggleViewAction());
		if( exportToolBar ) toggleToolbarMenu->addAction(exportToolBar->toggleViewAction());
		if( viewToolBar ) toggleToolbarMenu->addAction(viewToolBar->toggleViewAction());
	}
}

void QFormulatorViewWindow::updateZoomComboText( double scale )
{
	QString currentScale = QString::number(uint(scale * 100 + 0.5));
	if( currentScale == zoomAny->currentText() ) return;
	zoomAny->setEditText( currentScale );
	int currentScaleIdx = zoomAny->findText( currentScale );
	if( currentScaleIdx != -1 )
		zoomAny->setCurrentIndex( currentScaleIdx );
	else
	{
		zoomAny->addItem( currentScale );
		currentScaleIdx = zoomAny->findText( currentScale );
		if( currentScaleIdx != -1 )
			zoomAny->setCurrentIndex( currentScaleIdx );
	}
}

void QFormulatorViewWindow::updateZoomComboText( QFormulatorViewWindow *child )
{
	QFormulatorViewWindow::updateZoomComboText( child->fmlWidget()->scale() );
	zoom100Act->setChecked( child->fmlWidget()->scale() == 1.0 );
	zoom200Act->setChecked( child->fmlWidget()->scale() == 2.0 );
	zoom300Act->setChecked( child->fmlWidget()->scale() == 3.0 );
	zoom500Act->setChecked( child->fmlWidget()->scale() == 5.0 );
	zoom1000Act->setChecked( child->fmlWidget()->scale() == 10.0 );
}

//////////////////////////////////////////////////////////////////////////////

void QFormulatorViewWindow::initFormulatorCore()
{
	QFormulatorWidget::initFormulatorCore();
	//::FORMULATOR_loadStyles( 1 );
	//::FORMULATOR_loadToolBarSet();
	//::getOperatorEntryByName( QString(), __GLOBAL_INIT_MODE );
	//std::vector<QString> vec1V;
	//std::vector<long> vec2V;
	//QString sV;
	//::getMathMLChar2Style( vec1V, vec2V, sV, sV, __GLOBAL_INIT_MODE );
}

void QFormulatorViewWindow::showFormulatorSplash( QMainWindow * /*uiUpdater*/ )
{
	splash = new QSplashScreen( QPixmap(":/images/fmlidesplash.jpg") );
	splash->show();
}

void QFormulatorViewWindow::hideFormulatorSplash( QMainWindow *uiUpdater )
{
	if( splash ) splash->finish( uiUpdater );
}

//////////////////////////////////////////////////////////////////////////////

QAction *QFormulatorViewWindow::newAct = 0;
QAction *QFormulatorViewWindow::openAct = 0;
//QAction *QFormulatorViewWindow::recentFileActs[ QFormulatorViewWindow::MaxRecentFiles ] = {0, 0, 0, 0, 0};
QAction *QFormulatorViewWindow::saveAct = 0;
QAction *QFormulatorViewWindow::saveAsAct = 0;
QAction *QFormulatorViewWindow::exportPdfAct = 0;
QAction *QFormulatorViewWindow::exportSvgAct = 0;
QAction *QFormulatorViewWindow::exportImageAct = 0;
QAction *QFormulatorViewWindow::exportWebAct = 0;
QAction *QFormulatorViewWindow::copyAllAct = 0;
QAction *QFormulatorViewWindow::printAct = 0;
QAction *QFormulatorViewWindow::pageSetupAct = 0;
QAction *QFormulatorViewWindow::printPreviewAct = 0;
QAction *QFormulatorViewWindow::propertiesAct = 0;
QAction *QFormulatorViewWindow::zoomInAct = 0;
QAction *QFormulatorViewWindow::zoomOutAct = 0;
QAction *QFormulatorViewWindow::zoom100Act = 0;
QAction *QFormulatorViewWindow::zoom200Act = 0;
QAction *QFormulatorViewWindow::zoom300Act = 0;
QAction *QFormulatorViewWindow::zoom500Act = 0;
QAction *QFormulatorViewWindow::zoom1000Act = 0;
QAction *QFormulatorViewWindow::showNestingAct = 0;
QAction *QFormulatorViewWindow::showReadonlyAct = 0;
QActionGroup *QFormulatorViewWindow::zoomActionGroup = 0;
QMenu *QFormulatorViewWindow::fileMenu = 0;
QMenu *QFormulatorViewWindow::exportMenu = 0;
//QMenu *QFormulatorViewWindow::recentFilesMenu = 0
QMenu *QFormulatorViewWindow::viewMenu = 0;
QMenu *QFormulatorViewWindow::zoomMenu = 0;
QMenu *QFormulatorViewWindow::toggleToolbarMenu = 0;
QToolBar *QFormulatorViewWindow::fileToolBar = 0;
QToolBar *QFormulatorViewWindow::exportToolBar = 0;
QToolBar *QFormulatorViewWindow::viewToolBar = 0;
QComboBox *QFormulatorViewWindow::zoomAny = 0;
QSplashScreen *QFormulatorViewWindow::splash = 0;

//////////////////////////////////////////////////////////////////////////////
