
#ifndef SKELETONS_
#define SKELETONS_

#include "selector.h"
   // Use the automatically generated enumeration type.

namespace late_pntr 
{
   struct mathexpr;

   struct ratconst { rational val; };
   struct mathconst { };
   struct variable { std::string name; };
   struct arith1 { mathexpr* sub; };
   struct arith2 { mathexpr* sub1; mathexpr* sub2; };
   struct arithN { mathexpr* sub; size_t cap; size_t sz; };
   struct specfunc1 { mathexpr* sub; };
   struct specfunc2 { mathexpr* sub1; mathexpr* sub2; };

   union options
   {
      ratconst rat; 
      mathconst math;  
      variable var;
      arith1 ar1;
      arith2 ar2;
      arithN arN;
      specfunc1 spec1;
      specfunc2 spec2; 

      options( const variable& var )
         : var( var )
      { }

      options( ) : math( ) { }  
      ~options( ) { }             
   };

   struct mathexpr
   {
      selector sel;
      options opt; 

      mathexpr( selector sel, const std::string& var )
         : sel( sel ), opt( variable( var )) { }
   };
}


namespace early_pntr
{
   struct ratconst;
   struct mathconst;
   struct variable;  
   struct arith1;
   struct arith2;
   struct arithN;
   struct specfunc1; 
   struct specfunc2;

   union options
   {
      ratconst* rat;  
      mathconst* math; 
      variable* var;
      arith1* ar1;
      arith2* ar2;
      arithN* arN; 
      specfunc1* spec1;
      specfunc2* spec2;

      options( ) : math( nullptr ) { } 
      ~options( ) { } 
   };

   struct mathexpr 
   {
      selector sel;
      options opt; 
   };

   struct ratconst { rational val; };
   struct mathconst { };
   struct variable { std::string name; };

   struct arith1 { mathexpr sub; };
   struct arith2 { mathexpr sub1; mathexpr sub2; };
   struct arithN { size_t cap; size_t sz; mathexpr sub[1000]; };
      // Problematic, needs C99 flexible arrays.
   struct specfunc1 { mathexpr sub; };
   struct specfunc2 { mathexpr sub1; mathexpr sub2; };
} 



namespace derived
{
   struct option { virtual ~option( ) { } };
   struct mathexpr
   {
      selector sel;
      option* opt;
   };

   struct ratconst : public option { rational val; };
   struct mathconst : public option { };
   struct arith1 : public option { mathexpr sub; }; 
   struct arith2 : public option { mathexpr sub1; mathexpr sub2; };
   struct arithN : public option 
      { size_t cap; size_t sz; mathexpr sub[10]; }; 
   struct specfunc1 : public option { mathexpr sub; };
   struct specfunc2 : public option { mathexpr sub1; mathexpr sub2; };
}

#endif

