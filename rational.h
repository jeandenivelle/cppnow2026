
#ifndef RATIONAL_
#define RATIONAL_

#include <concepts>
#include "util/bigint.h"

struct rational
{
   bigint num;
   bigint denom;

   static bigint gcd( bigint num, bigint denom );

   void normalize( );

   rational( ) 
      : num(0), denom(1)
   { }

   template< std::convertible_to<bigint> N, std::convertible_to<bigint> D >
   rational( N&& num, D&& denom )
      : num( std::forward<N>( num )), denom( std::forward<D>( denom ))
   {
      normalize( );
   }

   template< std::convertible_to<bigint> N > 
   rational( N&& num )
      : num( std::forward<N>( num )), denom(1) 
   { }

   rational& operator *= ( const rational& r );
   rational& operator /= ( const rational& r );
 
   rational operator - ( ) const
   {
      auto res = *this;
      res. num = - res. num;
      return res;
   }

   double approximation( ) const
   {
      return num. approximation( ) / denom. approximation( );
   }

};


inline rational operator + ( const rational& r1, const rational& r2 )
{
   return rational( r1. num * r2. denom + r2. num * r1. denom,
                    r1. denom * r2. denom );
}

inline rational operator - ( const rational& r1, const rational& r2 )
{
   return rational( r1. num * r2. denom - r2. num * r1. denom,
                    r1. denom * r2. denom );
}

inline rational& operator += ( rational& r1, const rational& r2 )
{
   r1 = r1 + r2; 
   return r1;
}

inline rational& operator -= ( rational& r1, const rational& r2 )
{
   r1 = r1 - r2; 
   return r1;
}

inline rational operator * ( const rational& r1, const rational& r2 )
{
   return rational( r1. num * r2. num, r1. denom * r2. denom );
}

inline rational operator / ( const rational& r1, const rational& r2 )
{
   return rational( r1. num * r2. denom, r1. denom * r2. num );
}

inline bool operator == ( const rational& r1, const rational& r2 )
{
   return r1. num == r2. num && r1. denom == r2. denom; 
}

inline bool operator != ( const rational& r1, const rational& r2 )
{
   return r1. num != r2. num || r1. denom != r2. denom; 
      // Possible because of normalization. 
}

inline bool operator < ( const rational& r1, const rational& r2 )
{
   return r1. num * r2. denom < r1. denom * r2. num;
}

inline bool operator > ( const rational& r1, const rational& r2 )
{
   return r1. num * r2. denom > r1. denom * r2. num;
}

inline bool operator <= ( const rational& r1, const rational& r2 )
{
   return r1. num * r2. denom <= r1. denom * r2. num;
}

inline bool operator >= ( const rational& r1, const rational& r2 )
{
   return r1. num * r2. denom >= r1. denom * r2. num;
}

std::ostream& operator << ( std::ostream& stream, const rational& r );


#endif


