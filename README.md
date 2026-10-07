# Mini Shell – Linux Command-Line Shell

A custom Linux command-line shell developed in C using POSIX system programming concepts.

## Project Overview

This project implements a command-line shell that accepts user input, identifies commands as built-in or external commands, and executes them in a Linux environment.

The project was developed to gain hands-on experience with C programming, Linux system programming, process management, signals, file operations, and command execution.

## Features

- Command-line interface for Linux
- Built-in and external command classification
- Execution of external Linux commands
- Built-in command handling
- Process creation and execution
- Process synchronization
- Signal handling
- Environment variable operations
- File and directory operations
- Colored shell prompt
- Error handling for invalid commands

## Built-in Commands

The shell includes support for built-in commands such as:

- `echo`
- `printf`
- `read`
- `cd`
- `pwd`
- `pushd`
- `popd`
- `dirs`
- `set`
- `unset`
- `export`
- `declare`
- `typeset`
- `readonly`
- `source`
- `exit`
- `exec`
- `true`
- `type`
- `hash`
- `help`

## External Commands

The shell can execute Linux external commands such as:

- `ls`
- `cat`
- `cp`
- `mv`
- `mkdir`
- `rm`
- `grep`
- `find`
- `ps`
- `pwd`
- `echo`
- `chmod`
- `kill`
- `ping`
- `tar`
- `gzip`
- `gunzip`
- `sed`
- `head`
- `tail`

and other available Linux utilities.

## Technologies Used

- C
- Linux
- POSIX System Programming
- Process Management
- Signals
- File Descriptors
- System Calls
- Standard C Library

## Project Structure

```text
mini-shell-linux-c/
├── main.c
├── main.h
├── README.md
└── .gitignore
