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

#if !defined( FORMULATOR_N_CM_TREETEMPLATE_H )
#define FORMULATOR_N_CM_TREETEMPLATE_H

#include "HUtils/ixml.h"
#include "../../mathml/mml_defs.h"
#include "../../fmlcore_global.h"

///////////////////////////////////////////////////////////////////////////////

extern QString miV, moV, mnV, mrowV, msupV, msubV, mfencedV, mtableV, mtrV, mtdV, 
				imagV, timesV, expV, plusV, _emptyV, openV, closeV, separatorV, minus1V,
				mathvariantV, normalV, _tV, macrV, moverV, verbarV, lfloorV, rfloorV,
				lceilV, rceilV, langleV, rangleV, mfracV, msqrtV, mrootV, logV, typeV, 
				integerV, largeopV, trueV, setV, logbaseV, cnV, val10V, tendstoV, eqV,
				bvarV, applyV, boldV, mtextV, merrorV, mspaceV, blankV, fenceV, plusTagV;

///////////////////////////////////////////////////////////////////////////////

#define CONTENT_MATHML_CONVERTION2__ONLY_TEXT_BODY							1
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE							2
#define CONTENT_MATHML_CONVERTION2__PARSE_NAME_AND_RECURSIVE_CM_TREE		3
#define CONTENT_MATHML_CONVERTION2__TEMPLATE								5
#define CONTENT_MATHML_CONVERTION2__SELF_CLOSING							6
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_MATRIX					7
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_SET						8
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY							9
#define CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE			10
#define CONTENT_MATHML_CONVERTION2__HIDE_APPLY_OPERATOR_RECURSIVE_TREE		11
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP				12
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_MOMENTABOUT_ADDON_OP	13
#define CONTENT_MATHML_CONVERTION2__FRAME_NOT_MULTILINE						14
#define CONTENT_MATHML_CONVERTION2__SEMANTICS_LITERALLY						15
#define CONTENT_MATHML_CONVERTION2__SEMANTICS_PRESENTATION_SUBSTITUTE		16
#define CONTENT_MATHML_CONVERTION2__UNKNOWN_XML								17
#define CONTENT_MATHML_CONVERTION2__BACKRECURSIVE_TREE						18
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP__BACKRECURSIVE_TREE	19
#define CONTENT_MATHML_CONVERTION2__INTERVAL_REFERENCE						20
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART1					21
#define CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART2					22
///////////////////////////////////////////////////////////////////////////////

inline
QString getContentMathMLReferenceID_bvar( long i )
{
	return QString("bvar%1").arg(i);
}

inline
QString getContentMathMLReferenceID_degree( long i )
{
	return QString("degree%1").arg(i);
}

inline
QString getContentMathMLReferenceID_interval( long i )
{
	return QString("interval%1").arg(i);
}

inline
QString getContentMathMLReferenceID_separatorbvar()
{
	static QString buffer = _T("|sep4bvar");
	return buffer;
}

inline
QString getContentMathMLReferenceID_momentabout()
{
	static QString buffer = _T("momentabout");
	return buffer;
}


///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ MMLContent_TT_Root
{
public:
	MMLContent_TT_Root()
	{
		root = 0;
	}
	virtual ~MMLContent_TT_Root()
	{
		if( root )
		{ delete root; root = 0; }
	}
	AST_TXML_Tag* getRoot()
	{
		return root;
	}
        virtual void set( AST_TXML_Compound * /*a*/, AST_TXML_Compound * /*b*/ )
	{}
protected:
	AST_TXML_Tag *root;
};

class __HERMITECH_FORMULIB__ MMLContent_TT_Token : public MMLContent_TT_Root
{
public:
	MMLContent_TT_Token()
	{
		QString eV;
		root = new AST_TXML_Tag( &eV, new AST_TXML_Compound(), m_value = new AST_TXML_Value( &eV ) );
	}
	virtual ~MMLContent_TT_Token() {}
	void set( QString tag, QString val, std::vector<QString> *attr = 0 );
	void reset();
protected:
	AST_TXML_Value *m_value;
};

class __HERMITECH_FORMULIB__ MMLContent_TT_mrow : public MMLContent_TT_Root
{
public:
	MMLContent_TT_mrow();
	virtual ~MMLContent_TT_mrow() {}
	void set( AST_TXML_Compound *_node );
	void set( AST_TXML_Tag *_node );
	void setAutodetect();
	void reset()
	{
		if( root && root->getTags() )
			root->getTags()->detachChildren();
	}
};

class __HERMITECH_FORMULIB__ MMLContent_TT_mfenced : public MMLContent_TT_Root
{
public:
	MMLContent_TT_mfenced();
	virtual ~MMLContent_TT_mfenced() {}
	void set( AST_TXML_Tag *_node );
	void reset()
	{
		if( root && root->getTags() )
			root->getTags()->ReplaceChild( 0, NULL );
	}
};

class __HERMITECH_FORMULIB__ MMLContent_TT_mfenced_tuned : public MMLContent_TT_Root
{
public:
	MMLContent_TT_mfenced_tuned();
	virtual ~MMLContent_TT_mfenced_tuned() {}
	void set( AST_TXML_Tag *_node );
	void set( AST_TXML_Compound *_node );
	void setOpen( QString name );
	void setClose( QString name );
	void setSeparator( QString name );
	void reset()
	{
		setOpen( _T("") );
		setClose( _T("") );
		setSeparator( _T("") );
		if( root && root->getTags() )
			root->getTags()->detachChildren();
	}
};

class __HERMITECH_FORMULIB__ MMLContent_TT_Interval : public MMLContent_TT_Root
{
protected:
	MMLContent_TT_mrow mrow_1, mrow_2;
public:
	MMLContent_TT_Interval();
	virtual ~MMLContent_TT_Interval() {}
	void reset();
};

class __HERMITECH_FORMULIB__ MMLContent_TT_ciRational : public MMLContent_TT_Root
{
protected:
	MMLContent_TT_mrow mrow_num, mrow_denum;
public:
	MMLContent_TT_ciRational();
	virtual ~MMLContent_TT_ciRational() {}
	virtual void set( AST_TXML_Compound *num, AST_TXML_Compound *denum );
	void reset();
};

class __HERMITECH_FORMULIB__ MMLContent_TT_ciComplexCartesian : public MMLContent_TT_Root
{
protected:
	MMLContent_TT_mrow mrow_re, mrow_im;
	AST_TXML_Tag *plus;
public:
	MMLContent_TT_ciComplexCartesian();
	virtual ~MMLContent_TT_ciComplexCartesian();
	virtual void set( AST_TXML_Compound *_re, AST_TXML_Compound *_im );
	void reset();
};

class __HERMITECH_FORMULIB__ MMLContent_TT_ciComplexPolar : public MMLContent_TT_Root
{
protected:
	MMLContent_TT_mrow mrow_re, mrow_im;
	AST_TXML_Tag *times;
	AST_TXML_Compound *_row_tags;
public:
	MMLContent_TT_ciComplexPolar();
	virtual ~MMLContent_TT_ciComplexPolar();
	virtual void set( AST_TXML_Compound *_re, AST_TXML_Compound *_im );
	void reset();
};

class __HERMITECH_FORMULIB__ MMLContent_TT_cnBase : public MMLContent_TT_Root
{
protected:
	MMLContent_TT_mfenced mfenced;
	AST_TXML_Value *m_base;
public:
	MMLContent_TT_cnBase();
	virtual ~MMLContent_TT_cnBase() {}
	void setParent( MMLContent_TT_ciComplexCartesian& number )
	{
		mfenced.set( number.getRoot() );
		if( root && root->getTags() )
			root->getTags()->ReplaceChild( 0, mfenced.getRoot() );
	}
	void setParent( MMLContent_TT_ciComplexPolar& number )
	{
		mfenced.set( number.getRoot() );
		if( root && root->getTags() )
			root->getTags()->ReplaceChild( 0, mfenced.getRoot() );
	}
	void setParent( MMLContent_TT_ciRational& number )
	{
		if( root && root->getTags() )
			root->getTags()->ReplaceChild( 0, number.getRoot() );
	}
	void setParent( AST_TXML_Tag *_node )
	{
		if( root && root->getTags() )
			root->getTags()->ReplaceChild( 0, _node );
	}
	void setBase( QString base )
	{
		if( m_base )
			m_base->setText( base );
	}
	void reset()
	{
		mfenced.reset();
		if( root && root->getTags() )
			root->getTags()->ReplaceChild( 0, NULL );
	}
};


class __HERMITECH_FORMULIB__ MMLContent_TT_MSup : public MMLContent_TT_Root
{
protected:
	AST_TXML_Value *m_base, *m_parent;
	QString m_tagname;
public:
	MMLContent_TT_MSup( QString tagname = QString( FBL_TAG_msup ) );
	virtual ~MMLContent_TT_MSup() {}
	void setParent( QString parent )
	{
		if( m_parent )
			m_parent->setText( parent );
	}
	void setBase( QString base )
	{
		if( m_base )
			m_base->setText( base );
	}
	void reset()
	{
		setParent( _T("") );
		setBase( _T("") );
	}
};


class __HERMITECH_FORMULIB__ MMLContent_TT_MSupSub : public MMLContent_TT_Root
{
protected:
	QString m_tagname;
public:
	MMLContent_TT_MSupSub( QString tagname );
	virtual ~MMLContent_TT_MSupSub() {}
};


class __HERMITECH_FORMULIB__ MMLContent_TT_Vector : public MMLContent_TT_Root
{
protected:
	std::vector<AST_TXML_Compound*> todetach;
public:
	MMLContent_TT_Vector( std::vector<QString> *attr = 0 );
	virtual ~MMLContent_TT_Vector() {}
	virtual void set( std::vector<AST_TXML_Tag*>& children, int is_matrix );
	virtual void set_2columns( std::vector<AST_TXML_Tag*>& children );
	void reset();
};

///////////////////////////////////////////////////////////////////////////////

#define FBL_TAG_set_property	_T("set-property")
#define FBL_ATTR_node			_T("node")
#define FBL_ATTR_name			_T("name")
#define FBL_ATTR_value			_T("value")
#define FBL_VAL_base			_T("base")

__HERMITECH_FORMULIB__
QString getCM_XMLTemplate_set_property( long dest_node_child, long src_node_child, QString name, QString type );

inline
QString getCM_Template_cn_sep()
{
	static QString tmpl =
		_T("  ///$$$ 1st slot&#x0D;&#x0A;")
		_T("  <sep/>&#x0D;&#x0A;")
		_T("  ///$$$ 2nd slot&#x0D;&#x0A;");
	return tmpl;
}

inline
QString getCM_sep()
{
	static QString tmpl = _T("<sep/>");
	return tmpl;
}

///////////////////////////////////////////////////////////////////////////////

#endif
