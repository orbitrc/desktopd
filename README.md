# desktopd

desktopd is a daemon that implements the "desktopd Desktop D-Bus protocol".

To prevent fragmentation of the Linux desktop, desktop environment developers
are encouraged to implement this protocol.

However, as of now, this protocol is incomplete. Therefore, desktopd has its
own implementation of this protocol.

## desktopd Desktop D-Bus Protocol

TODO

## How It Works?

desktopd runs as a daemon and registers a service on the D-Bus session bus.
The name is 'io.orbitrc.Desktop'.

Users read properties, execute methods, or receive change signals through
the command-line interface or a configuration application.

desktopd runs DE-specific scripts for those actions.

For example, when the user executes the
`io.orbitrc.Desktop.Desktop.insertDesktop` method, the
`~/.local/share/desktopd/scripts-enabled/insertDesktop` script is executed.
It is a symbolic link to the
`/usr/share/desktopd/scripts/kde-insertDesktop` script if you are using
KDE Plasma desktop environment.

## desktop.conf

The `desktop.conf` file is a configuration file that is not dependent on
specific DE. It is in ini format.
It is located within the `~/.config` directory.

## License

desktopd is developed under the MIT license. For details, see the LICENSE file.
