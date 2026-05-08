
#include "rational.h"
#include "mathexpr.h"

#include "skeletons.h"
#include "raii.h"

#include "scalar_vector.h"
#include "scalar.h"
#include "memory.h"

#include "outermost.h"
#include "composition.h"

bool contains( selector sel, const auto& arN )
{
   for( size_t i = 0; i != arN. size( ); ++ i )
   {
      if( arN. sub(i). sel( ) == sel )
         return true; 
   } 
   return false;
}

bool equal( const mathexpr& expr, const rational& rat )
{
   return expr. sel( ) == sel_rat && 
          expr. view_ratconst( ). val( ) == rat; 
}


// Push_back all of from into to:

void push_back( const auto& from, const auto& to )
{
   for( size_t i = 0; i != from. size( ); ++ i )
      to. push_back( from. sub(i));
}


namespace efficient
{

   struct simplifier
   {
      uint64_t nruses;

      simplifier( )
         : nruses(0)
      { }

      mathexpr operator( ) ( mathexpr expr )
      {
         if( expr. sel( ) == sel_add )
         {
            auto arN = expr. view_arithN( );

            bool contains0 = false;
            for( size_t i = 0; i != arN. size( ); ++ i )
            {
               if( equal( arN. sub(i), rational(0)) )
                  contains0 = true;
            }

            if( contains0 )
            {
               auto res = mathexpr_add( {} );
               for( size_t i = 0; i != arN. size( ); ++ i )
               {
                  if( !equal( arN. sub(i), rational(0)) ) 
                     res. view_arithN( ). push_back( arN. sub(i));
               }
               ++ nruses; 
               return res;
            }
         }

         if( expr. sel( ) == sel_mul )
         {  
            auto arN = expr. view_arithN( );

            bool contains0 = false;
            bool contains1 = false;
            for( size_t i = 0; i != arN. size( ); ++ i )
            {
               if( equal( arN. sub(i), rational(1)))
                  contains1 = true;

               if( equal( arN. sub(i), rational(0)))
                  contains0 = true; 
            }

            if( contains0 )
            {
               ++ nruses;
               return mathexpr_rat(0);
            }

            if( contains1 )
            {
               auto res = mathexpr_mul( {} );  
               for( size_t i = 0; i != arN. size( ); ++ i )
               {
                  if( !equal( arN. sub(i), rational(1)))
                     res. view_arithN( ). push_back( arN. sub(i));
               }
               ++ nruses; 
               return res;
            }
         }
       
         if( expr. sel( ) == sel_neg )
         {
            auto ar1 = expr. view_arith1( );
            if( ar1. sub( ). sel( ) == sel_neg )
            {
               auto ar1a = ar1. sub( ). view_arith1( );
               return ar1a. sub( );
            }
         }
 
         return expr;
      }   

   };


   struct flattener
   {
      uint64_t nruses;

      flattener( ) 
         : nruses(0)
      { }
 
      mathexpr operator( ) ( mathexpr expr ) 
      {
         if( expr. sel( ) == sel_add && expr. view_arithN( ). size( ) == 0 )
         {
            ++ nruses; 
            return mathexpr_rat(0);
         }

         if( expr. sel( ) == sel_mul && expr. view_arithN( ). size( ) == 0 )
         {
            ++ nruses; 
            return mathexpr_rat(1);
         }

         if( expr. sel( ) == sel_add || expr. sel( ) == sel_mul )
         {
            auto arN = expr. view_arithN( );

            if( arN. size( ) == 1 )
               return arN. sub(0);   // Trivial case +(N) or *(N).

            if( contains( expr. sel( ), arN )) 
            {
               auto res = mathexpr( expr. sel( ), { } );
               for( size_t i = 0; i != arN. size( ); ++ i )
               {
                  if( arN. sub(i). sel( ) == expr. sel( ))
                  {
                     push_back( arN. sub(i). view_arithN( ), 
                                res. view_arithN( ));
                  }
                  else
                     res. view_arithN( ). push_back( arN. sub(i)); 
               }
               ++ nruses; 
               return res; 
            }
         }
         return expr; 
      }

   };

}


namespace inefficient
{
   struct simplifier 
   {
      efficient::simplifier simpl;
    
      simplifier( ) = default;

      mathexpr operator( ) ( mathexpr expr ) 
      {
         return simpl( expr );
      }
   };

}


mathexpr deriv( const mathexpr& expr, const std::string& var )
{
   switch( expr. sel( ))
   {
   case sel_rat:
      return mathexpr_rat( rational(0) );

   case sel_var: 
      {
         if( expr. view_variable( ). name( ) == var )
            return mathexpr_rat(1);
         else
            return mathexpr_rat(0);
      }

   case sel_sub:
      {
         auto ar2 = expr. view_arith2( );
         return mathexpr_sub( deriv( ar2. sub1( ), var ),
                              deriv( ar2. sub2( ), var ));
      }
 
   case sel_add:
      {
         auto arN = expr. view_arithN( );
         auto res = mathexpr_add( { } );
         for( size_t i = 0; i != arN. size( ); ++ i )
            res. view_arithN( ). push_back( deriv( arN. sub(i), var ));

         return res;
      }

   case sel_mul:
      {
         auto arN = expr. view_arithN( );  
         auto sum = mathexpr_add( {} );

         for( size_t i = 0; i != arN. size( ); ++ i )
         {
            auto prod = mathexpr_mul( {} ); 
            for( size_t i1 = 0; i1 != arN. size( ); ++ i1 )
            {
               if( i != i1 )
                  prod. view_arithN( ). push_back( arN. sub( i1 ));
               else
                  prod. view_arithN( ). push_back( 
                        deriv( arN. sub(i), var ));
            }
            sum. view_arithN( ). push_back( prod ); 
         } 

         return sum;
      }

   case sel_truediv:
      {
         auto ar2 = expr. view_arith2( );
         auto& sub1 = ar2. sub1( );
         auto& sub2 = ar2. sub2( );
         auto deriv1 = deriv( sub1, var );
         auto deriv2 = deriv( sub2, var );

         return mathexpr_truediv( 
            mathexpr_sub( 
               mathexpr_mul( { deriv1, sub2 } ),
               mathexpr_mul( { sub1, deriv2 } )),
            mathexpr_mul( { sub2, sub2 } ));
      }

   case sel_pow:
      {
         auto spec2 = expr. view_specfunc2( );

         if( spec2. sub1( ). sel( ) == sel_var &&
             spec2. sub2( ). sel( ) == sel_rat )
         {
            const auto& exp = spec2. sub2( ). view_ratconst( ). val( );
            if( exp == 0 )
               return mathexpr_rat( rational(0) );
            return mathexpr_mul( { mathexpr_rat( exp ), 
                      mathexpr_pow( spec2. sub1( ), 
                         mathexpr_rat( exp - 1 )) } );
         }
         break;
      }

   case sel_sin:
      return mathexpr_mul( { 
         mathexpr_cos( expr. view_specfunc1( ). sub( )),
                   deriv( expr. view_specfunc1( ). sub( ), var ) } );

   case sel_cos:
      return mathexpr_mul( { 
         mathexpr_neg(
            mathexpr_sin( expr. view_specfunc1( ). sub( ))),
                   deriv( expr. view_specfunc1( ). sub( ), var ) } );
   }

   std::cout << expr. sel( ) << "\n";
   throw std::logic_error( "deriv: unhandled case" );   
}


template< typename S, typename V >
void print( const shared::scalar_vector<S,V> & vect )
{
   std::cout << "shared::vector ";
   vect. printstate( );
   std::cout << "\n";
   std::cout << "scalar: " << vect. get( ) << "\n";
   for( const auto& v : vect )
      std::cout << "   " << v << "\n";
   std::cout << "\n";
}

int main( int argc, char *argv[] )
{
   bigint num = 10;
   bigint denom = 21;

   auto expr = mathexpr_mul( 
               { mathexpr_sin( mathexpr_var( "xxx" )), 
                 mathexpr_cos( mathexpr_var( "xxx" )), 
                 mathexpr_pow( mathexpr_var( "xxx" ), mathexpr_rat(2)) } );

   expr = mathexpr( sel_truediv, 
             mathexpr_sin( mathexpr_var( "x" )),
             mathexpr_cos( mathexpr_var( "x" )) );

   expr = deriv( expr, "x" );
   // std::cout << "derivative: " << expr << "\n\n";

   auto x = mathexpr_var( "x" ); 
   auto e = mathexpr_add( 
      { mathexpr_sin( mathexpr_neg( mathexpr_neg( x ))),
        mathexpr_cos( mathexpr_neg( mathexpr_neg( x ))) } );
   std::cout << e << "\n";
   efficient::simplifier simpl;
   e = outermost( simpl, std::move(e) );
   std::cout << "after simplification: " << e << "\n";

   efficient::composition< efficient::flattener, efficient::simplifier > red;


   return 0; 
}

