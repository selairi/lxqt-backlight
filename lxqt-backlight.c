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

#define True 1
#define False 0

static void set_backlight(char *driver, int value)
{
    char path[1024];
    sprintf(path, "/sys/class/backlight/%s/brightness", driver);
    FILE *out = fopen(path, "w");
    if( out != NULL ) {
        fprintf(out, "%d", value);
        fclose(out);
    } else {
        perror("Couldn't open /sys/class/backlight/driver/brightness");
    }
}

static int read_backlight(char *driver)
{
    char path[1024];
    sprintf(path, "/sys/class/backlight/%s/actual_brightness", driver);
    FILE *in = fopen(path, "r");
    if( in == NULL )
        return -1;
    int value;
    int ok = fscanf(in, "%d", &value);
    fclose(in);
    if( ok == EOF ) {
        value = 0;
    }
    return value;
}

static int read_max_backlight(char *driver)
{
    char path[1024];
    sprintf(path, "/sys/class/backlight/%s/max_brightness", driver);
    int value;
    FILE *in = fopen(path, "r");
    if( in == NULL )
        return -1;
    int ok = fscanf(in, "%d", &value);
    fclose(in);
    if( ok == EOF ) {
        value = 0;
    }
    return value;
}

typedef enum {FIRMWARE, PLATFORM, RAW, N_BACKLIGHT} BackligthTypes;

static char *get_driver()
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


static void show_blacklight()
{
    char *driver = get_driver();
    if( driver == NULL ) {
        fprintf(stderr, "Error: /sys/class/backlight is empty.");
        return;
    }
    int max_value = read_max_backlight(driver);
    int actual = read_backlight(driver);
    printf("%s %d %d\n", driver, max_value, actual);
    free(driver);
}

static void change_blacklight(int value, int percent_ok)
{
    char *driver = get_driver();
    if( driver == NULL ) {
        fprintf(stderr, "Error: /sys/class/backlight is empty.");
        return;
    }
    int max_value = read_max_backlight(driver);
    int actual = read_backlight(driver);
    if(percent_ok)
        value = (float)(max_value*value)/100.0;
    if(value<max_value && value>0) {
        set_backlight(driver, value);
    }
    free(driver);
}

static void increases_blacklight()
{
    char *driver = get_driver();
    if( driver == NULL ) {
        fprintf(stderr, "Error: /sys/class/backlight is empty.");
        return;
    }
    int max_value = read_max_backlight(driver);
    int actual = read_backlight(driver);
    int value = actual + (float)(max_value)/10.0;
    if( value > max_value)
        value = max_value;
    if(value<max_value && value>0) {
        set_backlight(driver, value);
    }
    free(driver);
}

static void decreases_blacklight()
{
    char *driver = get_driver();
    if( driver == NULL ) {
        fprintf(stderr, "Error: /sys/class/backlight is empty.");
        return;
    }
    int max_value = read_max_backlight(driver);
    int actual = read_backlight(driver);
    int value = actual - (float)(max_value)/10.0;
    if( value <= 0 )
        value = 1;
    if(value<max_value && value>0) {
        set_backlight(driver, value);
    }
    free(driver);
}

static void help(char *argv0)
{
    printf("%s [backlight-level [ %% ]] [--help]\n"
        "--help             Shows this message.\n"
        "--show             Shows actual brightness level.\n"
        "--inc              Increases actual brightness level.\n"
        "--dec              Decreases actual brightness level.\n"
        "backlight-level    Sets backlight\n"
        "backlight-level %%  Sets backlight from 1%% to 100%%\n"
        "This tool changes screen backlight.\n"
        "Example:\n"
        "%s 10 %%       Sets backlight level until 10%%.\n"
        , argv0, argv0
    );
}


int main(int argc, char *argv[])
{
    int value = -1, value_percent_ok = False;
    int n;
    for(n=1; n<argc; n++) {
        if( !strcmp(argv[n], "--help") ) {
            help(argv[0]);
            return 0;
        } if( !strcmp(argv[n], "--show") ) {
            show_blacklight();
            return 0;
        } if( !strcmp(argv[n], "--inc") ) {
            increases_blacklight();
            return 0;
        } if( !strcmp(argv[n], "--dec") ) {
            decreases_blacklight();
            return 0;
        } else if ( argv[n][0] != '-' ) {
            value = atoi(argv[1]);
        } else if ( argv[n][0] != '%' && strlen(argv[n])==1 ) {
            value_percent_ok = True;
        } else {
            help(argv[0]);
            return 0;
        }
    }
    
    if( argc == 1 ) {
        help(argv[0]);
        return 0;
    }
    
    change_blacklight(value, value_percent_ok);

    return 0;
}