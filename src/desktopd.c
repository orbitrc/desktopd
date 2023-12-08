#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dbus/dbus.h>

#include "introspections.h"
#include "interfaces.h"
#include "desktopd-server.h"

const char *const SERVER_BUS_NAME = "io.orbitrc.Desktop";

const char *const DESKTOP_APPEARANCE_INTROSPECTION =
    DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
    "<node>\n"
    "  <interface name=\"io.orbitrc.Desktop.Appearance\">\n"
    "    <signal name=\"colorSchemeChanged\" />\n"
    "    <property name=\"colorScheme\" access=\"read\" type=\"s\" />\n"
    "    <method name=\"setColorScheme\">\n"
    "      <arg name=\"value\" direction=\"in\" type=\"s\" />"
    "    </method>\n"
    "  </interface>\n"
    "</node>";


const char *const DESKTOP_KEYBOARD_INTROSPECTION =
    DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
    "<node>\n"
    "  <interface name=\"io.orbitrc.Desktop.Keyboard\">\n"
    "    <signal name=\"delayUntilRepeatChanged\" />\n"
    "    <property name=\"delayUntilRepeat\" access=\"read\" type=\"i\" />\n"
    "    <method name=\"setDelayUntilRepeat\">\n"
    "      <arg name=\"value\" direction=\"in\" type=\"i\" />\n"
    "    </method>\n"
    ""
    "    <signal name=\"keyRepeatChanged\" />\n"
    "    <property name=\"keyRepeat\" access=\"read\" type=\"i\" />\n"
    "    <method name=\"setKeyRepeat\">\n"
    "      <arg name=\"value\" direction=\"in\" type\"i\" />\n"
    "    </method>\n"
    "  </interface>\n"
    "</node>";

const char *DESKTOP_INTERFACE = "io.orbitrc.Desktop.Desktop";
const char *APPEARANCE_INTERFACE = "io.orbitrc.Desktop.Appearance";

void print_version()
{
    printf("desktopd version 0.1.0\n");
}

static void introspect_handler(desktopd_server *server,
                               DBusMessage *message,
                               const char *path)
{
    fprintf(stderr, "introspect_handler - path: %s\n", path);
    if (strcmp(path, "/") == 0) {
        desktopd_server_send_introspection(server, message,
            SERVER_INTROSPECTION);
    } else if (strcmp(path, "/io") == 0) {
        desktopd_server_send_introspection(server, message,
            IO_INTROSPECTION);
    } else if (strcmp(path, "/io/orbitrc") == 0) {
        desktopd_server_send_introspection(server, message,
            ORBITRC_INTROSPECTION);
    } else if (strcmp(path, "/io/orbitrc/Desktop") == 0) {
        desktopd_server_send_introspection(server, message,
            DESKTOP_INTROSPECTION);
    } else if (strcmp(path, "/io/orbitrc/Desktop/Appearance") == 0) {
        desktopd_server_send_introspection(server, message,
            DESKTOP_APPEARANCE_INTROSPECTION);
    } else if (strcmp(path, "/io/orbitrc/Desktop/Desktop") == 0) {
        desktopd_server_send_introspection(server, message,
            DESKTOP_DESKTOP_INTROSPECTION);
    } else if (strcmp(path, "/io/orbitrc/Desktop/Keyboard") == 0) {
        desktopd_server_send_introspection(server, message,
            DESKTOP_KEYBOARD_INTROSPECTION);
    } else {
        desktopd_server_send_introspection(server, message,
            SERVER_INTROSPECTION);
    }
}

static void property_handler(desktopd_server *server,
                             DBusMessage *message,
                             const char *interface,
                             const char *name)
{
    if (strcmp(interface, DESKTOP_INTERFACE) == 0) {
        if (strcmp(name, "NumberOfDesktops") == 0) {
            DBusMessage *ret;
            ret = dbus_message_new_method_return(message);

            int val = desktop_property_number_of_desktops();
            dbus_message_append_args(ret,
                DBUS_TYPE_INT32, &val,
                DBUS_TYPE_INVALID);
            dbus_connection_send(server->connection, ret, NULL);
            dbus_connection_flush(server->connection);

            dbus_message_unref(ret);
        } else if (strcmp(name, "CurrentDesktop") == 0) {
            DBusMessage *ret;
            DBusMessageIter iter;
            DBusMessageIter variant;
            ret = dbus_message_new_method_return(message);

            int val = desktop_property_current_desktop();

            dbus_message_iter_init_append(ret, &iter);
            dbus_message_iter_open_container(&iter, DBUS_TYPE_VARIANT,
                "i", &variant);
            dbus_message_iter_append_basic(&variant, DBUS_TYPE_INT32, &val);
            dbus_message_iter_close_container(&iter, &variant);

            /*
            dbus_message_append_args(ret,
                DBUS_TYPE_VARIANT, &variant,
                DBUS_TYPE_INVALID);
            */
            dbus_connection_send(server->connection, ret, NULL);
            dbus_connection_flush(server->connection);

            dbus_message_unref(ret);
        }
    }
}

int main(int argc, char *argv[])
{
    // Version.
    if (argc >= 2) {
        if (strcmp(argv[1], "--version") == 0) {
            print_version();
            return 0;
        }
    }

    desktopd_server server;
    desktopd_server_connect(&server, SERVER_BUS_NAME);
    server.introspect = introspect_handler;
    server.property = property_handler;
    desktopd_server_run(&server);

    DBusConnection *conn;
    DBusError dbus_error;

    dbus_error_init(&dbus_error);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error);

    if (dbus_error_is_set(&dbus_error)) {
        //
    }

    if (!conn) {
        fprintf(stderr, "D-Bus bus get error.\n");
        return 1;
    }

    // Request name.
    dbus_bus_request_name(conn, SERVER_BUS_NAME,
        DBUS_NAME_FLAG_DO_NOT_QUEUE, &dbus_error);

    // Main loop.
    while (dbus_connection_read_write_dispatch(conn, -1)) {
        //
        DBusMessage *message;

        message = dbus_connection_pop_message(conn);
        if (message == NULL) {
            return 1;
        }

        if (dbus_message_is_method_call(message,
                DBUS_INTERFACE_PROPERTIES, "Get")) {
            fprintf(stderr, "Hit!\n");
            DBusMessage *ret;

            ret = dbus_message_new_method_return(message);

            const char *interface;
            const char *property;

            dbus_message_get_args(message, &dbus_error,
                DBUS_TYPE_STRING, &interface,
                DBUS_TYPE_STRING, &property,
                DBUS_TYPE_INVALID);
            fprintf(stderr, " - Property: %s\n", property);

            //================
            // Get Property
            //================
            if (strcmp(property, "numberOfDesktops") == 0) {
                //
                int val = desktop_property_number_of_desktops();
                dbus_message_append_args(ret,
                    DBUS_TYPE_INT32, &val,
                    DBUS_TYPE_INVALID);
                dbus_connection_send(conn, ret, NULL);
                dbus_connection_flush(conn);

                dbus_message_unref(ret);
            } else if (strcmp(property, "colorScheme") == 0) {
                char *val = (char*)malloc(6);
                val[0] = 'l';
                val[1] = 'i';
                val[2] = 'g';
                val[3] = 'h';
                val[4] = 't';
                val[5] = '\0';
                dbus_message_append_args(ret,
                    DBUS_TYPE_STRING, &val,
                    DBUS_TYPE_INVALID);
                dbus_connection_send(conn, ret, NULL);
                dbus_connection_flush(conn);

                dbus_message_unref(ret);
                free(val);
            }
        } else if (dbus_message_is_method_call(message,
                "io.orbitrc.Desktop.Desktop", "setNumberOfDesktops")) {
            fprintf(stderr, "Method call: setNumberOfDesktops\n");
            int val;
            dbus_message_get_args(message, &dbus_error,
                DBUS_TYPE_INT32, &val,
                DBUS_TYPE_INVALID);
            if (dbus_error_is_set(&dbus_error)) {
                fprintf(stderr, " - Method call error!\n");
                continue;
            }
            fprintf(stderr, "%d\n", val);
            //===============
            // Send Signal
            //===============
            dbus_uint32_t serial = 0;
            DBusMessage *signal;

            signal = dbus_message_new_signal("/Desktop",
                "io.orbitrc.Desktop.Desktop",
                "numberOfDesktopsChanged");
            dbus_connection_send(conn, signal, &serial);
            dbus_connection_flush(conn);

            dbus_message_unref(message);
        } else {
            const char *iface = dbus_message_get_interface(message);
            const char *path = dbus_message_get_path(message);
            const char *member = dbus_message_get_member(message);
            fprintf(stderr, "-Else. Interface: %s, Path: %s, Member: %s\n",
                iface, path, member);
        }
    }

    return 0;
}
