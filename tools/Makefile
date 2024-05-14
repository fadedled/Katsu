#Makefile for compiling Katsu tools
#Compiler used:
CC       =	gcc

#Directroies
BUILD    :=	bin

.PHONY: clean static dynamic $(DIRS)

#Targets
all: 
	@echo "Builing Katsu tools"
	@echo "kt-image: image converter to Katsu format."
	@$(CC) ktimg/*.c -o kt-image -O2 -lm -Wall
	@echo "kt-text2inc: Converts textfile to include"
	@$(CC) text2inc/*.c -o kt-text2inc -O2 -Wall

clean:
	@echo "cleaning tools ..."
	@rm -f -d kt-image kt-text2inc
