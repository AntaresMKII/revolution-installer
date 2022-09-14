/*
 * main.c
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
/*
 * The main function should walk the user though the setup process following the
 * following steps:
 * 1. Display available keyboards layout and prompt the user for a valid keyboard
 * layout
 * 2. Prompt the user for a hostname
 * 3. Display available locales and prompt the user for a valid one
 * 4. Display available time zones and prompt the user for a valid one
 * 5. Prompt the user for a root password
 * 6. Prompt the user for a username and password. Allow also the user to set
 * user groups
 * 7. Display available devices and prompt the user for a device where to install
 * GRUB
 * 8. Prompt the user for a choice of cfdisk or fdisk, if user chooses cfdisk
 * display the devices available and prompt the user for one
 * 9. Display and prompt the user for a device and a file system
 * 10. Review the installation settings
 * 11. Install the system:
 *      a. Format and install filesystem
 *      b. Mount the drives
 *      c. Copy the system
 *      d. Set the hostname
 *      e. Set the locale
 *      f. Set the timezone
 *      g. Set the timezone
 *      h. Set root password
 *      i. Create the username and set groups
 *      j. Create /etc/fstab
 *      k. Install GRUB
 *      l. Reboot
 */

#include "include/revolution.h"

// determine if the system is booted in efi mode
int isEfi ()
{
    DIR *dir;
    dir = opendir("/sys/firmware/efi/efivars");

    if (dir == NULL) {
        if (errno == ENOENT)
            return 0;
        else
            return -1;
    }

    closedir(dir);
    return 1;
}

void cleanup (p_list *list)
{
    part *curr = list->first;
    part *next;
    while (curr != NULL) {
        next = curr->next;
        free(curr->path);
        free(curr->mnt_point);
        free(curr->fs);
        free(curr);
        curr = next;
    }

    free(list);
}

int main (int argc, char** argv)
{

    p_list part_list;
    part_list.first = NULL;

    int efi = isEfi();
    char* rootdev;

    puts("=== Disk Partition ===");
    if (dpart_loop()) {
        puts("Error partitioning disk\nQuitting...");
        exit(EXIT_FAILURE);
    }

    puts("=== File System Creation ===");
    if (fs_loop(&part_list)) {
        puts("Error creating file system!\n Quitting...");
        exit(EXIT_FAILURE);
    }

    puts("=== Mount Setup ===");
    mount_setup(&part_list);

    puts("=== Mounting Root File System ===");
    mount_root(&part_list);

    puts("=== Generating Base Directories ===");
    gen_base_dir();
    
    puts("=== Mounting Boot Partition ===");
    mount_boot(&part_list);
    
    puts("=== Mounting Remaining Disks ===");
    mount_dev(&part_list);

    puts("=== Extracting Squashfs Image ===");
    copy_sys_files(SQ_PATH, MNT);

    puts("=== Mounting Virtual Kernel File System ===");
    if (mount_virtkfs() == -1) {
        printf("Error mounting Virtual Kernel File System. ERRNO: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    puts("=== Entering chroot Environment ===");
    chroot(MNT);

    puts("=== Generating fstab ===");
    if (0 != generate_fstab(&part_list)) {
        printf("Error generating fstab. ERRNO:%s\n", strerror(errno));
    }

    puts("=== Setting up Boot Loader ===");
    gen_initrfs();
    puts("Enter the disk on which to install GRUB");
    rootdev = my_input();
    install_grub(efi, rootdev);

    cleanup(&part_list);

    puts("DONE!");
    return 0;
}
