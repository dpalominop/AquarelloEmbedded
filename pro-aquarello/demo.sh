#!/bin/sh
export TSLIB_TSDEVICE=/dev/input/ts
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_PLUGINDIR=/lib/ts
export TSLIB_CONSOLEDEVICE=none
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TSLIB_ROOT/lib
export QWS_MOUSE_PROTO="tslib:/dev/input/ts"
export QWS_KEYBOARD_PROTO="/dev/inputt/event0"
export QT_QWS_FONTDIR=/lib/fonts
export QT_PLUGIN_PATH=/lib/plugins
mknod /dev/input/ts c 11 0
./aquarello -qws 
