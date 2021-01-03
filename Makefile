BINDIR = /usr/bin
MANDIR = /usr/share/man/man1
CFLAGS = -Wall --std=c11

build:
	$(CC) $(CFLAGS) main.c -o app

install:
	mv app "$(BINDIR)"
	cp app.1 _app.1
	gzip -f _app.1
	mv _app.1.gz "$(MANDIR)/app.1.gz"

uninstall:
	rm "$(BINDIR)/app" "$(MANDIR)/app.1.gz"
