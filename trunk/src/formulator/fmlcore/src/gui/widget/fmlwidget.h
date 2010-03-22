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

#if !defined( __FMLCORE_FMLWIDGET_H__ )
#define __FMLCORE_FMLWIDGET_H__

#include <QWidget>
#include "fmlbasewidget.h"
#include "HUtils/idrawing.h"
#include "../../fmlcore_global.h"

class CFormulatorDoc;
class CRootNode;
class CUndoRedo;
class QPixmap;

/////////////////////////////////////////////////////////////////////////////

class FXmlBtn;

class __HERMITECH_FORMULIB__ QFormulatorWidget
	: public QWidget
	, public QFormulatorBaseWidget
{
	Q_OBJECT

protected:
	QObject*				m_uiUpdater;
	QWidget*				m_parentWindow;
	CFormulatorDoc*			m_pDocument;

	bool					m_removedSelection;
	bool					m_printingMode;
	bool					m_cursorOn;
	PointFde				m_ptCaret;
	SizeFde					m_szCaret;

	RealFde					m_viewScale;
	QColor					m_backColor;

// init methods
///////////////
public:
	QFormulatorWidget( QWidget *parent = 0, QObject *uiUpdater = 0 );
	virtual ~QFormulatorWidget();

// events
///////////////
protected:
	virtual void paintEvent(QPaintEvent *e);
	virtual void keyPressEvent( QKeyEvent *e );

// general access methods
///////////////
public:
	QObject *uiUpdater();
	RectFde clientRect();

// document methods
///////////////
public:
	bool isModified();

	void setMmlChanged( bool isChanged = true );
	bool isMmlChanged();

protected:
	CFormulatorDoc* document();
	void setModified( bool isModified = true );
	CRootNode* rootNode();
	CUndoRedo* undoRedo();
	void setCheckPoint( int fForcedCheck = 0 );

// MathML-specific document methods
///////////////
public:
	virtual int callDialog( QDialog *dlg );

// caret methods
///////////////
public:
	const SizeFde& caretSize( void );
	const PointFde& caretPos( void );
	QString caretInfo();
protected:
	void setCaretSize( const SizeFde& szCaret );
	void setCaretPos( const PointFde& ptCaret );
protected:
	virtual void setBlinkingCursorEnabled( bool enable ) { m_cursorOn = enable; }
        virtual void repaintCursor( PointFde /*pt*/, SizeFde /*sz*/ ) {}
	virtual void repaintCursor() {}

// draw/draw options methods
///////////////
public:
	void render( QPainter *painter, const RectFde& border );

protected:
	void setPrintingMode( bool enable = true );
	bool isPrintingMode();

	void draw( QPaintDevice *device, PointFde ofs, const RectFde& border, bool printingMode = false, bool printSelectionOnly = false, bool useScale = true );
	void draw( CFmlDrawEngine *pFde, PointFde ofs, const RectFde& border, bool printingMode = false, bool printSelectionOnly = false, bool useScale = true );
protected:
	QImage drawSelection2Image( bool useSelection );
	QPixmap drawSelection2QPixmap( bool useSelection );
public:
	static QPixmap drawSampleText( long style, int pointSize, QString txt = QString("Aa"), QSize size = QSize(0, 0) );
	static QPixmap drawSampleText( QFont font, QColor color, QString txt = QString("Aa"), QSize size = QSize(0, 0) );

public:
	void setScale( double value );
	double scale();
	virtual void setBackColor( QColor color );
	QColor backColor();
	void indents( RealFde& l, RealFde& t, RealFde& r, RealFde& b );
	void setIndents( RealFde l, RealFde t, RealFde r, RealFde b );
	void indents( RectFde& rect );
	void setIndents( RectFde rect );
	RealFde lineSpacing();
	void setLineSpacing( RealFde value );

// slots
///////////////
public slots:
	void filePrintPreview();
protected slots:
	int print( QPrinter *printer, bool printSelectionOnly = false );

public:
	int fileNew();
	int fileOpen( const QString &filePath );
	int fileReload( const QString &filePath, bool verbose );
	int fileOpenSilent( const QString &filePath );
	int fileReloadSilent( const QString &filePath );
	int fileOpen( const QString &filePath, bool verbose );
	int fileSave( const QString &filePath );
	int fileClose();
	int filePrint();
	int filePageSetup();
	int fileExportPdf( const QString &filePath );
	int fileExportSvg( const QString& filePath, const QString& title );
	//int fileEmf(const QString &fileName);
	int fileExportImage( const QString &fileName );
	int fileExportWeb( const QString &fileName, const QString& title );

	int copyAll();

	void viewShowNesting( bool state );
	void viewShowReadonly( bool state );

	int formatDefineStyle( int styleNumber, const QString* styleTitle, const int* styleID );
	int formatDefineSize();

// open/save methods
///////////////
public:
	QString text();

public:
	void clear();
	int setText( QString mmlText );
	int setTextVerbose( QString mmlText );
	int refreshViaMathML();
	int upload( const QString& newText );
	//
	int replaceMathML( const QString& strTask, const QString& strText, int fExclamation );
	int insertMathML( const QString& strTask, const QString& strText, int fExclamation );
	int insertNewNode( const QString &btnID );
	int insertImage();
	int insertSymbol();
	int insertStyledText( QString text, const QFont& currentFont, const QColor& currentColor );
	int insertSimpleText( const QString& strText );

protected:
	int insertNewNode( FXmlBtn *ptrLData_FormulaNode );

// update widget methods
///////////////
public:
	void recalculateSize();
	void initialUpdate();
	void updateViewDocument();
	int updateFormulatorWidget();
	int updateScrollSize();
	int updateScrollPosition();
	int updateScrollPosition( PointFde pt, SizeFde sz, int xmargin = 50, int ymargin = 50 );
	int updateCaret();
	int updateCurrentKegl();

// edit implementation
public:
	int setStyleOther();
	void applyStyle( long style );

	int setSizeOther();
	void applyKegl( RealFde kegl );
	void incDecKegl( RealFde value );
	int canApplyKegl( RealFde kegl );

// calculations
public:
	int evaluate();

signals:
	void closeSignal();
	void scaleChangedSignal();
	void widgetUpdateSignal();
	void docModifiedSignal( bool isModified );
	void copyAvailableSignal( bool enable );

public:
	static void initFormulatorCore();
};

/////////////////////////////////////////////////////////////////////////////

inline
QObject* QFormulatorWidget::uiUpdater()
{
	return m_uiUpdater;
}

/////////////////////////////////////////////////////////////////////////////

inline
void QFormulatorWidget::setBackColor( QColor color )
	{ m_backColor = color; }

inline
QColor QFormulatorWidget::backColor()
	{ return m_backColor; }

inline
double QFormulatorWidget::scale()
	{ return m_viewScale; }

/////////////////////////////////////////////////////////////////////////////

inline 
CFormulatorDoc* QFormulatorWidget::document()
	{ return m_pDocument; }

/////////////////////////////////////////////////////////////////////////////

inline
const SizeFde& QFormulatorWidget::caretSize( void )
	{ return m_szCaret; }
inline
void QFormulatorWidget::setCaretSize( const SizeFde& szCaret )
	{ m_szCaret = szCaret; }

inline
const PointFde& QFormulatorWidget::caretPos( void )
	{ return m_ptCaret; }
inline
void QFormulatorWidget::setCaretPos( const PointFde &ptCaret )
	{ m_ptCaret = ptCaret; }

/////////////////////////////////////////////////////////////////////////////

inline
int QFormulatorWidget::callDialog( QDialog *dlg )
	{ return dlg ? dlg->exec() : QDialog::Rejected; }

/////////////////////////////////////////////////////////////////////////////

inline
bool QFormulatorWidget::isPrintingMode()
	{ return m_printingMode; }
inline
void QFormulatorWidget::setPrintingMode( bool value )
{
	m_printingMode = value;
}

/////////////////////////////////////////////////////////////////////////////

#endif
