TARGET	= pokerclient
CC		= gcc
LINKER	= gcc -o
CFLAGS	= -Wall -Werror -pedantic -std=gnu99 -g -O3 -Wno-unused-result
CLIBS	= -lcurl -lm -lpthread

SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin

SOURCES		:= $(wildcard $(SRCDIR)/*.c)
INCLUDES	:= $(wildcard $(SRCDIR)/*.h)
OBJECTS		:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm			= rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $@ $(CFLAGS) $(OBJECTS) $(CLIBS)
	@echo -e "\n"$(TARGET)" built successfully.\n"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo -e "\tCompiling "$<
	@$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

clean:
	@echo "Removing object files"
	@$(rm) $(OBJECTS)

remove: clean
	@echo "Removing object files and binaries
	@$(rm) $(BINDIR)/$(TARGET)
