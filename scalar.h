
#ifndef SHARED_SCALAR_
#define SHARED_SCALAR_

#include <iostream>
#include <stdexcept> 
#include "util/hexprint.h"
#include <cstdint>

// Similar to std::shared_ptr, but different interface:

namespace shared
{
   template< typename S > 
   class scalar 
   {
      struct onheap
      {
         S scal;
         size_t nrrefs;

         onheap( ) :
            scal( S( )),
            nrrefs(1)
         { }

         template< std::convertible_to<S> S1 > onheap( S1&& scal )
            : scal( std::forward<S1> ( scal )),
              nrrefs(1)
         { }
      };

      onheap* ptr;

   public: 
      scalar( ) 
         : ptr( new onheap( ))
      { }

      template< std::convertible_to<S> ... S1 > scalar( S1 && ... scal )
         : ptr( new onheap( std::forward< S1 ... > ( scal ... )))
      { }

      scalar( scalar&& ) = delete;
      scalar& operator = ( scalar&& ) = delete;
 
      scalar( const scalar& other ) noexcept 
         : ptr( other. ptr )
      {
         ++ ( ptr -> nrrefs );
      }

      scalar& operator = ( const scalar& other ) 
      {
         ++ ( other. ptr -> nrrefs );
         if( -- ( ptr -> nrrefs ) == 0 )
            delete ptr;
         ptr = other. ptr;
         return *this; 
      }

      ~scalar( )
      {
         if( -- ( ptr -> nrrefs ) == 0 )
            delete ptr;
      }

      const S& get( ) const { return ptr -> scal; }

      template< std::convertible_to<S> S1 > S& set( S1&& scal )
      {
         if( isshared( ))
         {
            -- ( ptr -> nrrefs );
            ptr = new onheap( std::forward<S1> ( scal ));
            return ( ptr -> scal ); 
         }
         else
         {
            return ( ptr -> scal ) = scal;
         }
      }

      S extract( ) 
      {
         if( isshared( ))
            return ( ptr -> scal );
         else
            return std::move( ptr -> scal );
      }

      bool isshared( ) const { return ( ptr -> nrrefs ) != 1; }
      bool issharedwith( const scalar& other ) const 
         { return ptr == other. ptr; } 

      void printstate( std::ostream &out = std::cout ) const
      {
         auto s = ( uint16_t ) ( uintptr_t ) ptr;
         out << "(state: ";
         util::hexprint( out, s );
         out << ", #" << ( ptr -> nrrefs ) << ")";
      }

   };

}


#endif


