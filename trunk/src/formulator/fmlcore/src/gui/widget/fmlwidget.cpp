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
#include <QSvgGenerator>

#include <vector>

#include "HUtils/iustring.h"
#include "HUtils/idrawing.h"
#include "HMathML/isettings.h"
#include "HMathML/itoolbarset.h"
#include "HMathML/idictionary.h"

#include "../document/fmldocument.h"
#include "fmlwidget.h"

#include "../dlg/charactermap/characterdlg.h"
#include "../dlg/otherstyledlg.h"
#include "../dlg/definesizedlg.h"
#include "../dlg/definestyledlg.h"
#include "../dlg/config/configdlg.h"
#include "../dlg/pagesetup.h"

#include "../../utils/evaluate.h"
#include "../../mathml/mml_tag2mml.h"
#include "../../version/version.h"
#include "../../nodes/ni_root.h"
#include "../../nodes/rules/sel.h"
#include "../../nodes/extcontent/extc_classid.h"

/////////////////////////////////////////////////////////////////////////////
#include "HMathAST/icmml.h"

void QFormulatorWidget::initFormulatorCore()
{
	::FORMULATOR_loadStyles( 1 );
	::FORMULATOR_loadToolBarSet();
	::getOperatorEntryByName( QString(), __GLOBAL_INIT_MODE );
	std::vector<QString> vec1V;
	std::vector<long> vec2V;
	QString sV;
	::getMathMLChar2Style( vec1V, vec2V, sV, sV, __GLOBAL_INIT_MODE );
}

/////////////////////////////////////////////////////////////////////////////

QFormulatorWidget::QFormulatorWidget( QWidget *parent, QObject *uiUpdater )
	: QWidget( parent )
	, m_uiUpdater( uiUpdater )
{
	m_parentWindow = parent;
	m_pDocument = new CFormulatorDoc( this );
	if( m_uiUpdater )
	{
		connect(this, SIGNAL(closeSignal()), m_uiUpdater, SLOT(updateUI_OnCloseChild()));
		connect(this, SIGNAL(widgetUpdateSignal()), m_uiUpdater, SLOT(updateUI_OnWidgetUpdate()));
		connect(this, SIGNAL(scaleChangedSignal()), m_uiUpdater, SLOT(updateUI_OnChildScaleChanged()));
		connect(this, SIGNAL(docModifiedSignal(bool)), m_uiUpdater, SLOT(updateUI_OnChildDocModified(bool)));
		connect(undoRedo(), SIGNAL(undoAvailableSignal(bool)), m_uiUpdater, SLOT(undoAvailableSlot(bool)));
		connect(undoRedo(), SIGNAL(redoAvailableSignal(bool)), m_uiUpdater, SLOT(redoAvailableSlot(bool)));
		connect(this, SIGNAL(copyAvailableSignal(bool)), m_uiUpdater, SLOT(copyAvailableSlot(bool)));
	}
	setVerbose( false );
	setPrintingMode( true );
	setBlinkingCursorEnabled( false );

	setScale( ::Formulib_GetScale() );

	setLineSpacing( ::Formulib_GetLineSpacing() );
	RectFde rcIndents;
	::Formulib_GetIndents( rcIndents );
	setIndents( rcIndents );
	setBackColor( QColor(255, 255, 255) );
}

QFormulatorWidget::~QFormulatorWidget()
{
	::Formulib_SetScale( scale() );
	if( m_pDocument ) delete m_pDocument;
	emit closeSignal();
}

/////////////////////////////////////////////////////////////////////////////

bool QFormulatorWidget::isModified()
	{ return document()->isModified(); }

void QFormulatorWidget::setModified( bool isModified )
{
	document()->setModified( isModified );
	//if( fModified )
	//	cbFireEvent( FORMULIB_CALLBACK_CHANGE );
}

bool QFormulatorWidget::isMmlChanged()
	{ return document()->isMmlChanged(); }

void QFormulatorWidget::setMmlChanged( bool value )
{
	document()->setMmlChanged( value );
	//if( fModified )
	//	cbFireEvent( FORMULIB_CALLBACK_CHANGE );
}

CRootNode* QFormulatorWidget::rootNode()
	{ return document()->rootNode(); }

CUndoRedo* QFormulatorWidget::undoRedo()
	{ return document()->undoRedo(); }

int QFormulatorWidget::canApplyKegl( RealFde kegl )
	{ return kegl >= DEFAULT_SYMBOLSIZE_MIN && kegl <= DEFAULT_SYMBOLSIZE_MAX; }

/////////////////////////////////////////////////////////////////////////////

RectFde QFormulatorWidget::clientRect()
{
	RectFde rcf( - geometry().topLeft(), geometry().bottomRight() - geometry().topLeft() );
	if( m_parentWindow )
	{
		rcf.setWidth( m_parentWindow->geometry().width() );
		rcf.setHeight( m_parentWindow->geometry().height() );
	}
	return rcf;
}

/////////////////////////////////////////////////////////////////////////////

QString QFormulatorWidget::text()
{
	return document()->exportMathML();
}

int QFormulatorWidget::insertMathML( const QString& strTask, const QString& strText, int fExclamation )
{
	if( rootNode()->InsertMathML( strTask, strText, fExclamation ) != -1 )
	{
		updateViewDocument();
		return 1;
	}
	return 0;
}

int QFormulatorWidget::replaceMathML( const QString& strTask, const QString& strText, int fExclamation )
{
	if( rootNode()->ReplaceMathML( strTask, strText, fExclamation ) != -1 )
	{
		updateViewDocument();
		return 1;
	}
	return 0;
}

void QFormulatorWidget::clear()
{
	rootNode()->ClearAll();
}

int QFormulatorWidget::setText( QString mmlText )
{
	clear();
	return insertMathML( __FMLCORE_Product_Title, mmlText, 0 );
}

int QFormulatorWidget::setTextVerbose( QString mmlText )
{
	clear();
	return insertMathML( __FMLCORE_Product_Title, mmlText, 1 );
}

int QFormulatorWidget::insertNewNode( const QString &btn_id )
{
	long tbr, btn;
	if( ::getCurrentToolBarSet()->getButtonCoordByID( btn_id, tbr, btn ) == -1 )
		return 0;

	return insertNewNode( ::getCurrentToolBarSet()->getButtonCalc( tbr, btn ) );
}

int QFormulatorWidget::insertImage()
{
	void *pData = ::PrepareExtNodeData( EXTNODECONTENT_ID_IMG, 0, document()->getPath() );
	if( !pData ) return 0;
	CExtNode *extNode = ::CreateExtNode( EXTNODECONTENT_ID_IMG, pData );
	if( !extNode ) return 0;
	if( extNode->GetType() != NODE_PLANETEXT || rootNode()->InsertExternalNode( extNode ) == -1 )
	{
		delete extNode;
		return 0;
	}
	updateViewDocument();
	return 1;
}

int QFormulatorWidget::insertSymbol()
{
	CInsertCharacterDialog dlg( backColor(), this );
	if( dlg.exec() == QDialog::Accepted )
	{
		if( dlg.hasStyledText() )
			return insertStyledText( dlg.text(), dlg.currentFont(), dlg.currentColor() );
		else
			return insertSimpleText( dlg.text() );
	}
	return 0;
}

int QFormulatorWidget::insertNewNode( FXmlBtn *ptrLData_FormulaNode )
{
	if( rootNode()->Insert( ptrLData_FormulaNode ) == -1 )
		return 0;
	// just in case we have called some dialogs for the input properties
	setFocus();
	updateViewDocument();
	return 1;
}

int QFormulatorWidget::insertStyledText( QString text, const QFont& currentFont, const QColor& currentColor )
{
	FS_LogFont_Color lfc( currentFont.family(), currentFont.fixedPitch(), currentFont.bold(), currentFont.italic(), currentColor );
	int iRet = rootNode()->InsertPlaneText( text, ::getCurrentFormulatorStyle().addStyle( lfc ), currentFont.pointSize() );
	if( iRet == -1 )
		return 0;
	// just in case we have called some dialogs for the input properties
	setFocus();
	updateViewDocument();
	return 1;
}

int QFormulatorWidget::insertSimpleText( const QString& strText )
{
	if( rootNode()->InsertPlaneTextByDFA( strText ) == -1 )
		return 0;
	// just in case we have called some dialogs for the input properties
	setFocus();
	updateViewDocument();
	return 1;
}

/////////////////////////////////////////////////////////////////////////////

int QFormulatorWidget::fileNew()
{
	document()->afterNew();
	return 1;
}

int QFormulatorWidget::fileOpen( const QString &filePath )
{
	return fileOpen( filePath, true );
}

int QFormulatorWidget::fileOpenSilent( const QString &filePath )
{
	return fileOpen( filePath, false );
}

int QFormulatorWidget::fileReloadSilent( const QString &filePath )
{
	return fileReload( filePath, false );
}

int QFormulatorWidget::fileOpen( const QString &filePath, bool verbose )
{
	QFile file( filePath );
	if( !file.open(QFile::ReadOnly | QFile::Text) ) return 0;
	QTextStream in(&file);
	clear();
	int iRet = insertMathML( filePath, QString(in.readAll()), verbose ? 1 : 0 );
	file.close();
	document()->afterOpen( filePath );
	return iRet;
}

int QFormulatorWidget::fileReload( const QString &filePath, bool verbose )
{
	QFile file( filePath );
	if( !file.open(QFile::ReadOnly | QFile::Text) ) return 0;
	QTextStream in(&file);
	int iRet = replaceMathML( filePath, QString(in.readAll()), verbose ? 1 : 0 );
	file.close();
	document()->afterOpen( filePath );
	return iRet;
}

int QFormulatorWidget::fileSave( const QString &filePath )
{
	QFile file( filePath );
	if( !file.open(QFile::WriteOnly | QFile::Text) ) return 0;
	QTextStream out(&file);
	QString sRes = document()->exportMathML();
	out << sRes;
	file.close();
	document()->afterSave( filePath );
	return 1;
}

int QFormulatorWidget::fileClose()
{
	return 1;
}

int QFormulatorWidget::filePrint()
{
	QPrinter printer;
	QPrintDialog *dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Document"));
	dialog->setOption( QAbstractPrintDialog::PrintSelection, document()->isEnabledCopy() );
	dialog->setOption( QAbstractPrintDialog::PrintPageRange, false );
	if( dialog->exec() != QDialog::Accepted ) return 0;
	print( &printer, dialog->testOption( QAbstractPrintDialog::PrintSelection ) );
	return 1;
}

int QFormulatorWidget::filePageSetup()
{
	RectFde rectIndents;
	indents( rectIndents );
	CPageSetupDialog dlg( rectIndents, lineSpacing(), this );
	if( dlg.exec() == QDialog::Accepted )
	{
		bool recalc = false;
		if( dlg.indents() != rectIndents )
		{
			setIndents( dlg.indents() );
			recalc = true;
		}
		if( dlg.lineSpacing() != lineSpacing() )
		{
			setLineSpacing( dlg.lineSpacing() );
			recalc = true;
		}
		if( recalc )
		{
			rootNode()->RecalculateSizeFull();
			updateFormulatorWidget();
		}
		return 1;
	}
	return 0;
}

void QFormulatorWidget::filePrintPreview()
{
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(this);
    connect(dialog, SIGNAL(paintRequested(QPrinter *)), this, SLOT(print(QPrinter *)));
    dialog->exec();
}

int QFormulatorWidget::fileExportPdf( const QString& filePath )
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QPrinter printer;
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName( filePath );
	if( print( &printer ) == 0 )
	{
		QApplication::restoreOverrideCursor();
		if( isVerbose() )
			QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Failed to open file %1").arg(filePath)));
		return 0;
	}
	QApplication::restoreOverrideCursor();
	return 1;
}

int QFormulatorWidget::fileExportSvg( const QString& filePath, const QString& title )
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QSvgGenerator generator;
	generator.setFileName( filePath );
	generator.setSize( size() );
	generator.setTitle( title );
	generator.setDescription(QString(tr("SVG drawing created by %1 (%2, %3)")).arg(__FMLCORE_Product_Title).arg(__COMPANY_NAME__).arg(__COMPANY_URL_));
	QPainter p;
	if( !p.begin(&generator) )
	{
		QApplication::restoreOverrideCursor();
		if( isVerbose() )
			QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Failed to open file %1").arg(filePath)));
		return 0;
	}
	CFmlDrawEngine *pFde = ::CreateFDE( p );
	if( !pFde )
	{
		QApplication::restoreOverrideCursor();
		p.end();
		return 0;
	}
	draw( pFde, PointFde(0, 0), RectFde(0, 0, size().width(), size().height()), true, false, true );
	::DeleteFDE( pFde );
	p.end();
	QApplication::restoreOverrideCursor();
	return 1;
}

//int QFormulatorWidget::fileEmf( const QString& fileName )
//{
//}

int QFormulatorWidget::fileExportImage( const QString& fileName )
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QPixmap pixmap( size() );
	QPainter p;
	if( !p.begin(&pixmap) )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		if( isVerbose() )
			QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Failed to open file %1").arg(fileName)));
		return 0;
	}
	CFmlDrawEngine *pFde = ::CreateFDE( p );
	if( !pFde )
	{
		QApplication::setOverrideCursor( Qt::WaitCursor );
		p.end();
		return 0;
	}
	pixmap.fill( backColor() );
	draw( pFde, PointFde(0, 0), RectFde(0, 0, size().width(), size().height()), true, false, true );
	::DeleteFDE( pFde );
	p.end();
	QApplication::restoreOverrideCursor();
	return pixmap.save( fileName ) ? 1 : 0;
}

int QFormulatorWidget::fileExportWeb( const QString& fileName, const QString& title )
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	QFile file( fileName );
	if( !file.open(QFile::WriteOnly | QFile::Text) )
	{
		QApplication::restoreOverrideCursor();
		if( isVerbose() )
			QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Failed to open file %1").arg(fileName)));
		return 0;
	}
	QTextStream out(&file);
	QString sRes = rootNode()->to_xhtml( 
			FmlCoreSettings::getXslFile(),
			QString("%1 (%2, %3)").arg(__FMLCORE_Product_Title).arg(__COMPANY_NAME__).arg(__COMPANY_URL_),
			title );
	out << sRes;
	file.close();
	QApplication::restoreOverrideCursor();
	return 1;
}

int QFormulatorWidget::copyAll()
{
	QImage img = drawSelection2Image( false );
	return document()->copyAll( img.isNull() ? 0 : &img );
}

void QFormulatorWidget::viewShowNesting( bool state )
{
	::setCurrentFormulatorNestingState( state ? 1 : 0 );
	updateFormulatorWidget();
}

void QFormulatorWidget::viewShowReadonly( bool state )
{
	::setCurrentFormulatorReadOnlyState( state ? 1 : 0 );
	updateFormulatorWidget();
}

int QFormulatorWidget::refreshViaMathML()
{
	QString strMathML = document()->exportMathML();
	//::clearShouldRefreshMathML();
	return upload( strMathML );
}

int QFormulatorWidget::upload( const QString& strText )
{
	int iRet = 0;
	AST_TXML_Compound* bms = ::CompileMathML( __FMLCORE_Product_Title, strText );
	if( !bms )
	{
		if( isVerbose() )
		{
			bool shouldOverride = false;
			QCursor* currentCursor = QApplication::overrideCursor();
			if( currentCursor && currentCursor->shape() == Qt::WaitCursor )
				shouldOverride = true;
			if( shouldOverride ) QApplication::setOverrideCursor( Qt::ArrowCursor );

			QMessageBox::StandardButton ret = QMessageBox::question( this, __FMLCORE_Product_Title,
				tr("Current MathML text cannot be converted into expression because of error(s). Do you wish to insert this MathML text as a plain text?"),
				QMessageBox::Yes | QMessageBox::No );

			if( shouldOverride ) QApplication::restoreOverrideCursor();

			if( ret == QMessageBox::Yes )
			{
				rootNode()->ClearAll();
				iRet = rootNode()->InsertPlaneTextByDFA( strText );
			}
			else
				return 0;
		}
		else
			return 0;
	}
	else
	{
		rootNode()->ClearAll();
		iRet = (rootNode()->InsertMathML( __FMLCORE_Product_Title, bms ) == -1) ? 0 : 1;
	}

	updateViewDocument();
	return iRet;
}

/////////////////////////////////////////////////////////////////////////////

int QFormulatorWidget::formatDefineStyle( int styleNumber, const QString* styleTitle, const int* styleID )
{
	if( CDefineStyleDialog( backColor(), styleNumber - 1, styleTitle, styleID, this ).exec() == QDialog::Accepted )
	{
		::updateDefaults();
		::FORMULATOR_saveStyles( 1 );
		rootNode()->RecalculateSizeFull();
		updateFormulatorWidget();
		return 1;
	}
	return 0;
}

int QFormulatorWidget::formatDefineSize()
{
	if( CDefineSizeDialog( this ).exec() == QDialog::Accepted )
	{
		::updateDefaults();
		::FORMULATOR_saveStyles( 1 );
		rootNode()->RecalculateSizeFull();
		updateFormulatorWidget();
		return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorWidget::keyPressEvent( QKeyEvent *e )
{
	if( e->matches( QKeySequence::Copy ) )
	{
		if( document()->copyAll() )
		{
			QSize sz( (int) (rootNode()->GetSize().width() + 0.5), (int)  (rootNode()->GetSize().height() + 0.5) );
			QPixmap pixmap( sz );
			draw( &pixmap, PointFde(0, 0), RectFde(0, 0, rootNode()->GetSize().width(), rootNode()->GetSize().height()), true, false, false );
			QApplication::clipboard()->setPixmap( pixmap );
		}
		return;
	}
	else if( e->matches( QKeySequence::ZoomIn ) )
	{
		setScale( qMin( scale() + DEFAULT_VIEWSCALE_STEP, DEFAULT_VIEWSCALE_MAX ) );
		return;
	}
	else if( e->matches( QKeySequence::ZoomOut ) )
	{
		setScale( qMax( scale() - DEFAULT_VIEWSCALE_STEP, DEFAULT_VIEWSCALE_MIN ) );
		return;
	}
	else if( e->matches( QKeySequence::Print ) )
	{
		filePrint();
		return;
	}
	else if( e->matches( QKeySequence::Refresh ) )
	{
		refreshViaMathML();
		return;
	}
}

void QFormulatorWidget::paintEvent( QPaintEvent *e )
{
	draw( this, PointFde(0, 0), e->rect() );
}

/////////////////////////////////////////////////////////////////////////////

int QFormulatorWidget::print( QPrinter *printer, bool printSelectionOnly )
{
    if (!printer /*|| !printer->isValid()*/) return 0;
    QPainter p;
    if( !p.begin( printer ) ) return 0;

	const RealFde dpiScaleX = RealFde(printer->logicalDpiX()) / logicalDpiX();
	const RealFde dpiScaleY = RealFde(printer->logicalDpiY()) / logicalDpiY();
	// scale to dpi
	p.scale( dpiScaleX, dpiScaleY );
	SizeFde szTotal = rootNode()->GetSize();
	szTotal.rwidth() *= dpiScaleX;
	szTotal.rheight() *= dpiScaleY;
	CFmlDrawEngineQt::LPtoDPEx( szTotal, scale(), scale() );
	const SizeFde printerPageSize( printer->pageRect().size() );
	// scale to page
	p.scale( printerPageSize.width() / szTotal.width(), printerPageSize.height() / szTotal.height() );

	int columns = (int) (szTotal.width() / printerPageSize.width()) + 1;
	int rows = (int) (szTotal.height() / printerPageSize.height()) + 1;

	int docCopies, pageCopies;
	if( printer->collateCopies() == true )
	{
		docCopies = 1;
		pageCopies = printer->numCopies();
	}
	else
	{
		docCopies = printer->numCopies();
		pageCopies = 1;
	}

	int fromPage = printer->fromPage();
	int toPage = printer->toPage();
	bool ascending = true;

        if( fromPage < 1 || toPage < 1 || fromPage > toPage )
	{
		fromPage = 1;
		toPage = columns * rows;
	}
	// paranoia check
	fromPage = qMax(1, fromPage);
	toPage = qMin(columns * rows, toPage);

	if( printer->pageOrder() == QPrinter::LastPageFirst )
	{
		int tmp = fromPage;
		fromPage = toPage;
		toPage = tmp;
		ascending = false;
	}

    for( int i = 0; i < docCopies; i++ )
	{
        int page = fromPage;
        while (true)
		{
            for (int j = 0; j < pageCopies; ++j) {
				if( printer->printerState() == QPrinter::Aborted || printer->printerState() == QPrinter::Error )
					return 0;

				CFmlDrawEngine *pFde = ::CreateFDE( p );
				if( !pFde )
				{
					p.end();
					return 0;
				}
				
				RealFde ofsx = ((page - 1) % columns) * printerPageSize.width();
				RealFde ofsy = ((page - 1) / columns) * printerPageSize.height();
				RectFde rcPrinting( 0, 0, printerPageSize.width(), printerPageSize.height() );
				rcPrinting.translate( ofsx, ofsy );

				PointFde ofs( -(ofsx * dpiScaleX), -(ofsy * dpiScaleY) );
				CFmlDrawEngineQt::DPtoLPEx( ofs, scale(), scale() );

				draw( pFde, ofs, rcPrinting, true, printSelectionOnly );

				::DeleteFDE( pFde );
				if( j < pageCopies - 1 )
					printer->newPage();
            }

			if( page == toPage ) break;
			if( ascending )
				page++;
			else
				page--;
			printer->newPage();
        }

        if ( i < docCopies - 1)
            printer->newPage();
    }
	p.end();
	return 1;
}

void QFormulatorWidget::render( QPainter *painter, const RectFde& border )
{
	CFmlDrawEngine *pFde = ::CreateFDE( *painter );
	if( !pFde ) return;
	draw( pFde, PointFde(0, 0), border, true, false, true );
	::DeleteFDE( pFde );
}

void QFormulatorWidget::draw( QPaintDevice *device, PointFde ofs, const RectFde& border, bool printingMode, bool printSelectionOnly, bool useScale )
{
	CFmlDrawEngine *pFde = ::CreateFDE( device );
	if( !pFde ) return;
	draw( pFde, ofs, border, printingMode, printSelectionOnly, useScale );
	::DeleteFDE( pFde );
}

void QFormulatorWidget::draw( CFmlDrawEngine *pFde, PointFde ofs, const RectFde& border, bool printingMode, bool printSelectionOnly, bool useScale )
{
	if( isPrintingMode() )
		printingMode = true;

	RectFde rcf( border );
	pFde->SetPrinting( printingMode );
	if( useScale )
	{
		pFde->Identity();
		pFde->Scale( scale(), scale() );
		pFde->Translate( ofs.x(), ofs.y() );
		CFmlDrawEngineQt::DPtoLPEx( rcf, scale(), scale() );
		pFde->SetClip( rcf );
	}

	if( !printingMode )
	{
		std::vector<RectFde> sel;
		::DrawSelection( rcf, rootNode()->GetSelectInfo(), sel );
		for( long i = 0; i < (long) sel.size(); i++ )
		{
			RectFde rect( PointFde(sel.at(i).left() + ofs.x(), sel.at(i).top() + ofs.y()),
						  PointFde(sel.at(i).right() + ofs.x(), sel.at(i).bottom() + ofs.y()) );
			if( rect.isNull() ) continue;
			pFde->FillRectangle( rect.left(), rect.top(), rect.width(), rect.height(), FS_LogBrush(QColor(0x7F, 0x7F, 0x7F, 128), Qt::SolidPattern) );
		}
	}

	if( printSelectionOnly && rootNode()->IsSelection() )
	{
		CFrameNode *pFrameNode = rootNode()->RemoveOrCopySelection( 0 );
		if( pFrameNode )
		{
			CRootNode *pRootNode = new CRootNode( this );
			if( pRootNode->Insert( pFrameNode ) == 0 )
				pRootNode->Draw( *pFde, PointFde( 0.0, 0.0 ), rcf );
			delete pFrameNode;
			delete pRootNode;
		}
	}
	else
		rootNode()->Draw( *pFde, PointFde( 0.0, 0.0 ), rcf );

	if( useScale && !printingMode && m_cursorOn )
		pFde->FillRectangle( caretPos().x(), caretPos().y(), caretSize().width(), caretSize().height(), FS_LogBrush(QColor(0, 0, 0), Qt::SolidPattern) );
}

QPixmap QFormulatorWidget::drawSampleText( long style, int pointSize, QString txt, QSize size )
{
	const FS_LogFont_Color& lfc = ::getFontStyle( style );
	QFont font( lfc.m_name, pointSize, lfc.m_isBold ? QFont::Bold : QFont::Normal, lfc.m_isItalic ? true : false );
	font.setFixedPitch( lfc.m_isFixed );
	font.setStyleHint( lfc.m_hint );
	return drawSampleText( font, lfc.m_color, txt, size );
}

QPixmap QFormulatorWidget::drawSampleText( QFont font, QColor color, QString txt, QSize size )
{
	const int margin = 3;
	QRect rect(QPoint(0, 0), size);
	if( rect.width() == 0 || rect.height() == 0 )
		rect = QFontMetrics(font).boundingRect(QRect(0, 0, 0, 0), Qt::TextDontClip, txt).adjusted( 0, 0, margin + margin, margin + margin);
	QPixmap pixmap(rect.size());
	pixmap.fill( Qt::white );
	QPainter p;
	if( p.begin( &pixmap ) )
	{
		QRect frame(QPoint(0, 0), pixmap.size());
		p.setPen( QColor(0, 0, 0) );
		p.drawRect( frame.adjusted(1, 1, -2, -2) );
		p.setPen( color );
		p.setFont( font );
		p.drawText( frame, Qt::AlignVCenter | Qt::AlignCenter, txt );
		p.end();
	}
	return pixmap;
}

QImage QFormulatorWidget::drawSelection2Image( bool useSelection )
{
	return drawSelection2QPixmap( useSelection ).toImage();
}

QPixmap QFormulatorWidget::drawSelection2QPixmap( bool useSelection )
{
	CFrameNode *pFrameNode = 0, *pParentFrameNode = 0;
	SizeFde sz, szLogical;
	RealFde l = 0.0, t = 0.0, r = 0.0, b = 0.0;
	long nLevel = 0;
	if( useSelection )
	{
		if( rootNode()->IsSelection() )
		{
			const SelectNode& sn = rootNode()->GetSelectInfo().GetBegin();
			if( sn.GetNode() && (pParentFrameNode = sn.GetNode()->GetFrameNode()) != 0 )
				nLevel = pParentFrameNode->GetLevel();
		}
		pFrameNode = rootNode()->RemoveOrCopySelection( 0 );
		if( !pFrameNode ) return QPixmap();
		sz = szLogical = pFrameNode->GetSize();
	}
	else
	{
		indents( l, t, r, b );
		szLogical.rwidth() = rootNode()->GetSize().width() - l - r;
		szLogical.rheight() = rootNode()->GetSize().height() - t - b;
		sz = szLogical;
	}
	CFmlDrawEngineQt::LPtoDPEx( sz, scale(), scale() );
	QPixmap pixmap( (int) (sz.width() + 0.5), (int) (sz.height() + 0.5) );
	PointFde offs( 0, 0 );

	int iRet = 0;
	QPainter p;
	if( p.begin(&pixmap) )
	{
		CFmlDrawEngine *pFde = ::CreateFDE( p );
		if( pFde )
		{
			pFde->SetPrinting( true );
			pFde->Identity();
			pFde->Scale( scale(), scale() );
			pixmap.fill( backColor() );
			if( useSelection )
			{
				CRootNode *pRootNode = new CRootNode( this );
				pRootNode->setIndents( 0, 0, 0, 0 );
				if( pRootNode->Insert( pFrameNode ) == 0 )
				{
					if( nLevel > 0 )
					{
						pRootNode->SetLevel( nLevel );
						pRootNode->RecalculateSizeFull();
					}
					pRootNode->Draw( *pFde, offs, RectFde(offs, szLogical) );
					iRet = 1;
				}
				delete pFrameNode;
				delete pRootNode;
			}
			else
			{
				rootNode()->setIndents( 0, 0, 0, 0 );
				rootNode()->RecalculateSize( 0 );
				rootNode()->Draw( *pFde, offs, RectFde(offs, szLogical) );
				iRet = 1;
				rootNode()->setIndents( l, t, r, b );
				rootNode()->RecalculateSize( 0 );
			}
			::DeleteFDE( pFde );
		}
		p.end();
	}
	return iRet ? pixmap : QPixmap();
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorWidget::setScale( double value )
{
	if( scale() == value ) return;
	m_viewScale = value;
	emit scaleChangedSignal();
	updateFormulatorWidget();
}

void QFormulatorWidget::setLineSpacing( RealFde value )
{
	rootNode()->SetLineSpacing( value );
	rootNode()->RecalculateSizeFull();
	updateFormulatorWidget();
}

RealFde QFormulatorWidget::lineSpacing()
{
	return rootNode()->GetLineSpacing();
}

void QFormulatorWidget::indents( RealFde& l, RealFde& t, RealFde& r, RealFde& b )
{
	l = rootNode()->getLeftIndent();
	t = rootNode()->getTopIndent();
	r = rootNode()->getRightIndent();
	b = rootNode()->getBottomIndent();
}

void QFormulatorWidget::setIndents( RealFde l, RealFde t, RealFde r, RealFde b )
{
	rootNode()->setIndents( l, t, r, b );
	rootNode()->RecalculateSize( 0 );
	updateFormulatorWidget();
}

void QFormulatorWidget::indents( RectFde& rect )
{
	rect.setLeft( rootNode()->getLeftIndent() );
	rect.setTop( rootNode()->getTopIndent() );
	rect.setRight( rootNode()->getRightIndent() );
	rect.setBottom( rootNode()->getBottomIndent() );
}

void QFormulatorWidget::setIndents( RectFde rect )
{
	rootNode()->setIndents( rect.left(), rect.top(), rect.right(), rect.bottom() );
	rootNode()->RecalculateSize( 0 );
	updateFormulatorWidget();
}

QString QFormulatorWidget::caretInfo()
{
	QString result;
	rootNode()->GetCaretMessage( result );
	return result;
}

/////////////////////////////////////////////////////////////////////////////

void QFormulatorWidget::initialUpdate()
{
	Q_ASSERT( undoRedo() && rootNode() );

	// перед открытием проверяем инициализацию, после открытия никак нельзя, 
	// потому что инициализация происходит через редактирование текущего документа
	if( rootNode()->getAutodetectMulticharOp() == 0 )
		rootNode()->createAutodetectMulticharOp();

	if( undoRedo()->GetUndoCount() == 0 )
		undoRedo()->Init();

 	rootNode()->RecalculateSizeFull();

	// check if missing buttons exist
	QString sum = _T("");
	HKMap<QString,int> miss_b;
	rootNode()->iterateNodeTree( write2log_MissButton, &miss_b );
	if( miss_b.tofirst() )
	{
		int v;
		QString s = _T("");
		miss_b.get( sum, v );
		while( miss_b.tonext() )
		{
			miss_b.get( s, v );
			sum += _T(", ") + s;
		}
	}
	if( sum.length() )
		getCurrentDRMessageManager()->AddLine( 0, QString("some buttons are missing (%1)").arg(sum), -1 );

	// use global user settings about MathML translation preferences
	::mml_UsePreference_ContentTimes( ::getCurrentFormulatorContentMathML_TranslateTimes() );

	updateFormulatorWidget();
}

void QFormulatorWidget::setCheckPoint( int fForcedCheck )
{
	undoRedo()->CheckPoint( fForcedCheck );
}

void QFormulatorWidget::updateViewDocument()
{
	setCheckPoint();
	updateFormulatorWidget();
	setModified();
	setMmlChanged( true );

	emit docModifiedSignal( true );
}

void QFormulatorWidget::recalculateSize()
{
	rootNode()->RecalculateSizeFull();
	updateFormulatorWidget();
}

int QFormulatorWidget::updateFormulatorWidget()
{
	int nRet = 0;
	if( updateCaret() ) nRet = 1;
	if( updateScrollSize() ) nRet = 1;
	if( updateScrollPosition() ) nRet = 1;
	if( updateCurrentKegl() ) nRet = 1;
	emit copyAvailableSignal( document()->isEnabledCopy() );
	emit widgetUpdateSignal();
	update();
	return nRet;
}

int QFormulatorWidget::updateCaret()
{
	PointFde opt(caretPos()), ptf( 0.0, 0.0 );
	SizeFde osz(caretSize()), szf( 0.0, 0.0 );
	if( rootNode()->GetCaretInfo( ptf, szf ) != 0 ) return 0;
	setCaretPos( ptf );
	setCaretSize( szf );
	repaintCursor( opt, osz );
	repaintCursor();
	return 0;
} 

int QFormulatorWidget::updateScrollSize( void )
{
	SizeFde szf = rootNode()->GetSize();
	SizeFde sz( szf );
	CFmlDrawEngineQt::LPtoDPEx( sz, scale(), scale() );
	QSize szV( (int) (sz.width() + 0.5), (int) (sz.height() + 0.5) );
	resize( szV );
	setBaseSize( szV );
	return 1;
}

int QFormulatorWidget::updateScrollPosition()
{
	return updateScrollPosition( caretPos(), caretSize() );
}

int QFormulatorWidget::updateScrollPosition( PointFde pt, SizeFde sz, int xmargin, int ymargin )
{
	QScrollArea *parentScrollArea = m_parentWindow ? qobject_cast<QScrollArea*>(m_parentWindow) : 0;
	if( !parentScrollArea ) return 0;
	PointFde ptf = pt;
	PointFde ptf2( pt.x() + sz.width(), pt.y() + sz.height() );
	CFmlDrawEngineQt::LPtoDPEx( ptf, scale(), scale() );
	CFmlDrawEngineQt::LPtoDPEx( ptf2, scale(), scale() );
	parentScrollArea->ensureVisible( (int) (ptf.x() + 0.5), (int) (ptf.y() + 0.5), xmargin, ymargin );
	parentScrollArea->ensureVisible( (int) (ptf2.x() + 0.5), (int) (ptf2.y() + 0.5), xmargin, ymargin);
	return 1;
}

int QFormulatorWidget::updateCurrentKegl( void )
{
	::setCurrentKegl( rootNode()->GetCurrentKegl() );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int QFormulatorWidget::setStyleOther()
{
	COtherStyleDialog dlg( backColor(), this );
	if( dlg.exec() == QDialog::Accepted )
	{
		FS_LogFont_Color lfc( dlg.currentFont().family(), dlg.currentFont().fixedPitch(), dlg.currentFont().bold(), dlg.currentFont().italic(), dlg.currentColor() );
		long styleNo = ::getCurrentFormulatorStyle().getStyleNo( lfc );
		if( styleNo == -1 )
			styleNo = ::getCurrentFormulatorStyle().addStyle( lfc );

		::setCurrentUserChoiceStyle( styleNo );
		applyStyle( styleNo );
		setFocus();
		return 1;
	}
	setFocus();
	return 0;
}

void QFormulatorWidget::applyStyle( long style )
{
	if( rootNode()->ApplyStyle( style ) == 0 )
		updateViewDocument();
}

int QFormulatorWidget::setSizeOther()
{
	bool ok;
	int valueKegl = QInputDialog::getInt(this, tr("Select Other Size"), tr("Font Size (pt):"),
						(int) ::getCurrentKegl(), DEFAULT_SYMBOLSIZE_MIN, DEFAULT_SYMBOLSIZE_MAX, 1, &ok);
	setFocus();
	if( ok )
	{
		applyKegl( valueKegl );
		return 1;
	}
	return 0;
}

void QFormulatorWidget::applyKegl( RealFde kegl )
{
	if( canApplyKegl( kegl ) )
	{
		if( rootNode()->ApplyKegl( kegl ) == 0 )
			updateViewDocument();
		::setCurrentKegl( kegl );
	}
}

void QFormulatorWidget::incDecKegl( RealFde value )
{
	RealFde kegl = ::getCurrentKegl();
	if( canApplyKegl( kegl + value ) )
	{
		if( rootNode()->IncDecKegl( value ) == 0 )
			updateViewDocument();
		::setCurrentKegl( kegl + value );
	}
}

/////////////////////////////////////////////////////////////////////////////

int QFormulatorWidget::evaluate()
{
	QApplication::setOverrideCursor( Qt::WaitCursor );
	CFormulaNode *formulaNode = rootNode()->GetContentFormulaUnderCaret();
	if( formulaNode == 0 )
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Evaluation is unavailable.\n\nFailed to find Content MathML node under the caret.")));
		return 0;
	}
	bool recalc = rootNode()->PrepareContentEqual2Evaluate( formulaNode, undoRedo() );

	long cb;
	QString cmmlText = formulaNode->to_mathml( 1, cb );
	cmmlText = CFrameNode::to_mathml_pureContent( QString("<math>%1</math>").arg(cmmlText) );

	QFormulatorCalc calc("");
	//int iRet = calc.compile( cmmlText );

	formulaNode->setIgnoreContentConversion();
	QString varCmmlDefinition = document()->exportPureContentMathML( true /*ignore cache*/ );
	formulaNode->clearIgnoreContentConversion();

	QString calcResult;
	int iRet = calc.compile( varCmmlDefinition, cmmlText );
	if( iRet != 0 || calc.evaluate() != 0 || (calcResult = calc.lastItemValue()).isEmpty() )
	{
		QStringList errMsg = calc.report();
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(this, __FMLCORE_Product_Title, QString(tr("Evaluation is unavailable.\n\n%1")).arg(errMsg.join("\n")));
		return 0;
	}

	//QStringList calcResultLines = calc.itemsValue( cmmlText );
	//QString calcResult = calcResultLines.join("\n");
	//if( !calcResult.isEmpty() )
	//{
		if( recalc )
			insertMathML( __FMLCORE_Product_Title, calcResult, 0 );
		else
		{
			if( rootNode()->MoveCaret( formulaNode, 1 ) )
			{
				rootNode()->Char( '=', undoRedo() );
				insertMathML( __FMLCORE_Product_Title, calcResult, 0 );
			}
		}
	//}

	QApplication::restoreOverrideCursor();
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
