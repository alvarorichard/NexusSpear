<p align="center">
  <img src="https://i.imgur.com/b96sTVC.png" alt="NexusSpear Screenshot">
</p>


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
   $ ./server <server_ip> <port>
   $ ./client
2. 
The client will automatically connect to the server using the specified IP address and port.

Once connected, you can execute commands on the client machine by entering them on the server console. The output of the command will be displayed on the server.

To change the working directory on the client, use the cd command followed by the desired directory path:
```shell
$ cd /path/to/directory

```
The server will update and display the current working directory of the client.

To exit the reverse shell, enter the command exit on the server console.