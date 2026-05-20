
#include "mathexpr.h"

const mathexpr::opt_ratconst* mathexpr::try_ratconst( ) const 
{
   switch( _ssss )
   {
   case sel_rat:
      return &repr. fld_ratconst; 
   default:
      return nullptr;
   }
}

mathexpr::opt_ratconst* mathexpr::try_ratconst( ) 
{ 
   switch( _ssss )
   {
   case sel_rat:
      return &repr. fld_ratconst; 
   default:
      return nullptr;
   }
}

const mathexpr::opt_mathconst* mathexpr::try_mathconst( ) const
{
   switch( _ssss )
   {
   case sel_pi:
   case sel_e:
      return &repr. fld_mathconst;
   default:
      return nullptr;
   }
}

mathexpr::opt_mathconst* mathexpr::try_mathconst( )
{
   switch( _ssss )
   {
   case sel_pi:
   case sel_e:
      return &repr. fld_mathconst;
   default:
      return nullptr;
   }
}

const mathexpr::opt_variable* mathexpr::try_variable( ) const
{
   switch( _ssss )
   {
   case sel_var:
      return &repr. fld_variable;
   default:
      return nullptr;
   }
}

mathexpr::opt_variable* mathexpr::try_variable( )
{
   switch( _ssss )
   {
   case sel_var:
      return &repr. fld_variable;
   default:
      return nullptr;
   }
}

const mathexpr::opt_arith1* mathexpr::try_arith1( ) const
{
   switch( _ssss )
   {
   case sel_neg:
   case sel_recipr:
      return &repr. fld_arith1;
   default:  
      return nullptr;
   }
}  

mathexpr::opt_arith1* mathexpr::try_arith1( )
{
   switch( _ssss )
   {
   case sel_neg:
   case sel_recipr:
      return &repr. fld_arith1;
   default:
      return nullptr;
   }
}  

const mathexpr::opt_arith2* mathexpr::try_arith2( ) const
{
   switch( _ssss )
   {
   case sel_sub:
   case sel_truediv:
      return &repr. fld_arith2;
   default: 
      return nullptr;
   }
}

mathexpr::opt_arith2* mathexpr::try_arith2( ) 
{
   switch( _ssss )
   {
   case sel_sub:
   case sel_truediv:
      return &repr. fld_arith2;
   default:
      return nullptr;
   }
}


const mathexpr::opt_arithN* mathexpr::try_arithN( ) const
{
   switch( _ssss )
   {
   case sel_add:
   case sel_mul:
      return &repr. fld_arithN;
   default:
      return nullptr;
   }
}

mathexpr::opt_arithN* mathexpr::try_arithN( ) 
{
   switch( _ssss )
   {
   case sel_add:
   case sel_mul:
      return &repr. fld_arithN;
   default:
      return nullptr;
   }
}

const mathexpr::opt_specfunc1* mathexpr::try_specfunc1( ) const
{
   switch( _ssss )
   {
   case sel_sin:
   case sel_cos:
   case sel_exp:
   case sel_ln:
   case sel_sqrt:
      return &repr. fld_specfunc1;
   default:
      return nullptr;
   }
}

mathexpr::opt_specfunc1* mathexpr::try_specfunc1( )
{
   switch( _ssss )
   {
   case sel_sin:
   case sel_cos:
   case sel_exp:
   case sel_ln:
   case sel_sqrt:
      return &repr. fld_specfunc1;
   default:
      return nullptr;
   }
}

const mathexpr::opt_specfunc2* mathexpr::try_specfunc2( ) const
{
   switch( _ssss )
   {
   case sel_pow:
      return &repr. fld_specfunc2;
   default:
      return nullptr;
   }
}

mathexpr::opt_specfunc2* mathexpr::try_specfunc2( )
{
   switch( _ssss )
   {
   case sel_pow:
      return &repr. fld_specfunc2;
   default:
      return nullptr;
   }
}

mathexpr::mathexpr( const mathexpr& from )
   : _ssss( from. _ssss )
{
   // std::cout << "mathexpr( const mathexpr& " << from. _ssss << " )";

   switch( from. _ssss )
   {
   case sel_rat:
      tvm::init( repr. fld_ratconst, from. repr. fld_ratconst );
      return;
   case sel_e:
   case sel_pi:
      tvm::init( repr. fld_mathconst, from. repr. fld_mathconst );
      return;
   case sel_var:
      tvm::init( repr. fld_variable, from. repr. fld_variable );
      return;
   case sel_recipr:
   case sel_neg:
      tvm::init( repr. fld_arith1, from. repr. fld_arith1 );
      repr. fld_arith1. onheap. connect( );
      return;
   case sel_truediv:
   case sel_sub:
      tvm::init( repr. fld_arith2, from. repr. fld_arith2 );
      repr. fld_arith2. onheap. connect( );
      return;
   case sel_mul:
   case sel_add:
      tvm::init( repr. fld_arithN, from. repr. fld_arithN ); 
      repr. fld_arithN. onheap. connect( );
      return;
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
      tvm::init( repr. fld_specfunc1, from. repr. fld_specfunc1 );
      repr. fld_specfunc1. onheap. connect( );
      return;
   case sel_pow:
      tvm::init( repr. fld_specfunc2, from. repr. fld_specfunc2 );
      repr. fld_specfunc2. onheap. connect( );
      return;
   }
}

mathexpr::mathexpr( mathexpr&& from ) noexcept
   : _ssss( from. _ssss )
{
   switch( from. _ssss )
   {
   case sel_rat:
      tvm::init( repr. fld_ratconst, std::move( from. repr. fld_ratconst ));
      tvm::destroy( from. repr. fld_ratconst ); 
      break;
   case sel_e:
   case sel_pi:
      tvm::init( repr. fld_mathconst, std::move( from. repr. fld_mathconst ));
      tvm::destroy( from. repr. fld_mathconst );
      break;
   case sel_var:
      tvm::init( repr. fld_variable, std::move( from. repr. fld_variable ));
      tvm::destroy( from. repr. fld_variable );
      break;
   case sel_recipr:
   case sel_neg:
      tvm::init( repr. fld_arith1, std::move( from. repr. fld_arith1 ));
      tvm::destroy( from. repr. fld_arith1 );
      break;
   case sel_truediv:
   case sel_sub:
      tvm::init( repr. fld_arith2, std::move( from. repr. fld_arith2 ));
      tvm::destroy( from. repr. fld_arith2 );
      break;
   case sel_mul:
   case sel_add:
      tvm::init( repr. fld_arithN, std::move( from. repr. fld_arithN ));
      tvm::destroy( from. repr. fld_arithN );
      break;
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
      tvm::init( repr. fld_specfunc1, std::move( from. repr. fld_specfunc1 ));
      tvm::destroy( from. repr. fld_specfunc1 );
      break;
   case sel_pow:
      tvm::init( repr. fld_specfunc2, std::move( from. repr. fld_specfunc2 ));
      tvm::destroy( from. repr. fld_specfunc2 );
      break;
   }

   // Put from in the trivial state:

   from. _ssss = sel_pi;
   new (&from. repr) options( );
}

mathexpr& mathexpr::operator = ( const mathexpr& from )
{ 
   switch( from. _ssss )
   {
   case sel_recipr:
   case sel_neg:
      from. repr. fld_arith1. onheap. connect( );
      break;
   case sel_sub: 
   case sel_truediv:
      from. repr. fld_arith2. onheap. connect( );
      break;
   case sel_mul:
   case sel_add:
      from. repr. fld_arithN. onheap. connect( );
      break;
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
      from. repr. fld_specfunc1. onheap. connect( );
      break; 
   case sel_pow:
      from. repr. fld_specfunc2. onheap. connect( );
      break;
   }

   if( _ssss == from. _ssss )
   {
      switch( _ssss )
      {
      case sel_rat:
         tvm::assign( repr. fld_ratconst, from. repr. fld_ratconst );
         break;
      case sel_pi:
      case sel_e:
         tvm::assign( repr. fld_mathconst, from. repr. fld_mathconst );
         break;
      case sel_var:
         tvm::assign( repr. fld_variable, from. repr. fld_variable );
         break;
      case sel_recipr:
      case sel_neg:
         repr. fld_arith1. onheap. disconnect( );
         tvm::assign( repr. fld_arith1, from. repr. fld_arith1 );
         break;
      case sel_truediv:
      case sel_sub:
         repr. fld_arith2. onheap. disconnect( );
         tvm::assign( repr. fld_arith2, from. repr. fld_arith2 );
         break;
      case sel_mul:
      case sel_add:
         repr. fld_arithN. onheap. disconnect( );
         tvm::assign( repr. fld_arithN, from. repr. fld_arithN );
         break;
      case sel_sin:
      case sel_ln:
      case sel_cos:
      case sel_exp:
      case sel_sqrt:
         repr. fld_specfunc1. onheap. disconnect( );
         tvm::assign( repr. fld_specfunc1, from. repr. fld_specfunc1 );
         break;
      case sel_pow:
         repr. fld_specfunc2. onheap. disconnect( );
         tvm::assign( repr. fld_specfunc2, from. repr. fld_specfunc2 );
         break;
      }
      return *this; 
   }
   else
   {
      this -> ~mathexpr( );

      _ssss = from. _ssss;

      switch( _ssss )
      {
      case sel_rat:
         tvm::init( repr. fld_ratconst, from. repr. fld_ratconst );
         break;
      case sel_pi:
      case sel_e:
         tvm::init( repr. fld_mathconst, from. repr. fld_mathconst );
         break;
      case sel_var:
         tvm::init( repr. fld_variable, from. repr. fld_variable );
         break;
      case sel_recipr:
      case sel_neg:
         tvm::init( repr. fld_arith1, from. repr. fld_arith1 );
         break;
      case sel_truediv:
      case sel_sub:
         tvm::init( repr. fld_arith2, from. repr. fld_arith2 );
         break;
      case sel_mul:
      case sel_add:
         tvm::init( repr. fld_arithN, from. repr. fld_arithN );
         break;
      case sel_sin:
      case sel_ln:
      case sel_cos:
      case sel_exp:
      case sel_sqrt:
         tvm::init( repr. fld_specfunc1, from. repr. fld_specfunc1 );
         break;
      case sel_pow:
         tvm::init( repr. fld_specfunc2, from. repr. fld_specfunc2 ); 
         break;
      }

      return *this;
   }

}

mathexpr& mathexpr::operator = ( mathexpr&& from ) noexcept
{
   // This check should be based on same state: 

   if( _ssss == from. _ssss )
   {
      // This is safe in a subtle fashion, because there never
      // can be a non-const reference access to a subterm.

      switch( from. _ssss )
      {
      case sel_rat:
         repr. fld_ratconst = std::move( from. repr. fld_ratconst );  
         tvm::destroy( from. repr. fld_ratconst );
         break; 
      case sel_e:
      case sel_pi:
         repr. fld_mathconst = std::move( from. repr. fld_mathconst );
         tvm::destroy( from. repr. fld_mathconst );
         break;
      case sel_var:
         repr. fld_variable = std::move( from. repr. fld_variable );
         tvm::destroy( from. repr. fld_variable );
         break;
      case sel_neg:
      case sel_recipr:
         repr. fld_arith1. onheap. disconnect( );
         repr. fld_arith1 = std::move( from. repr. fld_arith1 );
         tvm::destroy( from. repr. fld_arith1 );
         break;
      case sel_truediv:
      case sel_sub:
         repr. fld_arith2. onheap. disconnect( );
         repr. fld_arith2 = std::move( from. repr. fld_arith2 );
         tvm::destroy( from. repr. fld_arith2 );
         break;
      case sel_mul:
      case sel_add:
         repr. fld_arithN. onheap. disconnect( );
         repr. fld_arithN = std::move( from. repr. fld_arithN );
         tvm::destroy( from. repr. fld_arithN );
         break;
      case sel_sin:
      case sel_ln:
      case sel_cos:
      case sel_exp:
      case sel_sqrt:
         repr. fld_specfunc1. onheap. disconnect( );
         repr. fld_specfunc1 = std::move( from. repr. fld_specfunc1 );
         tvm::destroy( from. repr. fld_specfunc1 );
         break; 
      case sel_pow:
         repr. fld_specfunc2. onheap. disconnect( );
         repr. fld_specfunc2 = std::move( from. repr. fld_specfunc2 );
         tvm::destroy( from. repr. fld_specfunc2 );
         break;
      }

      // We need to leave from in trivial state:

      from. _ssss = sel_pi;
      new (&from. repr) options( );
      return *this;
   }
   else
   {
      this -> ~mathexpr( );
      new (this) mathexpr( std::move( from )); 
      return *this;
   }
}


mathexpr::~mathexpr( ) noexcept
{
   // If there is something on the heap, we need to 
   // disconnect by yourselves. 
   // Rest is done by the destructor of the option.

   switch( _ssss )
   {
   case sel_rat:
      tvm::destroy( repr. fld_ratconst );
      break; 
   case sel_e:
   case sel_pi:
      tvm::destroy( repr. fld_mathconst );
      break;
   case sel_var:
      tvm::destroy( repr. fld_variable );
      break;
   case sel_neg:
   case sel_recipr:
      repr. fld_arith1. onheap. disconnect( );
      tvm::destroy( repr. fld_arith1 );
      break; 
   case sel_truediv:
   case sel_sub: 
      repr. fld_arith2. onheap. disconnect( ); 
      tvm::destroy( repr. fld_arith2 ); 
      break; 
   case sel_mul:
   case sel_add:
      repr. fld_arithN. onheap. disconnect( );
      tvm::destroy( repr. fld_arithN );
      break;
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
      repr. fld_specfunc1. onheap. disconnect( );
      tvm::destroy( repr. fld_specfunc1 );
      break; 
   case sel_pow:
      repr. fld_specfunc2. onheap. disconnect( );
      tvm::destroy( repr. fld_specfunc2 ); 
      break;
   }
}


bool mathexpr::strongly_equal_to( const mathexpr & other ) const
{
   if( _ssss != other. _ssss )
      return false;

   std::cout << "strongly equal\n";
   throw std::logic_error( "strong" );
}

void mathexpr::printstate( std::ostream& out ) const
{
   switch( _ssss )
   {
   case sel_neg:
   case sel_recipr:
      repr. fld_arith1. onheap. printstate( out );
      return;
   case sel_sub:
   case sel_truediv:
      repr. fld_arith2. onheap. printstate( out );
      return;
   case sel_mul:
   case sel_add:
      repr. fld_arithN. onheap. printstate( out );
      return;
   case sel_sin:
   case sel_ln:
   case sel_cos:
   case sel_exp:
   case sel_sqrt:
      repr. fld_specfunc1. onheap. printstate( out );
      return;
   case sel_pow:
      repr. fld_specfunc2. onheap. printstate( out );
      return;
   }
}


