
#ifndef TVM_SCALAR_VECTOR_
#define TVM_SCALAR_VECTOR_  

#include <new>
#include <cstdint>
#include <utility>

#include "util/hexprint.h"
#include "memory.h"

// Written by Hans de Nivelle and Dina Muktubayeva 
// October-November 2022.
// Revised by Hans de Nivelle, 2026.

namespace tvm
{

   // We will use the specialization for s = 0:

   template< typename S, typename V, size_t s > 
   struct priv_scal_vect
   {
      size_t nrrefs;   
      size_t sz;    
      size_t cap;  
      S scal;  
      V vect[s];

      priv_scal_vect( ) = delete;
      priv_scal_vect( const priv_scal_vect& ) = delete; 
      ~priv_scal_vect( ) = delete; 
         // To make sure that nobody will accidentally get the idea of
         // creating one. 
   };


   // Note that this an incomplete class. All memory management
   // is done by the tree class that uses it. 

   template< typename S, typename V > 
   struct priv_scal_vect<S,V,0> 
   {
      size_t nrrefs;
      size_t sz;
      size_t cap;
      S scal;

      // I could change the order of the arguments, and
      // make the scal parameter into a parameter pack,
      // but I think the added uglyness is not worth it.

      template< std::convertible_to<S> S1 >  
      priv_scal_vect( S1&& scal, size_t cap )
        : nrrefs(1), sz(0), cap(cap), scal( std::forward<S1> ( scal ))
      { }

      // We don't have V-s, but we pretend that we have them:

      V* vectbegin( )
         { return (( priv_scal_vect<S,V,1> * ) this ) -> vect; }

      const V* vectbegin( ) const
         { return (( const priv_scal_vect<S,V,1> * ) this ) -> vect; }

      V* vectend( )
         { return vectbegin( ) + sz; }

      const V* vectend( ) const
         { return vectbegin( ) + sz; }

      size_t size( ) const { return sz; }
      size_t capacity( ) const { return cap; } 

      ~priv_scal_vect( ) = default;
      priv_scal_vect( const priv_scal_vect& ) = delete;
      priv_scal_vect& operator = ( const priv_scal_vect& ) = delete; 
   };


   // I try to imitate the new of C++, but it doesn't fully,
   // because the vector is not initialized:

   template< typename S, typename V, std::convertible_to<S> S1 >
   priv_scal_vect<S,V,0> * our_new( S1&& scal, size_t cap )
   {
      using SV1 = priv_scal_vect<S,V,1> ;
      using heaptype = priv_scal_vect<S,V,0> ;

      static constexpr size_t offset0 = offsetof( SV1, vect );
     
      size_t a = offset0 + cap * sizeof(V);
      auto p = ( heaptype* ) std::malloc(a);
         // This may throw a std::bad_alloc.

      if constexpr( std::is_nothrow_convertible_v<S1,S> )
         new (p) heaptype( std::forward<S1> ( scal ), cap );
      else
      {
         try 
         {
            new (p) heaptype( std::forward<S1> ( scal ), cap ); 
         }
         catch( ... )
         {
            std::free(p);
            throw;
         }
      }
      return p;
   }


   template< typename S, typename V >
   void our_delete( priv_scal_vect<S,V,0> * ptr )
   {
      static_assert( std::is_nothrow_destructible<V>::value,
         "Scalar type for scalar_vector must not throw at destruction" );
 
      static_assert( std::is_nothrow_destructible<V>::value,
         "Vector type for scalar_vector must not throw at destruction" );

      if constexpr( !std::is_trivially_destructible_v<V> )
         destroy( ptr -> vectbegin( ), ptr -> vectend( ));

       ( ptr -> scal ). ~S( );

       std::free( ptr );
   }


   // This class does not support RAII. The user has to take
   // care of the reference counting through functions
   // connect( ) and disconnect( ). 

   template< typename S, typename V >
   struct scalar_vector 
   {
      priv_scal_vect<S,V,0> * ptr;

      template< std::convertible_to<S> S1 >
      scalar_vector( S1&& scal, size_t cap = 0 )
         : ptr( our_new<S,V> ( std::forward<S1> ( scal ), cap ))
      { }


      template< std::convertible_to<S> S1,
                std::random_access_iterator It >
      scalar_vector( S1&& scal, It begin, It end )
         : ptr( our_new<S,V> ( std::forward<S1> ( scal ), end - begin ))
      {
         V* into = ptr -> vectbegin( );
         try
         {
            copy_construct( begin, end, into ); 
         }
         catch( ... )
         {
            our_delete( ptr );
            throw;
         }
         ( ptr -> sz ) = end - begin; 
      }

      template< std::convertible_to<S> S1 >
      scalar_vector( S1&& scal, std::initializer_list<V> init )
         : scalar_vector( std::forward<S1> ( scal ), 
                          init. begin( ), init. end( ))
      { }


      const V* vectbegin( ) const 
         { return ptr -> vectbegin( ); }

      const V* vectend( ) const 
         { return ptr -> vectend( ); }
   
      size_t size( ) const 
         { return ptr -> size( ); }

      size_t capacity( ) const
         { return ptr -> capacity( ); }


      const S& access( ) const 
         { return ptr -> scal; }

      S& access( )
         { return ptr -> scal; }

      // Merrily unchecked:

      const V& access( size_t i ) const 
         { return ptr -> vectbegin( )[i]; }

      V& access( size_t i ) 
         { return ptr -> vectbegin( )[i]; }
     

      // Reserve will always make a unique copy: 

      void reserve( size_t new_cap )
      {
         if( isshared( ))
         {
            -- ( ptr -> nrrefs ); 
            auto oldptr = ptr;
            ptr = our_new<S,V> ( access( ), new_cap );
            copy_construct( oldptr -> vectbegin( ), oldptr -> vectend( ),
                            ptr -> vectbegin( ));
            ptr -> sz = oldptr -> sz; 
         }
         else
         {
            if( new_cap > capacity( ))
            {
               auto newptr = our_new<S,V> ( std::move( access( )), new_cap );

               if constexpr( std::is_nothrow_move_constructible_v<V> )
               {
                   move_construct( ptr -> vectbegin( ), ptr -> vectend( ),
                                   newptr -> vectbegin( ));
               } 
               else
               {
                  try
                  {
                     copy_construct( ptr -> vectbegin( ), ptr -> vectend( ),
                                     newptr -> vectbegin( ));
                  }
                  catch( ... )
                  {
                     our_delete( newptr );
                     throw;
                  }
               }

               ( newptr -> sz ) = ( ptr -> sz ); 
               our_delete( ptr );
               ptr = newptr; 
            } 
         }
      }

      // This function must be called only when the reference counters are
      // correctly set. 

      template< std::convertible_to<V> V1 >
      void push_back( V1&& v )
      {
         if( size( ) < capacity( )) 
            ensurenotshared( );   
         else
         {
            size_t cap = capacity( );
            size_t new_cap = cap + 1;
            if( new_cap < cap + cap )
               new_cap = cap + cap;

            reserve( new_cap );
         } 

         new ( ptr -> vectbegin( ) + ( ptr -> sz ++ )) 
             V( std::forward<V1> (v) ); 
      }

      void pop_back()
      {
         ensurenotshared( );
         size_t sz = -- ( ptr -> sz );
         ( ptr -> vectbegin( ) + sz ) -> ~V( ); 
      }

      void clear()
      {
         ensurenotshared( ); 
         destroy( ptr -> vectbegin( ), ptr -> vectend( ));      
         ( ptr -> sz ) = 0;
      }

      void erase( size_t i )
      {
         std::cout << "how to do this? what if there is an exception?\n";
         ensurenotshared( );
      } 

      void printstate( std::ostream &out = std::cout ) const
      {
         auto s = (uint16_t) (uintptr_t) ptr;
         out << "(state: ";
         util::hexprint( out, s );
         out << ", #" << ( ptr -> nrrefs ) << ", ";
         out << ( ptr -> sz ) << "/" << ( ptr -> cap ) << ")";
      }

      // Making these methods const solves a lot of problems:

      void connect( ) const
         { ++ ptr -> nrrefs; }
      
      void disconnect( ) const
      {
         if( -- ( ptr -> nrrefs ) == 0 )
            our_delete( ptr );
      }

      void ensurenotshared( )
         { reserve( capacity( )); }

      bool isshared() const { return ( ptr -> nrrefs ) != 1; }
      bool issharedwith( const scalar_vector& other ) const 
         { return ptr == other. ptr; }

      // Note that all lifecycle operators are default, and hence incomplete. 

   };
}

#endif

