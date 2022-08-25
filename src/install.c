/*
 * copy_sys_files.c
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
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int copy_sys_files(char *sq_path, char *target)
{
    int pid;
    int stat;
    int rc;

    pid = fork();

    if (pid == 0) {
        printf("Extracting image %s to %s\n", sq_path, target);
        rc = execl("/usr/bin/unsquashfs", "revolution-unsquashfs", "-f", "-d", target, sq_path);
        if (rc == -1) {
            printf("Error executing unsquashfs. ERRNO: %s\n", strerror(errno));
        }
        exit(-1);
    }

    waitpid(pid, &stat, 0);
    
    if (WIFEXITED(stat)) {
        printf("Exit status: %d\n", WEXITSTATUS(stat));
    }

    return 0;
}

int gen_base_dir()
{
    mkdir("/mnt/dev", 0555);
    mkdir("/mnt/media", 0555);
    mkdir("/mnt/mnt", 0755);
    mkdir("/mnt/proc", 0555);
    mkdir("/mnt/run", 0555);
    mkdir("/mnt/sys", 0555);
    mkdir("/mnt/tmp", 0555);
    mkdir("/mnt/boot", 0755);
    return 0;
}
