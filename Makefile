Files = ./src/*.cpp
Include = -I./include
Libs =
Version = --std=c++20
Flags = -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -ggdb  

build:
	g++ -o main ${Flags} ${Version} ${Libs} ${Include} ${Files} ${flags}

run:
	g++ -o main ${Version} ${Libs} ${Include} ${Files} ${flags}
	@./main
	@sleep 1
	@rm ./main

debug:
	g++ -o main ${Flags} ${Version} ${Libs} ${Include} ${Files} ${flags} --debug

