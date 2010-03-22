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

#include "mml_char.h"

extern const long __mml_char__font[ MML_CHAR__NUMBER ] =
{
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,

	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,

	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_OP,
	MML_STYLE_TEXT,
	MML_STYLE_TEXT,

	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
	MML_STYLE_GREEK,
};

extern const QString __mml_char__value[ MML_CHAR__NUMBER ] =
{
	_T("A"),
	_T("B"),
	_T("C"),
	_T("D"),
	_T("E"),
	_T("F"),
	_T("G"),
	_T("H"),
	_T("I"),
	_T("J"),
	_T("K"),
	_T("L"),
	_T("M"),
	_T("N"),
	_T("O"),
	_T("P"),
	_T("Q"),
	_T("R"),
	_T("S"),
	_T("T"),
	_T("U"),
	_T("V"),
	_T("W"),
	_T("X"),
	_T("Y"),
	_T("Z"),

	_T("a"),
	_T("b"),
	_T("c"),
	_T("d"),
	_T("e"),
	_T("f"),
	_T("g"),
	_T("h"),
	_T("i"),
	_T("j"),
	_T("k"),
	_T("l"),
	_T("m"),
	_T("n"),
	_T("o"),
	_T("p"),
	_T("q"),
	_T("r"),
	_T("s"),
	_T("t"),
	_T("u"),
	_T("v"),
	_T("w"),
	_T("x"),
	_T("y"),
	_T("z"),

	_T("<"),
	_T(">"),
	_T("/"),
	_T("\""),
	_T("{"),
	_T("}"),
	_T("`"),
	_T("@"),
	_T("#"),
	_T("^"),
	_T("&"),
	_T("_"),
	_T("-"),
	_T("|"),
	_T("\\"),
	_T("\t"),
	_T(" "),

	_T("?"),
	_T("-"),
	_T("?"),
	_T("i"),
	_T("¹"),
	_T("Ø"),
	_T("Ù"),
	_T("Ú"),
	_T("-"),
	_T("R"),
	_T("È"),//å
	_T("?"),
	_T("<"),
	_T("¨"),
	_T("¬"),
	_T("c"),
	_T("ª"),
	_T("ä")
};

extern const QString __mml_char__name[ MML_CHAR__NUMBER ] =
{
	_T("&Agr;"),
	_T("&Bgr;"),
	_T("&KHgr;"),
	_T("&Delta;"),
	_T("&Egr;"),
	_T("&Phi;"),
	_T("&Gamma;"),
	_T("&EEgr;"),
	_T("&Igr;"),
	_T("&thetav;"),
	_T("&Kgr;"),
	_T("&Lambda;"),
	_T("&Mgr;"),
	_T("&Ngr;"),
	_T("&Ogr;"),
	_T("&Pi;"),
	_T("&Theta;"),
	_T("&Rgr;"),
	_T("&Sigma;"),
	_T("&Tgr;"),
	_T("&Upsi;"),
	_T("&sigmav;"),
	_T("&Omega;"),
	_T("&Xi;"),
	_T("&Psi;"),
	_T("&Zgr;"),

	_T("&alpha;"),
	_T("&beta;"),
	_T("&chi;"),
	_T("&delta;"),
	_T("&epsiv;"),
	_T("&phi;"),
	_T("&gamma;"),
	_T("&eta;"),
	_T("&iota;"),
	_T("&phiv;"),
	_T("&kappa;"),
	_T("&lambda;"),
	_T("&mu;"),
	_T("&nu;"),
	_T("&ogr;"),
	_T("&pi;"),
	_T("&theta;"),
	_T("&rho;"),
	_T("&sigma;"),
	_T("&tau;"),
	_T("&upsilon;"),
	_T("&piv;"),
	_T("&omega;"),
	_T("&xi;"),
	_T("&psi;"),
	_T("&zeta;"),

	_T("&lt;"),                    // <   
	_T("&gt;"),                    // >   
	_T("&sol;"),                   // /   
	_T("&quot;"),                  // "   
	_T("&lcub;"),                  // {   
	_T("&rcub;"),                  // }   
	_T("&grave;"),                 // `   
	_T("&commat;"),                // @   
	_T("&num;"),                   // #   
	_T("&circ;"),                  // ^   
	_T("&amp;"),                   // &   
	_T("&lowbar;"),                // _   
	_T("&minus;"),                 // -   
	_T("&verbar;"),                // |   
	_T("&bsol;"),                  // '\'   
	_T("&Tab;"),                   // tb  
	_T("&nbsp;"),                  // sp  

	_T("&times;"),
	_T("&minus;"),
	_T("&le;"),
	_T("&ge;"),
	_T("&ne;"),
	_T("&not;"),
	_T("&and;"),
	_T("&or;"),
	_T("&isinv;"),
	_T("&notin;"),
	_T("&cup;"),
	_T("&cap;"),
	_T("&sub;"),
	_T("&sup;"),
	_T("&subE;"),
	_T("&supE;"),
	_T("&vnsub;"),
	_T("&emptyv;")
};

QString mml_getGreek( QChar a )
{
	if( a.unicode() >= 'A' && a.unicode() <= 'Z' )
		return __mml_char__name[ a.unicode() - 'A' ];
	else if( a >= 'a' && a <= 'z' )
		return __mml_char__name[ MML_CHAR__GREEK_NUMBER + a.unicode() - 'a' ];
	return QString( 1, a );
	//if( a.unicode() >= QChar('A').unicode() && a.unicode() <= QChar('Z').unicode() )
	//	return __mml_char__name[ a.unicode() - QChar('A').unicode() ];
	//else if( a >= QChar('a').unicode() && a <= QChar('z').unicode() )
	//	return __mml_char__name[ MML_CHAR__GREEK_NUMBER + a.unicode() - QChar('a').unicode() ];
	//return QString( 1, a );
}
