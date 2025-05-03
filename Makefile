SRC =	src/file.c \
	src/main.c 

CFLAGS += -I./include

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

OBJS=	$(SRC:.c=.o)

CFLAGS = -Wall -Wextra -pedantic -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -g -o $(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

cleanMan:
	$(RM) $(SRCMAN)$(COMPMAN)

fclean: clean cleanMan
	$(RM) $(NAME)

install: $(NAME) 
	$(CP) $(NAME) $(DESTDIR)
	$(COMPRESS)
	$(CP) $(SRCMAN)$(COMPMAN) $(MANDIR)
	$(MANDB)

re: fclean all

uninstall: $(NAME)
	$(RM) $(DESTDIR)$(NAME)
	$(RM) $(MANDIR)$(COMPMAN)
	$(MANDB)

.PHONY: all clean fclean install re uninstall
