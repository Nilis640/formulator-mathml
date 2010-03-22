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

///////////////////////////////////////////////////////////////////////////////

#include <QFile>
#include "cmmlCalcTest.h"

///////////////////////////////////////////////////////////////////////////////

CMMLCalcTest::CMMLCalcTest()
{
	m_dmm.clear();
	m_dmmPage = 0;
	m_cmmlCalc = NULL;
	m_xmlTree = NULL;
	m_vmcode = _T("");
	m_astTreeDump = _T("");
	m_vmVariables = _T("");
}

CMMLCalcTest::~CMMLCalcTest()
{
	if( m_xmlTree )
	{
		delete m_xmlTree;
		m_xmlTree = NULL;
	}
	if( m_cmmlCalc )
	{
		delete m_cmmlCalc;
		m_cmmlCalc = NULL;
	}
	for( unsigned long i = 0; i < m_dmm.size(); i++ )
		printf( "%s\n", m_dmm.getFormatedText(i, m_dmmPage).toAscii().data() );
}

int CMMLCalcTest::prepareInputData( int argc, char* argv[] )
{
	m_taskName = argv[ 0 ];
	if( argc > 1 )
		m_path = argv[ 1 ];
	if( m_path == NULL )
	{
		m_dmm.AddLine( m_dmmPage, _T("Usage: cmmCalcConsole <Content MathML Input File>\n"));
		return -1;
	}

	QFile file( m_path );
	if( !file.open( QIODevice::ReadOnly ) )
	{
		m_dmm.AddLine( m_dmmPage, _T("File is not found\n"), QString(m_path));
		return -1;
	}
	m_readBytes = file.readAll();
	file.close();
	if( m_readBytes.size() == 0 )
	{
		m_dmm.AddLine( m_dmmPage, _T("File is empty or cannot be read\n"), QString(m_path));
		m_readBytes = 0;
		return -1;
	}
	return 0;
}
 
int CMMLCalcTest::compileMathML() 
{
	if( m_readBytes.size() == 0 ) return -1;

	if( ::LS_CompileXML( m_path, QString( m_readBytes ), m_xmlTree, m_dmm, m_dmmPage ) == 0 && m_xmlTree != NULL )
		return 0;
	m_dmm.AddLine( m_dmmPage, _T("File cannot be compiled (error of Content MathML parsing)\n"), QString(m_path));
	return -1;
}

int CMMLCalcTest::convertMathML2AST()
{
	if( !m_xmlTree ) return -1;

	AST_TXML_Tag* mathTag = m_xmlTree->getFirstTag( FBL_TAG_math );
	if( !mathTag )
	{
		m_dmm.AddLine( m_dmmPage, _T("Tag <math> is missed\n"), QString(m_path));
		return -1;
	}

	m_cmmlCalc = new CMMLCalc( m_dmm, m_dmmPage, m_path );
	if( m_cmmlCalc->CreateAST( mathTag ) == CMMLError_NONE )
		return 0;
	m_dmm.AddLine( m_dmmPage, _T("File cannot be compiled (error of Content MathML parsing)\n"), QString(m_path));
	return -1;
}

int CMMLCalcTest::createVM()
{
	if( !m_cmmlCalc ) return -1;
	return m_cmmlCalc->CreateVM( DSRMM_FUNCTION, INT_MM_ID_FUNCTION_REAL_E );
}

///////////////////////////////////////////////////////////////////////////////

void CMMLCalcTest::reportVMTextCode()
{
	if( !m_cmmlCalc || !m_cmmlCalc->getVM() ) return;

	m_vmcode = ::getTextView( m_cmmlCalc->getVM()->getSmbTable(), *(m_cmmlCalc->getVM()) );
	m_dmm.AddLine( m_dmmPage, m_vmcode);
}

void CMMLCalcTest::reportVMTree()
{
	if( !m_cmmlCalc ) return;

	m_astTreeDump = _T("");
	m_cmmlCalc->debugPrint( m_astTreeDump );
	m_dmm.AddLine( m_dmmPage, m_astTreeDump );
}

void CMMLCalcTest::reportVMVariables()
{
	if( !m_cmmlCalc || !m_cmmlCalc->getVM() ) return;

	// exec main body
	m_cmmlCalc->execMain();
	// report values of variables
	m_cmmlCalc->getVM()->printVariableValue2Text( m_vmVariables, -1, 1 );
	m_dmm.AddLine( m_dmmPage, m_vmVariables );
}

///////////////////////////////////////////////////////////////////////////////
