# hasm-cpp


## Table of Contents
* [About](#about)
* [Getting Started](#getting-started)
  * [Building](#building)
* [Usage](#usage)
  * [Arguments](#arguments)
  * [Syntax](#syntax)
  * [Memory Layout](#memory-layout)
* [Contributions](#contributons)
  * [TODO](#todo)
* [Acknowledgements](#acknowledgements)
* [Contact](#contact)


## About
HASM and its sister project, SASM, is an esoteric language designed for any system.
HASM takes its name and many features from assembly-style languages.
Minimal syntactic sugar, combined with a simple instruction set, makes the simplest programs in HASM a challenge.

The HASM interpreter is written in C++ for all platforms.
Development of HASM started in January 2020, in Python.
The original (partly functional) project can be found on my [Gists](https://gist.github.com/Hayden977/f26fb8bfc31401f9e48e542a77773554).


## Getting Started


### Building
By default, HASM is built with g++ on all platforms.
In the base directory, there are compilation scripts for Windows and Linux systems.
Running the script for your platform will create the `hasm` executable in the `bin` directory.
For Linux systems, the compilation script will need to be made executable using `chmod`. 
Example build and execution processes for both systems are shown below.

#### Linux
```Bash
cd hasm-cpp
chmod +x ./compile.sh
./compile.sh
cd ./bin
./hasm
```

#### Windows
```Batch
cd hasm-cpp
compile
cd bin
hasm
```


## Usage
As of version 1.3 (8/27/2020), running `hasm` in the `bin` directory will result in the following output.
```
[HASM]:
```
This field accepts any valid instruction (see [Syntax](#syntax)), as well as the special commands `place` and `peek`.
`Peek` is only active when the `silent` flag is set, using the argument `-s`. 
Other command line arguments can be found in the [Arguments](#arguments) section.
If the silent flag is not set, the status of the HASM memory space is printed after every command or instruction.
By default, you can exit the HASM interpreter by typing `quit` or `q` at the prompt, or by pressing `^C`.

### Arguments

| Argument | Effect                                 | Exits? |
| -------- | -------------------------------------- | ------ |
| -c       | Sets the character flag                | n      |
| -d       | Treats the following `-e` as devices   | n      |
| -e       | Executes the specified file at startup | n      |
| -f       | Sets the flag shift flag (use with -e) | n      |
| -h       | Displays a help message                | Y      |
| -l       | Write commands to the specified file   | n      |
| -s       | Sets the silent flag                   | n      |
| -v       | Displays a version message             | Y      |

#### Character Mode
In character mode, the contents of the HASM memory space is shown as character.
The character flag can be used in combination with the silent flag.
Character mode is designed for showing string of text in the memory.
The character flag allows programs involving "strings" to be used.

#### Execute at Startup
With version 1.2 (7/5/20), the HASM interpreter now has the ability to execute files at startup.
To execute a file, start the interpreter with the `-e` flag. 
Specify the path of the file to be read after the `-e` flag. 
The contents of the file will be treated the same as many interpreter commands.
Comments can be specified using the ';' symbol, or can be left in the file unaltered.
However, using the ';' symbol will lead to the interpreter skipping the line, saving some performance.
Multiple execute flags can be chained to read multiple files.

#### Silent Mode
In silent mode, the `[HASM]:` message is not shown.
Additionally, the contents of the HASM memory space is hidden.
The contents of the memory space can be accessed using the `peek` command.
Silent mode is designed for minimalist users, or for users who are presenting their development.
As of version 1.1, there is no way to toggle the silent flag within the interpreter.

### Syntax

#### mov
Alias: `m`

Usage: `mov SOURCE DESTINATION`

Sets the memory at the address DESTINATION to the contents of the memory at the address SOURCE.
Additionally, zeros out the memory at the address SOURCE.

Example: `mov 5 6 # Move the memory at 5 to the memory at 6`

#### psh
Alias: `ps`

Usage: `psh SOURCE`

Decrements the stack pointer to move to the new address.
If the stack pointer is within bounds (greater than 0), the contents of the memory at SOURCE is placed at the stack pointer.
If the stack pointer is out of bounds, the stack pointer is reset to the location before the command was sent.
Additionally, the `mem_out` flag is set.

Example: `psh 3 # Pushes the memory at 3 onto the stack`

#### pop
Alias: `pp`

Usage: `pop DESTINATION`

Sets the contents of the memory at DESTINATION to the value at the stack pointer.
Additionally resets the value at the stack pointer to be 0.
If the stack pointer is within bounds (greater than 16), the stack pointer is incremented.
Otherwise, the stack pointer is left untouched.

Example: `pop 8 # Pops the value at the stack pointer to the memory at the address 8`

#### add
Alias: `a`

Usage: `add SOURCE DESTINATION`

Adds the contents of the memory at the address SOURCE to the contents of the memory at the address DESTINATION.
If the memory at the address DESTINATION is less than 0, the `neg` flag is set.

Example: `add 9 5 # Adds the value at the address 9 to the value at the address 5`

#### sub
Alias: `s`

Usage: `sub SOURCE DESTINATION`

Subtracts the contents of the memory at the address SOURCE from the contents of the memory at the address DESTINATION.
If the memory at the address DESTINATION is less than 0, the `neg` flag is set.

Example: `sub 2 5 # Subtracts the value at the address 2 from the value at the address 5`

#### inc
Alias: `i`

Usage: `inc DESTINATION`

Increments the contents of the memory at the address DESTINATION by one.
If the memory at the address DESTINATION is less than 0, the `neg` flag is set.

Example: `inc 1 # Increments the value at the address 1 by one`

#### dec
Alias: `d`

Usage: `dec DESTINATION`

Decrements the contents of the memory at the address DESTINATION by one.
If the memory at the address DESTINATION is less than 0, the `neg` flag is set.

Example: `dec 1 # Decrements the value at the address 1 by one`
#### place
Alias: `p`

Usage: `place VALUE DESTINATION`

Places the integer representation of the VALUE parameter into the memory at the address DESTINATION.
This is a interpreter only command, and is not part of the HASM instruction set.

Example: `place 65 1 # Places the value 65 at the address 1`

#### peek
Alias: `pe`

Prints the contents of the memory and the contents of the stack. 
`Peek` only works in silent mode.
In "normal" mode, the contents of the memory and the stack would be printed anyways, so no operations occur.

Example: `peek # Prints the contents of the memory and stack`


#### quit
Alias: `q`

Exits the HASM interpreter.

Example `quit # Exits the interpreter`

### Memory Layout

#### Flags and Registers
All flags and registers are stored in the main memory area [0 - 31].
The flags and registers can be read and written to with no protection.
The table below contains information about each flag.

| Address | Flag      | Usage                                                                                    | Boolean? |
| ------- | --------- | ---------------------------------------------------------------------------------------- | -------- |
| 0       | `jmp`     | Address for the next jump operation                                                      | n        |
| 1       | `greater` | If the previous greater-than operation succeeded                                         | Y        |
| 2       | `less`    | If the previous less-than operation succeeded                                            | Y        |
| 3       | `neg`     | If the last math operation resulted in a negative number                                 | Y        |
| 4       | `zero`    | If the last zero comparison operation resulted in true                                   | Y        |
| 5       | `none`    | If the last move operation placed a zero at a location where there was previously a zero | Y        |
| 6       | `out`     | If the stack was full when a `psh` operation was attempted                               | Y        |
| 7       | `used`    | If the `jmp` address was modified by a move operation                                    | Y        |


## Contributions
As of June 2020, HASM is closed to outside contributions. 
However, help is always appreciated, especially on the following tasks.

### TODO

#### Instructions
* ~~`inc` Increment the value of a register by 1~~
* ~~`dec` Decrement the value of a register by 1~~

#### Interpreter Features
* Possible C port?
* ~~Scanning and executing files on startup (command line argument)~~

##### Interfacing
As of right now there is no way for HASM to communicate with external processes.
The long term vision of HASM is to implement the instructions on a physical device.
This opens up the possibility for other devices to write to HASM's memory space.
However as HASM is only in software, there is no way to do this.

~~My ideal solution would be an external device system, accompanied with a command such as `dev`.~~
~~The `dev` command would open up a device sub-menu, which has a driver (written in HASM) that writes to the memory.~~
~~These devices should be able to update on every instruction / cycle, which can be emulated with a `-d` flag.~~
~~If anyone has ideas as to how I could implement this contact me with any of the methods listed under [Contact](#contact).~~
Update (8/27/2020): I have added the `-d` flag, which allows all following execute flags to be executed on every command.

##### Flagging Behaviors
Within the coming updates there will be a new argument for handling the behavior of commands in the flag memory space.
When a command is executed and if the supplied address(es) are within the flag memory, the command will operate differently.
The range for the flag memory space can be found in [Flags and Registers](#flags-and-registers).
When the argument is not supplied, the command behaves like it would regardless of if it is within the flag memory space.

#### Code Cleanup
From a personal standpoint, HASM has room for improvement in terms of being compliant with the C++ standards.
Performance does not seem to be an issue (for example, low process time on all systems).
However, this does not mean there is no room for improvement.
Again, contact me with any of the methods listed under [Contact](#contact).


## Acknowledgements
* [StackOverflow](https://stackoverflow.com/a/14267455) for the `tokenize` function

## Contact
Maintained by [Hayden977](https://github.com/hayden977).
Twitter: `@Hayden977_`
Discord: Hayden977#9614
