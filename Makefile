bin/classes.a : classes.cpp classes.h
	g++ -c -g -o $@ $<
bin/revenue.a : revenue.cpp revenue.h bin/classes.a
	g++ -c -g -o $@ $< 
bin/ams.out : ams.cpp bin/classes.a bin/revenue.a
	g++ -g -o  $@ $^
all : bin/ams.out