/*
 * revolution.h
 */

#ifndef REVOLUTION_H
#define REVOLUTION_H

// Debug:
// #define DEBUG 1
#ifdef DEBUG
#define SQ_PATH //put the test path for the squashfs image here
#endif

// Defines :
#ifndef DEBUG
#define SQ_PATH "/media/rootfs.sfs"
#endif
#define MNT "/mnt/soviet"

// Includes :
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/swap.h>
#include "utils.h"
#include "my_input.h"

// Structures :
typedef struct partition_node {
    char* path;
    char* mnt_point;
    char* fs;
    struct partition_node *next;
} part;

typedef struct partition_list {
    part *first;
} p_list;

// Prototypes :
// disk partition
extern int dpart_loop();
extern int list_dev();

// mount devices
extern int mount_setup(p_list *list);
extern int mount_virtkfs();
extern int mount_dev(p_list *list);
extern int mount_root(p_list *list);
extern int mount_boot(p_list *list);

// file system creation
extern int fs_loop(p_list *list);

// system installation
extern int copy_sys_files(char *sq_path, char *target);
extern int gen_base_dir();

// configuration
extern int generate_fstab(p_list *list);
extern int install_grub(int efi, char* rootdev);
extern int gen_initrfs();

#endif
