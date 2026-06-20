
This is an improved implementation of mathexpr,
created by hand. If I am still satisfied with it in a couple
of weeks, I will modify TreeGen to generate this code automatically.

There are two main differences:
- In the 2023 version, the fields of the union in mathexpr.h 
  were defined by the constructors 
  (i.e. having the same sequence of subtypes).
  This puts e.g. arith1 and specfunc1 in the same field. 
  As a consequence, the fields didn't correspond to the options.
  Because of that, separate view objects were necessary.
  In the new version, the fields of the union are exactly the options
  of the tree type, so that there is no need for separare view
  types. They can be just pointers to the fields. 
  This puts some extra work in the constructors, but the total result
  is much nicer. 
- I improved the implementations of scalar and scalar_vector. 
  They now now almost have RAII. I say 'almost' because the user has
  to be do the reference counting. 

