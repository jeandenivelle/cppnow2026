
This is an improved implementation of mathexpr,
created by hand. If I am still satisfied with it in a couple
of weeks, I will modify TreeGen to generate this code automatically.

There are two main differences:
- options of the struct repr are now the options of the tree type.
  Because of this, there is no need for separate view types,
  one can just use pointers to the options.
- I improved the implementations of scalar and scalar_vector. 
  They now now almost have RAII. I say 'almost' because the user has
  to be do the reference counting. 

