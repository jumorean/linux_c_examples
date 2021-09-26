//
// Created by chengdingan on 2021/9/26.
//


#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
        DIR *dir = opendir("/sys/class/thermal");

        if (chdir("/sys/class/thermal") || !dir) {
                return EXIT_FAILURE;
        }

        struct dirent *dirent = NULL;
        while ((dirent = readdir(dir))) {
                if (!strncmp(dirent->d_name, "thermal_zone", strlen("thermal_zone")))
                {
                        DIR *thermal_zone_dir = opendir(dirent->d_name);
                        if (!thermal_zone_dir) {
                                return EXIT_FAILURE;
                        }

                        struct dirent *dirent_thermal_zone;
                        while (dirent_thermal_zone = readdir(thermal_zone_dir)) {
                                printf("[%s]/%s\n", dirent->d_name, dirent_thermal_zone->d_name);
                        }
                }
        }

        return 0;
}

