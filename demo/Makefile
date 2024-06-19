CC       =	gcc
LD	 = 	ld

TARGET   :=	demo
BUILD    :=	build
INCLUDES :=	../include
LIBDIRS :=	../lib
SOURCES  :=	. res
CFLAGS	 :=	-O2 -Wall
RESLIB :=	res/tiles res/palettes

LIBS     := -lGL -lpthread -lpulse-simple -lX11 -lm -ldl -lkatsu

#All files to compile
CFILES	 :=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
TILEFILES :=	$(foreach dir,$(RESLIB),$(wildcard $(dir)/*.4bpp))
PALFILES :=	$(foreach dir,$(RESLIB),$(wildcard $(dir)/*.pal))

INCLUDE	 :=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(PALFILES:.pal=.o) $(TILEFILES:.4bpp=.o)
OFILES         := $(OFILES_SOURCES)
BINFILES       := $(BMPFILES:.bmp=.bin)

.PHONY: clean

%.o: %.c
	@echo $(notdir $<)
	@$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

%.o: %.4bpp
	@echo "Tile File:" $(notdir $<)
	@$(LD) -r -b binary -o $@ $<

%.o: %.pal
	@echo "Palette File:" $(notdir $<)
	@$(LD) -r -b binary -o $@ $<

$(TARGET): $(OFILES) ../lib/libkatsu.a
	@echo "Generating binary executable:" $@
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) -L$(LIBDIRS) $(LIBS)


clean:
	rm -f $(OFILES) demo
