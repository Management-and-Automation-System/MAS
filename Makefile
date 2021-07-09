CC=clang++

bin/classes.a : classes.cpp classes.h
	$(CC) -c -g -o $@ $<
bin/revenue.a : revenue.cpp revenue.h bin/classes.a
	$(CC) -c -g -o $@ $< 
bin/ams.out : ams.cpp bin/classes.a bin/revenue.a
	$(CC) -g -o  $@ $^
all : bin/ams.out