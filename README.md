# hasm-cpp


## Table of Contents
* [About](#about)
* [Getting Started](#getting-started)
  * [Building](#building)
* [Usage](#usage)
  * [Arguments](#arguments)
  * [Syntax](#syntax)
* [Contributions](#contributons)
  * [TODO](#todo)
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
Running the script for your platform will create the hasm executable in the `bin` directory.
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
As of version 1.0 (6/30/2020), running `hasm` in the `bin` directory will result in the following output.
```
[HASM]:
```
This field accepts any valid instruction (see [Syntax](#syntax)), as well as the special commands `place` and `peek`.
`Peek` is only active when the `silent` flag is set, using the argument `-s`. 
Other command line arguments can be found in the [Arguments](#arguments) section.
If the silent flag is not set, the status of the HASM memory space is printed after every command or instruction.
By default, you can exit the HASM interpreter by typing `quit` at the prompt, or by pressing `^C`.

### Arguments

| Argument | Effect                     | Exits? |
| -------- | -------------------------- | ------ |
| -h       | Displays a help message    | Y      |
| -s       | Sets the silent flag       | n      |
| -v       | Displays a version message | Y      |

#### Silent Mode
In silent mode, the `[HASM]:` message is not shown.
Additionally, the contents of the HASM memory space is hidden.
The contents of the memory space can be accessed using the `peek` command.
Silent mode is designed for minimalist users, or for users who are presenting their development.
As of version 1.0, there is no way to toggle the silent flag within the interpreter.

### Syntax

#### Mov
Alias: `m`

Usage: `mov SOURCE DESTINATION`

Sets the memory at the address DESTINATION to the contents of the memory at the address SOURCE.
Additionally, zeros out the memory at the address SOURCE.

Example: `mov 5 6 # Move the memory at 5 to the memory at 6`

#### Psh
Alias: `ps`

Usage: `psh SOURCE`

Decrements the stack pointer to move to the new address.
If the stack pointer is within bounds (greater than 0), the contents of the memory at SOURCE is placed at the stack pointer.
If the stack pointer is out of bounds, the stack pointer is reset to the location before the command was sent.
Additionally, the `mem_out` flag is set.

Example: `psh 3 # Pushes the memory at 3 onto the stack`

#### Pop
Alias: `pp`

Usage: `pop DESTINATION`

Sets the contents of the memory at DESTINATION to the value at the stack pointer.
Additionally resets the value at the stack pointer to be 0.
If the stack pointer is within bounds (greater than 16), the stack pointer is incremented.
Otherwise, the stack pointer is left untouched.

Example: `pop 8 # Pops the value at the stack pointer to the memory at the address 8`

#### Add
Alias: `a`

Usage: `add SOURCE DESTINATION`

Adds the contents of the memory at the address SOURCE to the contents of the memory at the address DESTINATION.
If the memory at the address DESTINATION is less than 0, the `neg` flag is set.

Example: `add 9 5 # Adds the value at the address 9 to the value at the address 5`

#### Sub
Alias: `s`

Usage: `sub SOURCE DESTINATION`

Subtracts the contents of the memory at the address SOURCE from the contents of the memory at the address DESTINATION.
If the memory at the address DESTINATION is less than 0, the `neg` flag is set.

Example `sub 2 5 # Subtracts the value at the address 2 from the value at the address 5`

#### Place
Alias: `p`

Usage: `place VALUE DESTINATION`

Places the integer representation of the VALUE parameter into the memory at the address DESTINATION.
This is a interpreter only command, and is not part of the HASM instruction set.
Without this command (as of version 1.0), all operation in the interpreter would be operating on zeros.

Example: `place 65 1 # Places the value 65 at the address 1`

#### Peek
Alias: `pe`

Prints the contents of the memory and the contents of the stack. 
`Peek` only works in silent mode.
In "normal" mode, the contents of the memory and the stack would be printed anyways, so no operations occur.

Example: `peek # Prints the contents of the memory and stack`


#### Quit

Exits the HASM interpreter.

Example `quit # Exits the interpreter`

## Contributions
As of June 2020, HASM is closed to outside contributions. 
However, help is always appreciated, espectially on the following tasks.


### TODO

#### Instructions
* `inc` Increment the value of a register by 1
* `dec` Decrement the value of a register by 1

#### Interfacing
As of right now there is no way for HASM to communicate with external processes.
The long term vision of HASM is to implement the instructions on a physical device.
This opens up the possibility for other devices to write to HASM's memory space.
However as HASM is only in software, there is no way to do this.

My ideal solution would be an external device system, accompanied with a command such as `dev`.
The `dev` command would open up a device sub-menu, which has a driver (written in HASM) that writes to the memory.
These devices should be able to update on every instruction / cycle, which can be emulated with a `-d` flag.
If anyone has ideas as to how I could implement this contact me with any of the methods listed under [Contact](#contact).

#### Code Cleanup
From a personal standpoint, HASM has room for improvement in terms of being compliant with the C++ standards.
Performance does not seem to be an issue (for example, low process time on all systems).
However, this does not mean there is no room for improvement.
Again, contact me with any of the methods listed under [Contact](#contact).


## Contact
Maintained by [Hayden977](https://github.com/hayden977).
Twitter: [`@Hayden977_`](`https://twitter.com/Hayden977_`)
Discord: Hayden977#9614
