# Installer Steps

This document explains each steps the installer takes to clone the system from
the iso image.

## Pre-installation

### Disk Partition

This step simply executes cfdisk accepting as an argument a device specified
by the user.

### File System Installation

In this step the user will select which partition to format and which file
system to use for the selected partition. The currently supported file systems
are ext4, btrfs*, fat, exfat*, ext3, ntfs*, xfs*, swap.

> **NOTE:** The file systems indicated with an asterisk are not tested and might
> not work

### Partition Mount Point

This step will prompt the user to enter a mount point for each partition
formatted. The user must at least specify a root partition.

### Mounting partitions

The system will then mount all partitions which a mount point has been specified.
It will start with the root partition then it will move on to the rest.
Between these two steps the base directories will also be created.

## Installation

### Extract squashfs image

This step will extract the squashfs image onto the mounted partitions, effectively
installing the system.

## Post-installation setup

### Mounting virtual kernel file systems & Chroot

The first step of the post-installation process includes the mounting of
the virtual kernel file system needed for the chroot. After, the program
will execute chroot() and change the root to the mount point of the new root
partition.

### Fstab generation

The system will automatically generate the /etc/fstab file from the informations
entered earlier.

### GRUB installation

This step will generate a new initramfs, then prompt the user to enter a device
where the boot loader will be installed onto.
