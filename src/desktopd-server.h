#ifndef _DESKTOPD_DBUS_SERVER_H
#define _DESKTOPD_DBUS_SERVER_H

#include <stdint.h>
#include <stdbool.h>

#include <dbus/dbus.h>

typedef struct desktopd_server desktopd_server;

typedef struct desktopd_server {
    DBusConnection *connection;
    void(*introspect)(desktopd_server *server,
                      DBusMessage *message,
                      const char *path);
    void(*property)(desktopd_server *server,
                    DBusMessage *message,
                    const char *interface,
                    const char *name);
} desktopd_server;

bool desktopd_server_connect(desktopd_server *server, const char *name);

void desktopd_server_send_introspection(desktopd_server *server,
                                        DBusMessage *message,
                                        const char *instrospection);

void desktopd_server_run(desktopd_server *server);

#endif /* _DESKTOPD_DBUS_SERVER_H */
