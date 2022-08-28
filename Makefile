OBJS = main.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = +Snake+

#This is the target that compiles our executable
all : $(OBJS)
	g++ main.cpp Game.cpp Game.hpp Gameobject.cpp Gameobject.hpp TextBox.hpp TextBox.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -o $(OBJ_NAME)
