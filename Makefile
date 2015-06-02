 CC = g++
.cpp:
    $(CC) -Wno-deprecated-declarations -framework GLUT -framework OpenGL *.cpp
