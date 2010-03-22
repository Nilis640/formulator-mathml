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

#include <QFile>
#include "HUtils/idfas.h"
#include "../dictionary/op_map.h"
#include "../mathml/la_mmlchar.h"
#include "../mathml/mml_format.h"
#include "../mathml/mml_operator.h"
#include "toolbarset.h"

///////////////////////////////////////////////////////////////////////////////

int FBtnToolbarSet::AddToolBar( const QString& path, DRMessageManager& dmm, long dmm_page )
{
	QFile file( path );
	if( !file.open( QIODevice::ReadOnly ) ) return -1;
	QByteArray readBytes = file.readAll();
	file.close();
	if( readBytes.size() == 0 ) return -1;

	QString oldFilename = dmm.getDefaultProperty_FileName();
	dmm.setDefaultProperty_FileName( path );
	TXML_Storage* bms = 0;
	int ret = (LS_CompileXML( path, QString( readBytes ), bms, dmm, dmm_page ) || !bms) ? -1 : AddToolBar( bms, dmm, dmm_page );
	dmm.setDefaultProperty_FileName( oldFilename );
	if( bms ) delete bms;
	return ret;
}

#define FBL_TAG_toolbar "toolbar"
int FBtnToolbarSet::AddToolBar( AST_TXML_Node *root, DRMessageManager& dmm, long dmm_page )
{
	std::vector<AST_TXML_Tag*> root_tags;
	root->getAllTags( FBL_TAG_toolbar, root_tags, -1 );
	return Read_XML( root_tags, dmm, dmm_page );
}

int FBtnToolbarSet::Read_XML( std::vector<AST_TXML_Tag*>& root_tags, DRMessageManager& dmm, long dmm_page )
{
	int ret = 0;
	for( long i = 0; i < (long) root_tags.size(); i++ )
	{
		FBtnToolbar tb;
		tb.setToolBarNo( size() );
		if( tb.Read_XML( root_tags[i], dmm, dmm_page ) == -1 )
			ret = -1;
		else
			push_back( tb );
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

void FBtnToolbarSet::FillMultiMap4Search( void )
{
	m_id2tb.clear();
	m_mmlname2tb.clear();
	m_mmlvalue2tb.clear();
	long t, b;
	for( t = 0; t < (long) size(); t++ )
	{
		for( b = 0; b < (long) at(t).size(); b++ )
		{
			m_id2tb.insert( at(t).at(b).getID(), QPair<qint32, qint32>(t, b) );
			m_mmlname2tb.insert( at(t).at(b).getMathMLName(), QPair<qint32, qint32>(t, b) );
			m_mmlvalue2tb.insert( at(t).at(b).getMathMLValue(), QPair<qint32, qint32>(t, b) );
		}
	}
}

int FBtnToolbarSet::getTextButtonCoordByID( const QString& id, long& _toolbar_no, long& _button_no )
{
	_toolbar_no = _button_no = -1;
	QList< QPair<qint32, qint32> > values = m_id2tb.values(id);
	if( values.size() > 0 )
		qSort(values.begin(), values.end());
	long t, b;
	for(long i = 0; i < values.size(); i++)
	{
		t = values.at(i).first;
		b = values.at(i).second;
		const FBtn& btn = getFBtn(t, b);
		if( btn.getNodeType() == NODE_PLANETEXT )
		{
			_toolbar_no = t;
			_button_no = b;
			return 0;
		}
	}
	return -1;
}

int FBtnToolbarSet::getButtonCoordByID( const QString& id, long& _toolbar_no, long& _button_no, long *argc )
{
	_toolbar_no = _button_no = -1;
	QList< QPair<qint32, qint32> > values = m_id2tb.values(id);
	if( values.size() > 0 )
	{
		qSort(values.begin(), values.end());
		_toolbar_no = values.at(0).first;
		_button_no = values.at(0).second;
		//const FBtn& btn = getFBtn(_toolbar_no, _button_no);
		if( argc ) *argc = getFBtn(_toolbar_no, _button_no).getChildCount();
		return 0;
	}
	return -1;
}

int FBtnToolbarSet::getButtonCoordByMathMLTag( const QString& tagname, long& _toolbar_no, long& _button_no )
{
	_toolbar_no = _button_no = -1;
	QList< QPair<qint32, qint32> > values = m_mmlname2tb.values(tagname);
	if( values.size() > 0 )
	{
		qSort(values.begin(), values.end());
		_toolbar_no = values.at(0).first;
		_button_no = values.at(0).second;
		return 0;
	}
	return -1;
}

int FBtnToolbarSet::getButtonCoordByMathMLValue( const QString& tagname, long& _toolbar_no, long& _button_no )
{
	_toolbar_no = _button_no = -1;
	QList< QPair<qint32, qint32> > values = m_mmlvalue2tb.values(tagname);
	if( values.size() > 0 )
		qSort(values.begin(), values.end());
	long t, b;
	for(long i = 0; i < values.size(); i++)
	{
		t = values.at(i).first;
		b = values.at(i).second;
		if( getFBtn(t, b).getMathMLData().frame.length() == 0 )
		{
			_toolbar_no = t;
			_button_no = b;
			return 0;
		}
	}
	return -1;
}

int FBtnToolbarSet::getButtonCoordByMathMLNameType( const QString& tagname, QString& tagtype, long& _toolbar_no, long& _button_no )
{
	_toolbar_no = _button_no = -1;
	QList< QPair<qint32, qint32> > values = m_mmlname2tb.values(tagname);
	if( values.size() > 0 )
		qSort(values.begin(), values.end());
	long t, b;
	for(long i = 0; i < values.size(); i++)
	{
		t = values.at(i).first;
		b = values.at(i).second;
		if( getFBtn(t, b).getMathMLData().type == tagtype )
		{
			_toolbar_no = t;
			_button_no = b;
			return 0;
		}
	}
	return -1;
}

int FBtnToolbarSet::text2DsrButton( const QString& body, FXmlBtn*& pFXmlBtn, FBtnAttr& ni )
{
	long _toolbar_no = -1;
	long _button_no = -1;

	LMMLChar mmlsl;
	if( mmlsl.Analisys( body.midRef(0) ) == 0 ) return -1;
	struct HMathMLAttr mml_attr( mmlsl.getTag(), body );
	HMathOpAttr *opinfo = ::mml_is_token_op_tag( mmlsl.getTag() ) ? ::getOperatorEntryByMML( body ) : 0;
	if( opinfo ) mml_attr.setMODefault( *opinfo );
	ni.setMathMLAttr( mml_attr );

	if( opinfo && opinfo->dsrname.length() )
	{
		if( getTextButtonCoordByID( opinfo->dsrname, _toolbar_no, _button_no ) != -1 )
		{
			pFXmlBtn = getButtonCalc( _toolbar_no, _button_no );
			if( pFXmlBtn )
			{
				if( pFXmlBtn->getNodeType() != NODE_PLANETEXT )
					return -1;
				// check fences
				{
					static const int ONECHAR_FENCE_COUNT = 6;
					static QString stringvec_ID_ONECHAR_FENCE_COUNT[ ONECHAR_FENCE_COUNT ] = 
					{
						_T("text ("), _T("text )"),
						_T("text ["), _T("text ]"),
						_T("text {"), _T("text }")
					};
					static LKeyword onecharkw( stringvec_ID_ONECHAR_FENCE_COUNT, ONECHAR_FENCE_COUNT );
					int ret = onecharkw.Analisys( pFXmlBtn->getID().midRef(0) );
					onecharkw.Reset();
					if( ret && ni.isStretchy() )
						ni.clearStretchy();
				}
			}
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
