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

#ifndef __COMMON_CDSRBASEMATRIX_H__
#define __COMMON_CDSRBASEMATRIX_H__

#include "cdsrbasevector.h"

template<class T> class CDSRBaseMatrix : public std::vector<T>
{
public:
	CDSRBaseMatrix( const CDSRBaseMatrix& matrix )
		: std::vector<T>( matrix.begin(), matrix.end() )
	{
		m_row = matrix.row();
		m_col = matrix.col();
	}
	CDSRBaseMatrix( typename std::vector<T>::iterator first, long row, long col )
		: std::vector<T>( first, first + row * col )
	{
		m_row = row;
		m_col = col;
	}
	CDSRBaseMatrix( typename std::vector<T>::const_iterator first, long row, long col )
		: std::vector<T>( first, first + row * col )
	{
		m_row = row;
		m_col = col;
	}
	CDSRBaseMatrix( long row = 0, long col = 0, const T& val = T() )
		: std::vector<T>( row * col, val )
	{
		m_row = row;
		m_col = col;
	}
	CDSRBaseMatrix( T **first, long row, long col )
		: std::vector<T>( row * col )
	{
		m_row = row;
		m_col = col;
		long i, j;
		for( i = 0; i < m_row; i++ )
			for( j = 0; j < m_col; j++ )
				(*this)( i, j ) = first[ i ][ j ];
	}
	CDSRBaseMatrix( const T **first, long row, long col )
		: std::vector<T>( row * col )
	{
		m_row = row;
		m_col = col;
		long i, j;
		for( i = 0; i < m_row; i++ )
			for( j = 0; j < m_col; j++ )
				(*this)( i, j ) = first[ i ][ j ];
	}
	virtual ~CDSRBaseMatrix()
	{
	}

	// 
	long row() const
	{
		return m_row;
	}
	long col() const
	{
		return m_col;
	}

	// оставлены для совместимости
	long n_row() const
	{
		return row();
	}
	long n_column() const
	{
		return col();
	}
	long n_col() const
	{
		return col();
	}

	typename std::vector<T>::const_reference at( long index ) const
	{
#ifdef _DEBUG
		assert( index < (long) std::vector<T>::size() );
#endif
		return std::vector<T>::at( index );
	}
	typename std::vector<T>::reference at( long index )
	{
#ifdef _DEBUG
		assert( index < (long) std::vector<T>::size() );
#endif
		return std::vector<T>::at( index );
	}

	typename std::vector<T>::const_reference operator[]( long index ) const 
	{
#ifdef _DEBUG
		assert( index < (long) std::vector<T>::size() );
#endif
		return std::vector<T>::operator[]( index );
	}
	typename std::vector<T>::reference operator[]( long index )
	{
#ifdef _DEBUG
		assert( index < (long) std::vector<T>::size() );
#endif
		return std::vector<T>::operator[]( index );
	}

	//
	typename std::vector<T>::const_reference operator()( long i, long j ) const
	{
#ifdef _DEBUG
		assert( i < m_row && j < m_col );
#endif
		return (*this)[ i * m_col + j ];
	}
	typename std::vector<T>::reference operator()( long i, long j )
	{
#ifdef _DEBUG
		assert( i < m_row && j < m_col );
#endif
		return (*this)[ i * m_col + j ];
	}

	void resize_matrix( long row, long col, const T& value = T() )
	{
		// change column count
		if ( col > m_col )
		{
			std::vector<T>::resize( m_row * col, value );

			// move elements
			long newpos = m_row * col - col + m_col - 1;
			long size = m_row * m_col;
			long pos = size - 1;

			for ( long i = m_col; i < size; i+= m_col )
			{
				long j;
				for ( j = 0; j < m_col; j++ )
					at( newpos-- ) = at( pos-- );
				for ( ; j < col; j++ )
					at( newpos-- ) = value;
			}

			m_col = col;
		}
		else if ( col < m_col )
		{
			long pos = col;
			long oldsize = m_row * m_col;
			long i;
			for ( i = m_col; i < oldsize; i += m_col)
				for ( long j = 0; j < col; j++ )
					at( pos++ ) = at( i + j );
	
			m_col = col;
			std::vector<T>::resize( m_row * m_col );
		}

		// change row count
		if ( row != m_row )
		{
			std::vector<T>::resize( row * m_col, value );
			m_row = row;
		}
	}

	void add_row( long count = 1, const T& value = T() )
	{
		resize_matrix( m_row + count, m_col, value );
	}
	void add_column( long count = 1, const T& value = T() )
	{
		resize_matrix( m_row, m_col + count, value );
	}

	void insert_row( long index, long count = 1, const T& value = T() )
	{
		if ( index < m_row )
		{
			std::vector<T>::insert( std::vector<T>::begin() + index * m_col, count * m_col, value );
			m_row += count;
		}
		else
		{
			resize_matrix( m_row + count, m_col );
		}
	}

	void insert_column( long index, long count = 1, const T& value = T() )
	{
		if ( index < m_col )
		{
			m_col += count;
			std::vector<T>::resize( m_row * m_col );

			// move elements
			long pos = m_col * m_row - 1;
			long last = m_col - count - index;
			long first = last + count;
			long i, j;
			for ( i = m_row * (m_col - count) - 1; i > 0; )
			{
				for ( j = 0; j < last; j++ )
					(*this)[ pos-- ] = at( i-- );
				for ( ; j < first; j++ )
					(*this)[ pos-- ] = value;
				for ( ; j < m_col; j++ )
					(*this)[ pos-- ] = at( i-- );
			}
		}
		else
		{
			resize_matrix( m_row, m_col + count );
		}
	}

	void insert_row( long index, const CDSRBaseVector<T> &vec )
	{
		insert_row( index );
		long count = min( vec.size(), m_col );
		for ( long j = 0; j < count; j++ )
			(*this)( index, j ) = vec[ j ];
	}
	void insert_column( long index, const CDSRBaseVector<T> &vec )
	{
		insert_column( index );
		long count = min( vec.size(), m_row );
		for ( long i = 0; i < count; i++ )
			(*this)( i, index ) = vec[ i ];
	}

	void erase_row( long first, long last )
	{
#ifdef _DEBUG
		assert( first < last && last <= m_row );
#endif
		if ( last < m_row )
		{
			std::vector<T>::erase( std::vector<T>::begin() + first * m_col, std::vector<T>::begin() + last * m_col );
			m_row -= last - first;
		}
		else
			resize_matrix( first, m_col );
	}
	void erase_column( long first, long last )
	{
#ifdef _DEBUG
		assert( first < last && last <= m_col );
#endif

		if ( last < m_col )
		{
			long i, j;
			long pos = 0;
			long size = m_row * m_col;
			for ( i = 0; i < size; )
			{
				for( j = 0; j < first; j++ )
					at( pos++ ) = at( i++ );
				i += last - first;
				for ( j = last; j < m_col; j++ )
					at( pos++ ) = at( i++ );
			}

			m_col -= last - first;
			std::vector<T>::resize( m_row * m_col );
		}
		else
			resize_matrix( m_row, first );
	}
	void erase_row( long index )
	{
		erase_row( index, index + 1 );
	}
	void erase_column( long index )
	{
		erase_column( index, index + 1 );
	}

	void erase_all_column()
	{
		resize_matrix( 0, 0 );
	}

	const CDSRBaseMatrix<T>& operator = ( const CDSRBaseMatrix<T>& matrix )
	{
		resize( matrix.size() );
		m_row = matrix.m_row;
		m_col = matrix.m_col;
		long i, j;
		for ( i = 0; i < m_row; i++ )
			for ( j = 0; j < m_col; j++ )
				(*this)( i, j ) = matrix( i, j );
		return (*this);
	}

	bool operator == ( const CDSRBaseMatrix<T>& matrix )
	{
		// Проверка на то, равны ли размерности матриц
		if ( m_row != matrix.m_row || m_col != matrix.m_col )
			return false;
		// Проверка на равенство элементов матриц
		long i, j;
		for ( i = 0; i < m_row; i++ )
			for ( j = 0; j < m_col; j++ )
				if ( (*this)( i, j ) != matrix( i, j ) )
				return false;
		return true;
	}

	CDSRBaseVector<T>* diag()
	{
		CDSRBaseVector<T> *pDiag = new CDSRBaseVector<T>( m_row );

		long i;
		for ( i = 0; i < m_row; i++ )
			(*pDiag)[ i ] = (*this)( i, i );
		return pDiag;
	}

	CDSRBaseVector<T>* row( long row )
	{
#ifdef _DEBUG
		assert( row < m_row );
#endif
		CDSRBaseVector<T> *pRow = new CDSRBaseVector<T>( m_col );

		long j;
		for ( j = 0; j < m_col; j++ )
			(*pRow)[ j ] = (*this)( row, j );
		return pRow;
	}

	CDSRBaseVector<T>* col( long col )
	{
		return column( col );
	}
	// оставлено для совместимости
	CDSRBaseVector<T>* column( long col )
	{
#ifdef _DEBUG
		assert( col < m_col );
#endif
		CDSRBaseVector<T> *pCol = new CDSRBaseVector<T>( m_row );

		long i;
		for ( i = 0; i < m_row; i++ )
			(*pCol)[ i ] = (*this)( i, col );
		return pCol;
	}

protected :
	long	m_row;
	long	m_col;
};

//////////////////////////////////////////////////////////////////////
#endif //__COMMON_CDSRBASEMATRIX_H__
