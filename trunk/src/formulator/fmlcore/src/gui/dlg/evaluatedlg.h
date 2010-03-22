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

#ifndef __FMLCORE_DLG_EVALUATEDLG_H__
#define __FMLCORE_DLG_EVALUATEDLG_H__

#include <QMainWindow>
#include <QString>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QSpinBox;
class QToolButton;
class QLabel;
QT_END_NAMESPACE

#include <QByteArray>
#include "HMathAST/icmml.h"

#include "HUtils/idrawstruct.h"
#include "../../fmlcore_global.h"

class __HERMITECH_FORMULIB__ CEvaluateDialog : public QDialog
{
    Q_OBJECT

protected:
	QLabel *m_resultLabel;

public:
	CEvaluateDialog( QWidget *parent );
    ~CEvaluateDialog();

public slots:
	virtual void accept();

protected:
	QString m_taskName;
	QString m_path;
	QByteArray m_readBytes;
	TXML_Storage* m_xmlTree;

	DRMessageManager m_dmm;
	long m_dmmPage;

	CMMLCalc	*m_cmmlCalc;

	// reports
	QString m_vmcode, m_astTreeDump, m_vmVariables;

public:
	void init();
	void dispose();

	int prepareInputData( int argc, char* argv[] );
	int compileMathML();
	int convertMathML2AST();
	int createVM();

	void reportVMTextCode();
	void reportVMTree();
	void reportVMVariables();

	int main();
};

#endif
