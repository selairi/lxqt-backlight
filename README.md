# lxqt-backlight

This is a library and tool in order to change screen backlight in Linux systems. 
It uses /sys/class/backlight and Polkit.


The library is written in C and it has a C++ wrapper for Qt5.


# The C library

The C library has got 4 functions:


    int lxqt_backlight_backend_get();
    int lxqt_backlight_backend_get_max();
    FILE *lxqt_backlight_backend_get_write_stream();
    char *lxqt_backlight_backend_get_driver();


This is an example:


    #include <lxqtbacklight_backend.h>
    
    ...
    
    // Get maximun value of backlight
    int max_backlight = lxqt_backlight_backend_get_max();
    if(max_backlight<0)
        return; // Backlight can not be controlled.
    // Get actual backlight level
    int value = lxqt_backlight_backend_get();
    // Increases actual backlight level in 5 units
    FILE *fout = lxqt_backlight_backend_write_stream();
    fprintf(fout, "%d\n", value+5);
    fflush(fout);
    // ... Do something ...
    fclose(fout);

The "lxqt_backlight_backend_get_driver" function returns the backlight-control directory in /sys/class/backlight.

# lxqtbacklight_backend: The command line tool

This is a simple command line tool to read and change backlight level.

    lxqtbacklight_backend [backlight-level [ % ]] [--help]

    --help             Shows this message.

    --show             Shows actual brightness level.

    --inc              Increases actual brightness level.

    --dec              Decreases actual brightness level.

    --stdin            Read backlight value from stdin.

    backlight-level    Sets backlight

    backlight-level %  Sets backlight from 1% to 100%


Examples:

Sets backlight level to 10%:

    pkexec lxqtbacklight_backend 10 %       

Sets backlight level to 10:

    pkexec lxqtbacklight_backend 10         


# The C++ library

It is a simple C++ wrapper of C library. This C++ library requires Qt5.

It defines LXQtBacklight class. This class contains the following important methods:


    #include <lxqtbacklight.h>
    class LXQtBacklight:public QObject
    {
        public:    
            LXQtBacklight(QObject *parent = 0);
            ~LXQtBacklight();
            bool isBacklightAvailable();
            void setBacklight(int value);
            int getBacklight();
            int getMaxBacklight();
        signals:
            void backlightChanged(int value);
    }
    
The backlightChanged signal is emited when backlight is changed. backlightChanged signal will detect if another tool has changed backlight level.

# lxqt-backlight-gui

It is a simple example of GUI to change the backlight level. It uses the LXQtBacklight class.


# About implementation

This library uses Linux /sys/class/backlight files to read and change the 
backlight level.


If screen backlight can be controlled, the Linux kernel will show inside 
/sys/class/backlight directory one or more directories. Each directory has
got the following files:
    
    /sys/class/backlight/driver/max_brightness
    
    /sys/class/backlight/driver/actual_brightness
    
    /sys/class/backlight/driver/brightness
    
    /sys/class/backlight/driver/type


The "max_brightness" file contains the maximum value that can be set to the
backlight level.


In "brightness" file you can write the value of backlight and the Linux 
kernel will set that value.


You must read actual backlight level from "actual_brightness" file. Never
read the backlight level from "brightness" file.


The "type" file is the type of control and it can be:
    
    firmware
    
    platform 
    
    raw


The firmware control should be preferred to platform control. The platform
control should be preferred to raw control.
      
If there are several directories in /sys/class/backlight/, you should use
the directory which its "type" file has got the "firmware" value.


In order to write in /sys/class/backlight/driver/brightness file root
permissions are needed. This library calls to a command line tool called 
"lxqtbacklight_backend". "lxqtbacklight_backend" has a policy in Polkit 
in order to write in /sys/class/backlight/driver/brightness file.
