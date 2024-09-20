Files = ./src/*.cpp
Include = -I./include/ttorrent -I./include/asio
Libs =
Version = --std=c++20
Flags = -Wall -Weffc++ -Wextra -ggdb # -Wconversion -Wsign-conversion -Werror  

build:
	g++ -o main ${Flags} ${Version} ${Libs} ${Include} ${Files} ${flags}

run:
	g++ -o main ${Version} ${Libs} ${Include} ${Files} ${flags}
	@./main
	@sleep 1
	@rm ./main

debug:
	g++ -o main ${Flags} ${Version} ${Libs} ${Include} ${Files} ${flags} --debug

