SRC = \
src/file.c \
src/main.c 

NAME = presuff

RM = rm -f

CP = cp -f

DESTDIR = /usr/bin/

SRCMAN = man/

MANPAGE = presuff.1

COMPMAN = presuff.1.gz 

COMPRESS = gzip < $(SRCMAN)$(MANPAGE) > $(SRCMAN)$(COMPMAN)

MANDIR = /usr/share/man/man1/

MANDB = mandb

CC = gcc

BIN_DIR = bin

BUILD_DIR = build

OBJS = $(SRC:src/%.c=$(BUILD_DIR)/%.o)

CFLAGS = -Wall -Wextra -pedantic -g -I include

all: bin $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJS)
	$(CC) -o $(BIN_DIR)/$(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -g $(CFLAGS) -c $< -o $@

bin:
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR)
	$(RM) $(OBJS)

cleanMan:
	$(RM) $(SRCMAN)$(COMPMAN)

fclean: clean cleanMan
	rm -rf $(BIN_DIR)

test: $(BIN_DIR)/$(NAME)
	./test/test.sh

install: $(BIN_DIR)/$(NAME) 
	cp -f -r $(BIN_DIR)/$(NAME) $(DESTDIR)
	$(COMPRESS)
	cp -f -r $(SRCMAN)$(COMPMAN) $(MANDIR)
	$(MANDB)

re: fclean all

uninstall: $(NAME)
	$(RM) $(DESTDIR)$(NAME)
	$(RM) $(MANDIR)$(COMPMAN)
	$(MANDB)

.PHONY: all clean cleanMan fclean install re test uninstall
