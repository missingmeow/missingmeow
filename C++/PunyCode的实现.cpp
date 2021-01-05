//////////////////////////////////////////////////////////////////////////
// copy from http://rfc-ref.org/RFC-TEXTS/3492/chapter12.html

enum { base = 36, tmin = 1, tmax = 26, skew = 38, damp = 700, initial_bias = 72, initial_n = 0x80, delimiter = 0x2D };

typedef unsigned int	punycode_uint;

#define basic(cp) ((punycode_uint)(cp) < 0x80)			// basic(cp) tests whether cp is a basic code point:
#define delim(cp) ((cp) == delimiter)					// delim(cp) tests whether cp is a delimiter:

/* decode_digit(cp) returns the numeric value of a basic code */
/* point (for use in representing integers) in the range 0 to */
/* base-1, or base if cp is does not represent a value.       */

static inline punycode_uint decode_digit(punycode_uint cp)
{
	return  cp - 48 < 10 ? cp - 22 :  cp - 65 < 26 ? cp - 65 : cp - 97 < 26 ? cp - 97 :  base;
}

/* encode_digit(d,flag) returns the basic code point whose value      */
/* (when used for representing integers) is d, which needs to be in   */
/* the range 0 to base-1.  The lowercase form is used unless flag is  */
/* nonzero, in which case the uppercase form is used.  The behavior   */
/* is undefined if flag is nonzero and digit d has no uppercase form. */
static char encode_digit(punycode_uint d, int flag)
{
	return d + 22 + 75 * (d < 26) - ((flag != 0) << 5);
	/*  0..25 map to ASCII a..z or A..Z */
	/* 26..35 map to ASCII 0..9         */
}

/* flagged(bcp) tests whether a basic code point is flagged */
/* (uppercase).  The behavior is undefined if bcp is not a  */
/* basic code point.                                        */
#define flagged(bcp) ((punycode_uint)(bcp) - 65 < 26)

/* encode_basic(bcp,flag) forces a basic code point to lowercase */
/* if flag is zero, uppercase if flag is nonzero, and returns    */
/* the resulting code point.  The code point is unchanged if it  */
/* is caseless.  The behavior is undefined if bcp is not a basic */
/* code point.                                                   */
static inline char encode_basic(punycode_uint bcp, int flag)
{
	bcp -= (bcp - 97 < 26) << 5;
	return bcp + ((!flag && (bcp - 65 < 26)) << 5);
}

/*** Platform-specific constants ***/

/* maxint is the maximum value of a punycode_uint variable: */
static const punycode_uint maxint = static_cast<punycode_uint>(-1);
/* Because maxint is unsigned, -1 becomes the maximum value. */

/*** Bias adaptation function ***/
static punycode_uint adapt( punycode_uint delta, punycode_uint numpoints, int firsttime )
{
	punycode_uint k;

	delta = firsttime ? delta / damp : delta >> 1;
	/* delta >> 1 is a faster way of doing delta / 2 */
	delta += delta / numpoints;

	for (k = 0;  delta > ((base - tmin) * tmax) / 2;  k += base) {
		delta /= base - tmin;
	}

	return k + (base - tmin + 1) * delta / (delta + skew);
}

typedef std::vector<punycode_uint>		typePunyCodeUintString;
static bool onePart2PunyCode( const typePunyCodeUintString & strSrc, tstring & strTar )
{
	punycode_uint n, delta, h, b, bias, m, q, k, t;

	/* Initialize the state: */
	n = initial_n;
	delta = 0;
	bias = initial_bias;

	/* Handle the basic code points: */
	typePunyCodeUintString::const_iterator it = strSrc.begin();
	typePunyCodeUintString::const_iterator itEnd = strSrc.end();
	for (; it != itEnd; ++it )
	{
		punycode_uint ch = *it;
		if ( basic( ch ) )
			strTar += static_cast<char>( ch );
	}

	h = b = strTar.size();

	if ( h == strSrc.size() )
		return false;

	/* h is the number of code points that have been handled, b is the  */
	/* number of basic code points, and out is the number of characters */
	/* that have been output.                                           */
	if ( h < strSrc.size() && b > 0) 
		strTar += static_cast<char>( delimiter );

	/* Main encoding loop: */

	while ( h < strSrc.size() ) 
	{
		/* All non-basic code points < n have been  handled already.  Find the next larger one: */
		for (m = maxint, it = strSrc.begin();  it != itEnd;  ++it)	/* if (basic(input[j])) continue;  (not needed for Punycode) */
			if ( static_cast<punycode_uint>(*it) >= n && static_cast<punycode_uint>(*it) < m ) 
				m = static_cast<punycode_uint>(*it);

		/* Increase delta enough to advance the decoder's <n,i> state to <m,0>, but guard against overflow: */
		delta += (m - n) * (h + 1);
		n = m;

		for ( it = strSrc.begin();  it != itEnd;  ++it)
		{
			punycode_uint ch = *it;
			if ( ch < n /* || basic(input[j]) */ )	// Punycode does not need to check whether input[j] is basic:
			{
				if (++delta == 0) 
					return false;
			}
			if ( ch == n) 
			{
				for (q = delta, k = base;  ;  k += base)	//Represent delta as a generalized variable-length integer:
				{
					t = k <= bias /* + tmin */ ? tmin :     /* +tmin not needed */
						k >= bias + tmax ? tmax : k - bias;
					if (q < t) break;
					strTar += encode_digit(t + (q - t) % (base - t), 0);
					q = (q - t) / (base - t);
				}
				strTar += encode_digit(q, 0);
				bias = adapt(delta, h + 1, h == b);
				delta = 0;
				++h;
			}
		}
		++delta, ++n;
	}
	return true;
}

static bool onePart2PunyCode( const twchar * pData, const twchar * pEnd, tstring & strTar )
{
	typePunyCodeUintString	strRealSrc;
	strRealSrc.reserve( pEnd - pData );
	for (const twchar * p = pData; p != pEnd; ++p)
	{
		//http://zh.wikipedia.org/wiki/UTF-16
		twchar ch = *p;
		if ( (ch & 0xF800) == 0xD800 )
		{
			if ( ++p == pEnd )
				break;
			twchar ch2 = *p;
			punycode_uint a = static_cast<punycode_uint>(ch & 0x3FF);
			punycode_uint b = static_cast<punycode_uint>(ch2 & 0x3FF);
			strRealSrc.push_back( ((a << 10) | b) + 0x10000 );
		}
		else
			strRealSrc.push_back( static_cast<punycode_uint>(ch) );
	}
	tstring strPart;
	strPart.reserve( strRealSrc.size() * 3 );
	bool boIsPunyCode = onePart2PunyCode( strRealSrc, strPart );
	if ( boIsPunyCode )
		strTar += "xn--";
	strTar += strPart;
	return boIsPunyCode;
}

static bool onePartFromPunyCode( const tstring & strSrc, typePunyCodeUintString & strTar )
{
	punycode_uint n, bias, b, oldi, in, w, k, digit, t, i, out;

	/* Initialize the state: */
	n = initial_n;
	i = 0;
	bias = initial_bias;

	/* Handle the basic code points:  Let b be the number of input code */
	/* points before the last delimiter, or 0 if there is none, then    */
	/* copy the first b code points to the output.                      */
	tstring::const_iterator it = strSrc.begin();
	tstring::const_iterator itEnd = strSrc.end();
	for (b = 0;  it != itEnd;  ++it, ++b ) 
	{
		if ( delim(*it) )
			break;
		else if ( !basic(*it) )
			return false;
		strTar.push_back( static_cast<punycode_uint>(*it) );
	}

	//no delimiter exist?
	if(b == strSrc.size())
	{
		b = 0;
		strTar.clear();
	}

	out = strTar.size();
	/* Main decoding loop:  Start just after the last delimiter if any  */
	/* basic code points were copied; start at the beginning otherwise. */
	punycode_uint input_length = static_cast<punycode_uint>( strSrc.size() );
	for (in = b > 0 ? b + 1 : 0;  in < input_length; ++out) 
	{
		/* in is the index of the next character to be consumed, and */
		/* out is the number of code points in the output array.     */
		/* Decode a generalized variable-length integer into delta,  */
		/* which gets added to i.  The overflow checking is easier   */
		/* if we increase i as we go, then subtract off its starting */
		/* value at the end to obtain delta.                         */
		for (oldi = i, w = 1, k = base;  ;  k += base) 
		{
			if (in >= input_length) 
				return false;
			digit = decode_digit( strSrc[in++] );
			if (digit >= base) 
				return false;
			if (digit > (maxint - i) / w) 
				return false;
			i += digit * w;
			t = k <= bias /* + tmin */ ? tmin :     /* +tmin not needed */
				k >= bias + tmax ? tmax : k - bias;
			if (digit < t) 
				break;
			if (w > maxint / (base - t)) 
				return false;
			w *= (base - t);
		}
		bias = adapt(i - oldi, out + 1, oldi == 0);

		/* i was supposed to wrap around from out+1 to 0,   */
		/* incrementing n each time, so we'll fix that now: */
		if (i / (out + 1) > maxint - n) 
			return false;
		n += i / (out + 1);
		i %= (out + 1);

		/* Insert n at position i of the output: */
		/* not needed for Punycode: */
		/* if (decode_digit(n) <= base) return punycode_invalid_input; */
		strTar.insert( strTar.begin() + i, n );
		++i;
	}

	return true;
}

static bool onePartFromPunyCode( const tstring & strSrc, twstring & strTar )
{
	if ( strSrc.size() > 4 && strSrc.compare( 0, 4, "xn--", 4 ) == 0 )
	{
		typePunyCodeUintString	strTemp;
		strTemp.reserve( strSrc.size() );
		onePartFromPunyCode( strSrc.substr( 4 ), strTemp );
		typePunyCodeUintString::const_iterator it = strTemp.begin();
		typePunyCodeUintString::const_iterator itEnd = strTemp.end();
		for (; it != itEnd; ++it )
		{
			punycode_uint ch = *it;
			if ( ch >= 0x10000 )
			{
				ch -= 0x10000;
				punycode_uint vh = (ch >> 10) & 0x3FF;
				punycode_uint vl = ch & 0x3FF;
				strTar += (static_cast<twchar>( vh ) | 0xD800);
				strTar += (static_cast<twchar>( vl ) | 0xDC00);
			}
			else
				strTar += static_cast<twchar>(ch);
		}
		return true;
	}
	tstring::const_iterator it = strSrc.begin();
	tstring::const_iterator itEnd = strSrc.end();
	for ( ; it != itEnd; ++it )
		strTar += (twchar)(*it);
	return false;
}

//////////////////////////////////////////////////////////////////////////

bool toPunyCode(const twstring & strSrc, tstring & strTar)
{
	strTar.reserve( strSrc.size() * 3 );
	bool boIsPunyCode = false;
	const twchar * pBegin = strSrc.data();
	const twchar * pEnd = strSrc.data() + strSrc.size();
	const twchar * p = pBegin;
	for ( ; p != pEnd; ++p )
	{
		if ( *p == '.' || *p == '@' )
		{
			if ( pBegin != p && onePart2PunyCode( pBegin, p, strTar ) )
				boIsPunyCode = true;
			strTar += (char)*p;
			pBegin = p + 1;
		}
	}
	if ( pBegin != p && onePart2PunyCode( pBegin, p, strTar ) )
		boIsPunyCode = true;
	return boIsPunyCode;
}

bool fromPunyCode(const tstring & strSrc, twstring & strTar)
{
	strTar.reserve( strSrc.size() );
	bool boIsPunyCode = false;
	size_t i = 0, b = 0;
	size_t n = strSrc.size();
	for ( ; i < n; ++i )
	{
		char ch = strSrc[i];
		if ( ch == '.' || ch == '@' )
		{
			if ( i != b && onePartFromPunyCode( strSrc.substr(b , i - b ), strTar ) )
				boIsPunyCode = true;
			strTar += twchar( ch );
			b = i + 1;
		}
	}
	if ( i != b && onePartFromPunyCode( strSrc.substr(b , i - b ), strTar ) )
		boIsPunyCode = true;

	return boIsPunyCode;
}

bool isPunyCode(const tstring & strSrc)
{
	tstring::size_type p = strSrc.find( "xn--" );
	if ( p == tstring::npos )
		return false;
	else if ( p == 0 )
		return true;
	else if ( strSrc[p - 1] =='.' || strSrc[p - 1] =='@' )
		return true;
	return false;
}
