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

#if !defined( __FMLCORE_FMLEDITWINDOW_H__ )
#define __FMLCORE_FMLEDITWINDOW_H__

#include <QAction>
#include <QScrollArea>
#include <QTimer>
#include <QFontComboBox>
#include <QLabel>

#include "fmlviewwindow.h"
#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////////////

class QMainWindow;
class HKAccel;

class QFormulatorEditWidget;

/////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ QFormulatorEditWindow
	: public QFormulatorViewWindow
{
	Q_OBJECT

protected:
	QFormulatorEditWidget		*m_fmlEditWidget;

// init methods
///////////////
public:
	QFormulatorEditWindow( QWidget *parent = 0, QObject *uiUpdater = 0 );
	virtual ~QFormulatorEditWindow();

// events
///////////////
protected:
	virtual void mousePressEvent( QMouseEvent *e );
	virtual void mouseMoveEvent( QMouseEvent *e );
	virtual void focusInEvent(QFocusEvent *e);
	virtual void contextMenuEvent( QContextMenuEvent *e );

// interface
///////////////
public slots:
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();

    void evaluate();

	void insertSymbol();
	void insertImage();
	void insertButton( const QString& buttonID );

	void setStyle( int no );
    void setStyleOther();
	void setSize( int no );
    void setSizeLargeSymbol();
    void setSizeRegular();
    void setSizeSubscript();
    void setSizeSubSubscript();
    void setSizeOther();
    void setSizeSmaller();
    void setSizeLarger();

	virtual bool formatDefineStyle();
	virtual bool formatDefineSize();

	QString caretInfo();

// access members for external GUI system
///////////////
public:
	QFormulatorEditWidget*	fmlEditWidget()		{ return m_fmlEditWidget; }

public:
	static void initGui( QMainWindow *uiUpdater );
	static void initFormulatorCore();
	static void showFormulatorSplash( QMainWindow *uiUpdater );
	static void hideFormulatorSplash( QMainWindow *uiUpdater );

protected:
	static void initCommonGui( QMainWindow *uiUpdater );
	static void initXmlBtnGui( QMainWindow *uiUpdater );
	static void initXmlBtnAccel( const HKAccel& accel, int *k );
	static void initActions( QMainWindow *uiUpdater );
	static void initEditMenu( QMainWindow *uiUpdater );
	static void initInsertMenu( QMainWindow *uiUpdater );
	static void initFormatMenu( QMainWindow *uiUpdater );
	static void initEditToolbar( QMainWindow *uiUpdater );
	static void initInsertToolbar( QMainWindow *uiUpdater );
	static void initFormatToolbar( QMainWindow *uiUpdater );
	static void adjustViewMenuToggleToolbars();

protected:
	static void initStyleSizeWidgets( QMainWindow *uiUpdater );
	static void updateSelectStyleCombo();
	static void updateSelectStyleUI();
	static void updateSelectSizeCombo();
	static void updateSelectSizeUI();

protected:
    static QAction					*undoAct;
	static QAction					*redoAct;
	static QAction					*cutAct;
	static QAction					*copyAct;
	static QAction					*pasteAct;
	static QAction					*selectAllAct;
	static QAction					*insertSymbolAct;
	static QAction					*insertImageAct;
	static QAction					*formatStyleMathAct;
	static QAction					*formatStyleTextAct;
	static QAction					*formatStyleFunctionAct;
	static QAction					*formatStyleVariableAct;
	static QAction					*formatStyleNumberAct;
	static QAction					*formatStyleGreekAct;
	static QAction					*formatStyleVectorMatrixAct;
	static QAction					*formatStyleFixedAct;
	static QAction					*formatStyleOpAct;
	static QAction					*formatStyleExtraMathAct;
	static QAction					*formatStyleUser1Act;
	static QAction					*formatStyleUser2Act;
	static QAction					*formatStyleOtherAct;
	static QAction					*formatSizeLargeSymbolAct;
	static QAction					*formatSizeRegularAct;
	static QAction					*formatSizeSubscriptAct;
	static QAction					*formatSizeSubSubscriptAct;
	static QAction					*formatSizeOtherAct;
	static QAction					*formatSizeSmallerAct;
	static QAction					*formatSizeLargerAct;
	static QActionGroup				*styleActionGroup;
	static QActionGroup				*sizeActionGroup;
	static QAction					*formatDefineStyleAct;
	static QAction					*formatDefineSizeAct;
	static QAction					*evaluateAct;
	static QAction					*insertAssignAct;
	static QVector<QAction*>		insertXmlBtn;
    static QMenu					*editMenu;
	static QMenu					*insertMenu;
	static QMenu					*formatMenu;
	static QMenu					*styleMenu;
	static QMenu					*sizeMenu;
	static QVector<QMenu*>			insertXmlBtnMenu;
	static QMap<QString,QMenu*>		menuGroups;
	static QToolBar					*editToolBar;
	static QToolBar					*insertToolBar;
	static QToolBar					*formatToolBar;
	static QComboBox				*selectStyleCombo;
	static QLabel					*selectStyleLabel;
	static QComboBox				*selectSizeCombo;
	static QLabel					*selectSizeLabel;
	static QFontComboBox			*fontCombo;
	static QComboBox				*fontStyleCombo;
	static QComboBox				*fontSizeCombo;
	static QMap<QString,QToolBar*>	toolbarGroups;
};

/////////////////////////////////////////////////////////////////////////////

#endif
