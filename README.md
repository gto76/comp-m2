Comp Mark II
============

Updated version of [**Comp**](https://github.com/gto76/comp-cpp) – Simple 4-bit/1 Hz virtual computer for learning purposes

![screenshot](doc/screenshot.png)

Memory
------
```
instructions - 4 bits
  |  +-- addresses - 4 bits
  v  v
----***-  <- 0  ----
--*-**-*  <- 1  ---*
---***--  <- 2  --*-
---*****  <- 3  --**
----***-  <- 4  -*--
---***-*  <- 5  -*-*
----**--  <- 6  -**-
---****-  <- 7  -***
-***---*  <- 8  *---
-*--****  <- 9  *--*
--------  <- 10 *-*-
--------  <- 11 *-**
--------  <- 12 **--
-------*  <- 13 **-*
-------*  <- 14 ***-
<OUTPUT>  <- 15 ****
```

* Execution starts at the first address (0). 
* Execution stops when program reaches last address (15).
* Whatever gets written to the last address is sent to the printer.
* Computer has one 8 bit register.
* When reading from the last address (15), we get a random byte value.

Instruction set
---------------

 Name                 | Code   | Description  
:----------- |:------:| -------------
**READ**       | `----` | Copies the value at the specified address into register.  
**WRITE**     | `---*` | Copies value of the register to the specified address.  
**ADD**        | `--*-` | Adds value at the specified address to the value of the register, and writes result to the register. If result is bigger than the maximum possible value of `255 = '********'`, then `255` gets written. 
**SUB**        | `--**` | Subtracts value at the speicfied address from the value of the register, and writes result to the register. If result is smaller than `0`, then `0` gets written.  
**JUMP IMD**   | `-*--` | Changes the value of the program counter to the specified address, meaning that in the next cycle execution will continue at that address.  
**`IF_MAX`**     | `-*-*` | Jumps to the specified address if register has value `255 = '********'`. 
**`IF_MIN`**     | `-**-` | Jumps to the specified address if register has value `0 = '--------'`. 
**`SHIFT_L/R`**  | `-***` | Moves bits in the register in the direction specified by the value. If first bit of the value is `'-'`, then it moves them to the left, otherwise to the right. Remaining three bits specify the number of spots, that they get moved.
**`READ_IN`**    | `*---` | Copies the value at the last address (random or stdin) to the specified address.
**`WRITE_OUT`**  | `*--*` | Copies the value at the specified address to the last address (printer or stdout).
**`ADD_IMD`**    | `*-*-` | Adds specified value to the value of the register, and writes result to the register. If result is bigger than the maximum possible value of `255 = '********'`, then `255` gets written. 
**`SUB_IMD`**    | `*-**` | Subtracts specified value from the value of the register, and writes result to the register. If result is smaller than `0`, then `0` gets written.
**`JUMP`**       | `**  ` |
**`IF_NOT_MAX`** | `**-*` | Jumps to the specified address if register does not have value `255 = '********'`. 
**IF NOT MIN** | `***-` | Jumps to the specified address if register does not have value `0 = '--------'`. 
**`AND/OR`**     | `****` | If first bit of the value is `'-'`, then AND gets executed, otherwise OR. Other three bits specify the address. Operation is executed between the value of register and value at the specified address, and result is written to register. Since only three bits are used for the address, this instruction can only be used with first eight addresses of the data ram.

Keys
----
* `space`, `f` - switch bit under cursor
* `enter` - start/pause execution
* `esc` - cancel execution
* `d` - delete word under cursor
* `j` - move word up
* `k` - move word down
* `s` - save ram in textfile named `saved-ram-<num>`. To load it, start program with `./comp <file>`

Non-interactive mode
------------------
Non-interactive mode is started if any input is piped to the program. In this mode computer prints to 'stdout', there is no
user interface, there is no pause between cycles, and when program reads from last address, instead of random value, it
gets one word from 'stdin' (pipe). In this word every '`*`' is interpreted as true and all other characters as false. If there
is not enough characters, all missing bits are set to false; and all characters after the eight one are ignored. If word starts with a digit, then it is red as a number and converted appropriately.

Multiplication:
```
$ echo "3 4" | ./comp examples/mulToSum | ./comp examples/sumAll
-----**-   6
----*--*   9
----**--  12
```

Moving a bit with 'a' and 'd' keys:
```
$ examples/keyFilter | ./comp examples/randomDot
----*---   8
-----*--   4
----*---   8
---*----  16
```


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






