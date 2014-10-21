#makefile

exe = run
objs = main.cpp

main: $(objs)
	g++ -Wall -o2 -o $(exe) $(objs)

clean:
	rm -f $(exe)

