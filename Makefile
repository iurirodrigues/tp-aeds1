ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: fluffysharp.exe
	
fluffysharp.exe: fluffysharp.o
	gcc -o fluffysharp.exe fluffysharp.o $(PATH_ALLEGRO)$(LIB_ALLEGRO) -Wl,--subsystem,windows 

fluffysharp.o: fluffysharp.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c fluffysharp.c -Wl,--subsystem,windows 
	
 
clean:
	del fluffysharp.o
	del fluffysharp.exe

