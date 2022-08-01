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
#include <unistd.h>

int generate_fstab (p_list* list)
{
    char* path = "/etc/fstab";
    char* proc_entry = "proc /proc proc nosuid,noexec,nodev 0 0\n";
    char* sysfs_entry = "sysfs /sys sysfs nosuid,noexec,nodev 0 0\n";
    char* devpts_entry = "devpts /dev/pts devpts gid=5,mode=620 0 0\n";
    char* tmpfs_entry = "tmpfs /run tmpfs defaults 0 0\n";
    char* devtmpfs_entry = "devtmpfs /dev devtmpfs mode=0755,nosuid 0 0\n";
    char* rootfs_entry = "defaults 1 1\n";
    char* otherfs_entry = "defaults 1 2\n";
    int fd;

    part* curr = list->first;

    if (curr == NULL) {
        exit(EXIT_FAILURE);
    }

    fd = open(path, O_CREAT | O_WRONLY);

    if (fd == -1) {
        return errno;
    }

    //write all devices in fstab
    while (curr != NULL) {
        write(fd, curr->path, strlen(curr->path));
        write(fd, " ", 1);
        write(fd, curr->mnt_point, strlen(curr->mnt_point));
        write(fd, " ", 1);
        write(fd, curr->fs, strlen(curr->fs));
        write(fd, " ", 1);

        if (strcmp(curr->mnt_point, "/") == 0) {
            write(fd, rootfs_entry, strlen(rootfs_entry));
        }
        else {
            write(fd, otherfs_entry, strlen(otherfs_entry));
        }

        curr = curr->next;
    }


    //write proc entry in fstab
    write(fd, proc_entry, strlen(proc_entry));

    //write sysfs entry
    write(fd, sysfs_entry, strlen(sysfs_entry));

    //write devpts entry
    write(fd, devpts_entry, strlen(devpts_entry));

    //write tmpfs entry
    write(fd, tmpfs_entry, strlen(tmpfs_entry));

    //write devtmpfs entry
    write(fd, devpts_entry, strlen(devpts_entry));

    close(fd);
    return 0;
}

int create_grub_conf(p_list* list)
{
    part* root = list->first;
    char* path = "/boot/grub/grub.cfg";
    int fd;
    char* buff;
    char* grub_disk; //for grub disk format
    char sndlast;

    while (root != NULL) {
        if (strcmp(root->mnt_point, "/")) {
            break;
        }

        root = root->next;
    }

    if (root == NULL) {
        return -1;
    }

    //this is not the best way to do this... Will need some rework

    fd = open(path, O_CREAT | O_WRONLY);

    buff = "set default=0\nset timeout=5\n\ninsmod ext2\nset root=";
    write(fd, buff, strlen(buff));

    sndlast = root->path[strlen(root->path) -2];

    if (sndlast == 'a') {
        sndlast = '0';
    }
    else if (sndlast == 'b') {
        sndlast = '1';
    }
    else if (sndlast == 'c') {
        sndlast = '2';
    }
    else {
        printf("This program can't count above 3! You will have to configure the rest of %s by yourself.\n", path);
    }

    grub_disk = (char*) malloc(7);
    grub_disk = strcat("(hd", &sndlast);
    grub_disk = strcat(grub_disk, &root->path[strlen(root->path)-1]);
    grub_disk = strcat(grub_disk, ")");

    buff = grub_disk;
    write(fd, buff, strlen(buff));

    buff = "\n\nmenuentry \"Soviet Linux\" {\n\tlinux /boot/vmlinuz root=";
    write(fd, buff, strlen(buff));
    write(fd, root->path, strlen(root->path));

    buff = " ro\n}";

    close(fd);

    return 0;
}
