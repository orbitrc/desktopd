#include "desktopd-server.h"

#include <stdio.h>

bool desktopd_server_connect(desktopd_server *server, const char *name)
{
    // Initialize.
    server->introspect = NULL;
    server->property = NULL;

    DBusError err;

    dbus_error_init(&err);

    server->connection = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (server->connection == NULL || dbus_error_is_set(&err)) {
        return false;
    }

    dbus_bus_request_name(server->connection, name,
        DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);
    if (dbus_error_is_set(&err)) {
        return false;
    }

    return true;
}

void desktopd_server_send_introspection(desktopd_server *server,
                                        DBusMessage *message,
                                        const char *introspection)
{
    DBusMessage *ret;

    ret = dbus_message_new_method_return(message);

    dbus_message_append_args(ret,
        DBUS_TYPE_STRING, &introspection,
        DBUS_TYPE_INVALID);
    dbus_connection_send(server->connection, ret, NULL);
    dbus_connection_flush(server->connection);

    dbus_message_unref(ret);
}

void desktopd_server_run(desktopd_server *server)
{
    DBusMessage *message;

    while (dbus_connection_read_write_dispatch(server->connection, -1)) {
        message = dbus_connection_pop_message(server->connection);
        if (message == NULL) {
            continue;
        }

        if (dbus_message_is_method_call(message,
            DBUS_INTERFACE_INTROSPECTABLE, "Introspect")) {
            const char *path = dbus_message_get_path(message);
            if (server->introspect != NULL) {
                server->introspect(server, message, path);
            }
        } else if (dbus_message_is_method_call(message,
            DBUS_INTERFACE_PROPERTIES, "Get")) {
            DBusError err;
            dbus_error_init(&err);

            const char *interface;
            const char *property;

            dbus_message_get_args(message, &err,
                                  DBUS_TYPE_STRING, &interface,
                                  DBUS_TYPE_STRING, &property,
                                  DBUS_TYPE_INVALID);
            fprintf(stderr, " - Interface: %s\n", interface);
            fprintf(stderr, " - Property: %s\n", property);

            if (server->property != NULL) {
                server->property(server, message, interface, property);
            }
        }

        dbus_message_unref(message);
    }
}
