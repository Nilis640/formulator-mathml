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
#include "mmlfileviewerwidget.h"

static void updateComboBox(QComboBox *comboBox);
#define DEFAULT_MASK "*.mml"
#define MAX_FILE_SIZE 0x100000
/////////////////////////////////////////////////////////////////////////////

QMathMLFileViewer::QMathMLFileViewer( QWidget *parent )
    : QDockWidget( tr("MathML Documents Explorer"), parent )
{
	m_iconSize = QSize(16, 16);
	m_isRecursiveSearch = true;

	m_currentDir.setPath( QDir::currentPath() );

	directoryComboBox = createComboBox( m_currentDir.absolutePath() );
	directoryComboBox->setMinimumWidth(150);
	QDirModel *dirModelCompleter = new QDirModel(this);
	dirModelCompleter->setFilter( QDir::AllDirs/*QDir::Dirs*/ );
	QCompleter *completer = new QCompleter( this );
    completer->setModel( dirModelCompleter );
    directoryComboBox->setCompleter( completer );
	connect(directoryComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(changeDir(const QString&)));

	QToolButton *btnBrowse = new QToolButton();
	btnBrowse->setIcon(QIcon(":/images/folderopen.png"));
	//btnBrowse->setIconSize( m_iconSize );
	btnBrowse->setToolTip(tr("Select a new folder"));
	connect(btnBrowse, SIGNAL(clicked()), this, SLOT(browse()));

	QToolButton *btnBrowseUp = new QToolButton();
	btnBrowseUp->setIcon(QIcon(":/images/up.png"));
	//btnBrowseUp->setIconSize( m_iconSize );
	btnBrowseUp->setToolTip(tr("Navagate one level up"));
	connect(btnBrowseUp, SIGNAL(clicked()), this, SLOT(browseUp()));

	QHBoxLayout *dirLayout = new QHBoxLayout();
	dirLayout->addWidget(directoryComboBox);
	dirLayout->addWidget(btnBrowseUp);
	dirLayout->addWidget(btnBrowse);
	dirLayout->addStretch(1);

	createFilesTree();
	createFilesTable();
	foundMessage = new QLabel(tr("No search results"));

	QHBoxLayout *toolLayout = setupToolLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(toolLayout);
    mainLayout->addLayout(dirLayout);
    mainLayout->addWidget(m_dirTree);
    mainLayout->addWidget(filesTable);
	mainLayout->addWidget(foundMessage);

	QDialog *centralDlg = new QDialog();
	centralDlg->setModal( false );
	centralDlg->setLayout( mainLayout );
	setWidget( centralDlg );
}

void QMathMLFileViewer::setRecursive( bool enable )
{
	m_isRecursiveSearch = enable;
}

/////////////////////////////////////////////////////////////////////////////

int QMathMLFileViewer::isValidSelection(QString filePath, int noParanoiaCheck)
{
	if( !filePath.isEmpty() )
	{
		QFileInfo info(filePath);
		if( !info.isFile() || !info.isReadable() )
			return 0;
		if( !noParanoiaCheck )
		{
			QStringList extList = fileComboBox->currentText().trimmed().split(".");
			if( (extList.size() > 0 && info.suffix() != extList.last()) || info.size() >= MAX_FILE_SIZE )
				return 0;
		}
		return 1;
	}
	return 0;
}

int QMathMLFileViewer::isValidSelection(int noParanoiaCheck)
{
	return isValidSelection(getSelected(), noParanoiaCheck);
}

QString QMathMLFileViewer::getSelected()
{
	if( filesTable && filesTable->hasFocus() )
	{
		int row = filesTable->currentRow();
		QTableWidgetItem *item = filesTable->item(row, 0);
			return m_currentDir.absoluteFilePath(item->text());
	}
	else if( m_dirTree /*&& m_dirTree->hasFocus()*/ )
	{
		const QItemSelectionModel *selModel = m_dirTree->selectionModel();
		QModelIndexList list = selModel->selectedIndexes();
		if( !list.isEmpty() )
		{
			QModelIndex item = list.first();
			return m_dirModel.filePath( item );
		}
	}
	return QString();
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLFileViewer::fileBack()
{
	int idx = filesTable->currentRow();
	if( idx > 0 )
	{
		idx--;
		filesTable->setCurrentCell( idx, 0 );
		//loadFileOfItem( idx, 0 );
	}
}

void QMathMLFileViewer::fileForward()
{
	int idx = filesTable->currentRow();
	if( idx >= 0 && idx < filesTable->rowCount() - 1 )
	{
		idx++;
		filesTable->setCurrentCell( idx, 0 );
		//loadFileOfItem( idx, 0 );
	}
}

void QMathMLFileViewer::fileRefresh()
{
	m_dirModel.refresh();
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLFileViewer::fileFind()
{
	filesTable->setRowCount(0);

	QString fileName = fileComboBox->currentText();
	QString path = directoryComboBox->currentText();
	updateComboBox(fileComboBox);
	updateComboBox(directoryComboBox);
	m_currentDir = QDir(path);
	if( fileName.isEmpty() ) fileName = DEFAULT_MASK;

	//QApplication::setOverrideCursor( Qt::WaitCursor );

	QStringList files;
	if( isRecursive() )
	{
		QProgressDialog progress("Searching files...", "Abort Search", 0, 100, this);
		progress.setWindowModality(Qt::WindowModal);
		progress.setMinimumDuration( 500 );
		progress.setValue( 0 );
		fileFind( files, m_currentDir, QStringList(fileName), &progress );
		//progress.setValue( 100 + files.size() );
		progress.setValue( progress.maximum() );
	}
	else
	{
		//QStringList newFiles = m_currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
		//for( QStringList::const_iterator item = newFiles.constBegin(); item != newFiles.constEnd(); item++ )
		//	files.append(m_currentDir.absoluteFilePath(*item));
		files = m_currentDir.entryList(QStringList(fileName), QDir::Files | QDir::NoSymLinks);
	}

	if(files.size() > 0)
	{
		filesTable->show();
		showFiles(files);
		filesTable->resizeRowsToContents();
		filesTable->resizeColumnsToContents();
		foundMessage->setText(QString(tr("%1 file(s) shown of %2 found").arg(filesTable->rowCount()).arg(files.size())));
		emit hasFound( true );
	}
	else
	{
		filesTable->hide();
		foundMessage->setText(QString(tr("No files found to match %1")).arg(fileName));
		emit hasFound( false );
	}

	//QApplication::restoreOverrideCursor();
}

void QMathMLFileViewer::fileFind( QStringList& files, const QDir& rootDir, const QStringList& mask, QProgressDialog *progressDlg )
{
	QCoreApplication::processEvents();
	if( progressDlg->wasCanceled() ) return;
	QStringList::const_iterator item;
    QStringList newFiles = rootDir.entryList(QStringList(mask), QDir::Files | QDir::NoSymLinks, QDir::Name);
	if( newFiles.size() )
	{
		for( item = newFiles.constBegin(); item != newFiles.constEnd(); item++ )
		{
			QCoreApplication::processEvents();
			if( progressDlg->wasCanceled() ) return;
			files.append(m_currentDir.relativeFilePath(rootDir.absoluteFilePath(*item)));
		}
	}
	if( progressDlg->value() < progressDlg->maximum() - 1 )
		progressDlg->setValue( progressDlg->value() + 1 );
	QStringList newDirs = rootDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);
	progressDlg->setMaximum( progressDlg->maximum() + newDirs.size() + 1 );
	for( item = newDirs.constBegin(); item != newDirs.constEnd(); item++ )
	{
		if( progressDlg->value() < progressDlg->maximum() - 1 )
			progressDlg->setValue( progressDlg->value() + 1 );
		QCoreApplication::processEvents();
		if( progressDlg->wasCanceled() ) return;
		QDir itemDir( rootDir );
		if( itemDir.cd(*item) )
			fileFind( files, itemDir, mask, progressDlg );
	}
}

/////////////////////////////////////////////////////////////////////////////

QWidget* QMathMLFileViewer::getFocusItem()
{
	if( filesTable && filesTable->hasFocus() )
		return filesTable;
	else if( m_dirTree )
		return m_dirTree;
	return this;
}

/////////////////////////////////////////////////////////////////////////////

int QMathMLFileViewer::emitLoad( QString filePath, int noParanoiaCheck )
{
	int iRet = isValidSelection(filePath, noParanoiaCheck);
	if( iRet ) emit loadFile( filePath, getFocusItem() );
	return iRet;
}

void QMathMLFileViewer::fileLoad()
{
	emitLoad(getSelected(), 1);
}

void QMathMLFileViewer::loadFileOfItem(const QModelIndex& index)
{
	emitLoad(m_dirModel.filePath( index ), 0);
}

void QMathMLFileViewer::loadFileOfItem(int row, int /* column */)
{
    QTableWidgetItem *item = filesTable->item(row, 0);
	if( item )
		emit hasSelectedItem( emitLoad( m_currentDir.absoluteFilePath( item->text() ), 0 ) != 0 );
}

void QMathMLFileViewer::loadFileOfItem(QTableWidgetItem *current, QTableWidgetItem * /*previous*/)
{
	if( current )
		emit hasSelectedItem( emitLoad( m_currentDir.absoluteFilePath( current->text() ), 0 ) != 0 );
}

/////////////////////////////////////////////////////////////////////////////

int QMathMLFileViewer::emitOpen( QString filePath, int noParanoiaCheck )
{
	int iRet = isValidSelection(filePath, noParanoiaCheck);
	if( iRet ) emit openFile( filePath, getFocusItem() );
	return iRet;
}

void QMathMLFileViewer::fileOpen()
{
	emitOpen( getSelected(), 1 );
}

void QMathMLFileViewer::openFileOfItem(const QModelIndex& index)
{
	emitOpen( m_dirModel.filePath( index ), 0 );
}

void QMathMLFileViewer::openFileOfItem(int row, int /* column */)
{
    QTableWidgetItem *item = filesTable->item(row, 0);
	if( item )
		emit hasSelectedItem( emitOpen( m_currentDir.absoluteFilePath( item->text() ), 0 ) != 0 );
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLFileViewer::pressed(const QModelIndex& /*index*/)
{
	emit hasSelectedItem( isValidSelection( 1 ) );
}

void QMathMLFileViewer::activated(const QModelIndex& index)
{
	changeDir( m_dirModel.filePath(index) );
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLFileViewer::browse()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Find Files"), m_currentDir.absolutePath());
	changeDir( directory );
}

void QMathMLFileViewer::browseUp()
{
	changeDir( ".." );
}

void QMathMLFileViewer::changeDir( QString path )
{
    if( !path.isEmpty() && m_currentDir.cd( path ) )
	{
		directoryComboBox->blockSignals(true);

		path = m_currentDir.absolutePath();
		if( directoryComboBox->findText(path) == -1 )
			directoryComboBox->addItem( path );
		directoryComboBox->setCurrentIndex( directoryComboBox->findText( path ) );

		const QModelIndex index = m_dirModel.index( path );
		m_dirTree->scrollTo( index );
		if( m_dirTree->currentIndex() != index )
			m_dirTree->setCurrentIndex( index );

		directoryComboBox->blockSignals(false);
	}
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLFileViewer::showFiles(const QStringList &files)
{
	const int base = 10;
	int maxValue = (int) (files.size() / base);
	QProgressDialog progress("Showing files...", "Cancel", 0, maxValue, this);
	progress.setWindowModality(Qt::WindowModal);
	progress.setMinimumDuration(500);
    for( int i = 0; i < files.size(); i++ )
	{
		if( i % base == 0 )
			progress.setValue( i / 10 );
		if( progress.wasCanceled() )
			break;

		QString filePath = m_currentDir.absoluteFilePath(files[i]);
		QFile file( filePath );
		QDateTime fileDate = QFileInfo( file ).lastModified();

        QTableWidgetItem *fileNameItem = new QTableWidgetItem( files[i] );
        fileNameItem->setFlags( fileNameItem->flags() ^ Qt::ItemIsEditable );
        //QTableWidgetItem *fileDateItem = new QTableWidgetItem( fileDate.toString(Qt::SystemLocaleShortDate) );
        //fileDateItem->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        //fileDateItem->setFlags( fileDateItem->flags() ^ Qt::ItemIsEditable );

        int row = filesTable->rowCount();
        filesTable->insertRow( row );
        filesTable->setItem( row, 0, fileNameItem );
        //filesTable->setItem( row, 1, fileDateItem );
    }
	progress.setValue( maxValue );
}

/////////////////////////////////////////////////////////////////////////////

void QMathMLFileViewer::readSettings( QSettings& settings )
{
	QString path = settings.value("FmlIde/mmlfileviewer/dir", QVariant(QDir::currentPath())).toString();
	changeDir( path );
	//if( m_dirTree )
	//	m_dirTree->setCurrentIndex( m_dirModel.index( m_currentDir.absolutePath() ) );
	setRecursive(settings.value("FmlIde/mmlfileviewer/recursive", QVariant(true)).toBool());
	if( btnRecursive ) btnRecursive->setChecked( isRecursive() );
}

void QMathMLFileViewer::writeSettings( QSettings& settings )
{
	settings.setValue("FmlIde/mmlfileviewer/dir", m_currentDir.absolutePath() );
	settings.setValue("FmlIde/mmlfileviewer/recursive", isRecursive());
}

/////////////////////////////////////////////////////////////////////////////

static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}

QComboBox *QMathMLFileViewer::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void QMathMLFileViewer::createFilesTree()
{
	m_dirModel.setSorting( QDir::Name );
	m_dirModel.setLazyChildCount( true );

	m_dirTree = new QTreeView();
    m_dirTree->setModel( &m_dirModel );
    m_dirTree->setIndentation(20);
	m_dirTree->setSelectionMode( QAbstractItemView::SingleSelection );
	m_dirTree->setColumnWidth(0, 100);
	m_dirTree->hideColumn(1);
	m_dirTree->hideColumn(2);
	m_dirTree->hideColumn(3);
	//const QModelIndex& index = m_dirModel.index( m_currentDir.absolutePath() );
	//m_dirTree->scrollTo( index );
	//m_dirTree->setCurrentIndex( index );
	m_dirTree->sortByColumn(0, Qt::AscendingOrder);
    //m_dirTree->setSortingEnabled(true);
	connect(m_dirTree, SIGNAL(activated(const QModelIndex&)), this, SLOT(activated(const QModelIndex&)));
	connect(m_dirTree, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressed(const QModelIndex&)));
    connect(m_dirTree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(loadFileOfItem(const QModelIndex&)));
    //connect(m_dirTree, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(openFileOfItem(const QModelIndex&)));
}

void QMathMLFileViewer::createFilesTable()
{
    filesTable = new QTableWidget(0, 1);
    filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStringList labels;
    labels << tr("File Name");// << tr("Date");
    filesTable->setHorizontalHeaderLabels(labels);
    //filesTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
	filesTable->setWordWrap(false);
    filesTable->verticalHeader()->hide();
    filesTable->setShowGrid(false);
	filesTable->hide();

	//connect(filesTable, SIGNAL(cellClicked(int, int)), this, SLOT(loadFileOfItem(int, int)));
	connect(filesTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(loadFileOfItem(QTableWidgetItem*, QTableWidgetItem*)));
	//connect(filesTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(openFileOfItem(int, int)));
}

QHBoxLayout* QMathMLFileViewer::setupToolLayout()
{
	QHBoxLayout *toolLayout = new QHBoxLayout();

	QToolButton *btnRefresh = new QToolButton();
	btnRefresh->setIcon(QIcon(":/images/reload.png"));
	//btnRefresh->setIconSize(m_iconSize);
	btnRefresh->setToolTip(tr("Refresh contents of the current directory"));
	connect(btnRefresh, SIGNAL(clicked()), this, SLOT(fileRefresh()));
	toolLayout->addWidget( btnRefresh );

	QToolButton *btnView = new QToolButton();
	btnView->setIcon(QIcon(":/images/printpreview.png"));
	//btnView->setIconSize(m_iconSize);
	btnView->setToolTip(tr("Open the selected file as a new read-only window in the editor - for viewing purposes only"));
	connect(btnView, SIGNAL(clicked()), this, SLOT(fileLoad()));
	connect(this, SIGNAL(hasSelectedItem(bool)), btnView, SLOT(setEnabled(bool)));
	btnView->setEnabled( false );
	toolLayout->addWidget( btnView );

	QToolButton *btnOpen = new QToolButton();
	btnOpen->setIcon(QIcon(":/images/open.png"));
	//btnOpen->setIconSize(m_iconSize);
	btnOpen->setToolTip(tr("Open the selected file as a new window in the editor"));
	connect(btnOpen, SIGNAL(clicked()), this, SLOT(fileOpen()));
	connect(this, SIGNAL(hasSelectedItem(bool)), btnOpen, SLOT(setEnabled(bool)));
	btnOpen->setEnabled( false );
	toolLayout->addWidget( btnOpen );

	toolLayout->addSpacing( 12 );

	QLabel *labelMask = new QLabel(tr("Mask:"));
    fileComboBox = createComboBox(tr(DEFAULT_MASK));
	fileComboBox->setMinimumWidth( 80 );
	//fileComboBox->setIconSize(m_iconSize);
	toolLayout->addWidget( labelMask );
	toolLayout->addWidget( fileComboBox );

	QToolButton *btnFind = new QToolButton();
	btnFind->setIcon(QIcon(":/images/find.png"));
	//btnFind->setIconSize(m_iconSize);
	btnFind->setToolTip(tr("Find all files with a given name starting from the current directory"));
	connect(btnFind, SIGNAL(clicked()), this, SLOT(fileFind()));
	toolLayout->addWidget( btnFind );

	btnRecursive = new QToolButton();
	btnRecursive->setObjectName("__qt__fmlide_widget_QMathMLFileViewer_button_Recursive");
	btnRecursive->setIcon(QIcon(":/images/contents.png"));
	btnRecursive->setToolTip(tr("Set on/off recursive search from the current folder"));
	btnRecursive->setToolButtonStyle( Qt::ToolButtonIconOnly );
	btnRecursive->setCheckable( true );
	btnRecursive->setChecked( isRecursive() );
	connect(btnRecursive, SIGNAL(toggled(bool)), this, SLOT(setRecursive(bool)));
	toolLayout->addWidget( btnRecursive );

	QToolButton *btnBack = new QToolButton();
	btnBack->setIcon(QIcon(":/images/back.png"));
	//btnBack->setIconSize(m_iconSize);
	btnBack->setToolTip(tr("Select the previous found file"));
	connect(btnBack, SIGNAL(clicked()), this, SLOT(fileBack()));
	connect(this, SIGNAL(hasFound(bool)), btnBack, SLOT(setEnabled(bool)));
	btnBack->setEnabled( false );
	toolLayout->addWidget( btnBack );

	QToolButton *btnForward = new QToolButton();
	btnForward->setIcon(QIcon(":/images/forward.png"));
	//btnForward->setIconSize(m_iconSize);
	btnForward->setToolTip(tr("Select the next found file"));
	connect(btnForward, SIGNAL(clicked()), this, SLOT(fileForward()));
	connect(this, SIGNAL(hasFound(bool)), btnForward, SLOT(setEnabled(bool)));
	btnForward->setEnabled( false );
	toolLayout->addWidget( btnForward );

	toolLayout->addStretch( 1 );

	return toolLayout;
}

/////////////////////////////////////////////////////////////////////////////
