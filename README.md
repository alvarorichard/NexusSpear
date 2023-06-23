

# NexusSpear is a reverse shell tool 
that allows for remote command execution on a target machine.

## Features

- Establishes a reverse shell connection between a client and server.
- Executes commands on the client and receives the output on the server.
- Allows changing the working directory on the client machine.
- Supports basic command execution and file system navigation.

## Prerequisites

- Linux operating system
- `clang` compiler (or any C compiler of your choice)

## Usage

1. Compile the server and client programs:

   ```shell
   $ make
