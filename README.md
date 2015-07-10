Comp Mark II
============

Updated version of [**Comp**](https://github.com/gto76/comp-cpp) – Simple 4-bit/1 Hz virtual computer for learning purposes

![screenshot](doc/screenshot.png)

How to run on…
--------------

### Windows

* Install *Tiny Core Linux* on *VirtualBox* using this [**instructions**](https://github.com/gto76/my-linux-setup/tree/gh-pages/conf-files/tiny-core-linux).
* Run the *UNIX* commands.

### UNIX
```bash
git clone https://github.com/gto76/comp-m2.git
cd comp-m2
./run
```

Keys
----
* `space`, `f` - switch bit under cursor
* `tab` - switch address space
* `enter` - start/pause execution
* `esc` - cancel execution
* `d` - delete word under cursor
* `j` - move word up
* `k` - move word down
* `s` - save ram in textfile named `saved-ram-<num>`. To load it, start program with `./comp <file>`

Overview
--------

* Procesor has one 8 bit register.
* Ram is separated into two address spaces; one for instructions, called "CODE", and one for data, called "DATA".
* All instructions are 8 bits long.
* Execution starts at the first address (0) of "CODE" ram. 
* Execution stops when program reaches last address (15) of "CODE" ram.
* Most of instructions consist of instruction code and address:
```
instruction code - 4 bits
  |  +-- address - 4 bits
  v  v
----***-  ->  READ 14  ->  Copy value stored at address 14 of "DATA" ram to register.
```
* All addresses specified by instructions refer to "DATA" ram, except for addresses of "Execution Control" instructions (JUMP, IF MAX, IF MIN...). They refer to "CODE" part of ram.
* Some instructions (logic, READ REG, JUMP REG) do not specify address. They operate on register (SHIFT L/R, NOT, ...) or between register and first address of "DATA" ram (AND, OR, ...)
* Whatever gets written to the last address is sent to the printer, or to 'stdout' if program is running in non-interactive mode.
* When reading from the last address (15), we get a random byte value, or a single word from 'stdin' if program is running in non-interactive mode.


Instruction set
---------------

### Read and Write
 _Name_        | _Code_   | _Description_  
:------------- |:--------:| ---------------------------------------------------
**READ**       | ----     | Copies the value at the specified address into register.  
**WRITE**      | ---\*    | Copies value of the register to the specified address. 
**READ_***     | \*---    | Copies the value that specified address is pointing to into register.  
**WRITE_***    | \*--\*   | Copies value of the register to the address, that specified address is pointing to.  
**PRINT**      | \*-\*\*  | Copies the value at the specified address to the last address of data ram, thus sends it to the printer.
**READ_REG**   | -\*\*\*-\*\*- | Copies the value that register is pointing to into register.  

### Arithmetic
 _Name_        | _Code_   | _Description_  
:------------- |:--------:| ---------------------------------------------------
**ADD**        | --\*--   | Adds value at the specified address to the value of the register, and writes result to the register. If result is bigger than the maximum possible value of _255_ = '********', then _255_ gets written. 
**SUB**        | --\*-\*  | Subtracts value at the speicfied address from the value of the register, and writes result to the register. If result is smaller than _0_, then _0_ gets written.  
**INC**        | --\*--   | Adds 1 to value at the specified address, and copies it to the register.
**DEC**        | --\*-\*  | Subtracts 1 from value at the speicfied address, and copies it to the register.

### Execution Control
 _Name_        | _Code_   | _Description_  
:------------- |:--------:| ---------------------------------------------------
**JUMP**       | -\*--    | Changes the value of the program counter to the specified address, meaning that in the next cycle execution will continue at that address.  
**IF_MAX**     | -\*-\*   | Jumps to the specified address if register has value _255_ = '********'. 
**IF_MIN**     | -\*\*-   | Jumps to the specified address if register has value _0_ = '--------'. 
**IF_NOT_MAX** | \*\*-*   | Jumps to the specified address if register does not have value _255_ = '********'. 
**IF_NOT_MIN** | \*\*\*-  | Jumps to the specified address if register does not have value _0_ = '--------'. 
**JUMP_REG**   | -\*\*\*-\*\*\* | Jumps to the address that register is pointing to.

### Bitwise/Logic
 _Name_        | _Code_        | _Description_  
:------------- |:-------------:| ---------------------------------------------------
**SHIFT_L**    | -\*\*\*----   | Moves bits in the register one spot to the left.
**SHIFT_R**    |  -\*\*\*---\* | Moves bits in the register one spot to the right.
**AND**        | -\*\*\*--\*-  | Bitwise AND is executed between the value of the register and value at the first address of data ram, and result is written to the register.
**OR**         | -\*\*\*--\*\* | Bitwise OR is executed between the value of the register and value at the first address of data ram, and result is written to the register.
**NOT**        | -\*\*\*-\*--  | Changes value of every bit in the the register. 

Non-interactive mode
--------------------
Non-interactive mode is started if input is piped to the program. In this mode:  
* computer prints to 'stdout', 
* there is no user interface, 
* there is no pause between cycles, 
* and when program reads from last address, it gets one word from 'stdin' (pipe). In this word every '`*`' is interpreted as true and all other characters as false. If word starts with a digit, then it is red as a number and converted appropriately.

Examples
--------
* Multiplication:
```
$ echo "3 4" | ./comp examples/mulToSum | ./comp examples/sumAll
-----**-   6
----*--*   9
----**--  12
```

* Moving single bit with 'a' and 'd' keys:
```
$ examples/keyFilter | ./comp examples/randomDot
----*---   8
-----*--   4
----*---   8
---*----  16
```







