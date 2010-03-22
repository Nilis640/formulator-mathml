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

#include <QFileInfo>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <QTextDocument>
#include <QDesktopServices>
#include <QUrl>

#include <time.h>

#include "HMathML/isettings.h"
#include "dir2html.h"
#include "../gui/renderer/fmlrenderer.h"

/////////////////////////////////////////////////////////////////////////////

QFmlTestDir2Html::QFmlTestDir2Html(QString rootDir, QString destPath, QString mmlExt, QString pictExt, QObject *parent)
{
	m_engine = new QFormulatorRenderer( parent );
	m_rootDir = rootDir;
	m_destPath = destPath;
	m_destDir = QFileInfo(destPath).absoluteDir();
	m_mmlExt = mmlExt;
	m_pictExt = pictExt;
}

QFmlTestDir2Html::~QFmlTestDir2Html()
{
	if( m_engine )
	{
		delete m_engine;
		m_engine = 0;
	}
}

QFormulatorRenderer* QFmlTestDir2Html::renderer()
{
	return m_engine;
}

void QFmlTestDir2Html::run()
{
	QFile file(m_destPath);
	if( !file.open(QFile::WriteOnly | QFile::Text) )
		return;

	double scale = m_engine->scale();
	m_engine->setScale(1.0);
	int state = ::isPureContentMathMLOutput();
	if( state ) clearPureContentMathMLOutput();
	m_text.clear();

	time_t ct;
	qsrand(time(&ct));
	renderDir(QDir(m_rootDir));

	if( state ) setPureContentMathMLOutput();
	m_engine->setScale(scale);

	QTextStream out(&file);
	out << m_text;
	file.close();

	QUrl localUrl(m_destPath);
	QDesktopServices::openUrl(localUrl);
}

void QFmlTestDir2Html::renderDir(const QDir& rootDir)
{
	const static QString dirTemplate = "<table><tbody><tr><td>%1</td></tr>\n%2</tbody></table><br/><br/><hr/><br/><br/>\n\n";
	const static QString itemTemplate =
		"<tr><td>%1</td><td>%3</td><td>%5</td></tr>\n"
		"<tr><td>%2</td><td>%4</td><td>%6</td></tr>\n";
	const static QString textTemplate = "<pre>%1</pre>";
	const static QString imgTemplate = "<img src=\"file://%1\"></img>";

	QStringList::const_iterator item;
	QStringList mmlFiles = rootDir.entryList(QStringList(m_mmlExt), QDir::Files | QDir::NoSymLinks, QDir::Name);

	if( mmlFiles.size() )
	{
		QString mml1, mml2, mml3, img1, img2, img3, tableBody;
		QString mmlPath, imgName, baseName;
		for( item = mmlFiles.constBegin(); item != mmlFiles.constEnd(); item++ )
		{
			mmlPath = rootDir.absoluteFilePath(*item);
			QFile file(mmlPath);
			if( !m_engine->load(mmlPath) || !file.open(QFile::ReadOnly | QFile::Text) )
			{
				mml1 = QString(QObject::tr("cannot load %1")).arg(mmlPath);
				mml2 = mml3 = "";
				img1 = img2 = img3 = "";
			}
			else
			{
				QTextStream in(&file);
				mml1 = "";						
				mml2 = QString(in.readAll());	
				mml3 = m_engine->text();		
				mml3.replace(QString("\r\n"), QString("\n"));
				file.close();
			    
				// initial (etalon) picture
				baseName = QFileInfo(*item).completeBaseName();
				imgName = QString("%1.%2").arg(baseName).arg(m_pictExt);
				if( rootDir.exists(imgName) )
					img1 = imgTemplate.arg(rootDir.absoluteFilePath(imgName));
				else
					img1 = "";
			    
				// 1-order derivative exported picture
				imgName = QString("%1/%2-1-%4.%3").arg(m_destDir.absolutePath()).arg(baseName).arg(m_pictExt).arg(qrand());
				if( m_engine->exportImage(imgName) )
					img2 = imgTemplate.arg(imgName);
				else
					img2 = "";
			    
				if( !m_engine->reload() )
				{
					img3 = QObject::tr("cannot load derivative MathML text");
				}
				else
				{
					// 2-order derivative exported picture
					imgName = QString("%1/%2-2-%4.%3").arg(m_destDir.absolutePath()).arg(baseName).arg(m_pictExt).arg(qrand());
					if( m_engine->exportImage(imgName) )
						img3 = imgTemplate.arg(imgName);
					else
						img3 = "";
				}
			}
			tableBody += itemTemplate
				.arg(textTemplate.arg(Qt::escape(mml1))).arg(img1)
				.arg(textTemplate.arg(Qt::escape(mml2))).arg(img2)
				.arg(textTemplate.arg(Qt::escape(mml3))).arg(img3);
		}
		m_text += dirTemplate.arg(QDir(m_rootDir).relativeFilePath(rootDir.path())).arg(tableBody);
	}

	QStringList newDirs = rootDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDir::Name);
	for( item = newDirs.constBegin(); item != newDirs.constEnd(); item++ )
	{
		QCoreApplication::processEvents();
		QDir itemDir( rootDir );
		if( itemDir.cd(*item) ) renderDir( itemDir );
	}
}

/////////////////////////////////////////////////////////////////////////////
