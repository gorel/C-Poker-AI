CC		= gcc
LINKER	= gcc -o
CFLAGS	= -Wall -Werror -pedantic -std=gnu99 -Wno-unused-result -O3 -unroll-aggressive -m64
CLIBS	= -lcurl -lm -lpthread
rm		= rm -f

SRCDIR	= src
TESTDIR = test
OBJDIR	= obj
BINDIR	= bin

COMMONDIR 		= $(SRCDIR)/common
CLIENTDIR 		= $(SRCDIR)/client
WINPROBDIR 		= $(SRCDIR)/winprob
TESTCOMMONDIR 	= $(TESTDIR)/common
TESTALLDIR  	= $(TESTDIR)/unit
TESTAIDIR 		= $(TESTDIR)/ai

CLIENT_INCSRC 	= $(COMMONDIR) $(CLIENTDIR)
WINPROB_INCSRC  = $(COMMONDIR) $(WINPROBDIR)
TEST_INCSRC 	= $(COMMONDIR) $(TESTCOMMONDIR)
TESTALL_INCSRC 	= $(COMMONDIR) $(TESTCOMMONDIR) $(TESTALLDIR)
TESTAI_INCSRC 	= $(COMMONDIR) $(TESTCOMMONDIR) $(TESTAIDIR)

CLIENT_INC 		= $(foreach d, $(CLIENT_INCSRC), -I$d)
WINPROB_INC		= $(foreach d, $(WINPROB_INCSRC), -I$d)
TEST_INC 		= $(foreach d, $(TEST_INCSRC), -I$d)
TESTALL_INC 	= $(foreach d, $(TESTALL_INCSRC), -I$d)
TESTAI_INC 		= $(foreach d, $(TESTAI_INCSRC), -I$d)

COMMON_SOURCES 		= $(wildcard $(COMMONDIR)/*.c)
CLIENT_SOURCES 		= $(wildcard $(CLIENTDIR)/*.c)
WINPROB_SOURCES 	= $(wildcard $(WINPROBDIR)/*.c)
TESTCOMMON_SOURCES 	= $(wildcard $(TESTCOMMONDIR)/*.c)
TESTALL_SOURCES 	= $(wildcard $(TESTALLDIR)/*.c)
TESTAI_SOURCES 		= $(wildcard $(TESTAIDIR)/*.c)

COMMON_OBJECTS 		:= $(patsubst $(COMMONDIR)/%.c, $(OBJDIR)/%.o, $(COMMON_SOURCES))
CLIENT_OBJECTS 		:= $(patsubst $(CLIENTDIR)/%.c, $(OBJDIR)/%.o, $(CLIENT_SOURCES))
WINPROB_OBJECTS 	:= $(patsubst $(WINPROBDIR)/%.c, $(OBJDIR)/%.o, $(WINPROB_SOURCES))
TESTCOMMON_OBJECTS 	:= $(patsubst $(TESTCOMMONDIR)/%.c, $(OBJDIR)/%.o, $(TESTCOMMON_SOURCES))
TESTALL_OBJECTS 	:= $(patsubst $(TESTALLDIR)/%.c, $(OBJDIR)/%.o, $(TESTALL_SOURCES))
TESTAI_OBJECTS 		:= $(patsubst $(TESTAIDIR)/%.c, $(OBJDIR)/%.o, $(TESTAI_SOURCES))
OBJECTS 			:= $(wildcard $(OBJDIR)/*.o)

TARGETS 			:= pokerclient winprob testall testai
TARGETS 			:= $(foreach t, $(TARGETS), $(BINDIR)/$t)

all: $(TARGETS)

$(BINDIR)/pokerclient: $(COMMON_OBJECTS) $(CLIENT_OBJECTS)
	@echo -e "\t[link] "$@
	@$(LINKER) $@ $(CFLAGS) $(CLIENT_INC) $(COMMON_OBJECTS) $(CLIENT_OBJECTS) $(CLIBS)
	@echo -e "\n"$@" built successfully.\n"

$(BINDIR)/winprob: $(COMMON_OBJECTS) $(WINPROB_OBJECTS)
	@echo -e "\t[link] "$@
	@$(LINKER) $@ $(CFLAGS) $(WINPROB_INC) $(COMMON_OBJECTS) $(WINPROB_OBJECTS) $(CLIBS)
	@echo -e "\n"$@" built successfully.\n"

$(BINDIR)/testall: $(COMMON_OBJECTS) $(TESTCOMMON_OBJECTS) $(TESTALL_OBJECTS)
	@echo -e "\t[link] "$@
	@$(LINKER) $@ $(CFLAGS) $(TESTALL_INC) $(COMMON_OBJECTS) $(TESTCOMMON_OBJECTS) $(TESTALL_OBJECTS) $(CLIBS)
	@echo -e "\n"$@" built successfully.\n"

$(BINDIR)/testai: $(COMMON_OBJECTS) $(TESTCOMMON_OBJECTS) $(TESTAI_OBJECTS)
	@echo -e "\t[link] "$@
	@$(LINKER) $@ $(CFLAGS) $(TESTAI_INC) $(COMMON_OBJECTS) $(TESTCOMMON_OBJECTS) $(TESTAI_OBJECTS) $(CLIBS)
	@echo -e "\n"$@" built successfully.\n"

$(COMMON_OBJECTS): $(OBJDIR)/%.o : $(COMMONDIR)/%.c
	@echo -e "\t[compile] "$<
	@$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)

$(CLIENT_OBJECTS): $(OBJDIR)/%.o : $(CLIENTDIR)/%.c
	@echo -e "\t[compile] "$<
	@$(CC) $(CFLAGS) $(CLIENT_INC) -c $< -o $@ $(CLIBS)

$(WINPROB_OBJECTS): $(OBJDIR)/%.o : $(WINPROBDIR)/%.c
	@echo -e "\t[compile] "$<
	@$(CC) $(CFLAGS) $(WINPROB_INC) -c $< -o $@ $(CLIBS)

$(TESTCOMMON_OBJECTS): $(OBJDIR)/%.o : $(TESTCOMMONDIR)/%.c
	@echo -e "\t[compile] "$<
	@$(CC) $(CFLAGS) $(TEST_INC) -c $< -o $@ $(CLIBS)

$(TESTALL_OBJECTS): $(OBJDIR)/%.o : $(TESTALLDIR)/%.c
	@echo -e "\t[compile] "$<
	@$(CC) $(CFLAGS) $(TESTALL_INC) -c $< -o $@ $(CLIBS)

$(TESTAI_OBJECTS): $(OBJDIR)/%.o : $(TESTAIDIR)/%.c
	@echo -e "\t[compile] "$<
	@$(CC) $(CFLAGS) $(TESTAI_INC) -c $< -o $@ $(CLIBS)

clean:
	@echo "Removing object files"
	@$(rm) $(OBJECTS)

remove: clean
	@echo "Removing object files and binaries
	@$(rm) $(TARGETS)
