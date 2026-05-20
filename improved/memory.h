
#ifndef TVM_MEMORY_
#define TVM_MEMORY_

#include <iterator>

namespace tvm
{

   // Written by Hans de Nivelle, December 2022.
   // Made polymorphic in May 2026.

   template< typename X, typename ... From >
   void init( X& var, From&& ... args )
      { new (&var) X( std::forward< From > ( args ) ... ); }

   template< typename X, typename From >
   void assign( X& var, From&& val )
      { var = std::forward< From > ( val ); }

   template< std::destructible X >
   void destroy( X& var ) { (&var ) -> ~X( ); }



   // Written by Hans de Nivelle, April 2026.

   template< std::input_iterator It >
   void destroy( It gone1, It gone2 )
   {
      using tp = std::iter_value_t< It > ;

      if constexpr( !std::is_trivially_destructible_v< tp > )
      {
         while( gone2 != gone1 )
            ( -- gone2 ) -> ~tp( );
      }
   }


   template< std::input_iterator In, 
             std::output_iterator< std::iter_value_t< In >> Out > 
   inline void copy_construct( In from1, In from2, Out into )
   {
      // std::cout << "copying into " << new_heap << "\n";

      using tp = std::iter_value_t< In > ;

      auto t = into; 
      try
      {
         while( from1 != from2 ) 
         {
            new (t) tp( *from1 ++ );
            ++ t;
         }
      }
      catch( ... )
      {
         while( t != into )
            ( --t ) -> ~tp( ); 
         throw;
      }
   }


   template< std::input_iterator In,  
             std::output_iterator< std::iter_value_t< In >> Out >
   void move_construct( In from1, In from2, Out into )
   {
      // If D allows noexcept moving, we prefer that:

      using tp = std::iter_value_t< In > ;

      if constexpr( std::is_nothrow_move_constructible_v<tp> )
      {
         while( from1 != from2 )
            new ( into ++ ) tp( std::move( * from1 ++ )); 
      }
      else
      {
         copy_construct( from1, from2, into );
      }
   } 
}

#endif


