/*
 * revolution.h
 */

typedef struct partition_node {
    char* path;
    char* mnt_point;
    char* fs;
    struct partition_node *next;
} part;

typedef struct partition_list {
    part *first;
} p_list;

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
