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

#include "HMathAST/icmml.h"
#include "HMathAST/iast.h"
#include "HMathAST/ivm.h"
#include "HMathAST/ismbtbl.h"

#include "evaluate.h"
#include "HMathML/isettings.h"

/////////////////////////////////////////////////////////////////////////////

QFormulatorCalc::QFormulatorCalc( QString taskID )
	: m_taskID( taskID )
{
	init();
}

QFormulatorCalc::~QFormulatorCalc()
{
	dispose();
}

void QFormulatorCalc::init()
{
	m_dmm.clear();
	m_dmmPage = 0;
	m_cmmlCalc = NULL;
	m_xmlTree = NULL;
	m_taskID = m_cmmlText = m_varText = "";
}

void QFormulatorCalc::dispose()
{
	m_dmm.clear();
	m_dmmPage = 0;
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
}

///////////////////////////////////////////////////////////////////////////////

int QFormulatorCalc::compile( QString cmmlText )
{
	m_cmmlText = cmmlText;
	if( ::LS_CompileXML( m_taskID, m_cmmlText, m_xmlTree, m_dmm, m_dmmPage ) != 0 || m_xmlTree == NULL )
	{
		m_dmm.AddLine( m_dmmPage, _T("Error while parsing Content MathML text\n"), QString(m_taskID));
		return -1;
	}
	AST_TXML_Tag* mathTag = m_xmlTree->getFirstTag( FBL_TAG_math );
	if( !mathTag )
	{
		m_dmm.AddLine( m_dmmPage, _T("Tag <math> is missed\n"), QString(m_taskID));
		return -1;
	}
	m_cmmlCalc = new CMMLCalc( m_dmm, m_dmmPage, m_taskID );
	if( m_cmmlCalc->CreateAST( mathTag ) != CMMLError_NONE )
	{
		m_dmm.AddLine( m_dmmPage, _T("Error while parsing Content MathML text\n"), QString(m_taskID));
		return -1;
	}

	return m_cmmlCalc->CreateVM( DSRMM_FUNCTION, INT_MM_ID_FUNCTION_REAL_E );
}

int QFormulatorCalc::compile( QString varText, QString mainBodyText, int useOnlyAssign )
{
	m_varText = varText;
	m_cmmlText = mainBodyText;

	int i, iRet = ::LS_CompileXML( m_taskID, m_varText, m_xmlTree, m_dmm, m_dmmPage );
	AST_TXML_Tag* mathRoot = 0;
	std::vector<AST_TXML_Tag*> assignList;
	if( iRet == 0 && m_xmlTree && (mathRoot = m_xmlTree->getFirstTag( FBL_TAG_math )) != 0 && mathRoot->getTags() )
	{
		if( useOnlyAssign )
		{
			QString tagBody;
			for( i = 0; i < mathRoot->getTags()->getChildNumber(); i++ )
			{
				AST_TXML_Node *_xml_child = mathRoot->getTags()->getChild( i );
				if( _xml_child && _xml_child->getNodeType() == ASTT_TXML_Tag &&
					((AST_TXML_Tag*) _xml_child)->getTags() &&
					!xml_strcmp( ((AST_TXML_Tag*) _xml_child)->getTagName(), FBL_TAG_apply ) )
				{
					AST_TXML_Node* xmlNode = ((AST_TXML_Tag*) _xml_child)->getTags()->getChild( 0 );
					if( xmlNode && xmlNode->getNodeType() == ASTT_TXML_Tag &&
						!xml_strcmp( ((AST_TXML_Tag*) xmlNode)->getTagName(), FBL_TAG_csymbol ) )
					{
						tagBody = ((AST_TXML_Tag*) xmlNode)->getText();
						if( ::mml_isAssignOpBody( tagBody ) )
						{
							assignList.push_back( (AST_TXML_Tag*) _xml_child );
							mathRoot->getTags()->ReplaceChild( i, NULL );
						}
					}
				}
			}
		}
		else
		{
			mathRoot->getAllTags2Vector( assignList );
			mathRoot->getTags()->Flush();
		}
		delete m_xmlTree;
		m_xmlTree = NULL;
	}

	if( ::LS_CompileXML( m_taskID, m_cmmlText, m_xmlTree, m_dmm, m_dmmPage ) != 0 || m_xmlTree == NULL )
	{
		m_dmm.AddLine( m_dmmPage, _T("Error while parsing Content MathML text\n"), QString(m_taskID));
		return -1;
	}
	mathRoot = m_xmlTree->getFirstTag( FBL_TAG_math );
	if( !mathRoot )
	{
		m_dmm.AddLine( m_dmmPage, _T("Tag <math> is missed\n"), QString(m_taskID));
		return -1;
	}
	if( mathRoot->getTags() )
		mathRoot->getTags()->Prepend( assignList );

	m_cmmlCalc = new CMMLCalc( m_dmm, m_dmmPage, m_taskID );
	if( m_cmmlCalc->CreateAST( mathRoot ) != CMMLError_NONE )
	{
		m_dmm.AddLine( m_dmmPage, _T("Error while parsing Content MathML text\n"), QString(m_taskID));
		return -1;
	}

	return m_cmmlCalc->CreateVM( DSRMM_FUNCTION, INT_MM_ID_FUNCTION_REAL_E );
}

int QFormulatorCalc::evaluate()
{
	if( !m_cmmlCalc || !m_cmmlCalc->getVM() ) return -1;
	if( m_cmmlCalc->execMain() != CMMLError_NONE ) return -1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

QStringList QFormulatorCalc::report()
{
	QStringList res;
	for( unsigned long i = 0; i < m_dmm.size(); i++ )
		res.append(m_dmm.getFormatedText(i, m_dmmPage));
	return res;
}

QString QFormulatorCalc::variableValue( QString name )
{
	UniWord value;
	if( !m_cmmlCalc || !m_cmmlCalc->getVM() )
		return QString();
	if( m_cmmlCalc->getVM()->getVariable_getValue( name, value ) == -1 )
		return QString();
	//return value.getFormulatorView( m_cmmlCalc->getVM()->getSmbTable() );
	return value.getMmlView( m_cmmlCalc->getVM()->getSmbTable() );
}

//QStringList QFormulatorCalc::itemsValue()
//{
//	QStringList res;
//	if( !m_cmmlCalc ) return res;
//	QMap<QString,QString>::const_iterator i = m_cmmlCalc->getInternalVars().constBegin();
//	while(i != m_cmmlCalc->getInternalVars().constEnd())
//	{
//		res.append( variableValue( i.key() ) );
//		i++;
//	}
//	return res;
//}

QStringList QFormulatorCalc::itemsValue()
{
	QStringList res;
	if( !m_cmmlCalc ) return res;
	for( int i = 0; i < m_cmmlCalc->getInternalVars().size(); i++ )
		res.append( variableValue( m_cmmlCalc->getInternalVars().at(i) ) );
	return res;
}

QString QFormulatorCalc::lastItemValue()
{
	QString res;
	if( !m_cmmlCalc || m_cmmlCalc->getInternalVars().size() == 0 ) return QString();
	return variableValue( m_cmmlCalc->getInternalVars().last() );
}

///////////////////////////////////////////////////////////////////////////////

void QFormulatorCalc::setRealPrecision( int digits )
{
	if( m_cmmlCalc && m_cmmlCalc->getVM() )
		m_cmmlCalc->getVM()->getSmbTable().setPrint_DoublePrec( qBound(int(0), digits, int(DBL_DIG)) );
}

///////////////////////////////////////////////////////////////////////////////
