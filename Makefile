 #
 # BERKE PİTE
 # ENES SOYLU
 # AHMET PEKDEMİR
 # ÜZEYİR EMRE TÜRKMEN
 # HASAN YASİR ARSLAN
 #

all: setup link

setup:
	mkdir -p bin
	mkdir -p lib

link: lib/main.o lib/yonlendirme.o lib/borulama.o lib/komut.o
	gcc lib/main.o lib/yonlendirme.o lib/borulama.o lib/komut.o -o bin/shell

lib/main.o: src/main.c
	gcc -Wall -c -I./include src/main.c -o lib/main.o

lib/yonlendirme.o: src/yonlendirme.c
	gcc -Wall -c -I./include src/yonlendirme.c -o lib/yonlendirme.o

lib/borulama.o: src/borulama.c
	gcc -Wall -c -I./include src/borulama.c -o lib/borulama.o

lib/komut.o: src/komut.c
	gcc -Wall -c -I./include src/komut.c -o lib/komut.o

clean:
	rm -rf bin
	rm -rf lib
