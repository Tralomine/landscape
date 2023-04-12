CC := g++

SFML := -lsfml-graphics -lsfml-window -lsfml-system

LIB := $(SFML)
#LIB += json.o

FLAGS := -std=c++17 -Wall

OBJ := main.o noise.o tree.o json.o

all: $(OBJ)
	${CC} ${OBJ} -o landscape ${LIB} ${FLAGS}

%.o: %.cpp
	${CC} -c $< -o $@ ${FLAGS}

clean:
	rm landscape *.o


INCLUDES := -I"E:\SFML-2.5.0\include" -I"E:\json\include"
LINKS := -L"E:\SFML-2.5.0\lib" -L"E:\json\lib"

WSFML_SYSTEM := -lsfml-system-s -lwinmm
WSFML_WINDOW := -lsfml-window-s -lopengl32 -lwinmm -lgdi32
WSFML_GRAPHICS := -lsfml-graphics-s -lfreetype -lopengl32

WSFML := -DSFML_STATIC $(WSFML_GRAPHICS) $(WSFML_WINDOW) $(WSFML_SYSTEM)

WLIB := $(WSFML)
WLIB += -ljson

windows: main.o noise.o tree.o
	$(CC) main.o noise.o tree.o -o landscape $(WLIB) $(FLAGS) $(LINKS)
