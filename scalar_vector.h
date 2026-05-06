
// Written by Hans de Nivelle with help of Olzhas Zhangeldinov.
// 2022 (revised 2026). 

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
         size_t ss;
         size_t cc;
         S scal; 
         V vect[s];
 
         ~onheap( ) = delete;
            // It's never constructed
      };

      // I had problems with specialization, possibly a gcc bug:

      struct onheap0
      {
         size_t nrrefs;
         size_t ss;
         size_t cc;
         S scal;

         template< std::convertible_to<S> S1 > 
         onheap0( S1&& scal, size_t cc )
            : nrrefs(1),
              ss(0),
              cc(cc),
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
         { return vectbegin( ptr ) + ( ptr -> ss ); } 

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
         ( ptr -> ss ) = end - begin; 
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

      size_t size( ) const { return ptr -> ss; }

      using const_iterator = const V*;

      const_iterator begin() const { return vectbegin( ptr ); } 
      const_iterator end() const { return vectend( ptr ); } 

      template< std::convertible_to<S> S1 > S& set( S1&& scal )
      {
         if( isshared( ))
         {
            -- ( ptr -> nrrefs );
            auto oldptr = ptr;
            ptr = alloc( std::forward<S1> ( scal ), ptr -> cc );
            copy_construct( vectbegin( oldptr ), vectend( oldptr ), 
                            vectbegin( ptr ));
            ptr -> ss = oldptr -> ss;
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

      void reserve( size_t new_cc )
      {
         if( isshared( ))
         {
            -- ( ptr -> nrrefs );
            auto oldptr = ptr;
            ptr = alloc( get( ), new_cc );
            copy_construct( vectbegin( oldptr ), vectend( oldptr ),
                            vectbegin( ptr ));
            ptr -> ss = oldptr -> ss;
         }
         else
         {
            if( new_cc > ( ptr -> cc ))
            {
               auto newptr = alloc( std::move( get( )), new_cc );
               // std::cout << newptr -> ss << "/" << newptr -> cc << "\n";
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

               ( newptr -> ss ) = ( ptr -> ss ); 
               destroy( ptr );
               ptr = newptr; 
            } 
         }
      }


      template< std::convertible_to<V> V1 >
      void push_back( V1&& v )
      {
         if( ptr -> ss < ptr -> cc )
            reserve( ptr -> ss );
         else
         {
            size_t cc = ( ptr -> cc );
            size_t new_cc = cc + 1;
            if( new_cc < cc + cc )
               new_cc = cc + cc;
            reserve( new_cc );
         } 
         new ( vectbegin( ptr ) + ( ptr -> ss ++ )) 
                                    V( std::forward<V1> (v) ); 
      }

      void pop_back()
      {
         if( isshared( ))
            set( get( ));
         size_t ss = -- ( ptr -> ss );
         ( vectbegin( ptr ) + ss ) -> ~V( ); 
      }

      void clear()
      {
         if( isshared( ))
            set( get( ));
         shared::destroy( vectbegin( ptr ), vectend( ptr ));      
         ( ptr -> ss ) = 0;
      }

      // Print with internals:

      void printstate( std::ostream &out = std::cout ) const
      {
         auto s = (uint16_t) (uintptr_t) ptr;
         out << "(state: ";
         util::hexprint( out, s );
         out << ", #" << ( ptr -> nrrefs ) << ", ";
         out << ( ptr -> ss ) << "/" << ( ptr -> cc ) << ")";
      }

      bool isshared() const { return ( ptr -> nrrefs ) != 1; }
      bool issharedwith( const scalar_vector& other ) const 
         { return ptr == other. ptr; }

   };

}

#endif


