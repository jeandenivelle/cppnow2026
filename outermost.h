
#ifndef OUTERMOST_
#define OUTERMOST_

#include <iostream>
#include <optional>
#include <concepts>

#include "mathexpr.h"

namespace inefficient
{

   template< typename R >
   concept rewrite_rule =
      requires( R repl, mathexpr expr )
      { { repl( expr ) } -> std::same_as< mathexpr > ; };

   template< rewrite_rule R >
   mathexpr outermost( R&& repl, mathexpr expr )
   {
      {
         mathexpr expr2 = repl( expr );
         if( !expr2. very_equal_to( expr ))
            return expr2;
      }

      switch( expr. sel( ))   
      {
      case sel_rat:
      case sel_pi:
      case sel_e:
      case sel_var:
         return expr;

      case sel_neg:
      case sel_recipr:
         {
            auto ar1 = expr. view_arith1( );
            ar1. update_sub( outermost( repl, ar1. sub( )));
         }
         return expr; 

      case sel_sub:
      case sel_truediv:
         {
            auto ar2 = expr. view_arith2( );
            ar2. update_sub1( outermost( repl, ar2. sub1( )) );
            ar2. update_sub2( outermost( repl, ar2. sub2( )) );
         } 
         return expr;
      
      case sel_add:
      case sel_mul:
         {
            auto arN = expr. view_arithN( );
            for( size_t i = 0; i != arN. size( ); ++ i )
               arN. update_sub( i, outermost( repl, arN. sub(i)) );
         }
         return expr;

      case sel_sin:
      case sel_cos:
      case sel_exp:
      case sel_ln:
      case sel_sqrt:
         {
            auto spec1 = expr. view_specfunc1( );
            spec1. update_sub( outermost( repl, spec1. sub( )) );
         }
         return expr;

      case sel_pow:
         {
            auto spec2 = expr. view_specfunc2( );
            spec2. update_sub1( outermost( repl, spec2. sub1( )) );
            spec2. update_sub2( outermost( repl, spec2. sub2( )) );
         }
         return expr;
      }

      std::cout << "unhandled selector " << expr. sel( ) << "\n";
      throw std::logic_error( "reached the unreachable" ); 
   }
}

namespace efficient
{

   template< typename R >
   concept rewrite_rule =
      requires( R repl, mathexpr expr )
      { { repl. nruses } -> std::same_as< uint64_t& > ;
        { repl( expr ) } -> std::same_as< mathexpr > ; 
      };

   template< rewrite_rule R >
   mathexpr outermost( R&& repl, mathexpr expr )
   {

      {
         auto u = repl. nruses;
         expr = repl( std::move( expr ));
         if( u != repl. nruses )
            return expr;
      }

      switch( expr. sel( ))   
      {
      case sel_rat:
      case sel_pi:
      case sel_e:
      case sel_var:
         return expr;

      case sel_neg:
      case sel_recipr:
         {
            auto ar1 = expr. view_arith1( );
            ar1. update_sub( outermost( repl, ar1. extr_sub( )));
         }
         return expr; 

      case sel_sub:
      case sel_truediv:
         {
            auto ar2 = expr. view_arith2( );
            ar2. update_sub1( outermost( repl, ar2. extr_sub1( )) );
            ar2. update_sub2( outermost( repl, ar2. extr_sub2( )) );
         } 
         return expr;
      
      case sel_add:
      case sel_mul:
         {
            auto arN = expr. view_arithN( );
            for( size_t i = 0; i != arN. size( ); ++ i )
               arN. update_sub( i, outermost( repl, arN. extr_sub(i)) );
         }
         return expr;

      case sel_sin:
      case sel_cos:
      case sel_exp:
      case sel_ln:
      case sel_sqrt:
         {
            auto spec1 = expr. view_specfunc1( );
            spec1. update_sub( outermost( repl, spec1. extr_sub( )) );
         }
         return expr;

      case sel_pow:
         {
            auto spec2 = expr. view_specfunc2( );
            spec2. update_sub1( outermost( repl, spec2. extr_sub1( )) );
            spec2. update_sub2( outermost( repl, spec2. extr_sub2( )) );
         }
         return expr;
      }

      std::cout << "unhandled selector " << expr. sel( ) << "\n";
      throw std::logic_error( "reached the unreachable" ); 
   }


   template< rewrite_rule R > 
   mathexpr normalize( R&& repl, mathexpr expr )
   {
      auto nr = repl. nruses;
         // Needed because otherwise not visible in 'while'.

      do
      {
         nr = repl. nruses;
         expr = outermost( repl, std::move( expr ));
      }
      while( nr != repl. nruses );

      return expr;
   }

}

namespace optional
{

   template< typename R >
   concept rewrite_rule =
      requires( R repl, mathexpr expr )
      { { repl( expr ) } -> std::same_as< std::optional< mathexpr >> ; };


   template< rewrite_rule R > 
   std::optional< mathexpr > outermost( R&& repl, const mathexpr& expr )
   {
      {
         auto expr2 = repl( expr );
            // Must be in local scope, otherwise
            // expr and expr2 will be shared.

         if( expr2. has_value( ))
            return expr2;
      }

      // I didn't include all cases. It is clear that this is ugly.

      switch( expr. sel( ))
      {
      case sel_rat:
      case sel_pi:
      case sel_e:
      case sel_var:
         return { };

      case sel_neg:
      case sel_recipr:
         {
            std::optional< mathexpr > res; 
            auto ar1 = expr. view_arith1( );
            auto e = outermost( repl, ar1. sub( ));
            if( e. has_value( ))
            {
               if( !res. has_value( )) 
                  res = expr;
               res. value( ). view_arith1( ). update_sub( e. value( ));
            }
            return res;
         }

      case sel_sub:
      case sel_truediv:
         {
            std::optional< mathexpr > res;
            auto ar2 = expr. view_arith2( );
            {
               auto e = outermost( repl, ar2. sub1( ));
               if( e. has_value( ))
               {
                  if( !res. has_value( ))
                     res = expr;
                  res. value( ). view_arith2( ). update_sub1( e. value( ));
               }

               e = outermost( repl, ar2. sub2( )); 
               if( e. has_value( ))
               {
                  if( !res. has_value( ))
                     res = expr;
                  res. value( ). view_arith2( ). update_sub2( e. value( )); 
               } 
            }
            return res;
         }

      case sel_add:
      case sel_mul:
         {
            std::optional< mathexpr > res; 
            auto arN = expr. view_arithN( );
            for( size_t i = 0; i != arN. size( ); ++ i ) 
            {
               auto e = outermost( repl, arN. sub(i) ); 
               if( e. has_value( ))
               {
                  if( !res. has_value( ))
                     res = expr;
                  res. value( ). view_arithN( ). update_sub( i, e. value( )); 
               }
            }
            return res; 
         }

      }
   }
}


#endif


