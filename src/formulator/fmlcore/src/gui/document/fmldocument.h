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

#if !defined( __FMLCORE_FMLDOCUMENT_H__ )
#define __FMLCORE_FMLDOCUMENT_H__

#include "HUtils/idrawing.h"
#include "HUtils/iundoredo.h"
#include "../../fmlcore_global.h"

/////////////////////////////////////////////////////////////////////////////

class QMimeData;

class CRootNode;
class QFormulatorBaseWidget;

/////////////////////////////////////////////////////////////////////////////

#define FORMULATOR_MIME_FORMAT "application/formulator-internal"

class __HERMITECH_FORMULIB__ CFormulatorDoc 
	: public CHaveUndoRedo
{
protected:
	QFormulatorBaseWidget *m_owner;
	CRootNode*		m_pRootNode;		// to store
	QString			m_mmlCache;
	QString			m_cmmlCache;
	CUndoRedo*		m_pUndoRedo;		// undo - redo 
	QColor			m_clrBkgrnd;		// background color
	bool			m_isModified;		//
	bool			m_isMmlChanged;		//
	QString			m_path;

public: 
	CFormulatorDoc( QFormulatorBaseWidget *owner );
	virtual ~CFormulatorDoc();
	QString getPath() { return m_path; }
	void setPath(const QString& path) { m_path = path; }
	void setModified( bool isModified );
	bool isModified();
	void setMmlChanged( bool isMmlChanged );
	bool isMmlChanged();
	CRootNode* rootNode();
	CUndoRedo* undoRedo();

public:
	virtual void StoreDocument( QDataStream& ar ) const;
	virtual void LoadDocument( QDataStream& ar );

public:
	void afterNew();
	void afterOpen( const QString &filePath );
	void afterSave( const QString &filePath );

public:
	QString exportMathML();
	QString exportPureContentMathML( bool ignoreCache = false );
	static QString exportMathML( CRootNode *pRootNode );

public:
	static QMimeData* copyToMimeData( CRootNode *pRootNode, QImage *pImage = 0 );
	static int copy( CRootNode *pRootNode, QImage *pImage = 0 );
	int copy( QImage *pImage = 0, bool isCut = false, bool needSelection = true );
	int copyAll( QImage *pImage = 0 );
	int paste();
	int paste( const QMimeData *mimeData, bool deleteOldSelection );
	static int isAcceptedMimeFormat( const QMimeData *mimeData );

	bool isEnabledUndo();
	bool isEnabledRedo();
	bool isEnabledCopy();
	bool isEnabledPaste();
	bool isEnabledDelete();
	bool isEnabledSelectAll();
};

/////////////////////////////////////////////////////////////////////////////

inline
CRootNode* CFormulatorDoc::rootNode() 
	{ return m_pRootNode; }

inline
CUndoRedo* CFormulatorDoc::undoRedo()
	{ return m_pUndoRedo; }

inline
void CFormulatorDoc::setModified( bool isModified )
{
	m_isModified = isModified;
}

inline
bool CFormulatorDoc::isModified()
{
	return m_isModified;
}

inline
bool CFormulatorDoc::isMmlChanged()
{
	return m_isMmlChanged;
}

inline
void CFormulatorDoc::setMmlChanged( bool isMmlChanged )
{
	m_isMmlChanged = isMmlChanged;
}

/////////////////////////////////////////////////////////////////////////////

#endif
