ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: ghero-0.exe
	
ghero-0.exe: ghero-0.o
	gcc -o ghero-0.exe ghero-0.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

ghero-0.o: ghero-0.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ghero-0.c 
	
 
clean:
	del ghero-0.o
	del ghero-0.exe

