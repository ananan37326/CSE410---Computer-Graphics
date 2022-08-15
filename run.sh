g++ -Wall -g -I"C:\MinGW\include" -c $1 -o $1.o
g++ -L"C:\MinGW\lib" -o $1.exe $1.o -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32
