# THIS REPOSITORY IS FOR BACKUP ONLY! PLEASE CONTRIBUTE ON OUR GITLAB

# revolution-installer

The installer of the Soviet-Linux operating system written in C.

## How to install

### Dependencies

- gcc
- cfdisk
- mkfs
- squashfs-tools

### Installation

#### Automatic
1. Curl and run the script
`# curl raw.githubusercontent/Soviet-Linux/revolution-installer/main/install.sh | sh`

#### Manually

1. Clone this repository:
`# git clone https://github.com/Soviet-Linux/revolution-installer`

2. Enter the revolution-installer directory:
`# cd revolution-installer`

3. Build the binaries:
`# make`

4. Copy binaries into /bin or /usr/bin
`# cp bin/revolution /usr/bin`

## Usage

Revolution-installer is designed to work only on the official ISO of Soviet Linux
and it will not work on an installed system.

1. Execute the revolution command as the root user:
`# revolution`

2. Follow the steps indicated by the interactive program.

## Docs
You can view the docs in the DOC/ folder. It contains a pdf version as well as
the LaTeX source files.

## Contributing

You are free to contribute ideas and implementations to this project.

To submit code follow these steps:

1. Fork this repository

2. Create a new branch. Choose an appropriate name.

3. Commit your changes

4. Open a pull request

## Licence

Distributed under the GNU GPLv3.0 licence. Refer to [LICENCE](https://github.com/Soviet-Linux/revolution-installer/blob/main/LICENSE)
for more info.
