

#ifndef MATHEXPR_
#define MATHEXPR_

#include "selector.h"
   // Use the automatically generated enumeration type.

#include "rational.h"
#include "tvm2026/includes.h"

struct mathexpr
{

   struct opt_ratconst 
   {
      rational local;

      template< std::convertible_to<rational> T >  
      opt_ratconst( T&& val )
         : local( val )
      { }
      
      const rational& val( ) const
         { return local; }

      void update_val( const rational& newval );
      rational extr_val( );
   };

   struct opt_mathconst 
   { 
   };

   struct opt_variable
   {
      std::string local;

      template< std::convertible_to<rational> T >
      opt_variable( T&& name )
         : local( name )
      { }

      const std::string& name( ) const
         { return local; }

      void update_name( const std::string& newval );
      std::string extr_name( ); 
   };

   struct opt_arith1 
   { 
      tvm::scalar< mathexpr > onheap; 

      opt_arith1( const mathexpr& sub )
         : onheap( sub )  
      { }

      const mathexpr& sub( ) const
         { return onheap. access( ); }

      void update_sub( const mathexpr& newval )
      {
         if( tvm::distinct( sub( ), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access( ) = newval;
         }
      }

      mathexpr extr_sub( )
      {
         if( onheap. isshared( ))
            return onheap. access( );
         else
            return std::move( onheap. access( ));
      }
   };


   struct opt_arith2 
   { 
      tvm::scalar< std::pair< mathexpr, mathexpr >> onheap;

      opt_arith2( const mathexpr& sub1, const mathexpr& sub2 )
         : onheap( std::pair< mathexpr, mathexpr > ( sub1, sub2 ))
      { }

      const mathexpr& sub1( ) const
         { return onheap. access( ). first; } 
      const mathexpr& sub2( ) const
         { return onheap. access( ). second; }

      void update_sub1( const mathexpr& newval )
      {
         if( tvm::distinct( sub1( ), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access( ). first = newval;
         }
      }

      void update_sub2( const mathexpr& newval )
      { 
         if( tvm::distinct( sub2( ), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access( ). second = newval;
         }
      }

      mathexpr extr_sub1( )
      {
         if( onheap. isshared( ))
            return onheap. access( ). first;
         else
            return std::move( onheap. access( ). first );
      }

      mathexpr extr_sub2( )
      {
         if( onheap. isshared( ))
            return onheap. access( ). second;
         else
            return std::move( onheap. access( ). second );
      }

   };


   struct opt_arithN 
   { 
      tvm::scalar_vector< tvm::unit, mathexpr > onheap; 

      template< std::random_access_iterator It >
      requires requires( It i ) {{ *i } -> std::convertible_to< mathexpr > ; }
      opt_arithN( It begin, It end )
         : onheap( tvm::unit( ), begin, end ) 
      { }
   
      const mathexpr& sub( size_t i ) const
         { return onheap. access(i); }

      size_t size( ) const
         { return onheap. size( ); }

      void update_sub( size_t i, const mathexpr& newval )
      {
         if( tvm::distinct( sub(i), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access(i) = newval;
         }
      }

      mathexpr extr_sub( size_t i )
      {
         if( onheap. isshared( ))
            return onheap. access(i);
         else
            return std::move( onheap. access(i));
      }

      template< std::convertible_to< mathexpr > V > 
      void push_back( V&& val )
      {
         onheap. push_back( std::forward<V> ( val )); 
      }

      void pop_back( );
   };


   struct opt_specfunc1 
   { 
      tvm::scalar< mathexpr > onheap; 

      opt_specfunc1( const mathexpr& sub )
         : onheap( sub )
      { }

      const mathexpr& sub( ) const
         { return onheap. access( ); }

      void update_sub( const mathexpr& newval )
      {
         if( tvm::distinct( sub( ), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access( ) = newval;
         }
      }

      mathexpr extr_sub( )
      {
         if( onheap. isshared( ))
            return onheap. access( );
         else
            return std::move( onheap. access( ));
      }
   };


   struct opt_specfunc2 
   { 
      tvm::scalar< std::pair< mathexpr, mathexpr >> onheap; 

      opt_specfunc2( const mathexpr& sub1, const mathexpr& sub2 )
         : onheap( std::pair< mathexpr, mathexpr > ( sub1, sub2 ))
      { }

      const mathexpr& sub1( ) const
         { return onheap. access( ). first; }
      const mathexpr& sub2( ) const
         { return onheap. access( ). second; }

      void update_sub1( const mathexpr& newval )
      {
         if( tvm::distinct( sub1( ), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access( ). first = newval;
         }
      }

      void update_sub2( const mathexpr& newval )
      {
         if( tvm::distinct( sub2( ), newval ))
         {
            onheap. ensurenotshared( );
            onheap. access( ). second = newval;
         }
      }

      mathexpr extr_sub1( )
      {
         if( onheap. isshared( ))
            return onheap. access( ). first;
         else
            return std::move( onheap. access( ). first );
      }

      mathexpr extr_sub2( )
      {
         if( onheap. isshared( ))
            return onheap. access( ). second;
         else
            return std::move( onheap. access( ). second );
      }

   };


   union options 
   {
      opt_ratconst fld_ratconst; 
      opt_mathconst fld_mathconst; 
      opt_variable fld_variable; 
      opt_arith1 fld_arith1; 
      opt_arith2 fld_arith2;
      opt_arithN fld_arithN; 
      opt_specfunc1 fld_specfunc1;
      opt_specfunc2 fld_specfunc2; 

      options( ) noexcept : fld_mathconst( ) { }
      ~options( ) noexcept { } 
   };

private:
   selector _ssss;
   options repr;

   static void wrong_selector( selector sel ) 
      { throw std::logic_error( "wrong selector for constructor" ); }

    static constexpr bool check = true;
public:

   mathexpr( ) = delete; 
   mathexpr( const mathexpr& );
   mathexpr( mathexpr&& ) noexcept;
   mathexpr& operator = ( const mathexpr& );
   mathexpr& operator = ( mathexpr&& ) noexcept;
   ~mathexpr( ) noexcept;

   selector sel( ) const { return _ssss; }

   bool strongly_equal_to( const mathexpr& ) const;
   void printstate( std::ostream& out = std::cout ) const;
    
   const opt_ratconst* view_ratconst( ) const { return &repr. fld_ratconst; }
   opt_ratconst* view_ratconst( ) { return &repr. fld_ratconst; }

   const opt_mathconst* view_mathconst( ) const { return &repr. fld_mathconst; }
   opt_mathconst* view_mathconst( ) { return &repr. fld_mathconst; }

   const opt_variable* view_variable( ) const { return &repr. fld_variable; }
   opt_variable* view_variable( ) { return &repr. fld_variable; }
 
   const opt_arith1* view_arith1( ) const { return &repr. fld_arith1; }
   opt_arith1* view_arith1( ) { return &repr. fld_arith1; } 

   const opt_arith2* view_arith2( ) const { return &repr. fld_arith2; }
   opt_arith2* view_arith2( ) { return &repr. fld_arith2; }   

   const opt_arithN* view_arithN( ) const { return &repr. fld_arithN; }
   opt_arithN* view_arithN( ) { return &repr. fld_arithN; }

   const opt_specfunc1* view_specfunc1( ) const { return &repr. fld_specfunc1; }
   opt_specfunc1* view_specfunc1( ) { return &repr. fld_specfunc1; }

   const opt_specfunc2* view_specfunc2( ) const { return &repr. fld_specfunc2; }
   opt_specfunc2* view_specfunc2( ) { return &repr. fld_specfunc2; }


   const opt_ratconst* try_ratconst( ) const; 
   opt_ratconst* try_ratconst( );

   const opt_mathconst* try_mathconst( ) const; 
   opt_mathconst* try_mathconst( ); 

   const opt_variable* try_variable( ) const; 
   opt_variable* try_variable( ); 

   const opt_arith1* try_arith1( ) const; 
   opt_arith1* try_arith1( ); 
   
   const opt_arith2* try_arith2( ) const;
   opt_arith2* try_arith2( ); 
   
   const opt_arithN* try_arithN( ) const;
   opt_arithN* try_arithN( );

   const opt_specfunc1* try_specfunc1( ) const;
   opt_specfunc1* try_specfunc1( );
   
   const opt_specfunc2* try_specfunc2( ) const;
   opt_specfunc2* try_specfunc2( ); 
   
   mathexpr( selector sel, const rational& val )
      : _ssss( sel )
   {
      switch( _ssss )
      {
      case sel_rat:
         tvm::init( repr. fld_ratconst, opt_ratconst( val ));
         return;
      default:
         wrong_selector( sel );
      }   
   } 

   mathexpr( selector sel )
      : _ssss( sel )
   {
      switch( _ssss )
      {
      case sel_pi:
      case sel_e:
         return; 
      default:
         wrong_selector( sel );
      }
   }
 
   mathexpr( selector sel, const std::string& var )
      : _ssss( sel ) 
   {
      switch( _ssss )
      {
      case sel_var:
         tvm::init( repr. fld_variable, opt_variable( var ));
         return;
      default:
         wrong_selector( sel );
      } 
   }

   mathexpr( selector sel, const mathexpr& sub )
      : _ssss( sel )
   {
      switch( _ssss )
      {
      case sel_neg:
      case sel_recipr:
         tvm::init( repr. fld_arith1, opt_arith1( sub )); 
            return;
      case sel_sin:
      case sel_cos:
      case sel_exp:
      case sel_ln:
      case sel_sqrt:
         tvm::init( repr. fld_specfunc1, opt_specfunc1( sub ));
         return; 
      default:
         wrong_selector( sel );
      }
   }
        
   mathexpr( selector sel, const mathexpr& sub1, const mathexpr& sub2 ) 
      : _ssss( sel )
   {
      switch( _ssss )
      {
      case sel_sub:
      case sel_truediv:
         tvm::init( repr. fld_arith2, opt_arith2( sub1, sub2 )); 
         return;
      case sel_pow:
         tvm::init( repr. fld_specfunc2, opt_specfunc2( sub1, sub2 )); 
         return;
      default:
         wrong_selector( sel );
      }
   }

   template< std::random_access_iterator It >
   requires requires( It i ) {{ *i } -> std::convertible_to< mathexpr > ; }
   mathexpr( selector sel, It begin, It end )
      : _ssss( sel )
   {
      switch( sel )
      {
      case sel_add:
      case sel_mul:
         tvm::init( repr. fld_arithN, opt_arithN( begin, end ));
         return;
      default:
         wrong_selector( sel );
      }
   }

   mathexpr( selector sel, std::initializer_list< mathexpr > sub ) 
      : mathexpr( sel, sub. begin( ), sub. end( ))
   { }
  
};


inline mathexpr mathexpr_rat( const rational& val )
   { return mathexpr( sel_rat, val ); }

inline mathexpr mathexpr_pi( )
   { return mathexpr( sel_pi ); }

inline mathexpr mathexpr_e( )
   { return mathexpr( sel_e ); }

inline mathexpr mathexpr_var( const std::string& name )
   { return mathexpr( sel_var, name ); }

inline mathexpr mathexpr_neg( const mathexpr& sub )
   { return mathexpr( sel_neg, sub ); }

inline mathexpr mathexpr_recipr( const mathexpr& sub )
   { return mathexpr( sel_recipr, sub ); }

inline mathexpr mathexpr_sub( const mathexpr& sub1, const mathexpr& sub2 )
   { return mathexpr( sel_sub, sub1, sub2 ); }

inline
mathexpr mathexpr_truediv( const mathexpr& sub1, const mathexpr& sub2 )
   { return mathexpr( sel_truediv, sub1, sub2 ); }

template< std::random_access_iterator It >
requires std::convertible_to< std::iter_value_t<It>, mathexpr >
inline mathexpr mathexpr_add( It begin, It end )
   { return mathexpr( sel_add, begin, end ); }

inline mathexpr mathexpr_add( std::initializer_list< mathexpr > args )
   { return mathexpr_add( args. begin( ), args. end( )); }

template< std::random_access_iterator It >
requires std::convertible_to< std::iter_value_t<It>, mathexpr >
inline mathexpr mathexpr_mul( It begin, It end )
   { return mathexpr( sel_mul, begin, end ); }

inline mathexpr mathexpr_mul( std::initializer_list< mathexpr > sub )
   { return mathexpr_mul( sub. begin( ), sub. end( )); }

inline mathexpr mathexpr_sin( const mathexpr& sub )
   { return mathexpr( sel_sin, sub ); }

inline mathexpr mathexpr_cos( const mathexpr& sub )
   { return mathexpr( sel_cos, sub ); }

inline mathexpr mathexpr_exp( const mathexpr& sub )
   { return mathexpr( sel_exp, sub ); }

inline mathexpr mathexpr_ln( const mathexpr& sub )
   { return mathexpr( sel_ln, sub ); }

inline mathexpr mathexpr_sqrt( const mathexpr& sub )
   { return mathexpr( sel_sqrt, sub ); }

inline mathexpr mathexpr_pow( const mathexpr& sub1, const mathexpr& sub2 )
   { return mathexpr( sel_pow, sub1, sub2 ); }


std::ostream& operator << ( std::ostream& out, const mathexpr& expr );

#endif

