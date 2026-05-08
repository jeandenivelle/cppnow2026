
#ifndef SHARED_MEMORY_
#define SHARED_MEMORY_

#include <iterator>

namespace shared
{

   template< std::input_iterator It >
   void destroy( It gone1, It gone2 )
   {
      using tp = std::iter_value_t< It > ;

      if constexpr( !std::is_trivially_destructible_v< tp > )
      {
         while( gone2 != gone1 )
            ( -- gone2 ) -> tp::~tp( );
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
         copy_construct_range( from1, from2, into );
   } 
}

#endif


