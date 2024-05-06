CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Werror
CFLAGS += -O3 
CFLAGS += -fPIC

LDFLAGS = -lm

SRCDIR = simutil
LIBDIR = lib
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
LIBRARY = libsimutils.so
TARGET = $(LIBDIR)/$(LIBRARY)

.PHONY: all clean debug install uninstall

all: $(TARGET) | $(LIBDIR)

install: $(TARGET) | $(LIBDIR)
	@ sudo cp $(TARGET) /usr/lib/$(LIBRARY);\
	sudo mkdir -p /usr/include/$(SRCDIR);\
	sudo cp $(SRCDIR)/*.h /usr/include/$(SRCDIR);\
	sudo ldconfig

uninstall:
	@ sudo rm -rf /usr/include/$(SRCDIR);\
	sudo rm -f /usr/lib/$(LIBRARY)

$(TARGET): $(OBJ) | $(LIBDIR)
	$(CC) -shared -o $(TARGET) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR): $(DUMPSDIR)
	mkdir -p $(OBJDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

debug: CFLAGS := $(filter-out -O3, $(CFLAGS)) -g
debug: $(TARGET) | $(LIBDIR)


clean:
	@ echo cleaning directory...;\
    rm -rf $(TARGET) $(LIBDIR) $(OBJDIR) *.mat *.vec
