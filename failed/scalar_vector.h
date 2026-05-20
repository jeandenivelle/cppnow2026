
// Written by Hans de Nivelle with help of Olzhas Zhangeldinov.
// 2022 (revised 2026). 
// Note that this implementation of scalar_vector is not usable. 
// The problem is that I wanted to move reference counting
// to the scalar_vector (here), but that that is not possible
// in all cases. There are also some stylistic problems
// that are not worth fixing, since the data structure as a whole
// is unusable. It's a pity, because it would have been elegant.


#ifndef SHARED_SCALAR_VECTOR_
#define SHARED_SCALAR_VECTOR_

#include <stdexcept>
#include <iostream>
#include "memory.h"
#include "util/hexprint.h"

namespace shared
{

   template< typename S, typename V >
   class scalar_vector
   {
      
      template< size_t s >
      struct onheap
      {
         size_t nrrefs;
         size_t sz;
         size_t cap;
         S scal; 
         V vect[s];
 
         ~onheap( ) = delete;
            // It's never constructed
      };

      // I had problems with specialization, possibly a gcc bug:

      struct onheap0
      {
         size_t nrrefs;
         size_t sz;
         size_t cap;
         S scal;

         template< std::convertible_to<S> S1 > 
         onheap0( S1&& scal, size_t cap )
            : nrrefs(1),
              sz(0),
              cap(cap),
              scal( std::forward<S1> ( scal ))
         { }

      };

      onheap0* ptr;

      using SV = onheap<1> ;

      constexpr static size_t offset0 = offsetof( SV, vect );
         // Would be nice if this could be replaced by something portable. 

      static constexpr V* vectbegin( onheap0* ptr ) 
         { return (( onheap<1> * ) ptr ) -> vect; }

     static constexpr V* vectend( onheap0* ptr ) 
         { return vectbegin( ptr ) + ( ptr -> sz ); } 

      inline static void destroy( onheap0* ptr )
      {
         static_assert( std::is_nothrow_destructible<V>::value,
            "Scalar type for scalar_vector must not throw at destruction" );
 
         static_assert( std::is_nothrow_destructible<V>::value,
            "Vector type for scalar_vector must not throw at destruction" );
       
         ( ptr -> scal ). ~S( );
           
         // If destruction is trivial, we skip the destruction: 

         if constexpr( !std::is_trivially_destructible_v<V> )
            shared::destroy( vectbegin( ptr ), vectend( ptr ));

         std::free( ptr );
      }

public:

      template< std::convertible_to<S> S1 >
      static onheap0* alloc( S1&& scal, size_t cap )
      {
         size_t a = offset0 + cap * sizeof(V);
         auto p = (onheap0*) std::malloc(a);
            // This might throw a std::bad_alloc.

         if constexpr( std::is_nothrow_convertible_v< S1, S > )
            new (p) onheap0( std::forward<S1> ( scal ), cap );
         else
         {
            try 
            {
               new (p) onheap0( std::forward<S1> ( scal ), cap ); 
            }
            catch( ... )
            {
               std::free(p);
               throw;
            }
         }
         return p;
      }

public:
      template< std::convertible_to<S> S1 > 
      scalar_vector( S1&& scal )
         : ptr( alloc( std::forward<S1> ( scal ), 0 ))
      { }

      template< std::convertible_to<S> S1,
                std::random_access_iterator It >
      scalar_vector( S1&& scal, It begin, It end )
         : ptr( alloc( std::forward<S1> ( scal ), end - begin ))
      {
         V* into = vectbegin( ptr );
         copy_construct( begin, end, into ); 
         ( ptr -> sz ) = end - begin; 
      }

      template< std::convertible_to<S> S1 >
      scalar_vector( S1&& scal, std::initializer_list<V> init )
         : scalar_vector( std::forward<S1> ( scal ), 
                          init. begin( ), init. end( ))
      { }

      scalar_vector( scalar_vector&& other ) = delete;
      scalar_vector& operator = ( scalar_vector&& other ) = delete;
         // I think that there is no reasonable definition for the
         // moving operators, because of the scalar element. 
         // Our intended use is as option in a tree. 
         // Moving should be implemented by switching the tree to
         // another option.

      scalar_vector( const scalar_vector& other ) noexcept
         : ptr( other. ptr )
      {
         ++ ( ptr -> nrrefs );
      }

      scalar_vector& operator = ( const scalar_vector& other )
      {
         ++ ( other. ptr -> nrrefs ); 
         if( -- ( ptr -> nrrefs ) == 0 )
            destroy( ptr ); 

         ptr = other. ptr; 
         return *this;
      }

      ~scalar_vector()
      {
         if( ( -- ptr -> nrrefs ) == 0 )
            destroy( ptr ); 
      }


      const S& get( ) const { return ptr -> scal; }
      const V& get( size_t i ) const { return vectbegin( ptr )[i]; }
         // Merrily unchecked.

      size_t size( ) const { return ptr -> sz; }

      using const_iterator = const V*;

      const_iterator begin() const { return vectbegin( ptr ); } 
      const_iterator end() const { return vectend( ptr ); } 

      template< std::convertible_to<S> S1 > S& set( S1&& scal )
      {
         if( isshared( ))
         {
            -- ( ptr -> nrrefs );
            auto oldptr = ptr;
            ptr = alloc( std::forward<S1> ( scal ), ptr -> cap );
            copy_construct( vectbegin( oldptr ), vectend( oldptr ), 
                            vectbegin( ptr ));
            ptr -> sz = oldptr -> sz;
            return ( ptr -> scal );
         }
         else
            return ( ptr -> scal ) = scal;
      }

      template< std::convertible_to<V> V1 > V& set( size_t i, V1&& val )
      {
         if( isshared( ))
            set( get( )); 
         
         return vectbegin( ptr )[i] = std::forward<V1>( val );
      }

      S extract( )
      {
         if( isshared( ))
            return ( ptr -> scal );
         else
            return std::move( ptr -> scal );
      }

      V extract( size_t i ) 
      {
         if( isshared( ))
            return vectbegin( ptr )[i];
         else
            return std::move( vectbegin( ptr )[i] );
      }

      // Call this only if you are certain that you will change the vector:

      void reserve( size_t new_cap )
      {
         if( isshared( ))
         {
            -- ( ptr -> nrrefs );
            auto oldptr = ptr;
            ptr = alloc( get( ), new_cap );
            copy_construct( vectbegin( oldptr ), vectend( oldptr ),
                            vectbegin( ptr ));
            ptr -> sz = oldptr -> sz;
         }
         else
         {
            if( new_cap > ( ptr -> cap ))
            {
               auto newptr = alloc( std::move( get( )), new_cap );
               // std::cout << newptr -> sz << "/" << newptr -> cap << "\n";
               if constexpr( std::is_nothrow_move_constructible_v<V> )
               {
                   move_construct( vectbegin( ptr ), vectend( ptr ),
                                   vectbegin( newptr ));
               } 
               else
               {
                  try
                  {
                     copy_construct( vectbegin( ptr ), vectend( ptr ),
                                     vectbegin( newptr ));
                  }
                  catch( ... )
                  {
                     std::free( newptr );
                     throw;
                  }
               }

               ( newptr -> sz ) = ( ptr -> sz ); 
               destroy( ptr );
               ptr = newptr; 
            } 
         }
      }


      template< std::convertible_to<V> V1 >
      void push_back( V1&& v )
      {
         if( ptr -> sz < ptr -> cap )
            reserve( 1 + ptr -> sz );
         else
         {
            size_t cap = ( ptr -> cap );
            size_t new_cap = cap + 1;
            if( new_cap < cap + cap )
               new_cap = cap + cap;
            reserve( new_cap );
         } 
         new ( vectbegin( ptr ) + ( ptr -> sz ++ )) 
                                    V( std::forward<V1> (v) ); 
      }

      void pop_back()
      {
         if( isshared( ))
            set( get( ));
         size_t sz = -- ( ptr -> sz );
         ( vectbegin( ptr ) + sz ) -> ~V( ); 
      }

      void clear()
      {
         if( isshared( ))
            set( get( ));
         shared::destroy( vectbegin( ptr ), vectend( ptr ));      
         ( ptr -> sz ) = 0;
      }

      // Print with internals:

      void printstate( std::ostream &out = std::cout ) const
      {
         auto s = (uint16_t) (uintptr_t) ptr;
         out << "(state: ";
         util::hexprint( out, s );
         out << ", #" << ( ptr -> nrrefs ) << ", ";
         out << ( ptr -> sz ) << "/" << ( ptr -> cap ) << ")";
      }

      bool isshared() const { return ( ptr -> nrrefs ) != 1; }
      bool issharedwith( const scalar_vector& other ) const 
         { return ptr == other. ptr; }

   };

}

#endif


