

# Automatically generated Makefile
# Makefile-generator created by Hans de Nivelle, 2023

# differentiation

CPP = g++

Source = .
Treegen = /home/nivelle/programs/treegen2023
Tvm = /home/nivelle/programs
Util = /home/nivelle/programs

Libs =

Options = -Wall -Wno-switch -Wno-invalid-offsetof -Wno-unused-function -pedantic-errors -std=c++23

# Linking:

$(Source)/diff : $(Source)/diff.o $(Source)/mathexpr.o $(Source)/print.o $(Source)/selector.o $(Source)/rational.o $(Util)/util/bigint.o
	$(CPP) $(Options) -o $(Source)/diff $(Source)/diff.o $(Source)/mathexpr.o $(Source)/print.o $(Source)/selector.o $(Source)/rational.o $(Util)/util/bigint.o $(Libs)

# Preprocessors:

$(Source)/selector.h $(Source)/selector.cpp $(Source)/mathexpr.h $(Source)/mathexpr.cpp : $(Source)/mathexpr.rec
	$(Treegen)/treegen mathexpr.rec

# Compiling the source files:

$(Source)/diff.o : $(Source)/diff.cpp $(Source)/concat.h $(Source)/mathexpr.h $(Source)/outermost.h $(Source)/rational.h $(Source)/selector.h $(Source)/skeletons.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/bigint.h
	$(CPP) -c $(Options) $(Source)/diff.cpp -o $(Source)/diff.o -I $(Util)

$(Source)/mathexpr.o : $(Source)/mathexpr.cpp $(Source)/mathexpr.h $(Source)/rational.h $(Source)/selector.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/bigint.h
	$(CPP) -c $(Options) $(Source)/mathexpr.cpp -o $(Source)/mathexpr.o -I $(Util)

$(Source)/print.o : $(Source)/print.cpp $(Source)/mathexpr.h $(Source)/rational.h $(Source)/selector.h $(Util)/tvm/concepts.h $(Util)/tvm/distinct.h $(Util)/tvm/field.h $(Util)/tvm/hexprint.h $(Util)/tvm/includes.h $(Util)/tvm/local.h $(Util)/tvm/scalar.h $(Util)/tvm/scalar_list.h $(Util)/tvm/scalar_vector.h $(Util)/tvm/unit.h $(Util)/util/bigint.h
	$(CPP) -c $(Options) $(Source)/print.cpp -o $(Source)/print.o -I $(Util)

$(Source)/selector.o : $(Source)/selector.cpp $(Source)/selector.h
	$(CPP) -c $(Options) $(Source)/selector.cpp -o $(Source)/selector.o -I $(Util)

$(Source)/rational.o : $(Source)/rational.cpp $(Source)/rational.h $(Util)/util/bigint.h
	$(CPP) -c $(Options) $(Source)/rational.cpp -o $(Source)/rational.o -I $(Util)

$(Util)/util/bigint.o : $(Util)/util/bigint.cpp $(Util)/util/bigint.h
	$(CPP) -c $(Options) $(Util)/util/bigint.cpp -o $(Util)/util/bigint.o -I $(Util)

