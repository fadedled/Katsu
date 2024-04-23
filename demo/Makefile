CC       =	gcc

TARGET   :=	demo
BUILD    :=	build
INCLUDES :=	../include
LIBDIRS :=	../lib
SOURCES  :=	.
CFLAGS	 :=	-O2 -Wall


LIBS     := -lGL -lpthread -lpulse-simple -lX11 -ldl -lkatsu

#All files to compile
CFILES	 :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

INCLUDE	 :=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir))

OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o)
OFILES         := $(OFILES_SOURCES)

.PHONY: clean

%.o: %.c
	@echo $(notdir $<)
	@$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)


$(TARGET): $(OFILES)
	@echo "Generating binary executable:" $@
	$(CC) -o $@ $< $(CFLAGS) $(INCLUDE) -L$(LIBDIRS) $(LIBS)


clean:
	rm -f *.o demo
