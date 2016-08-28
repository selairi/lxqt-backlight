/*
 * Copyright (C) 2016  P.L. Lucas
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lxqtbacklight_backend.h"

#define True 1
#define False 0

static int read_backlight(char *driver);
static int read_max_backlight(char *driver);

int lxqt_backlight_backend_get()
{
    char *driver = lxqt_backlight_backend_get_driver();
    if( driver == NULL ) {
        fprintf(stderr, "Error: /sys/class/backlight is empty.");
        return -1;
    }
    int value = read_backlight(driver);
    free(driver);
    return value;
}

int lxqt_backlight_backend_get_max()
{
    char *driver = lxqt_backlight_backend_get_driver();
    if( driver == NULL ) {
        fprintf(stderr, "Error: /sys/class/backlight is empty.");
        return -1;
    }
    int value = read_max_backlight(driver);
    free(driver);
    return value;
}

FILE *lxqt_backlight_backend_get_write_stream()
{
    FILE *stream = popen("pkexec lxqt-backlight_backend --stdin", "w");
    return stream;
}

static int read_int(char *path)
{
    FILE *in = fopen(path, "r");
    if( in == NULL ) {
        char buffer[1024];
        sprintf(buffer, "Couldn't open %s", path);
        perror(buffer);
        return -1;
    }
    int value;
    int ok = fscanf(in, "%d", &value);
    fclose(in);
    if( ok == EOF ) {
        value = 0;
    }
    return value;
}

static int read_backlight(char *driver)
{
    char path[1024];
    sprintf(path, "/sys/class/backlight/%s/actual_brightness", driver);
    return read_int(path);
}

static int read_max_backlight(char *driver)
{
    char path[1024];
    sprintf(path, "/sys/class/backlight/%s/max_brightness", driver);
    return read_int(path);
}

typedef enum {FIRMWARE, PLATFORM, RAW, OTHER, N_BACKLIGHT} BackligthTypes;

char *lxqt_backlight_backend_get_driver()
{
    DIR *dirp;
    struct dirent *dp;
    
    char *drivers[N_BACKLIGHT];
    char *driver;
    BackligthTypes n;
    char path[1024], type[1024];
    
    for(n=0;n<N_BACKLIGHT;n++)
        drivers[n] = NULL;

    if ((dirp = opendir("/sys/class/backlight")) == NULL) {
        perror("Couldn't open /sys/class/backlight");
        return NULL;
    }

    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
            if( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
                continue;
            driver = dp->d_name;
            sprintf(path, "/sys/class/backlight/%s/type", driver);
            FILE *in = fopen(path, "r");
            if( in == NULL )
                continue;
            int ok = fscanf(in, "%s", type);
            fclose(in);
            if( ok != EOF ) {
                // firmware control should be preferred to platform control should be preferred to raw control.
                if( ! strcmp("firmware", type) ) {
                    drivers[FIRMWARE] = strdup(driver);
                    break;
                } else if( ! strcmp("platform", type) )
                    drivers[PLATFORM] = strdup(driver);
                else if( ! strcmp("raw", type) )
                    drivers[RAW] = strdup(driver);
                else // Only, firmware, platform and raw are defined, but...
                    drivers[OTHER] = strdup(driver);
            }
        }
    } while (dp != NULL);

    closedir(dirp);

    if (errno != 0)
        perror("Error reading directory");    
    
    driver = NULL;
    for(n=0;n<N_BACKLIGHT;n++) {
        if( drivers[n] != NULL && driver == NULL )
            driver = drivers[n];
        else
            free(drivers[n]);
    }

    return driver;
}


