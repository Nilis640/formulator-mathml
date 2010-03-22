
#ifndef __COMMON_BITSETT_H__
#define __COMMON_BITSETT_H__

#include <string>

class bitsetT {
	typedef unsigned long _Ty;
public:
	typedef bool element_type;

	///////////////////////////////////////////////////
	// class reference
	class reference {
	friend class bitsetT;
	public:
		reference& operator=(bool _X)
			{_Pbs->set(_Off, _X);
			return (*this); }
		reference& operator=(const reference& _Bs)
			{_Pbs->set(_Off, bool(_Bs));
			return (*this); }
		reference& flip()
			{_Pbs->flip(_Off);
			return (*this); }
		bool operator~() const
			{return (!_Pbs->test(_Off)); }
		operator bool() const
			{return (_Pbs->test(_Off)); }
	private:
		reference(bitsetT& _X, size_t _P)
			: _Pbs(&_X), _Off(_P) {}
		bitsetT *_Pbs;
		size_t _Off;
		};
	//	end class reference
	//////////////////////////////////////////////////////
	//_STCONS(size_t, bitset_size, _N);
	bool at(size_t _P) const
		{if (_N <= _P)
			_Xran();
		return (test(_P)); }
	reference at(size_t _P)
		{if (_N <= _P)
			_Xran();
		return (reference(*this, _P)); }
	bool operator[](size_t _P) const
		{return (test(_P)); }
	reference operator[](size_t _P)
		{return (reference(*this, _P)); }

	//////////////////////////
	// constructors
	bitsetT(long n)
	{
		_Nb = CHAR_BIT * sizeof (_Ty);
		_N = n;
		_Nw = _N == 0 ? 0 : ((_N - 1) / _Nb);
		bitset_size	= n;
		_A = new _Ty[_Nw + 1];
		_Tidy();
	}
	bitsetT(unsigned long _X, long n)
	{
		_Nb = CHAR_BIT * sizeof (_Ty);
		_N = n;
		_Nw = _N == 0 ? 0 : ((_N - 1) / _Nb);
		bitset_size	= n;
		_A = new _Ty[_Nw + 1];
		_Tidy();

		for (size_t _P = 0; _X != 0 && _P < _N; _X >>= 1, ++_P)
			if (_X & 1)
				set(_P);
	}
	explicit bitsetT(const std::string& _S, size_t _P = 0,
		size_t _L = (size_t)(-1), long n = 0)
	{
		_Nb = CHAR_BIT * sizeof (_Ty);
		_N = n;
		_Nw = _N == 0 ? 0 : ((_N - 1) / _Nb);
		bitset_size	= n;
		_A = new _Ty[_Nw + 1];


		size_t _I;
		if (_S.size() < _P)
			_Xran();
		if (_S.size() - _P < _L)
			_L = _S.size() - _P;
		if (_N < _L)
			_L = _N;
		_Tidy(), _P += _L;
		for (_I = 0; _I < _L; ++_I)
			if (_S[--_P] == '1')
				set(_I);
			else if (_S[_P] != '0')
				_Xinv();
	}
	//	end constructors
	/////////////////////////

	////////////////////////
	// destructor
	virtual ~bitsetT()
	{
		if(_Nw == 0)
			delete _A;
		else
			delete[] _A;
	};

	// end destructor
	////////////////////////
	

	bitsetT& operator&=(const bitsetT& _R)
		{for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] &= _R._W(_I);
		return (*this); }
	bitsetT& operator|=(const bitsetT& _R)
		{for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] |= _R._W(_I);
		return (*this); }
	bitsetT& operator^=(const bitsetT& _R)
		{for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] ^= _R._W(_I);
		return (*this); }
	bitsetT& operator<<=(size_t _P)
		{if (_P < 0)
			return (*this >>= -_P);
		const int _D = _P / _Nb;
		if (_D != 0)
			for (int _I = _Nw; 0 <= _I; --_I)
				_A[_I] = _D <= _I ? _A[_I - _D] : 0;
		if ((_P %= _Nb) != 0)
			{for (int _I = _Nw; 0 < _I; --_I)
				_A[_I] = (_A[_I] << _P)
					| (_A[_I - 1] >> (_Nb - _P));
			_A[0] <<= _P, _Trim(); }
		return (*this); }
	bitsetT& operator>>=(size_t _P)
		{if (_P < 0)
			return (*this <<= -_P);
		const int _D = _P / _Nb;
		if (_D != 0)
			for (int _I = 0; _I <= _Nw; ++_I)
				_A[_I] = _D <= _Nw - _I ? _A[_I + _D] : 0;
		if ((_P %= _Nb) != 0)
			{for (int _I = 0; _I < _Nw; ++_I)
				_A[_I] = (_A[_I] >> _P)
					| (_A[_I + 1] << (_Nb - _P));
			_A[_Nw] >>= _P; }
		return (*this); }
	bitsetT& set()
		{_Tidy(~(_Ty)0);
		return (*this); }
	bitsetT& set(size_t _P, bool _X = true)
		{if (_N <= _P)
			_Xran();
		if (_X)
			_A[_P / _Nb] |= (_Ty)1 << _P % _Nb;
		else
			_A[_P / _Nb] &= ~((_Ty)1 << _P % _Nb);
		return (*this); }
	bitsetT& reset()
		{_Tidy();
		return (*this); }
	bitsetT& reset(size_t _P)
		{return (set(_P, 0)); }
	bitsetT operator~() const
		{return (bitsetT(*this).flip()); }
	bitsetT& flip()
		{for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] = ~_A[_I];
		_Trim();
		return (*this); }
	bitsetT& flip(size_t _P)
		{if (_N <= _P)
			_Xran();
		_A[_P / _Nb] ^= (_Ty)1 << _P % _Nb;
		return (*this); }
	//unsigned long to_ulong() const
	//	{enum {_Assertion = 1 /
	//		(sizeof (unsigned long) % sizeof (_Ty) == 0)};
	//	int _I = _Nw;
	//	for (; sizeof (unsigned long) / sizeof (_Ty) <= _I; --_I)
	//		if (_A[_I] != 0)
	//			_Xoflo();
	//	unsigned long _V = _A[_I];
	//	for (; 0 <= --_I; )
	//		_V = _V << _Nb | _A[_I];
	//	return (_V); }
	std::string to_string() const
		{std::string _S;
		_S.reserve(_N);
		for (size_t _P = _N; 0 < _P; )
			_S += test(--_P) ? _T('1') : _T('0');
		return (_S); }
	size_t count() const
		{size_t _V = 0;
		for (int _I = _Nw; 0 <= _I; --_I)
			for (_Ty _X = _A[_I]; _X != 0; _X >>= 4)
				_V += "\0\1\1\2\1\2\2\3"
					"\1\2\2\3\2\3\3\4"[_X & 0xF];
		return (_V); }
	size_t size() const
		{return (_N); }
	bool operator==(const bitsetT& _R) const
		{for (int _I = _Nw; 0 <= _I; --_I)
			if (_A[_I] != _R._W(_I))
				return (false);
		return (true); }
	bool operator!=(const bitsetT& _R) const
		{return (!(*this == _R)); }
	bool test(size_t _P) const
		{if (_N <= _P)
			_Xran();
		return ((_A[_P / _Nb] & ((_Ty)1 << _P % _Nb)) != 0); }
	bool any() const
		{for (int _I = _Nw; 0 <= _I; --_I)
			if (_A[_I] != 0)
				return (true);
		return (false); }
	bool none() const
		{return (!any()); }
	bitsetT operator<<(size_t _R) const
		{return (bitsetT(*this) <<= _R); }
	bitsetT operator>>(size_t _R) const
		{return (bitsetT(*this) >>= _R); }
	friend bitsetT operator&(const bitsetT& _L,
		const bitsetT& _R)
		{return (bitsetT(_L) &= _R); }
	friend bitsetT operator|(const bitsetT& _L,
		const bitsetT& _R)
		{return (bitsetT(_L) |= _R); }
	friend bitsetT operator^(const bitsetT& _L,
		const bitsetT& _R)
		{return (bitsetT(_L) ^= _R); }
		// TEMPLATE operator>>
	friend std::istream& operator>>(istream& _I, bitsetT& _R)
	{std::ios_base::iostate _St = std::ios_base::goodbit;
	bool _Chg = false;
	std::string _X;
	const std::istream::sentry _Ok(_I);
	if (_Ok)
		{_TRY_IO_BEGIN
		int _C = _I.rdbuf()->sgetc();
		for (size_t _M = _R.size(); 0 < _M;
			_C = _I.rdbuf()->snextc(), --_M)
			{if (_C == EOF)
				{_St |= ios_base::eofbit;
				break; }
			else if (_C != '0' && _C != '1')
				break;
			else if (_X.max_size() <= _X.size())
				{_St |= ios_base::failbit;
				break; }
			else
				_X.append(1, (char)_C), _Chg = true; }
		_CATCH_IO_(_I); }
	if (!_Chg)
		_St |= ios_base::failbit;
	_I.setstate(_St);
	_R = bitsetT(_X);
	return (_I); }
	_Ty _W(size_t _I) const
		{return (_A[_I]); }
private:
	void _Tidy(_Ty _X = 0)
		{for (int _I = _Nw; 0 <= _I; --_I)
			_A[_I] = _X;
		if (_X != 0)
			_Trim(); }
	void _Trim()
		{if (_N % _Nb != 0)
			_A[_Nw] &= ((_Ty)1 << _N % _Nb) - 1; }
	void _Xinv() const
		{_THROW(invalid_argument, _T("invalid bitsetT<N> char")); }
	void _Xoflo() const
		{_THROW(overflow_error, _T("bitsetT<N> conversion overflow")); }
	void _Xran() const
		{_THROW(out_of_range, _T("invalid bitsetT<N> position")); }

	/////////////////////////////////////
	// variables
	size_t bitset_size;
	long _Nb;
	long _Nw;
	long _N;
	_Ty* _A;
};


#endif //_BITSETT_H_
