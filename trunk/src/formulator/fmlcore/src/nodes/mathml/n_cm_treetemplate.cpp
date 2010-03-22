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

#include "../node.h"
#include "n_cm_treetemplate.h"

///////////////////////////////////////////////////////////////////////////////

QString applyV(FBL_TAG_apply);
QString cnV(FBL_TAG_cn);
QString mspaceV(FBL_TAG_mspace);
QString merrorV(FBL_TAG_merror);
QString mtextV(FBL_TAG_mtext);
QString miV(FBL_TAG_mi);
QString moV(FBL_TAG_mo);
QString mnV( FBL_TAG_mn );
QString mrowV(FBL_TAG_mrow);
QString mfracV(FBL_TAG_mfrac);
QString msqrtV(FBL_TAG_msqrt);
QString mrootV(FBL_TAG_mroot);
QString msupV(FBL_TAG_msup);
QString msubV(FBL_TAG_msub);
QString moverV(FBL_TAG_mover);
QString mfencedV(FBL_TAG_mfenced);
QString mtableV(FBL_TAG_mtable);
QString mtrV(FBL_TAG_mtr);
QString mtdV(FBL_TAG_mtd);
QString imagV("&ImaginaryI;");
QString timesV("&InvisibleTimes;");
QString expV("&ExponentialE;");
QString plusV("+");
QString plusTagV("plus");
QString _emptyV("");
QString openV( FBL_ATTR_open );
QString closeV( FBL_ATTR_close );
QString separatorV( FBL_ATTR_separator );
QString minus1V("&minus;1");
QString mathvariantV(FBL_ATTR_mathvariant);
QString normalV(FBL_VAL_normal);
QString _tV("T");
QString fenceV(FBL_ATTR_fence);
QString macrV("&macr;");
QString verbarV("&verbar;");
QString lfloorV("&lfloor;");
QString rfloorV("&rfloor;");
QString lceilV("&lceil;");
QString rceilV("&rceil;");
QString langleV("&langle;");
QString rangleV("&rangle;");
QString logV( FBL_TAG_log );
QString typeV("type");
QString integerV("integer");
QString largeopV(FBL_ATTR_largeop);
QString trueV(FBL_VAL_true);
QString setV(FBL_TAG_set);
QString logbaseV( FBL_TAG_logbase );
QString val10V("10");
QString blankV(" ");
QString tendstoV(FBL_TAG_tendsto);
QString eqV(FBL_TAG_eq);
QString bvarV(FBL_TAG_bvar);
QString boldV(FBL_VAL_bold);

///////////////////////////////////////////////////////////////////////////////

void MMLContent_TT_Token::set( QString tag, QString val, std::vector<QString> *attr )
{
	if( !root ) return;
	
	root->setTagName( tag );
	if( m_value ) m_value->setText( val );
	if( root->getAttributes() && attr && attr->size() > 0 && (attr->size() % 2 == 0) )
	{
		for( unsigned long i = 0; i + 1 < attr->size(); i += 2 )
			root->getAttributes()->Add( new AST_TXML_Attribute( &((*attr)[ i ]), &((*attr)[ i + 1 ]) ) );
	}
}

void MMLContent_TT_Token::reset()
{
	if( !root ) return;
	
	root->setTagName( _T("") );
	QString emptyV;
	if( m_value ) m_value->setText( emptyV );
	if( root->getAttributes() )
		root->getAttributes()->Flush();
}

MMLContent_TT_mrow::MMLContent_TT_mrow()
{
	QString mrowV( FBL_TAG_mrow );
	root = new AST_TXML_Tag( &mrowV, 0, new AST_TXML_Compound() );
	root->getFlags().setFlag(OLIMP_MATHML_FLAG__MROW_KEEP1CHILD);
}

void MMLContent_TT_mrow::set( AST_TXML_Compound *_node )
{
	if( !_node ) return;
	
	for( long i = 0; i < _node->getChildNumber(); i++ )
		if( _node->getChild( i ) && _node->getChild( i )->getNodeType() == ASTT_TXML_Tag )
			root->getTags()->Add( _node->getChild( i ) );
}

void MMLContent_TT_mrow::set( AST_TXML_Tag *_node )
{
	if( !_node ) return;

	root->getTags()->Add( _node );
}

void MMLContent_TT_mrow::setAutodetect()
{
	root->getFlags().setFlag(OLIMP_MATHML_FLAG__MROW_CONTENT_AUTODETECT);
}

MMLContent_TT_mfenced::MMLContent_TT_mfenced()
{
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( 0 );
	QString strV( FBL_TAG_mfenced );
	root = new AST_TXML_Tag( &strV, 0, _tags );
}

void MMLContent_TT_mfenced::set( AST_TXML_Tag *_node )
{
	if( !root || !_node ) return;

	root->getTags()->ReplaceChild( 0, _node );
}

MMLContent_TT_Interval::MMLContent_TT_Interval()
{
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( mrow_1.getRoot() );
	_tags->Add( mrow_2.getRoot() );
	QString sV( FBL_TAG_interval );
	root = new AST_TXML_Tag( &sV, 0, _tags );
}

void MMLContent_TT_Interval::reset()
{
	mrow_1.reset();
	mrow_2.reset();

	if( root && root->getTags() )
	{
		root->getTags()->ReplaceChild( 0, NULL );
		root->getTags()->ReplaceChild( 1, NULL );
	}
}
MMLContent_TT_ciRational::MMLContent_TT_ciRational()
{
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( 0 );
	_tags->Add( 0 );
	QString sV( FBL_TAG_mfrac );
	root = new AST_TXML_Tag( &sV, 0, _tags );
}

void MMLContent_TT_ciRational::set( AST_TXML_Compound *num, AST_TXML_Compound *denum )
{
	if( !root || !root->getTags() ) return;

	if( num )
	{
		mrow_num.set( num );
		root->getTags()->ReplaceChild( 0, mrow_num.getRoot() );
	}
	if( denum )
	{
		mrow_denum.set( denum );
		root->getTags()->ReplaceChild( 1, mrow_denum.getRoot() );
	}
}

void MMLContent_TT_ciRational::reset()
{
	mrow_num.reset();
	mrow_denum.reset();

	if( root && root->getTags() )
	{
		root->getTags()->ReplaceChild( 0, NULL );
		root->getTags()->ReplaceChild( 1, NULL );
	}
}

MMLContent_TT_ciComplexCartesian::MMLContent_TT_ciComplexCartesian()
{
	plus  = new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &plusV ) );

	AST_TXML_Tag *i, *times;
	i     = new AST_TXML_Tag( &miV, 0, new AST_TXML_Value( &imagV ) );
	times = new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &timesV ) );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( 0 );
	_tags->Add( 0 );
	_tags->Add( i );
	_tags->Add( times );
	_tags->Add( 0 );

	root = new AST_TXML_Tag( &mrowV, 0, _tags );
	root->getFlags().setFlag(OLIMP_MATHML_FLAG__MROW_KEEP1CHILD);
}

MMLContent_TT_ciComplexCartesian::~MMLContent_TT_ciComplexCartesian()
{
	if( plus )
	{ delete plus; plus = 0; }
}

void MMLContent_TT_ciComplexCartesian::set( AST_TXML_Compound *_re, AST_TXML_Compound *_im )
{
	if( !root || !root->getTags() ) return;

	if( _re )
	{
		mrow_re.set( _re );
		root->getTags()->ReplaceChild( 0, mrow_re.getRoot() );
		root->getTags()->ReplaceChild( 1, plus );
	}
	if( _im ) mrow_im.set( _im );
	root->getTags()->ReplaceChild( 4, mrow_im.getRoot() );
}

void MMLContent_TT_ciComplexCartesian::reset()
{
	mrow_re.reset();
	mrow_im.reset();

	if( !root || !root->getTags() ) return;
	root->getTags()->ReplaceChild( 0, NULL );
	root->getTags()->ReplaceChild( 1, NULL );
	root->getTags()->ReplaceChild( 4, NULL );
}

MMLContent_TT_ciComplexPolar::MMLContent_TT_ciComplexPolar()
{
	_row_tags = new AST_TXML_Compound();
	_row_tags->Add( 
		new AST_TXML_Tag( &miV, 0, new AST_TXML_Value( &imagV ) )
	);
	_row_tags->Add( 
		new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &timesV ) )
	);
	_row_tags->Add( 0 );

	AST_TXML_Compound *_sup_tags = new AST_TXML_Compound();
	_sup_tags->Add( 
		new AST_TXML_Tag( &miV, 0, new AST_TXML_Value( &expV ) )
	);
	_sup_tags->Add( new AST_TXML_Tag( &mrowV, 0, _row_tags ) );
	AST_TXML_Tag *sup = new AST_TXML_Tag( &msupV, 0, _sup_tags );

	times = new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &timesV ) );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( 0 );
	_tags->Add( 0 );
	_tags->Add( sup );

	root = new AST_TXML_Tag( &mrowV, 0, _tags );
	root->getFlags().setFlag(OLIMP_MATHML_FLAG__MROW_KEEP1CHILD);
}

MMLContent_TT_ciComplexPolar::~MMLContent_TT_ciComplexPolar()
{
	if( times )
	{ delete times; times = 0; }
}

void MMLContent_TT_ciComplexPolar::set( AST_TXML_Compound *_re, AST_TXML_Compound *_im )
{
	if( !root || !root->getTags() ) return;

	if( _re )
	{
		mrow_re.set( _re );
		root->getTags()->ReplaceChild( 0, mrow_re.getRoot() );
		root->getTags()->ReplaceChild( 1, times );
	}
	if( _im ) mrow_im.set( _im );
	_row_tags->ReplaceChild( 2, mrow_im.getRoot() );
}

void MMLContent_TT_ciComplexPolar::reset()
{
	mrow_re.reset();
	mrow_im.reset();

	if( !root || !root->getTags() ) return;
	root->getTags()->ReplaceChild( 0, NULL );
	root->getTags()->ReplaceChild( 1, NULL );
	_row_tags->ReplaceChild( 2, NULL );
}

MMLContent_TT_cnBase::MMLContent_TT_cnBase()
{
	m_base = new AST_TXML_Value( &_emptyV );
	AST_TXML_Tag *_mn = new AST_TXML_Tag( &mnV, 0, m_base );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( 0 );
	_tags->Add( _mn );
	root = new AST_TXML_Tag( &msubV, 0, _tags );
}

///////////////////////////////////////////////////////////////////////////////

MMLContent_TT_MSup::MMLContent_TT_MSup( QString tagname )
{
	m_tagname = tagname;

	m_base = new AST_TXML_Value( &_emptyV );
	m_parent = new AST_TXML_Value( &_emptyV );
	AST_TXML_Tag *_rt_base = new AST_TXML_Tag( &mnV, 0, m_base );
	AST_TXML_Tag *_rt_parent = new AST_TXML_Tag( &miV, 0, m_parent );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( _rt_parent );
	_tags->Add( _rt_base );
	root = new AST_TXML_Tag( &m_tagname, 0, _tags );
}

MMLContent_TT_MSupSub::MMLContent_TT_MSupSub( QString tagname )
{
	m_tagname = tagname;

	AST_TXML_Tag *_rt_base = new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 );
	AST_TXML_Tag *_rt_parent = new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( _rt_parent );
	_tags->Add( _rt_base );
	root = new AST_TXML_Tag( &m_tagname, 0, _tags );
}

///////////////////////////////////////////////////////////////////////////////

MMLContent_TT_Vector::MMLContent_TT_Vector( std::vector<QString> *attr )
{
	AST_TXML_Compound *attrSet = 0;
	if( attr && attr->size() > 0 && (attr->size() % 2 == 0) )
	{
		attrSet = new AST_TXML_Compound();
		for( unsigned long i = 0; i + 1 < attr->size(); i += 2 )
			attrSet->Add( new AST_TXML_Attribute( &((*attr)[ i ]), &((*attr)[ i + 1 ]) ) );
	}

	root = new AST_TXML_Tag( &mtableV, attrSet, new AST_TXML_Compound() );
}

void MMLContent_TT_Vector::set( std::vector<AST_TXML_Tag*>& children, int is_matrix )
{
	if( !root || !root->getTags() ) return;

	AST_TXML_Compound *_tags = root->getTags();
	todetach.erase(todetach.begin(), todetach.end());
	_tags->detachChildren();

	long i, j;
	if( is_matrix )
	{
		AST_TXML_Compound *_row = 0;
		AST_TXML_Tag *newchild = 0;
		for( i = 0; i < (long) children.size(); i++ )
		{
			_row = new AST_TXML_Compound();
			if( children[ i ] && children[ i ]->getTags() )
			{
				for( j = 0; j < children[ i ]->getTags()->getChildNumber(); j++ )
				{
					if( children[ i ]->getTags()->getChild( j ) &&
						children[ i ]->getTags()->getChild( j )->getNodeType() == ASTT_TXML_Tag )
					{
						newchild = new AST_TXML_Tag( &mtdV, 0, 
							(AST_TXML_Tag*)(children[ i ]->getTags()->getChild( j )) );
						todetach.push_back( newchild->getTags() );
						_row->Add( newchild );
					}
				}
			}
			//_tags->Add( _row );
			_tags->Add( new AST_TXML_Tag( &mtrV, 0, _row ) );
		}
	}
	else
	{
		AST_TXML_Tag *newchild = 0;
		AST_TXML_Tag *mtd = 0;
		for( i = 0; i < (long) children.size(); i++ )
		{
			newchild = new AST_TXML_Tag( &mtrV, 0, 
							mtd = new AST_TXML_Tag( &mtdV, 0, 
								children[ i ] ) );
			todetach.push_back( mtd->getTags() );
			_tags->Add( newchild );
		}
	}
}

void MMLContent_TT_Vector::set_2columns( std::vector<AST_TXML_Tag*>& children )
{
	if( !root || !root->getTags() ) return;

	AST_TXML_Compound *_tags = root->getTags();
	todetach.erase(todetach.begin(), todetach.end());
	_tags->detachChildren();

	AST_TXML_Compound *_row = 0;
	AST_TXML_Tag *newchild = 0;
	for( long i = 0; i < (long) children.size(); i++ )
	{
		_row = new AST_TXML_Compound();
		newchild = new AST_TXML_Tag( &mtdV, 0, (AST_TXML_Tag*)0 );
		todetach.push_back( newchild->getTags() );
		_row->Add( newchild );
		newchild = new AST_TXML_Tag( &mtdV, 0, (AST_TXML_Tag*)0 );
		todetach.push_back( newchild->getTags() );
		_row->Add( newchild );
		_tags->Add( new AST_TXML_Tag( &mtrV, 0, _row ) );
	}
}

void MMLContent_TT_Vector::reset()
{
	for( long i = 0; i < (long) todetach.size(); i++ )
		if( todetach[ i ] )
			todetach[ i ]->detachChildren();
	todetach.erase( todetach.begin(), todetach.end() );
	if( root && root->getTags() )
		root->getTags()->Flush();
}

///////////////////////////////////////////////////////////////////////////////

MMLContent_TT_mfenced_tuned::MMLContent_TT_mfenced_tuned()
{
	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &_emptyV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &_emptyV ) );
	_attr->Add( new AST_TXML_Attribute( &separatorV, &_emptyV ) );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	root = new AST_TXML_Tag( &mfencedV, _attr, _tags );
}

void MMLContent_TT_mfenced_tuned::set( AST_TXML_Compound *_node )
{
	if( !_node ) return;

	for( long i = 0; i < _node->getChildNumber(); i++ )
	{
		if( _node->getChild( i ) && 
			_node->getChild( i )->getNodeType() == ASTT_TXML_Tag )
		{
			root->getTags()->Add( _node->getChild( i ) );
		}
	}
}

void MMLContent_TT_mfenced_tuned::set( AST_TXML_Tag *_node )
{
	if( !_node ) return;

	root->getTags()->Add( _node );
}

void MMLContent_TT_mfenced_tuned::setOpen( QString name )
{
	AST_TXML_Attribute *_attr = root->getAttribute( FBL_ATTR_open );
	if( _attr ) _attr->setStringValue( name );
}

void MMLContent_TT_mfenced_tuned::setClose( QString name )
{
	AST_TXML_Attribute *_attr = root->getAttribute( FBL_ATTR_close );
	if( _attr ) _attr->setStringValue( name );
}

void MMLContent_TT_mfenced_tuned::setSeparator( QString name )
{
	AST_TXML_Attribute *_attr = root->getAttribute( FBL_ATTR_separator );
	if( _attr ) _attr->setStringValue( name );
}

///////////////////////////////////////////////////////////////////////////////

QString getCM_XMLTemplate_set_property( long dest_node_child, long src_node_child, QString name, QString type )
{
	static QString ret;
	static QString tmpl = 
_T("<apply>")
_T("<set-property node='%1' name='%2' type='%3' value='%4' />")
_T("</apply>");
	ret = tmpl.arg(dest_node_child).arg(name).arg(type).arg(src_node_child);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
