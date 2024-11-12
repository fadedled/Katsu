#Makefile for compiling the Katsu library
#Compiler used:
CC       =	gcc

#Directroies
LIBDIR   :=	lib
BUILD    :=	build
INCLUDES :=	include 
SOURCES  :=	src src/platform/linux src/opengl
CFLAGS	 :=	-O2 -Wall -fPIC -I/usr/include/pipewire-0.3 -I/usr/include/spa-0.2 -D_REENTRANT
GLSLDIR  := 	src/opengl/shaders
BIN2INC	 := 	tools/kt-bin2inc

#All files to compile
DIRS     := $(LIBDIR) $(BUILD)
CFILES	 := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
GLSLFILES := $(wildcard $(GLSLDIR)/*.glsl)
INCLUDE	 := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

GLSLINCS := $(GLSLFILES:.glsl=.inc)
OBJS     := $(CFILES:.c=.o)

ifeq ($(KT_DEBUG), 1)
	CFLAGS += -DKT_DEBUG -g
endif

.PHONY: clean static dynamic $(DIRS)

#Targets
static: $(BIN2INC) $(DIRS) $(GLSLINCS) $(OBJS)
	@echo "Generating static library libkatsu.a"
	@ar rcs $(LIBDIR)/libkatsu.a $(foreach obj, $(notdir $(OBJS)), $(BUILD)/$(obj)) 

dynamic: static
	@echo "Generating dynamic library libkatsu.so"
	@$(CC) -shared -o  $(LIBDIR)/libkatsu.so $(LIBDIR)/libkatsu.a $(CFLAGS) $(INCLUDE)

$(BIN2INC):
	$(MAKE) -C tools 
	
$(DIRS):
	@mkdir -p $@

%.inc: %.glsl
	@echo $(notdir $<)
	@$(BIN2INC) -nc $<

%.o: %.c 
	@echo $(notdir $<)
	@$(CC) -c -o $(BUILD)/$(notdir $(<:.c=.o)) $< $(CFLAGS) $(INCLUDE)

clean:
	@echo "clean ..."
	@rm -f -d $(BUILD)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so $(GLSLDIR)/*.inc $(DIRS)
	@$(MAKE) -C tools clean
	
docs:	Doxyfile include/katsu
	@echo "Generating docs..."
	doxygen Doxyfile
