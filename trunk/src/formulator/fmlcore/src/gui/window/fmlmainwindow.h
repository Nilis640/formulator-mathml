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

#if !defined( __FMLCORE_FMLMAINWINDOW_H__ )
#define __FMLCORE_FMLMAINWINDOW_H__

/////////////////////////////////////////////////////////////////////////////

#include <QMainWindow>
#include <QFileInfo>
#include <time.h>

class QSplashScreen;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QActionGroup;
class QToolButton;
class QComboBox;

class CUpdateRequest;
class QMathMLTextEdit;
class QMathMLFileViewer;

#include "fmleditwindow.h"
#include "fmlviewwindow.h"
#include "HUtils/idrawing.h"
#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ QFormulatorMainWindow 
	: public QMainWindow
{
	Q_OBJECT

protected:
	bool				m_isEditMode;

	QMathMLFileViewer	*m_mmlViewer;

	QMathMLTextEdit		*m_mmlEditor;
	time_t				m_mmlEditorUpdateTime;

	QMdiArea			*mdiArea;
    QSignalMapper		*windowMapper;
	QLabel				*m_inputStatusLabel, *m_hiddenStatusLabel;
	QToolButton			*m_presetBtn;
	int					m_lastAppliedPreset;
	CUpdateRequest		*m_updater;

	// store to improve usability
	int					m_configLastPage;
	QString				m_currentPath;

    QMenu *recentFilesMenu, *toolsMenu, *windowMenu, *helpMenu;
    QAction *recentFileActs[ QFormulatorViewWindow::MaxRecentFiles ], *exitAct, *closeAct, *closeAllAct, *tileAct, *cascadeAct;
    QAction *nextAct, *previousAct, *separatorAct, *aboutAct, *manualCheckUpdateAct;
	QAction *frmWebAct, *frmWebDonateAct;
	QAction *toolsBuildToolbarsetAct, *configDlgAct;

	RectFde m_indents;
	RealFde m_ls;

public:
	QFormulatorMainWindow( bool isEditMode = true, QWidget *parent = 0, Qt::WFlags flags = 0 );
	~QFormulatorMainWindow();

protected:
	void initGui( bool isEditMode );
    void closeEvent(QCloseEvent *event);

	void indents( RectFde& rect );
	void setIndents( RectFde rect );
	RealFde lineSpacing();
	void setLineSpacing( RealFde value );

protected slots:
	void updateUI_OnChildScaleChanged();
	void updateUI_OnCloseChild();
	void updateUI_OnClipboardChanged();
	void updateUI_OnChildDocModified( bool visibility );
	void updateUI_OnWidgetUpdate();
	void updateUI_OnOptionsChanged();
	void updateMmlEditor();
	void updateMmlEditor( bool enable );
	void undoAvailableSlot( bool enable );
	void redoAvailableSlot( bool enable );
	void copyAvailableSlot( bool enable );

protected slots:
    void newFile();
    void open();
	void openByFileViewer(QString fileName, QWidget *focusBack);
	void loadByFileViewer(QString fileName, QWidget *focusBack);
    void openFile(QString path);
    void openFileForView(QString path);
    void openRecentFile();
	void uploadMathML();
	void loadMathML();
    void save();
    void saveAs();
	void exportPdf();
	//void exportEmf();
	void exportSvg();
	void exportImage();
	void exportWeb();
	void print();
	void printPreview();
	void pageSetup();
    void undo();
    void redo();
    void cut();
    void copy();
    void copyAll();
    void paste();
    void selectAll();
	void properties();
    void evaluate();
	void insertAssign();
	void zoomValue( int value );
	void zoomIn();
	void zoomOut();
	void zoom100();
	void zoom200();
	void zoom300();
	void zoom500();
	void zoom1000();
	void showNesting( bool state );
	void showReadonly( bool state );
	void viewMathMLText( bool state );
	void insertSymbol();
	void insertImage();
	void insertButton();
	void showSenderStatusText();
	void showSenderStatusText(QAction *action);
	void insertButtonViaToolbar();
	void setStyleByCombo( int value );
	void setStyleSelected(QAction *action = 0);
	void setSizeByCombo( int value );
    void setSizeLargeSymbol();
    void setSizeRegular();
    void setSizeSubscript();
    void setSizeSubSubscript();
    void setSizeOther();
    void setSizeSmaller();
    void setSizeLarger();
	void formatDefineStyle();
	void formatDefineSize();
	void buildToolbarset();
	void mmlsoftWeb();
	void mmlsoftDonateWeb();
	void manualCheckUpdate();
    void about();
	void configDlg();
    void switchLayoutDirection();
	
	void automaticCheckUpdate();
	void checkUpdate();
	void updateOk();

	void presetWord();
	void presetPmml();
	void presetCmml();
	void presetCmmlPmml();
	void presetCmmlA();

	void updateOnActiveWindow( QMdiSubWindow *window );
    void updateWindowMenu();

	QFormulatorViewWindow* createSubWindow();
	QFormulatorViewWindow* reuseSubViewWindow();
	QFormulatorViewWindow* createSubViewWindow();
	void setActiveSubWindow( QWidget *window );

protected:
	void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
	void updateStatusBar();

	void setPresetBtnIcon( int presetNo );
	void readCoreFmlSettings();
    void readSettings();
    void writeSettings();

    void updateRecentFileActions();
	QString strippedName( const QString &fullFileName );

    QMdiSubWindow*				findSubWindow(const QString &fileName);
	QMdiSubWindow*				findSubWindow( QFormulatorViewWindow *child2find );
    QFormulatorViewWindow*		activeViewSubWindow();
	QFormulatorEditWindow*		activeEditSubWindow();
	QFormulatorWidget*			activeViewWidget();
	QFormulatorEditWidget*		activeEditWidget();

	enum
	{ MinPeriodMmlEditorUpdate = 1 };

signals:
	void hasChildSignal( bool state );
	void hasEditChildSignal( bool state );
	void hasClipboardForChildSignal( bool state );
	void undoAvailableSignal( bool enable );
	void redoAvailableSignal( bool enable );
	void copyAvailableSignal( bool enable );
};

/////////////////////////////////////////////////////////////////////////////

inline
QString QFormulatorMainWindow::strippedName( const QString &fullFileName )
{
	return QFileInfo(fullFileName).fileName();
}

/////////////////////////////////////////////////////////////////////////////

#endif
