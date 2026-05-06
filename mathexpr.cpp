
// This code was automatically built by TreeGen
// Written by Hans de Nivelle, see www.compiler-tools.eu

#include "mathexpr.h"

mathexpr::mathexpr( const mathexpr& from )
   : _ssss( from. _ssss )
{
   // std::cout << "mathexpr( const mathexpr& " << from. _ssss << " )";
   
   switch( from. _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      repr. _fld00. heap = takeshare( from. repr. _fld00. heap );
      break;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      repr. _fld01. heap = takeshare( from. repr. _fld01. heap );
      break;
   case sel_mul:
   case sel_add:
      repr. _fld02. heap = takeshare( from. repr. _fld02. heap );
      break;
   case sel_e:
   case sel_pi:
      break;
   case sel_rat:
      tvm::init( repr. _fld04. loc, from. repr. _fld04. loc );
      break;
   case sel_var:
      tvm::init( repr. _fld05. loc, from. repr. _fld05. loc );
      break;
   }
}

mathexpr::mathexpr( mathexpr&& from ) noexcept
   : _ssss( from. _ssss )
{
   // std::cout << "mathexpr( mathexpr&& " << from. _ssss << " )";
   
   switch( from. _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      repr. _fld00. heap = from. repr. _fld00. heap;
      break;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      repr. _fld01. heap = from. repr. _fld01. heap;
      break;
   case sel_mul:
   case sel_add:
      repr. _fld02. heap = from. repr. _fld02. heap;
      break;
   case sel_e:
   case sel_pi:
      break;
   case sel_rat:
      tvm::init( repr. _fld04. loc, std::move( from. repr. _fld04. loc ) );
      tvm::destroy( from. repr. _fld04. loc );
      break;
   case sel_var:
      tvm::init( repr. _fld05. loc, std::move( from. repr. _fld05. loc ) );
      tvm::destroy( from. repr. _fld05. loc );
      break;
   }

   // Leave from in trivial state:
   
   from. _ssss = sel_pi;
   new (&from. repr) options( );
}

// Note that the implementation of assignment is minimalistic.
// One should create special cases for when *this and from are 
// in the same state.

const mathexpr & mathexpr::operator = ( const mathexpr& from )
{
   if( this == &from )
      return *this;
   
   switch( from. _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      takeshare( from. repr. _fld00. heap );
      break;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      takeshare( from. repr. _fld01. heap );
      break;
   case sel_mul:
   case sel_add:
      takeshare( from. repr. _fld02. heap );
      break;
   }

   this -> ~mathexpr( );
   
   _ssss = from. _ssss;
   
   switch( _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      repr. _fld00. heap = from. repr. _fld00. heap;
      break;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      repr. _fld01. heap = from. repr. _fld01. heap;
      break;
   case sel_mul:
   case sel_add:
      repr. _fld02. heap = from. repr. _fld02. heap;
      break;
   case sel_e:
   case sel_pi:
      break;
   case sel_rat:
      tvm::init( repr. _fld04. loc, from. repr. _fld04. loc );
      break;
   case sel_var:
      tvm::init( repr. _fld05. loc, from. repr. _fld05. loc );
      break;
   }

   return *this;
}

// We don't check self assignment in the moving case, because it is UB:

const mathexpr & mathexpr::operator = ( mathexpr&& from ) noexcept
{
   if( _ssss == from. _ssss )
   {
      switch( _ssss )
      {
      case sel_sin:
      case sel_ln:
      case sel_cos:
      case sel_exp:
      case sel_sqrt:
      case sel_recipr:
      case sel_neg:
         dropshare( repr. _fld00. heap );
         repr. _fld00. heap = from. repr. _fld00. heap;
         break;
      case sel_pow:
      case sel_truediv:
      case sel_sub:
         dropshare( repr. _fld01. heap );
         repr. _fld01. heap = from. repr. _fld01. heap;
         break;
      case sel_mul:
      case sel_add:
         dropshare( repr. _fld02. heap );
         repr. _fld02. heap = from. repr. _fld02. heap;
         break;
      case sel_e:
      case sel_pi:
         break;
      case sel_rat:
         tvm::assign( repr. _fld04. loc, std::move( from. repr. _fld04. loc ) );
         tvm::destroy( from. repr. _fld04. loc );
         break;
      case sel_var:
         tvm::assign( repr. _fld05. loc, std::move( from. repr. _fld05. loc ) );
         tvm::destroy( from. repr. _fld05. loc );
         break;
      }

      // Leave from in trivial state:

      from. _ssss = sel_pi;
      new (&from. repr) options( );
      return *this;
   }
   else
   {
      // I believe that this wll be safe, because we have
      // the only reference to other: 

      this -> ~mathexpr( );

      new (this) mathexpr( std::move( from ));
      return *this;
   }
}

mathexpr::~mathexpr( ) noexcept
{
   // If there are prefix fields, they will be destroyed automatically

   switch( _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      dropshare( repr. _fld00. heap );
      break;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      dropshare( repr. _fld01. heap );
      break;
   case sel_mul:
   case sel_add:
      dropshare( repr. _fld02. heap );
      break;
   case sel_e:
   case sel_pi:
      break;
   case sel_rat:
      tvm::destroy( repr. _fld04. loc );
      break;
   case sel_var:
      tvm::destroy( repr. _fld05. loc );
      break;
   }
}

bool mathexpr::very_equal_to( const mathexpr & other ) const
{
   if( _ssss != other. _ssss )
      return false;

   switch( _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      if( repr. _fld00. heap != other. repr. _fld00. heap )
         return false;
      return true;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      if( repr. _fld01. heap != other. repr. _fld01. heap )
         return false;
      return true;
   case sel_mul:
   case sel_add:
      if( repr. _fld02. heap != other. repr. _fld02. heap )
         return false;
      return true;
   case sel_e:
   case sel_pi:
      return true;
   case sel_rat:
      if( tvm::distinct( repr. _fld04. loc, other. repr. _fld04. loc ))
         return false;
      return true;
   case sel_var:
      if( tvm::distinct( repr. _fld05. loc, other. repr. _fld05. loc ))
         return false;
      return true;
   }
}

void mathexpr::printstate( std::ostream& out ) const
{
   switch( _ssss )
   {
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
   case sel_recipr:
   case sel_neg:
      tvm::printstate( repr. _fld00. heap, out );
      break;
   case sel_pow:
   case sel_truediv:
   case sel_sub:
      tvm::printstate( repr. _fld01. heap, out );
      break;
   case sel_mul:
   case sel_add:
      tvm::printstate( repr. _fld02. heap, out );
      break;
   }
}

