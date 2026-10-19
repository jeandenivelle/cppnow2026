

diff( Expr, Var, 0 ) :- atom( Expr ), Expr \= Var. 
diff( Var, Var, 1 ).

diff( - Expr, Var, - Deriv ) :- diff( Expr, Var, Deriv ).

diff( Expr1 + Expr2, Var, Deriv1 + Deriv2 ) :-
   diff( Expr1, Var, Deriv1 ), diff( Expr2, Var, Deriv2 ).

diff( Expr1 - Expr2, Var, Deriv1 - Deriv2 ) :-
   diff( Expr1, Var, Deriv1 ), diff( Expr2, Var, Deriv2 ).

diff( Expr1 * Expr2, Var, Deriv1 * Expr2 + Expr1 * Deriv2 ) :-
   diff( Expr1, Var, Deriv1 ), 
   diff( Expr2, Var, Deriv2 ).

diff( Expr1 / Expr2, Var, 
      ( Deriv1 * Expr2 - Expr1 * Deriv2 ) / ( Expr2 * Expr2 )) :-
   diff( Expr1, Var, Deriv1 ),
   diff( Expr2, Var, Deriv2 ).

diff( pow(Var,0), Var, 0 ).
   % diff( Var^0, Var ) -> 0.

diff( pow(Var1,R), Var2, 0 ) :-
   atom( Var1 ), atom( Var2 ), Var1 \= Var2, number(R).
      % diff( Var1^N, Var2 ) -> 0 (if Var1 is distinct from Var2.)
 
diff( pow(Var,R), Var, R * pow( Var, R1 )) :-
   number(R), R \= 0, R1 is R - 1, !.

diff( pow( Expr, R ), Var, R * pow( Expr, R1 ) * Deriv ) :-
   number(R), R \= 0, \+ atom( Expr ), R1 is R - 1, 
   diff( Expr, Var, Deriv ).

diff( sin( Expr ), Var, cos( Expr ) * Deriv ) :-
   diff( Expr, Var, Deriv ). 
diff( cos( Expr ), Var, -sin( Expr ) * Deriv ) :- 
   diff( Expr, Var, Deriv ).
diff( exp( Expr ), Var, exp( Expr ) * Deriv ) :-
   diff( Expr, Var, Deriv ).
diff( ln( Expr ), Var, ( 1 / Expr ) * Deriv ) :- 
   diff( Expr, Var, Deriv ).



simplify( X + 0, X ).
simplify( 0 + X, X ).

simplify( 1 * X, X ).
simplify( X * 1, X ).

simplify( 0 * _X, 0 ).
simplify( _X * 0, 0 ).
simplify( - - X, X ).
simplify( - 0 , 0 ).

simplify( -X * Y, - ( X * Y )).
simplify( X * -Y, - ( X * Y )).

simplify( X + ( Y + Z ), X + Y + Z ).
simplify( X * ( Y * Z ), X * Y * Z ).


outermost( Expr, Res ) :- simplify( Expr, Res ), !.

% This handles constants too! (with ExprList = []):

outermost( Expr, Res ) :-
   Expr =.. [ F | ExprList ],
   outermostlist( ExprList, ResList ),
   Res =.. [ F | ResList ].

outermostlist( [], [] ).
outermostlist( [ E1 | R1 ], [ E2 | R2 ] ) :-
   outermost( E1, E2 ),
   outermostlist( R1, R2 ).

% Apply outermost as long as possible:

normalize( X, Z ) :-
   outermost( X, Y ), X \= Y, 
   !, 
   normalize( Y, Z ).

normalize( X, X ). 


