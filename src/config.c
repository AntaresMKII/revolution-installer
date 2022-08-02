/*
 * generate_fstab.c
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
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

char* get_uuid(char* dev)
{
    DIR *dir = opendir("/dev/disk/by-uuid");
    char* link = (char*) malloc(sizeof(char) * 256);
    char* dev_link = (char*) malloc(sizeof(char) * 256);
    int size;
    char* uuid;

    if (dir == NULL) {
        return NULL;
    }

    if (link == NULL || dev_link == NULL)
        return NULL;


    for (struct dirent* entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (entry->d_type == DT_LNK) {
            strcpy(link, "/dev/disk/by-uuid");
            strcat(link, entry->d_name);

            size = readlink(link, dev_link, 256);

            if (size > -1 && strcmp(dev, dev_link) == 0) {
                uuid = (char*) malloc(size);
                if (uuid == NULL)
                    return NULL;
                strcpy(uuid, entry->d_name);
                break;
            }
        }
    }

    return uuid;
}

int generate_fstab(p_list *list)
{
    //get uids
    //write the device
    //write the type
    //write oprtions
    //dump and pass

    char* fpath = "/etc/fstab";
    FILE *file;
    char* buff = (char*) malloc(sizeof(char) * 255);
    part* curr = list->first;

    if (curr == NULL) {
        return -1;
    }

    file = fopen(fpath, "w");

    while (curr != NULL) {
        strcat(buff, "UUID=");
        strcat(buff, get_uuid(curr->path));
        strcat(buff, "\t");
        strcat(buff, curr->mnt_point);
        strcat(buff, "\t");
        if (strcmp(curr->fs, "fat") == 0) {
            strcat(buff, "vfat");
        }
        else {
            strcat(buff, curr->fs);
        }
        strcat(buff, "\t");
        strcat(buff, "defaults 0 ");
        if (strcmp(curr->mnt_point, "/") == 0) {
            strcat(buff, "1");
        }
        else {
            strcat(buff, "2");
        }

        fwrite(buff, sizeof(char), strlen(buff), file);
    }

    fclose(file);
    return 0;
}

int install_grub(int efi, char* rootdev)
{
    char* command = "/usr/sbin/grub-install";
    int pid;

    pid = fork();
    if (pid == 0) {
        if (efi) {
            execl(command, "grub-install",
                    "--target=x86_64-efi",
                    "--efi-directory=/boot/efi",
                    "--bootloader-id=SOVIET");
        }
        else {
            execl(command, "grub-install",
                    "--target=i386-pc",
                    rootdev);
        }
    }

    waitpid(pid, NULL, 0);
    return 0;
}

int gen_initrfs()
{
    char* command = "/usr/bin/dracut";
    int pid;

    pid = fork();
    if (pid == 0) {
        execl(command, "revolution-dracut");
    }

    waitpid(pid, NULL, 0);
    return 0;
}
