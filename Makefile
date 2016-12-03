CF = -Wall --std=c++11 -I PEGTL/ -O2
LF = -Wall --std=c++11 -lSDL2 -lSDL2_image -lportmidi -lsndfile -luv

CXX = g++

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TRG = tt

all: $(TRG)

obj/%.o: src/%.cpp Makefile
	@mkdir -p obj/
	$(CXX) $(CF) $< -c -o $@

$(TRG): $(OBJ) Makefile
	$(CXX) $(OBJ) $(LF) -o $@

ogg:
	ffmpeg -i log.wav -c:a libvorbis -qscale:a 7 log.ogg

clean:
	rm -rf $(TRG) obj/ log.wav log.ogg
