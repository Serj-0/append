BINDIR = /usr/bin
MANDIR = /usr/share/man/man1
CFLAGS = -Wall --std=c11

build:
	$(CC) $(CFLAGS) main.c -o app

install:
	mv app "$(BINDIR)"
	gzip -f -k app.1
	mv app.1.gz "$(MANDIR)"

uninstall:
	rm "$(BINDIR)/app" "$(MANDIR)/app.1.gz"
