bin/classes.a : classes.cpp classes.hpp 
	g++ -c -o $@ $<
bin/revenue.a : revenue.cpp revenue.hpp bin/classes.a
	g++ -c -o $@ $<
bin/ams.out : ams.cpp bin/revenue.a bin/classes.a
	g++ -o $@ $^
all : bin/ams.out