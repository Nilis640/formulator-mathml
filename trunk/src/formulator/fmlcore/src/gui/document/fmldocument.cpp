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

#include <QApplication>
#include <QClipboard>
#include <QMimeData>

#include "fmldocument.h"
#include "../widget/fmlbasewidget.h"

#include "../../settings/options.h"
#include "../../version/version.h"
#include "../../nodes/ni_root.h"

/////////////////////////////////////////////////////////////////////////////

CFormulatorDoc::CFormulatorDoc( QFormulatorBaseWidget *owner )
{
	m_mmlCache = "";
	m_cmmlCache = "";
	m_owner = owner;
	m_pRootNode	= new CRootNode( owner );
	m_pUndoRedo	= new CUndoRedo( this );
	if( undoRedo()->GetUndoCount() == 0 )
		undoRedo()->Init();
	m_path = "";
	setModified( false );
	setMmlChanged( true );
}

CFormulatorDoc::~CFormulatorDoc()
{
	if( m_pRootNode ) delete m_pRootNode;
	if( m_pRootNode ) delete m_pUndoRedo;
}

/////////////////////////////////////////////////////////////////////////////

QString CFormulatorDoc::exportMathML( CRootNode *pRootNode )
{
	if( !pRootNode )
		return QString();
	//else if( pRootNode == rootNode() )
	//	return exportMathML();

	QString sRet = ::isPureContentMathMLOutput() ? pRootNode->to_mathml_pureContent() : pRootNode->to_mathml();
	::hstr_0a_to_0d0a( sRet );
	::hstr_del_redundant_0d0a( sRet );
	return sRet;
}

QString CFormulatorDoc::exportMathML()
{
	if( isMmlChanged() )
	{
		QString sRet = ::isPureContentMathMLOutput() ? rootNode()->to_mathml_pureContent() : rootNode()->to_mathml();
		::hstr_0a_to_0d0a( sRet );
		::hstr_del_redundant_0d0a( sRet );
		m_mmlCache = sRet;
		setMmlChanged( false );
	}
	return m_mmlCache;
}

QString CFormulatorDoc::exportPureContentMathML( bool ignoreCache )
{
	if( !ignoreCache )
	{
		if( isMmlChanged() )
		{
			QString sRet = rootNode()->to_mathml_pureContent();
			::hstr_0a_to_0d0a( sRet );
			::hstr_del_redundant_0d0a( sRet );
			m_cmmlCache = sRet;
			setMmlChanged( false );
		}
		return m_cmmlCache;
	}
	QString sRet = rootNode()->to_mathml_pureContent();
	::hstr_0a_to_0d0a( sRet );
	::hstr_del_redundant_0d0a( sRet );
	return sRet;
}

/////////////////////////////////////////////////////////////////////////////

void CFormulatorDoc::StoreDocument( QDataStream& ar ) const
{
	Q_ASSERT( m_pRootNode != NULL );
	m_pRootNode->Store( ar );
}

void CFormulatorDoc::LoadDocument( QDataStream& ar )
{
	Q_ASSERT( rootNode() != NULL );
	rootNode()->Load( ar );
}

QMimeData* CFormulatorDoc::copyToMimeData( CRootNode *pRootNode, QImage *pImage )
{
	Q_ASSERT( pRootNode );
	if( !QApplication::clipboard() ) return 0;
	QString mmlTextData = exportMathML( pRootNode );
	QByteArray mmlBinaryData;
	QDataStream ar( &mmlBinaryData, QIODevice::WriteOnly );
	pRootNode->Store( ar );
    
	QMimeData *mimeData = new QMimeData();
	mimeData->setText( mmlTextData );
	mimeData->setData( FORMULATOR_MIME_FORMAT, mmlBinaryData );
	if( pImage ) mimeData->setImageData( *pImage );
	return mimeData;
}

int CFormulatorDoc::copy( CRootNode *pRootNode, QImage *pImage )
{
	QMimeData *mimeData = CFormulatorDoc::copyToMimeData( pRootNode, pImage );
	if( !mimeData ) return 0;
	QApplication::clipboard()->setMimeData( mimeData );
	return 1;
}

int CFormulatorDoc::copy( QImage *pImage, bool isCut, bool needSelection )
{
	if( !QApplication::clipboard() ) return 0;
	if( needSelection && !rootNode()->IsSelection() ) return 0;

	int iRet = 0;
	CFrameNode *pFrameNode = rootNode()->RemoveOrCopySelection( isCut ? 1 : 0 );
	if( pFrameNode )
	{
		CRootNode *pRootNode = new CRootNode( m_owner );
		if( pRootNode->Insert( pFrameNode ) == 0 )
		{
			CFormulatorDoc::copy( pRootNode, pImage );
			iRet = 1;
		}
		delete pRootNode;
		delete pFrameNode;
	}
	return iRet;
}

int CFormulatorDoc::copyAll( QImage *pImage )
{
	return CFormulatorDoc::copy( rootNode(), pImage );
}

int CFormulatorDoc::isAcceptedMimeFormat( const QMimeData *mimeData )
{
	return (mimeData->hasFormat( FORMULATOR_MIME_FORMAT ) || mimeData->hasText() || mimeData->hasImage()) ? 1 : 0;
}

int CFormulatorDoc::paste()
{
	int iRet = 0;
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard ? clipboard->mimeData() : 0;
	if( !mimeData ) return iRet;
	return paste( mimeData, true );

}

int CFormulatorDoc::paste( const QMimeData *mimeData, bool deleteOldSelection )
{
	int iRet = 0;
	if( mimeData->hasFormat( FORMULATOR_MIME_FORMAT ) )
	{
		//QByteArray mmlBinaryData = mimeData->data( FORMULATOR_MIME_FORMAT );
		QDataStream ar( mimeData->data( FORMULATOR_MIME_FORMAT ) );
		CRootNode *pNode = new CRootNode( m_owner );
		pNode->Load( ar );
		if( deleteOldSelection && rootNode()->IsSelection() )
			rootNode()->MoveDel( 0 );
		iRet = (rootNode()->Insert( pNode ) == 0 ? 1 : 0);
		delete pNode;
	}
	else if( mimeData->hasText() )
	{
		iRet = (rootNode()->InsertMathML( __FMLCORE_Product_Title, mimeData->text(), 1 ) == 0 ? 1 : 0);
	}
	//else if( mimeData->hasImage() )
	//	setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
	return iRet;
}

/////////////////////////////////////////////////////////////////////////////

bool CFormulatorDoc::isEnabledUndo()
{
	return undoRedo()->CanUndo();
}

bool CFormulatorDoc::isEnabledRedo()
{
	return undoRedo()->CanRedo();
}

bool CFormulatorDoc::isEnabledCopy()
{
	return rootNode()->IsSelection();
}

bool CFormulatorDoc::isEnabledPaste()
{
	const QMimeData *mimeData = QApplication::clipboard()->mimeData();
	return mimeData->hasText() || mimeData->hasFormat( FORMULATOR_MIME_FORMAT )/* || mimeData->hasImage()*/;
}

bool CFormulatorDoc::isEnabledDelete()
{
	return true;
}

bool CFormulatorDoc::isEnabledSelectAll()
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////

void CFormulatorDoc::afterNew() 
{
	afterOpen(QString());
}

void CFormulatorDoc::afterOpen( const QString &filePath )
{
	setPath(filePath);
	setMmlChanged( true );
	setModified( false );

	//setModified( true );
	//exportMathML();
	//setModified( false );
}

void CFormulatorDoc::afterSave( const QString &filePath )
{
	setPath(filePath);
	setModified( false );
}

/////////////////////////////////////////////////////////////////////////////
