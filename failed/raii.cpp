
#include "raii.h"

const raii::mathexpr&
raii::specfunc1::get_sub( ) const { return heap. get( ); }

void raii::specfunc1::update_sub( const mathexpr& val )
{
   if( tvm::distinct( heap. get( ), val ))
      heap. set( val );
}

raii::mathexpr raii::specfunc1::extr_sub( )
{
   return heap. extract( );
}

bool raii::mathexpr::very_equal_to( const mathexpr& other ) const
{
   throw std::logic_error( "not implemented" ); 
}

