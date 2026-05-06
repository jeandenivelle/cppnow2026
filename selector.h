
// This code was automatically built by TreeGen
// Written by Hans de Nivelle, see www.compiler-tools.eu

#ifndef SELECTOR_
#define SELECTOR_

#include <iostream>


enum selector
{   
   sel_ln, sel_exp, sel_rat, sel_pow, 
   sel_sin, sel_recipr, sel_neg, sel_truediv, 
   sel_mul, sel_sub, sel_add, sel_e, 
   sel_cos, sel_var, sel_pi, sel_sqrt
};

const char* getcstring( selector );

inline std::ostream& operator << ( std::ostream& out, selector sel )
   { out << getcstring( sel ); return out; }


#endif


