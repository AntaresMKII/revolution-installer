/*
 * mount_setup.c
 */
/*
 * This file is part of revolution-installer.
 * revolution-installer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * revolution-installer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with revolution-installer. If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2022 AntaresMKII
 */
#include "include/revolution.h"

int mount_dev(p_list *list)
{
    part *curr = list->first;
    char mnt_point[100] = MNT;

    while (curr != NULL) {
        if (curr->mnt_point != NULL &&
            strcmp(curr->mnt_point, "/") != 0) {

            if (strcmp(curr->fs, "swap") != 0) {
                strcat(mnt_point, curr->mnt_point);
                mkdir(mnt_point, 0777);
                mount(curr->path, mnt_point, curr->fs, 0, NULL);
                strcpy(mnt_point, MNT);
            }
            else {
                 swapon(curr->path, 0);
            }
        }

        curr = curr->next;
    }

    return 0;
}

int mount_boot(p_list *list)
{
    part *curr = list->first;
    char *mnt_point = MNT;

    while (curr != NULL) {
        if (strcmp(curr->mnt_point, "/boot") == 0) {
            mount(curr->path, mnt_point, curr->fs, 0, NULL);
            break;
        }

        curr = curr->next;
    }

    return 0;
}

int mount_root(p_list *list)
{
    part *curr = list->first;
    char *mnt_point = MNT;

    while (curr != NULL) {
        if (strcmp(curr->mnt_point, "/") == 0) {
            mount(curr->path, mnt_point, curr->fs, 0, NULL);
            break;
        }

        curr = curr->next;
    }

    return 0;
}


int mount_setup(p_list *list)
{
    char* mnt_point;
    part *curr = list->first;

    if (curr == NULL) {
        return 1;
    }

    puts("Choose the mounting point for the following partitions.");
    puts("You can continue to the next step by entering c.");

    while (curr != NULL) {
        printf("Partition %s of type %s :", curr->path, curr->fs);
        mnt_point = my_input();

        if (strcmp(mnt_point, "c") == 0) {
            puts("Remaining partitions will not be mounted");
            break;
        }
        else {
            curr->mnt_point = (char *) malloc(strlen(mnt_point));
            if (curr->mnt_point == NULL){
                exit(EXIT_FAILURE);
            }
            strcpy(curr->mnt_point, mnt_point);

            curr = curr->next;
        }
    }

    return 0;
}


//this is what I mean by "unless specified otherwise", had to change this seperately. - Kat
int mount_virtkfs()
{
    char* dev_dir = MNT;
    char* dev_pts = MNT;
    char* proc = MNT;
    char* sysfs = MNT;
    char* tmpfs = MNT;

    strcat(dev_dir, "/dev");
    strcat(dev_pts, "/dev/pts");
    strcat(proc, "/proc");
    strcat(sysfs, "/sys");
    strcat(tmpfs, "/tmpfs");

    int rc = 0;

    rc += mount("/dev", dev_dir, NULL, MS_BIND, NULL);
    rc += mount("/dev/pts", dev_pts, NULL, MS_BIND, NULL);
    rc += mount("proc", proc, "proc", 0, NULL);
    rc += mount("sysfs", sysfs, "sysfs", 0, NULL);
    rc += mount("tmpfs", tmpfs, "tmpfs", 0, NULL);

    return rc;
}
//If you are reading this go to this URL: https://www.youtube.com/watch?v=dQw4w9WgXcQ
//Best comment right here ^^^^
