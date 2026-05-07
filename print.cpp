
#include "mathexpr.h"

std::ostream& operator << ( std::ostream& out, const mathexpr& expr )
{
   if constexpr ( true )
   {
      out << "{"; 
      expr. printstate( std::cout ); 
      out << "}";
   }

   if( expr. option_is_ratconst( ))
   {
      out << expr. view_ratconst( ). val( );
      return out;
   }

   if( expr. option_is_variable( )) 
   {
      out << expr. view_variable( ). name( );
      return out;
   }

   if( expr. option_is_arith1( ))
   {
      auto ar1 = expr. view_arith1( );
      out << expr. sel( ) << "( " << ar1. sub( ) << " )";
      return out;
   }

   if( expr. option_is_arith2( ))
   {
      auto ar2 = expr. view_arith2( );
      out << expr. sel( ) << "( ";
      out << ar2. sub1( ) << ", " << ar2. sub2( ); 
      out << " )";
      return out;
   }

   if( expr. option_is_arithN( ))
   {
      auto arN = expr. view_arithN( ); 
      out << expr. sel( ) << "(";
      for( size_t i = 0; i != arN. size( ); ++ i )
      {
         if(i) 
            out << ", ";
         else
            out << " ";
         out << arN. sub(i);
      }
      out << " )";
      return out; 
   }

   if( expr. option_is_specfunc1( ))
   {
      auto spec1 = expr. view_specfunc1( ); 
      out << expr. sel( ) << "( " << spec1. sub( ) << " )";
      return out;
   }

   if( expr. option_is_specfunc2( ))
   {
      auto spec2 = expr. view_specfunc2( );
      out << expr. sel( ) << "( ";
      out << spec2. sub1( ) << ", " << spec2. sub2( );
      out << " )";
      return out;
   }

}

