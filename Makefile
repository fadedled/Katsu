#Makefile for compiling the Katsu library
#Compiler used:
CC       =	gcc

#Directroies
LIBDIR   :=	lib
BUILD    :=	build
INCLUDES :=	include
SOURCES  :=	src src/platform/linux src/opengl
CFLAGS	 :=	-O2 -Wall -fPIC #-DKT_DEBUG
GLSLDIR  := 	src/opengl/shaders

#All files to compile
DIRS     := $(LIBDIR) $(BUILD)
CFILES	 := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
GLSLFILES := $(wildcard $(GLSLDIR)/*.glsl)
INCLUDE	 := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

GLSLINCS := $(GLSLFILES:.glsl=.inc)
OBJS     := $(CFILES:.c=.o)

.PHONY: clean static dynamic $(DIRS)

#Targets
static: $(DIRS) $(GLSLINCS) $(OBJS)
	@echo "Generating static library libkatsu.a"
	@ar rcs $(LIBDIR)/libkatsu.a $(foreach obj, $(notdir $(OBJS)), $(BUILD)/$(obj)) 

dynamic: static
	@echo "Generating dynamic library libkatsu.so"
	@$(CC) -shared -o  $(LIBDIR)/libkatsu.so $(LIBDIR)/libkatsu.a $(CFLAGS) $(INCLUDE)

$(DIRS):
	@mkdir -p $@

%.inc: %.glsl
	@echo $(notdir $<)
	@tools/kt-bin2inc -nc $<

%.o: %.c 
	@echo $(notdir $<)
	@$(CC) -c -o $(BUILD)/$(notdir $(<:.c=.o)) $< $(CFLAGS) $(INCLUDE)

clean:
	@echo "clean ..."
	@rm -f -d $(BUILD)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so $(GLSLDIR)/*.inc $(DIRS)
