CC       =	gcc
LD	 = 	ld

TARGET   :=	demo
BUILD    :=	build
INCLUDES :=	../include
LIBDIRS :=	../lib
SOURCES  :=	. res
CFLAGS	 :=	-O2 -Wall `pkg-config --cflags libpipewire-0.3`

LIBS     := -lGL -lpthread `pkg-config --libs libpipewire-0.3` -lX11 -lm -ldl -lkatsu

#All files to compile
CFILES	 :=	$(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
TILEFILES :=	$(foreach dir, res/tiles,$(wildcard $(dir)/*.png))
PALFILES :=	$(foreach dir, res/palettes,$(wildcard $(dir)/*.png))

INCLUDE	 :=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(TILEFILES:.png=.o)
OFILES         := $(OFILES_SOURCES)
BINFILES       := $(BMPFILES:.bmp=.bin)

.PHONY: clean

%.o: %.c
	@echo $(notdir $<)
	@$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

%.c: %.png
	@echo "Tile File:" $(notdir $<)
	@../tools/kt-image -c -palexp -p res/palettes/$(notdir $(basename $<))_pal.png $<

$(TARGET): $(OFILES) ../lib/libkatsu.a
	@echo "Generating binary executable:" $@
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) -L$(LIBDIRS) $(LIBS)


clean:
	rm -f $(OFILES) demo
