CC := clang

CFLAGS := -Wall -Wextra -Wpedantic -Werror
CFLAGS += -O3
CFLAGS += -march=native -mavx -ftree-vectorize 
CFLAGS += -fPIC

LDFLAGS := -lm

SRCDIR := simutil
LIBDIR := lib
BINDIR := bin
OBJDIR := obj

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
LIB = libsimutils.so

TESTDIR := test
TESTSRC := $(wildcard $(TESTDIR)/*.c)
TESTOBJ := $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(TESTSRC))

TARGET = $(LIBDIR)/$(LIB)
TEST := $(BINDIR)/test

.PHONY: all clean debug install uninstall test

all: $(TARGET) | $(LIBDIR)

test: LDFLAGS += -lsimutils
test: $(TEST) | $(BINDIR)

$(TEST): $(TESTOBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

install: $(TARGET) | $(LIBDIR)
	@ sudo cp $(TARGET) /usr/lib/$(LIB);\
	sudo mkdir -p /usr/include/$(SRCDIR);\
	sudo cp $(SRCDIR)/*.h /usr/include/$(SRCDIR);\
	sudo ldconfig

uninstall:
	@ sudo rm -rf /usr/include/$(SRCDIR);\
	sudo rm -f /usr/lib/$(LIB)

$(TARGET): $(OBJ) | $(LIBDIR)
	$(CC) -shared -o $(TARGET) $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR): $(DUMPSDIR)
	mkdir -p $(OBJDIR)

$(LIBDIR):
	mkdir -p $(LIBDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

debug: CFLAGS := $(filter-out -O3, $(CFLAGS)) -g
debug: $(TARGET) | $(LIBDIR)


clean:
	@ echo cleaning directory...;\
    rm -rf $(TARGET) $(LIBDIR) $(OBJDIR) $(BINDIR)
