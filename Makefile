PKG_CONFIG=`pkg-config --cflags --libs dbus-1`

default:
	./gen-introspections.py
	mkdir -p bin
	$(CC) -O2 src/desktopd.c src/interfaces.c src/desktopd-server.c $(PKG_CONFIG) -o bin/desktopd

dev:
	DESKTOPD_SCRIPTS_ENABLED_DIR=./usr/share/desktopd/scripts-enabled bin/desktopd

install:
	rm -rf /usr/share/desktopd
	mkdir -p /usr/share/desktopd
	cp -r scripts /usr/share/desktopd/
