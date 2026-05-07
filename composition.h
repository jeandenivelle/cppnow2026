
#ifndef EFFICIENT_COMPOSITION_
#define EFFICIENT_COMPOSITION_

// Composition of two rewrite_rules.
// First try the first, if that does not result
// in a reduction, try the second.

namespace efficient
{
   template< rewrite_rule R1, rewrite_rule R2 >
   struct composition
   {
      R1 r1;
      R2 r2;

      uint64_t nruses;

      composition( ) :
         nruses(0)
      { } 

      mathexpr operator( ) ( mathexpr expr )
      {
         auto nr = r1. nruses;
         expr = r1( std::move( expr ));
         if( nr != r1. nruses )
         {
            nruses = r1. nruses + r2. nruses;
            return expr;
         }

         nr = r2. nruses;
         expr = r2( std::move( expr ));
         if( nr != r2. nruses )
         {
            nruses = r1. nruses + r2. nruses;
            return expr;
         }

         return expr;
      } 
   };

}

#endif

