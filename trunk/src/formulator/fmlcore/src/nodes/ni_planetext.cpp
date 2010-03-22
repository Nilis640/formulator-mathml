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

#include "node.h"
#include "dump/dumptags.h"
#include "../mathml/mml_uchar.h"
#include "../dictionary/op_iddefs.h"
#include "../dictionary/entity_map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlaneText::CPlaneText() 
	: CNode( NODE_PLANETEXT )
	, CNodeExInfo()
	, m_string()
	, m_kegl( NODE_PLANETEXT__POINT_SIZE_DEFAULT )
	, m_plfc( NULL )
{
	long nStyle = getCurrentFormulatorStyle_OnChar();
	setFStyle( nStyle, ::getCurrentFormulatorStyle().getFontStyle( FSTYLE_VARIABLE ) );
}

//////////////////////////////////////////////////////////////////////

CPlaneText::CPlaneText( QString strText, long nStyle, CNodeInfo &ni, CNodeExInfo &ex_ni ) 
	: CNode( NODE_PLANETEXT )
	, CNodeExInfo()
	, m_string( strText )
	, m_kegl( NODE_PLANETEXT__POINT_SIZE_DEFAULT )
	, m_plfc( NULL )
{
	if( nStyle == -1 )
		nStyle = ::getCurrentFormulatorStyle().getStyleByChar( strText[ 0 ] );
	setFStyle( nStyle, ::getCurrentFormulatorStyle().getFontStyle( nStyle ) );

	setNodeInfo( ni );
	setNodeExInfo( ex_ni );
}

//////////////////////////////////////////////////////////////////////

CPlaneText::CPlaneText( CPlaneText* pPlaneText )
	: CNode( pPlaneText )
	, CNodeExInfo( pPlaneText )
	, m_string( pPlaneText->m_string )
	, m_kegl( pPlaneText->m_kegl )
	, m_plfc( NULL )
{
	setFStyle( pPlaneText->getFStyle(), pPlaneText->getOtherStyleFont() );
}

//////////////////////////////////////////////////////////////////////

CPlaneText::CPlaneText( CPlaneText* pPlaneText, long nFrom, long nTo )
	: CNode( pPlaneText )
	, CNodeExInfo( pPlaneText ) 
	, m_string( pPlaneText->m_string )
	, m_kegl( pPlaneText->m_kegl )
	, m_plfc( NULL )
{
	setFStyle( pPlaneText->getFStyle(), pPlaneText->getOtherStyleFont() );
	if( nFrom < 0 )
		nFrom = 0;
	if( nTo > 0 )
	{
		pPlaneText->m_string.remove( nFrom, nTo - nFrom );
		m_string = m_string.mid( nFrom, nTo - nFrom );
	}
	else
	{
		pPlaneText->m_string.truncate( nFrom );
		m_string = m_string.mid( nFrom );
	}
	//pPlaneText->m_string.remove( nFrom, nTo - nFrom );
	//if( nFrom > 0 )
	//	m_string.remove( 0, nFrom );
	//if( nTo > 0 )
	//	m_string.remove( nTo, m_string.size() - nTo );
}

//////////////////////////////////////////////////////////////////////

CPlaneText::~CPlaneText()
{
	if( m_plfc != NULL )
	{
		delete m_plfc;
		m_plfc = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

void CPlaneText::Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const
{
	CNode::Store( ar, sn, bracketSn );
	ar << (const CNodeExInfo&)(*this);

	// value
	ar << m_string;

	// positon
	qint32 nLong;
	nLong = (sn.GetNode() == this);
	ar << nLong;
	if( nLong )
	{
		nLong = sn.GetPosition();
		ar << nLong;
	}

	ar << m_kegl;
	ar << m_nFStyleNo;

	if( !::isFStyleHasReservedRecord( getFStyle() ) )
	{
		Q_ASSERT( m_plfc != NULL );
		ar << *m_plfc;
	}

	StoreBrackets( ar, bracketSn );
}

void CPlaneText::Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad )
{
	CNode::Load( ar, sn, bracketSn, isLeftBrLoad );
	ar >> (CNodeExInfo&)(*this);

	// value
	ar >> m_string;

	// load positon
	qint32 nLong;
	ar >> nLong;
	if( nLong )
	{
		sn.SetNode( this );
		ar >> nLong;
		sn.SetPosition( nLong );
	}

	ar >> m_kegl;
	ar >> m_nFStyleNo;

	if( !::isFStyleHasReservedRecord( getFStyle() ) )
	{
		if( m_plfc == NULL )
			m_plfc = new FS_LogFont();
		ar >> *m_plfc;
	}

	LoadBrackets( ar, bracketSn, isLeftBrLoad );
}

//////////////////////////////////////////////////////////////////////

void CPlaneText::setTextKeglSize( RealFde kegl )
{
	if( kegl == NODE_PLANETEXT__POINT_SIZE_DEFAULT )
	{
		m_kegl = kegl;
	}
	else
	{
		RealFde offset;
		QVector<long> level;
		::kegl2LevelOffset( kegl, level, offset );

		if( offset == 0.0 )
		{
			long nValue = qMin( GetLevel(), long(FTEXT_HEIGHT_LAST) );
			long i, count = level.size();
			for( i = 0; i < count; i++ )
			{
				if( level.at(i) == nValue )
				{
					m_kegl = NODE_PLANETEXT__POINT_SIZE_DEFAULT;
					break;
				}
			}
			if( i == count )
				m_kegl = qMax( RealFde(MML_MATHSIZE_MIN), kegl );
		}
		else
			m_kegl = qMax( RealFde(MML_MATHSIZE_MIN), kegl );
	}
}

//////////////////////////////////////////////////////////////////////

void CPlaneText::setFStyle( long st, const FS_LogFont_Color& lfs )
{
	if( isFStyleHasReservedRecord( st ) )
	{
		m_nFStyleNo = st;

		if( m_plfc != NULL )
		{
			delete m_plfc;
			m_plfc = NULL;
		}
	}
	else
	{
		m_nFStyleNo = FSTYLE_OTHER_STYLE;

		if( m_plfc == NULL )
			m_plfc = new FS_LogFont_Color();
		*m_plfc = lfs;
	}
}

//////////////////////////////////////////////////////////////////////

const FS_LogFont_Color& CPlaneText::getNeedFont( void )
{
	switch( getFStyle() )
	{
	case FSTYLE_OTHER_STYLE:
		return getOtherStyleFont();
	case FSTYLE_NONE:
		return ::getFontStyle( FSTYLE_VARIABLE );
	default:
		return ::getFontStyle( getFStyle() );
	}
}

//////////////////////////////////////////////////////////////////////

void CPlaneText::SetLevel( long nLevel )
{
	if( isLargeOp() && nLevel == FTEXT_HEIGHT_FRAME )
		CNode::SetLevel( FTEXT_HEIGHT_LARGEOP );
	else
		CNode::SetLevel( nLevel );
}

//////////////////////////////////////////////////////////////////////

int CPlaneText::isGluing( CPlaneText *pPlaneText )
{
	if( !isSingle() && !pPlaneText->isSingle() && 
		getFStyle() == pPlaneText->getFStyle() && 
		getTextKeglSize() == pPlaneText->getTextKeglSize() )
	{
		if( getFStyle() == FSTYLE_OTHER_STYLE && pPlaneText->getFStyle() == FSTYLE_OTHER_STYLE )
			return (getOtherStyleFont() == pPlaneText->getOtherStyleFont());
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////

int CPlaneText::CopyText( const QString& lpszString, long nFStyle, const FS_LogFont_Color& lfs, int fUnicode )
{
	if( fUnicode )
		setUnicode();

	setFStyle( nFStyle, lfs );
	m_string = lpszString;

	return 1;
}

//////////////////////////////////////////////////////////////////////

int CPlaneText::CopyNodeInfo( CNode* pNode )
{
	CNode::CopyNodeInfo( pNode );

	if( GetType() == NODE_PLANETEXT )
		setNodeExInfo( *((CPlaneText*) pNode) );

	return 1;
}

//////////////////////////////////////////////////////////////////////

int CPlaneText::RunCalc()
{
	int ret = 0;

	if( !getCalc() )
	{
		if( getCalcButtonID().length() )
			setMissButton();
		return 0;
	}
	if( !getCalc()->isPositionRunExist() ) return 0;

	QVector<RealFde> this_ltrb;
	this_ltrb.fill( 0.0, 4 );
	QVector<RealFde> this_margin_ltrb;
	this_margin_ltrb.fill( 0.0, 4 );
	QVector<RealFde> primitive_ltrb;
	primitive_ltrb.fill( 0.0, 4 * getGraphPrimNumber() );

	this_ltrb[ SVG_NodeTemplate_Parameter_Left ] = GetPosition().x();
	this_ltrb[ SVG_NodeTemplate_Parameter_Top ] = GetPosition().y();
	this_ltrb[ SVG_NodeTemplate_Parameter_Right ] = GetPosition().x() + GetSize().width();
	this_ltrb[ SVG_NodeTemplate_Parameter_Bottom ] = GetPosition().y() + GetSize().height();

	long i/*, cb = 0*/;
	for( i = 0; i < getGraphPrimNumber(); i++ )
	{
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 0 ) );
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 1 ) );
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 2 ) );
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 3 ) );
	}

	UseBtnCalc_InitByThisSize( *getCalc() );
	ret = getCalc()->PositionRun( this_ltrb, this_margin_ltrb, primitive_ltrb );

	// reset parent margins
	UseBtnCalc_ChangeThisMargin( *getCalc() );
	UseBtnCalc_ChangeThisSize( *getCalc() );

	for( i = 0; i < getGraphPrimNumber(); i++ )
	{
		if( getCalc()->isFormulaTypeGraphics( i ) )
		{
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Left,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Left ] );
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Top,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Top ] );
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Right,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Right ] );
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Bottom,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Bottom ] );
		}
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////

int CPlaneText::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc )
{
	Q_ASSERT( isExtNode() == 0 );

	int rr = -1;
	QColor dest_bkcolor( QColor( 255, 255, 255 ) );
	QString _mathbackground = getMathMLAttr().getFilteredAttr( FBL_ATTR_mathbackground );
	if( _mathbackground.length() )
	{
		QString backcolor_str = _mathbackground;
		rr = ::mml_mathcolor2color( backcolor_str, dest_bkcolor, QColor( 0, 0, 0 ) );
	}

	if( rr != -1 )
		fde.FillRectangle( tl.x(), tl.y(), GetSize().width(), GetSize().height(), FS_LogBrush(dest_bkcolor, Qt::SolidPattern) );

	if( isHidden() )
		return 0;

	RealFde cy = 0.0;
	SizeFde szV = getDrawSize().size();
	if( !canDraw( szV, cy ) )
	{
		CNodeExInfo_GraphPrimitive::Draw( fde, tl, this );
		// draw brackets
		DrawBrackets( fde, tl, rc );
		return 0;
	}

	// special case for sweeden
	RealFde addone = isOption_CalcTextByGlyph() ? getTopBearingMin( 0, -1 ) : 0;
	fde.DrawString( tl.x() + getLeftAdding() + getLeftBearing(), tl.y() + getTopIndent() - addone, GetString(), getFont() );

	if( getCurrentFormulatorReadOnlyState() && isReadOnly() )
	{
		// draw border around "CannotBeMoved" or "CannotBeEdited" text
		fde.DrawRectangle( tl.x(), tl.y(), GetSize().width(), GetSize().height(), QColor( 255, 0, 0 ) );
	}

	// draw brackets
	DrawBrackets( fde, tl, rc );

	return 1;
}

//////////////////////////////////////////////////////////////////////

RectFde CPlaneText::getDrawSize( long nfr, long nto, RealFde *percent )
{
	FS_LogFont lf = getFont();

	RectFde rc( 0.0, 0.0, 0.0, 0.0 );
	SizeFde strSz = CFmlDrawEngineQt::MeasureStringEx( GetString(), nfr, nto, lf );

	if( nfr == 0 )
		strSz.rwidth() += getLeftBearing();
	if( nto < 0 || nto >= GetStringLength() )
		strSz.rwidth() += getRightBearing();

	rc.setRight( rc.left() + strSz.width() );
	rc.setBottom( rc.top() + strSz.height() );

	// special case of overbar/underbar
	if( isOption_CalcTextByGlyph() )
		rc.setBottom( rc.bottom() - getTopBearingMin( nfr, nto ) );

	if( percent != NULL )
		*percent = CFmlDrawEngineQt::GetAscentEx( lf );

	if( isCannotDrawText() )
	{
		rc.setRight( rc.left() );
		if( getTopIndent() + getBottomIndent() != 0 )
			rc.setBottom( rc.top() );
	}

	return rc;
}

//////////////////////////////////////////////////////////////////////

RectFde CPlaneText::getDrawSize( long pos, RealFde *percent )
{
	return getDrawSize( 0, pos, percent );
}

RealFde CPlaneText::getAscent()
{
	return CFmlDrawEngineQt::GetAscentEx( getFont() );
}

RealFde CPlaneText::getDescent()
{
	return CFmlDrawEngineQt::GetDescentEx( getFont() );
}

RealFde CPlaneText::getLeftBearing()
{
	RealFde value = GetStringLength() ? 
		CFmlDrawEngineQt::GetLeftBearingEx( getFont(), GetString().at(0) ) : 0;
	return value < 0 ? fabs(value) : 0;
}

RealFde CPlaneText::getRightBearing()
{
	RealFde value = GetStringLength() ? 
		CFmlDrawEngineQt::GetRightBearingEx( getFont(), GetString().at( GetStringLength() - 1 ) ) : 0;
	return value < 0 ? fabs(value) : 0;
}

RealFde CPlaneText::getTopBearingMin( long nfr, long nto )
{
	RealFde value = 0;
	if( GetStringLength() )
	{
		if( nto < 0 ) nto = GetStringLength(); 
		value = CFmlDrawEngineQt::GetTopBearingMin( getFont(), GetString().constData() + nfr, nto - nfr );
	}
	return value < 0 ? fabs(value) : 0;
}

/////////////////////////////////////////////////////////////////////

RealFde CPlaneText::getLeftAdding( void )
	{ return ((m_nFStyleNo == FSTYLE_TEXT) ? getLeftIndent() : getLeftIndent() + getLSpace()); }

RealFde CPlaneText::getRightAdding( void )
	{ return ((m_nFStyleNo == FSTYLE_TEXT) ? getRightIndent() : getRightIndent() + getRSpace()); }

/////////////////////////////////////////////////////////////////////

long CPlaneText::getPosFromPoint( const PointFde &pt )
{
	if( isCannotDrawText() ) return 0;

	FS_LogFont lf = getFont();

	SizeFde szPrev( 0, 0 ), szNext( 0, 0 );
	PointFde apt = pt - GetAbsolutePosition();
	long i, nLen = GetStringLength();
	if( nLen == 1 )
	{
		szNext = CFmlDrawEngineQt::MeasureStringEx( GetString(), 1, lf );
		szNext.rwidth() += getLeftBearing() + getRightBearing();
		i = (apt.x() <= (szNext.width() / 2.0) ? 0 : 1);
	}
	else
	{
		for( i = 1; i <= nLen; i++ )
		{
			szNext = CFmlDrawEngineQt::MeasureStringEx( GetString(), i, lf );
			szNext.rwidth() += getLeftBearing();
			if( i >= nLen )
				szNext.rwidth() += getRightBearing();
			if( apt.x() <= szNext.width() )
			{
				if( apt.x() <= ((szPrev.width() + szNext.width()) / 2.0) )
					i--;
				break;
			}
			szPrev = szNext;
		}
		if( i > nLen )
			i = nLen;
	}
	return i;
}

/////////////////////////////////////////////////////////////////////

int CPlaneText::canDraw( SizeFde& sz, RealFde& height )
{
	if( isCannotDrawText() )
	{
		height = sz.height();
		return 0;
	}
	else if( GetParent() && GetParent()->GetType() == NODE_LINE && isStretchy() && getGraphPrimNumber() > 0 )
	{
		height = ((CLineNode*) GetParent())->getMaxCYNotStretchy();
		return sz.height() >= height || ((height - sz.height()) / height) <= FORMULATOR_PLANETEXT_CMPVALUE;
	}
	return 1;
}


/////////////////////////////////////////////////////////////////////

void CPlaneText::setMathSpace( CNodeInfo_Property& ni )
{
	setSpecialVerticalCenter( FBtnChildPos::MSPACE_Exact_HeightDepth, 0 );

	if( ni.getMMLHeight() < 0 ) ni.setMMLHeight( 0 );
	if( ni.getMMLDepth() < 0 ) ni.setMMLDepth( 0 );
	if( to_mathml_data.mathml_template_string.length() == 0 )
		to_mathml_data.mathml_template_string = ::mml_get_space_template( ni.getMathMLAttr(), ::getCurrentFormulatorStyle().getUnit2PX() );
}

/////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////

void CPlaneText::RecalculateSize_Base( SizeFde& sz, RealFde& ascent )
{
	sz = getDrawSize( -1, &ascent ).size();
	sz.rwidth() += getLeftAdding() + getRightAdding();
	sz.rheight() += getTopIndent() + getBottomIndent();

	if( getAlignmentType() == FBtnChildPos::MSPACE_Exact_HeightDepth )
	{
		if( getMMLWidth() > 0 )
			sz.rwidth() = (RealFde) getMMLWidth();
		if( getMMLHeight() > 0 || getMMLDepth() > 0 )
			sz.rheight() = (RealFde) (getMMLHeight() + getMMLDepth());
	}
}

/////////////////////////////////////////////////////////////////////

RealFde CPlaneText::Recalculate_VerticalCenter( SizeFde sz, RealFde nAscent, int cannotdraw )
{
	RealFde asc = ::getCurrentDefaultAscent( GetLevel() );
	RealFde h = ::getCurrentDefaultSize( GetLevel() ).height();

	if( getAlignmentType() == FBtnChildPos::MSPACE_Exact_HeightDepth )
	{
		if( getMMLHeight() > 0 )
			return getMMLHeight() - asc + h / 2.0;
		if( getMMLDepth() > 0 && (sz.height() - getMMLDepth()) >= 0 )
			return (sz.height() - getMMLDepth()) - asc + h / 2.0;
	}

	if( cannotdraw )
	{
		if( sz.height() != getTopIndent() + getBottomIndent() )
			return sz.height() / 2.0;
		else
			return getTopIndent() - h / 2.0;
	}

	// special case of overbar/underbar
	RealFde addone = isOption_CalcTextByGlyph() ? getTopBearingMin( 0, -1 ) : 0;
	return nAscent - asc + h / 2.0 - addone;
}


RealFde evalArgCannotDrawFormula( RealFde argument, const QString& coef )
{
	double ret = 0.0;
	if( coef.length() )
	{
		switch( coef.at(0).toAscii() )
		{
		case '*':
			ret = argument * coef.mid(1).toDouble();
			break;
		case '/':
			ret = coef.mid(1).toDouble();
			if( ret != 0.0 )
				ret = argument / ret;
			break;
		case '+':
			ret = argument + coef.mid(1).toDouble();
			break;
		case '-':
			ret = argument - coef.mid(1).toDouble();
			break;
		default:
			ret = coef.toDouble();
		}
	}
	return (RealFde) ret;
}

RealFde calcCannotDrawFormula( RealFde argument, QString coefficients )
{
	std::vector<RealFde> coef;
	RealFde value;
	static const RealFde defaultRet = 0.0;

	if( coefficients.length() == 0 ) return defaultRet;
	QStringList lst = coefficients.split(";", QString::SkipEmptyParts);
	for( long i = 0; i < lst.size(); i++ )
	{
		value = ::evalArgCannotDrawFormula( argument, lst.at(i) );
		coef.push_back( value );
	}

	if( coef.size() != 4 ) return defaultRet;
	return std::min(argument < coef[0] ? coef[1] : coef[2], coef[3]);
}

void CPlaneText::RecalculateSize( int bRecalc )
{
	SizeFde sz;
	RealFde ascent = 0.0;
	RealFde height = 0.0;
	RecalculateSize_Base( sz, ascent );

	int cannotdraw = !canDraw( sz, height );
	if( cannotdraw )
	{
		sz.rheight() = height;
		if( getCalc() )
		{
			sz.rwidth()  += ::calcCannotDrawFormula( height, getCalc()->getCannotDraw_Width() );
			sz.rheight() += ::calcCannotDrawFormula( height, getCalc()->getCannotDraw_Height() );
		}
	}

	SetSize( sz );
	SetVerticalCenter( Recalculate_VerticalCenter( sz, ascent, cannotdraw ) );
	if( cannotdraw )
	{
		SetCoord( SVG_NodeTemplate_Parameter_Y, 0.0 );
		if( RecalculateGrph( this, regetActiveCalc() ) )
		{
			RecalculateSize_Base( sz, ascent );
			SetSize( sz );
			SetVerticalCenter( Recalculate_VerticalCenter( sz, ascent, cannotdraw ) );
		}
	}
	if( bRecalc )
		GetParent()->RecalculateSize( bRecalc );
}

void CPlaneText::RecalculateSizeFull()
{
	RecalculateSize( 0 );
}

/////////////////////////////////////////////////////////////////////

QString CPlaneText::GetNodeText()
{
	if( isUnicode() )
	{
		QString res = __mml_unicode_char2string( GetString().at(0).unicode() );
		if( ::isOption_UnicodeAsName() )
		{
			std::vector<QString> dest_str;
			std::vector<long> dest_style;
			QString emptyV;
			::getMathMLChar2Style( dest_str, dest_style, res, emptyV, __GLOBAL_USER_MODE3 );
		}
		return QString( res );
	}
	return QString( GetString() );
}

/////////////////////////////////////////////////////////////////////

int CPlaneText::isNewPlaneText( RealFde kegl, long fstyle )
{
	if( !isSingle() && getTextKeglSize() == kegl )
	{
		if( getFStyle() == FSTYLE_OTHER_STYLE )
		{
			return !(getOtherStyleFont() == ::getFontStyle( fstyle ));
		}
		if( getFStyle() == fstyle )
			return 0;
		if( (getFStyle() == FSTYLE_VARIABLE || getFStyle() == FSTYLE_FUNCTION) && 
			(fstyle == FSTYLE_NUMBER || fstyle == FSTYLE_VARIABLE || fstyle == FSTYLE_FUNCTION) )
			return 0;
		return getFStyle() != fstyle;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////

QString CPlaneText::getMathMLNodeDescription()
{
	QString ret = to_mathml__getOneTag();
	return ret.length() ? (QString("\"") + ret + QString("\" token element")) : ret;
}

/////////////////////////////////////////////////////////////////////////////
