
// This code was automatically built by TreeGen
// Written by Hans de Nivelle, see www.compiler-tools.eu

#include "selector.h"

const char* getcstring( selector sel )
{
   switch( sel )
   {
   case sel_ln :
      return "sel_ln";
   case sel_exp :
      return "sel_exp";
   case sel_rat :
      return "sel_rat";
   case sel_pow :
      return "sel_pow";
   case sel_sin :
      return "sel_sin";
   case sel_recipr :
      return "sel_recipr";
   case sel_neg :
      return "sel_neg";
   case sel_truediv :
      return "sel_truediv";
   case sel_mul :
      return "sel_mul";
   case sel_sub :
      return "sel_sub";
   case sel_add :
      return "sel_add";
   case sel_e :
      return "sel_e";
   case sel_cos :
      return "sel_cos";
   case sel_var :
      return "sel_var";
   case sel_pi :
      return "sel_pi";
   case sel_sqrt :
      return "sel_sqrt";
   }
   return "???";
}

