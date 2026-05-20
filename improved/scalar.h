
// Written by Hans de Nivelle, December 2022.
// Modified by Hans de Nivelle in May 2026.

#ifndef TVM_SCALAR_
#define TVM_SCALAR_    

#include <cstddef>
#include <cstdint>
#include <concepts>
#include <utility>

#include "util/hexprint.h"

namespace tvm
{

   template< typename S > 
   struct priv_scal
   {
      size_t nrrefs;
      S scal;  

      template< std::convertible_to<S> ... S1 > priv_scal( S1&& ... scal )
         : nrrefs(1), scal( std::forward<S1> ( scal ) ... )
      { }

      priv_scal( ) = delete;
      priv_scal( const priv_scal& ) = delete;
      priv_scal& operator = ( const priv_scal& ) = delete; 
   };


   template< typename S >
   struct scalar 
   {
      priv_scal<S> * ptr;

      template< std::convertible_to<S> ... S1 > scalar( S1&& ... scal )
         : ptr( new priv_scal<S> ( std::forward<S1> ( scal ) ... ))
      { }


      const S& access( ) const
         { return ptr -> scal; }

      S& access( )
         { return ptr -> scal; }

      void printstate( std::ostream& out = std::cout ) const
      {
         auto s = ( uint16_t ) ( uintptr_t ) ptr;
         out << "(state: ";
         util::hexprint( out, s );
         out << ", #" << ( ptr -> nrrefs ) << ")";
      }      

      // Making these methods const solves a lot of problems:

      void connect( ) const
         {  ++ ptr -> nrrefs; }

      void disconnect( ) const
      {
         if( -- ( ptr -> nrrefs ) == 0 )
            delete( ptr );
      }
  
      void ensurenotshared( )
      {
         if( isshared( )) 
         {
            -- ( ptr -> nrrefs );  
            ptr = new priv_scal<S> ( ptr -> scal );
         }
      }
 
      bool isshared() const { return ( ptr -> nrrefs ) != 1; }
      bool issharedwith( const scalar& other ) const
         { return ptr == other. ptr; }

   };

}

#endif

