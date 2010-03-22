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

#if !defined( __FMLCORE_FMLVIEWWINDOW_H__ )
#define __FMLCORE_FMLVIEWWINDOW_H__

#include <QScrollArea>

class QAction;
class QActionGroup;
class QScrollArea;
class QTimer;
class QMainWindow;
class QToolBar;
class QMenu;
class QSplashScreen;
class QComboBox;

class QFormulatorWidget;

#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ QFormulatorViewWindow
	: public QScrollArea
{
	Q_OBJECT

protected:
	QFormulatorWidget	*m_fmlWidget;
    QString				m_curFile;
    bool				m_isUntitled;

// init methods
///////////////
public:
	QFormulatorViewWindow( QFormulatorWidget *fml, QWidget *parent = 0, QObject *uiUpdater = 0 );
	QFormulatorViewWindow( QWidget *parent = 0, QObject *uiUpdater = 0 );
	virtual ~QFormulatorViewWindow();
	void init( QFormulatorWidget *fml, QObject *uiUpdater );

// draw/draw options methods
///////////////
public:
	//virtual void setBackColor( QColor color );

// events
///////////////
protected:
    virtual void closeEvent(QCloseEvent *e);

// interface
///////////////
public slots:
	void newFile();
    bool loadFile(const QString &fileName);
	bool loadFileSilent(const QString &fileName);
	bool reloadFileSilent(const QString &fileName);
    bool save(QString currentPath = QString());
    bool saveAs(QString currentPath = QString());
    bool saveFile(const QString &fileName);

	void copyAll();
	void upload( const QString& newText );

	void exportPdf(const QString &fileName);
	//void exportEmf(const QString &fileName);
	void exportImage(const QString &fileName);
	void exportWeb(const QString &fileName);
	void exportSvg(const QString& filePath);

	void print();
	void printPreview();
	void pageSetup();

	void zoomIn();
	void zoomOut();
	void zoomValue( double value );
	void zoomValueQuick( double value );

	void showNesting( bool state );
	void showReadonly( bool state );

	void refreshViaMathML();
	void viewMathMLText( bool state );
	void properties();

	virtual bool formatDefineStyle();
	virtual bool formatDefineSize();
protected:
    enum { styleNumber = 13 };
	static QString m_styleTitle[ QFormulatorViewWindow::styleNumber ];
	static int m_styleID[ QFormulatorViewWindow::styleNumber ];

// access members for external GUI system
///////////////
public:
	QFormulatorWidget*	fmlWidget()		{ return m_fmlWidget; }

    QString userFriendlyCurrentFile();
    QString currentFile() { return m_curFile; }

	int setText( QString mmlText );
	QString text();
	void setCurrentFileName( const QString& fileName );

protected:
    bool maybeSave();
    void setCurrentFile( const QString& fileName );
    QString strippedName( const QString& fullFileName );

public:
	static void initGui( QMainWindow *uiUpdater );
	static void initFormulatorCore();
	static void showFormulatorSplash( QMainWindow *uiUpdater );
	static void hideFormulatorSplash( QMainWindow *uiUpdater );
	static void updateZoomComboText( QFormulatorViewWindow *child );

protected:
	static void initCommonGui( QMainWindow *uiUpdater );
	static void initActions( QMainWindow *uiUpdater );
	static void initFileMenu( QMainWindow *uiUpdater );
	static void initViewMenu( QMainWindow *uiUpdater );
	static void initFileToolbar( QMainWindow *uiUpdater );
	static void initExportToolbar( QMainWindow *uiUpdater );
	static void initViewToolbar( QMainWindow *uiUpdater );
	static void adjustViewMenuToggleToolbars();
	static void updateZoomComboText( double scale );

public:
    enum { MaxRecentFiles = 5, IconSizeX = 24, IconSizeY = 24 };
protected:
    static QAction				*newAct;
	static QAction				*openAct;
    static QAction				*saveAct;
	static QAction				*saveAsAct;
    static QAction				*exportPdfAct;
    static QAction				*exportSvgAct;
	static QAction				*exportImageAct;
	static QAction				*exportWebAct;
	static QAction				*copyAllAct;
	static QAction				*printAct;
	static QAction				*pageSetupAct;
	static QAction				*printPreviewAct;
    static QAction				*zoomInAct;
	static QAction				*zoomOutAct;
	static QAction				*zoom100Act;
	static QAction				*zoom200Act;
	static QAction				*zoom300Act;
	static QAction				*zoom500Act;
	static QAction				*zoom1000Act;
	static QActionGroup			*zoomActionGroup;
    static QAction				*showNestingAct;
	static QAction				*showReadonlyAct;
	static QAction				*propertiesAct;
    static QMenu				*fileMenu;
	static QMenu				*exportMenu;
	static QMenu				*viewMenu;
	static QMenu				*zoomMenu;
	static QMenu				*toggleToolbarMenu;
	static QToolBar				*fileToolBar;
	static QToolBar				*exportToolBar;
	static QToolBar				*viewToolBar;
	static QComboBox			*zoomAny;
	static QSplashScreen		*splash;
};

/////////////////////////////////////////////////////////////////////////////

#define FORMULATOR_QTGUI_OBJECT_NAME_MENU		"__qt__fmlide_menu_%1"
#define FORMULATOR_QTGUI_OBJECT_NAME_TOOLBAR	"__qt__fmlide_toolbar_%1"

/////////////////////////////////////////////////////////////////////////////

#endif
