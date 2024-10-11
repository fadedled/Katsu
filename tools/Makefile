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
	@echo "kt-bin2inc: Converts binary file to include as c file."
	@$(CC) bin2inc/*.c -o kt-bin2inc -O2 -Wall

clean:
	@echo "cleaning tools ..."
	@rm -f -d kt-image kt-bin2inc
