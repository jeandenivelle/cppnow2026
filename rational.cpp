
#include <utility>
#include "rational.h"

bigint rational::gcd( bigint num, bigint denom )
{
   while( denom != 0 )
      num = std::exchange( denom, num % denom );
   return num;
}

void rational::normalize( )
{
   bigint g = gcd( num, denom );
   num /= g;
   denom /= g;

   if( denom < 0 )
   {
      num = - num;
      denom = - denom;
   }

   if( num == 0 )
      denom = 1;
}

rational& rational::operator *= ( const rational& r )
{
   num *= r. num; 
   denom *= r. denom;
   normalize( );
   return *this;
}

rational& rational::operator /= ( const rational& r ) 
{
   num *= r. denom;
   denom *= r. num;
   normalize( );
   return *this;
}

std::ostream& operator << ( std::ostream& out, const rational& r )
{ 
   if( r. denom != 1 )
      out << r. num << '/' << r. denom;
   else
      out << r. num; 

   return out;
}


