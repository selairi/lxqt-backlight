# lxqt-backlight

This tool changes screen backlight in Linux system. It uses /sys/class/backlight and Polkit.


lxqt-backlight [backlight-level [ % ]] [--help]

--help             Shows this message.

--show             Shows actual brightness level.

--inc              Increases actual brightness level.

--dec              Decreases actual brightness level.

backlight-level    Sets backlight

backlight-level %  Sets backlight from 1% to 100%


Example:

Sets backlight level until 10%:

pkexec lxqt-backlight 10 %       

Sets backlight level to 10:

pkexec lxqt-backlight 10         
