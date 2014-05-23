all:
	g++ -g -Wall main.cc decode.cc thumbsim_driver.cc parse.cc execute.cc -o thumbsim.out

run:
	./thumbsim -i -d -f inputs/fib.sim

clean:
	rm -rf ./*.o ./thumbsim
