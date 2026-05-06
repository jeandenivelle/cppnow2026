
// This approach seemed nice, but I think that it cannot be extended
// into something that really works.
// I tried to create classes scalar and scalar_vector with
// proper value semantics, that can be put in a variant.
// I think, however, that it cannot be made to work with
// moving. I believe that the management of reference counters
// must be done by the main tree class, so that scalar and scalar_vector
// will not have value semantics. This also implies that
// one must use union, not variant. 

#ifndef RAI_
#define RAI_

#include <variant>

#include "selector.h"
   // Use the automatically generated enumeration type.

#include "scalar.h"
#include "scalar_vector.h"
#include "rational.h"
#include "tvm/includes.h"

namespace raii
{
   struct mathexpr;

   struct ratconst { rational val; };
   struct mathconst { };
   struct variable { std::string name; };
   struct arith1 { shared::scalar< mathexpr > arith1heap; };
   struct arith2 
      { shared::scalar< std::pair< mathexpr, mathexpr >> arith2heap; };
   struct arithN { shared::scalar_vector< tvm::unit, mathexpr > arithNheap; };
   struct specfunc1 
   { 
      shared::scalar< mathexpr > heap; 

      const mathexpr& get_sub( ) const; 
      void update_sub( const mathexpr& val );
      mathexpr extr_sub( );
   };
   struct specfunc2 { shared::scalar< std::pair< mathexpr, mathexpr >> specfunc2heap; };

   using options = std::variant< ratconst, mathconst, variable, 
                                 arith1, arith2, arithN,
                                 specfunc1, specfunc2 > ;

   struct mathexpr
   {
      selector sel;
      options opt; 

      void wrong( selector sel )
      {
         throw std::logic_error( "wrong selector for constructor" );
      }
 
      mathexpr( selector sel, const rational& val )
         : sel( sel )
      {
         switch( sel )
         {
         case sel_rat:
            opt = ratconst( val ); return;
         default:
            wrong( sel );
         }   
      } 

      mathexpr( selector sel )
         : sel( sel )
      {
         switch( sel )
         {
         case sel_pi:
         case sel_e:
            opt = mathconst( ); return;
         default:
            wrong( sel );
         }
      }
 
      mathexpr( selector sel, const std::string& var )
         : sel( sel ) 
      {
         switch( sel )
         {
         case sel_rat:
            opt = variable( var ); return;
         default:
            wrong( sel );
         } 
      }

      mathexpr( selector sel, const mathexpr& sub )
         : sel( sel )
      {
         switch( sel )
         {
         case sel_neg:
         case sel_recipr:
            opt = arith1( sub ); return;
         case sel_sin:
         case sel_cos:
         case sel_exp:
         case sel_ln:
         case sel_sqrt:
            opt = specfunc1( sub ); return; 
         default:
            wrong( sel );
         }
      }
        
      mathexpr( selector sel, const mathexpr& sub1, const mathexpr& sub2 ) 
         : sel( sel )
      {
         switch( sel )
         {
         case sel_sub:
         case sel_truediv:
            opt = arith2( std::pair( sub1, sub2 )); return;
         case sel_pow:
            opt = specfunc2( std::pair( sub1, sub2 )); return;
         default:
            wrong( sel );
         }
      }

      template< std::random_access_iterator It >
      requires requires( It i ) {{ *i } -> std::convertible_to< mathexpr > ; }
      mathexpr( selector sel, It begin, It end )
         : sel( sel )
      {
         switch( sel )
         {
         case sel_add:
         case sel_mul:
            opt = arithN( shared::scalar_vector< tvm::unit, mathexpr > 
                          ( tvm::unit( ), begin, end ));
            return;
         default:
            wrong( sel );
         }
      }

      mathexpr( selector sel, std::initializer_list< mathexpr > sub ) 
         : mathexpr( sel, sub. begin( ), sub. end( ))
      { }
     
      bool very_equal_to( const mathexpr& ) const;
 
   };
}

#endif

