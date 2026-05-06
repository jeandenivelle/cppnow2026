
// This code was automatically built by TreeGen
// Written by Hans de Nivelle, see www.compiler-tools.eu

#ifndef MATHEXPR_
#define MATHEXPR_

#include <iostream>
#include <utility>
#include <tuple>
#include <initializer_list>
#include "selector.h"


#line 19 "mathexpr.rec"
#include "tvm/includes.h"
        #include "rational.h"

class mathexpr
{
private:
   selector _ssss;

   using _loc00 = tvm::unit;
   using _scal00 = mathexpr;
   using _rep00 = tvm::unit;
   using _loc01 = tvm::unit;
   using _scal01 = std::pair<mathexpr,mathexpr>;
   using _rep01 = tvm::unit;
   using _loc02 = tvm::unit;
   using _scal02 = tvm::unit;
   using _rep02 = mathexpr;
   using _loc03 = tvm::unit;
   using _scal03 = tvm::unit;
   using _rep03 = tvm::unit;
   using _loc04 = rational;
   using _scal04 = tvm::unit;
   using _rep04 = tvm::unit;
   using _loc05 = std::string;
   using _scal05 = tvm::unit;
   using _rep05 = tvm::unit;

   static constexpr bool check = true;

   union options
   {
      tvm::field< _loc00, _scal00, _rep00 > _fld00;
      tvm::field< _loc01, _scal01, _rep01 > _fld01;
      tvm::field< _loc02, _scal02, _rep02 > _fld02;
      tvm::field< _loc03, _scal03, _rep03 > _fld03;
      tvm::field< _loc04, _scal04, _rep04 > _fld04;
      tvm::field< _loc05, _scal05, _rep05 > _fld05;

      options( ) : _fld03( ) { }
      ~options( ) noexcept { }
   };

private:
   options repr;

public:
   mathexpr( ) = delete;
   mathexpr( const mathexpr& );
   mathexpr( mathexpr&& ) noexcept;
   const mathexpr& operator = ( const mathexpr& );
   const mathexpr& operator = ( mathexpr&& ) noexcept;
   ~mathexpr( ) noexcept;
   
   selector sel( ) const { return _ssss; }
   bool very_equal_to( const mathexpr& ) const;
   void printstate( std::ostream& out ) const;
   
   mathexpr( selector sel, const mathexpr & _xx00 )
      : _ssss( sel )
   {
      if constexpr( check )
      {
         switch( _ssss )
         {
         case sel_sin:
         case sel_ln:
         case sel_cos:
         case sel_exp:
         case sel_sqrt:
         case sel_recipr:
         case sel_neg:
            break;
         default:
            throw std::invalid_argument( "wrong selector for constructor" );
         }
      }
      repr. _fld00. heap = takeshare( tvm::constr_scalar< _scal00 > ( _xx00 ));
   }

   mathexpr( selector sel, const mathexpr & _xx00, const mathexpr & _xx01 )
      : _ssss( sel )
   {
      if constexpr( check )
      {
         switch( _ssss )
         {
         case sel_pow:
         case sel_truediv:
         case sel_sub:
            break;
         default:
            throw std::invalid_argument( "wrong selector for constructor" );
         }
      }
      repr. _fld01. heap = takeshare( tvm::constr_scalar< _scal01 > ( std::pair( _xx00, _xx01 ) ));
   }

   template< tvm::const_iterator< _rep02 > It >
   mathexpr( selector sel, It begin, It end )
      : _ssss( sel )
   {
      if constexpr( check )
      {
         switch( _ssss )
         {
         case sel_mul:
         case sel_add:
            break;
         default:
            throw std::invalid_argument( "wrong selector for constructor" );
         }
      }
      repr. _fld02. heap = takeshare( tvm::constr_scalar_vector< _scal02, _rep02 > ( tvm::unit( ), begin, end ));
   }

   mathexpr( selector sel, std::initializer_list< _rep02 > repeated )
      : mathexpr( sel, repeated. begin( ), repeated. end( ) )
   { }

   mathexpr( selector sel )
      : _ssss( sel )
   {
      if constexpr( check )
      {
         switch( _ssss )
         {
         case sel_e:
         case sel_pi:
            break;
         default:
            throw std::invalid_argument( "wrong selector for constructor" );
         }
      }
   }

   mathexpr( selector sel, const rational & _xx00 )
      : _ssss( sel )
   {
      if constexpr( check )
      {
         switch( _ssss )
         {
         case sel_rat:
            break;
         default:
            throw std::invalid_argument( "wrong selector for constructor" );
         }
      }
      tvm::init( repr. _fld04. loc, _xx00 );
   }

   mathexpr( selector sel, const std::string & _xx00 )
      : _ssss( sel )
   {
      if constexpr( check )
      {
         switch( _ssss )
         {
         case sel_var:
            break;
         default:
            throw std::invalid_argument( "wrong selector for constructor" );
         }
      }
      tvm::init( repr. _fld05. loc, _xx00 );
   }

   bool option_is_arith1( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_recipr:
      case sel_neg:
         return true;
      default:
         return false;
      }
   }

   struct const_arith1
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_arith1( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }

      const mathexpr & sub( ) const { return _xxxx -> repr. _fld00. heap -> scal; }
   };

   const_arith1 view_arith1( ) const
   {
      if constexpr( check )
      {
         if( !option_is_arith1( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_arith1
   {
      mathexpr* _xxxx;
      mut_arith1( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      const mathexpr & sub( ) const { return _xxxx -> repr. _fld00. heap -> scal; }
      mathexpr extr_sub( ) const {
         if( iswritable( _xxxx -> repr. _fld00. heap ))
            return std::move( _xxxx -> repr. _fld00. heap -> scal );
         else
            return _xxxx -> repr. _fld00. heap -> scal;
      }
      void update_sub( const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld00. heap -> scal, repl ))
         {
            _xxxx -> repr. _fld00. heap = takeshare( replacebywritable( _xxxx -> repr. _fld00. heap ));
            _xxxx -> repr. _fld00. heap -> scal = repl;
         }
      }
   };

   mut_arith1 view_arith1( )
   {
      if constexpr( check )
      {
         if( !option_is_arith1( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_arith2( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_truediv:
      case sel_sub:
         return true;
      default:
         return false;
      }
   }

   struct const_arith2
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_arith2( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }

      const mathexpr & sub1( ) const { return _xxxx -> repr. _fld01. heap -> scal. first; }
      const mathexpr & sub2( ) const { return _xxxx -> repr. _fld01. heap -> scal. second; }
   };

   const_arith2 view_arith2( ) const
   {
      if constexpr( check )
      {
         if( !option_is_arith2( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_arith2
   {
      mathexpr* _xxxx;
      mut_arith2( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      const mathexpr & sub1( ) const { return _xxxx -> repr. _fld01. heap -> scal. first; }
      mathexpr extr_sub1( ) const {
         if( iswritable( _xxxx -> repr. _fld01. heap ))
            return std::move( _xxxx -> repr. _fld01. heap -> scal. first );
         else
            return _xxxx -> repr. _fld01. heap -> scal. first;
      }
      void update_sub1( const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld01. heap -> scal. first, repl ))
         {
            _xxxx -> repr. _fld01. heap = takeshare( replacebywritable( _xxxx -> repr. _fld01. heap ));
            _xxxx -> repr. _fld01. heap -> scal. first = repl;
         }
      }
      const mathexpr & sub2( ) const { return _xxxx -> repr. _fld01. heap -> scal. second; }
      mathexpr extr_sub2( ) const {
         if( iswritable( _xxxx -> repr. _fld01. heap ))
            return std::move( _xxxx -> repr. _fld01. heap -> scal. second );
         else
            return _xxxx -> repr. _fld01. heap -> scal. second;
      }
      void update_sub2( const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld01. heap -> scal. second, repl ))
         {
            _xxxx -> repr. _fld01. heap = takeshare( replacebywritable( _xxxx -> repr. _fld01. heap ));
            _xxxx -> repr. _fld01. heap -> scal. second = repl;
         }
      }
   };

   mut_arith2 view_arith2( )
   {
      if constexpr( check )
      {
         if( !option_is_arith2( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_arithN( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_mul:
      case sel_add:
         return true;
      default:
         return false;
      }
   }

   struct const_arithN
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_arithN( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      size_t size( ) const { return _xxxx -> repr. _fld02. heap -> size( ); }
      const mathexpr & sub( size_t _iiii ) const
         { return _xxxx -> repr. _fld02. heap -> begin( ) [ _iiii ]; }
   };

   const_arithN view_arithN( ) const
   {
      if constexpr( check )
      {
         if( !option_is_arithN( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_arithN
   {
      mathexpr* _xxxx;
      mut_arithN( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      size_t size( ) const { return _xxxx -> repr. _fld02. heap -> size( ); }
      void push_back( const mathexpr & xx00 ) const
      {
         _xxxx -> repr. _fld02. heap = tvm::push_back( _xxxx -> repr. _fld02. heap, xx00 );
      }
      void pop_back( ) const { _xxxx -> repr. _fld02. heap = tvm::pop_back( _xxxx -> repr. _fld02. heap ); }
      const mathexpr& sub( size_t _iiii ) const
         { return _xxxx -> repr. _fld02. heap -> begin( ) [ _iiii ]; }
      mathexpr extr_sub( size_t _iiii ) const
      {
         if( iswritable( _xxxx -> repr. _fld02. heap ))
            return std::move( _xxxx -> repr. _fld02. heap -> begin( ) [ _iiii ] );
         else
            return _xxxx -> repr. _fld02. heap -> begin( ) [ _iiii ];
      }
      void update_sub( size_t _iiii, const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld02. heap -> begin( ) [ _iiii ], repl ))
         {
            _xxxx -> repr. _fld02. heap = takeshare( replacebywritable( _xxxx -> repr. _fld02. heap ));
            _xxxx -> repr. _fld02. heap -> begin( ) [ _iiii ] = repl;
         }
      }
   };

   mut_arithN view_arithN( )
   {
      if constexpr( check )
      {
         if( !option_is_arithN( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_mathconst( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_e:
      case sel_pi:
         return true;
      default:
         return false;
      }
   }

   struct const_mathconst
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_mathconst( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
   };

   const_mathconst view_mathconst( ) const
   {
      if constexpr( check )
      {
         if( !option_is_mathconst( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_mathconst
   {
      mathexpr* _xxxx;
      mut_mathconst( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }
   };

   mut_mathconst view_mathconst( )
   {
      if constexpr( check )
      {
         if( !option_is_mathconst( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_ratconst( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_rat:
         return true;
      default:
         return false;
      }
   }

   struct const_ratconst
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_ratconst( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }

      const rational & val( ) const { return _xxxx -> repr. _fld04. loc; }
   };

   const_ratconst view_ratconst( ) const
   {
      if constexpr( check )
      {
         if( !option_is_ratconst( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_ratconst
   {
      mathexpr* _xxxx;
      mut_ratconst( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      rational & val( ) const { return _xxxx -> repr. _fld04. loc; }
      rational extr_val( ) const { return std::move( _xxxx -> repr. _fld04. loc ); }
      void update_val( const rational & from ) const { _xxxx -> repr. _fld04. loc = from; }
   };

   mut_ratconst view_ratconst( )
   {
      if constexpr( check )
      {
         if( !option_is_ratconst( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_specfunc1( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_sqrt:
      case sel_exp:
      case sel_cos:
      case sel_ln:
      case sel_sin:
         return true;
      default:
         return false;
      }
   }

   struct const_specfunc1
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_specfunc1( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }

      const mathexpr & sub( ) const { return _xxxx -> repr. _fld00. heap -> scal; }
   };

   const_specfunc1 view_specfunc1( ) const
   {
      if constexpr( check )
      {
         if( !option_is_specfunc1( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_specfunc1
   {
      mathexpr* _xxxx;
      mut_specfunc1( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      const mathexpr & sub( ) const { return _xxxx -> repr. _fld00. heap -> scal; }
      mathexpr extr_sub( ) const {
         if( iswritable( _xxxx -> repr. _fld00. heap ))
            return std::move( _xxxx -> repr. _fld00. heap -> scal );
         else
            return _xxxx -> repr. _fld00. heap -> scal;
      }
      void update_sub( const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld00. heap -> scal, repl ))
         {
            _xxxx -> repr. _fld00. heap = takeshare( replacebywritable( _xxxx -> repr. _fld00. heap ));
            _xxxx -> repr. _fld00. heap -> scal = repl;
         }
      }
   };

   mut_specfunc1 view_specfunc1( )
   {
      if constexpr( check )
      {
         if( !option_is_specfunc1( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_specfunc2( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_pow:
         return true;
      default:
         return false;
      }
   }

   struct const_specfunc2
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_specfunc2( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }

      const mathexpr & sub1( ) const { return _xxxx -> repr. _fld01. heap -> scal. first; }
      const mathexpr & sub2( ) const { return _xxxx -> repr. _fld01. heap -> scal. second; }
   };

   const_specfunc2 view_specfunc2( ) const
   {
      if constexpr( check )
      {
         if( !option_is_specfunc2( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_specfunc2
   {
      mathexpr* _xxxx;
      mut_specfunc2( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      const mathexpr & sub1( ) const { return _xxxx -> repr. _fld01. heap -> scal. first; }
      mathexpr extr_sub1( ) const {
         if( iswritable( _xxxx -> repr. _fld01. heap ))
            return std::move( _xxxx -> repr. _fld01. heap -> scal. first );
         else
            return _xxxx -> repr. _fld01. heap -> scal. first;
      }
      void update_sub1( const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld01. heap -> scal. first, repl ))
         {
            _xxxx -> repr. _fld01. heap = takeshare( replacebywritable( _xxxx -> repr. _fld01. heap ));
            _xxxx -> repr. _fld01. heap -> scal. first = repl;
         }
      }
      const mathexpr & sub2( ) const { return _xxxx -> repr. _fld01. heap -> scal. second; }
      mathexpr extr_sub2( ) const {
         if( iswritable( _xxxx -> repr. _fld01. heap ))
            return std::move( _xxxx -> repr. _fld01. heap -> scal. second );
         else
            return _xxxx -> repr. _fld01. heap -> scal. second;
      }
      void update_sub2( const mathexpr & repl ) const
      {
         if( tvm::distinct( _xxxx -> repr. _fld01. heap -> scal. second, repl ))
         {
            _xxxx -> repr. _fld01. heap = takeshare( replacebywritable( _xxxx -> repr. _fld01. heap ));
            _xxxx -> repr. _fld01. heap -> scal. second = repl;
         }
      }
   };

   mut_specfunc2 view_specfunc2( )
   {
      if constexpr( check )
      {
         if( !option_is_specfunc2( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   
   bool option_is_variable( ) const noexcept
   {
      switch( _ssss )
      {
      case sel_var:
         return true;
      default:
         return false;
      }
   }

   struct const_variable
   {
      const mathexpr* _xxxx;
      const mathexpr & operator * ( ) const { return * _xxxx; }
      const_variable( const mathexpr* _xxxx ) : _xxxx( _xxxx ) { }

      const std::string & name( ) const { return _xxxx -> repr. _fld05. loc; }
   };

   const_variable view_variable( ) const
   {
      if constexpr( check )
      {
         if( !option_is_variable( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }

   struct mut_variable
   {
      mathexpr* _xxxx;
      mut_variable( mathexpr* _xxxx ) : _xxxx( _xxxx ) { }
      const mathexpr & operator * ( ) const { return * _xxxx; }

      std::string & name( ) const { return _xxxx -> repr. _fld05. loc; }
      std::string extr_name( ) const { return std::move( _xxxx -> repr. _fld05. loc ); }
      void update_name( const std::string & from ) const { _xxxx -> repr. _fld05. loc = from; }
   };

   mut_variable view_variable( )
   {
      if constexpr( check )
      {
         if( !option_is_variable( ))
            throw std::invalid_argument( "wrong selector for view" );
      }
      return this;
   }
   


#line 25 "mathexpr.rec"
void print( std::ostream& out ) const;

};


#line 30 "mathexpr.rec"
std::ostream& operator << ( std::ostream& out, const mathexpr& expr );

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


#endif

