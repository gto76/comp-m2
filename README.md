Comp Mark II
============

Updated version of [**Comp**](https://github.com/gto76/comp-cpp) – Simple 4-bit/1 Hz virtual computer for learning purposes

![screenshot](doc/screenshot.png)

Overview
--------

* Processor has one 8 bit register.
* Ram is separated into two address spaces; one for instructions, called CODE, and one for data, called DATA.
* All instructions are 8 bits long.
* Execution starts at the first address (0) of CODE ram.
* Execution stops when program reaches last address (15) of CODE ram.
* Most of instructions consist of instruction code and address:
```
instruction code - 4 bits
  |  +-- address - 4 bits
  v  v
----***-  ->  READ 14  ->  Copy value stored at address 14 of "DATA" ram to register.
```
* All addresses specified by instructions refer to DATA ram, except for addresses of *Execution Control* instructions (`JUMP`, `IF MAX`, `IF MIN`, ...). They refer to CODE part of ram.
* Some instructions ([`JRI~<>&VX`](##Instruction set)) do not specify address. They operate on register (`SHIFT L/R`, `NOT`, ...) or between register and predefined DATA address (`AND`, `OR`, ...).
* Whatever gets written to the last address is sent to the printer, or to 'stdout' if program is running in non-interactive mode.
* When reading from the last address (15), we get a random byte value, or a single word from 'stdin' if program is running in non-interactive mode.
* In this word every `*` is interpreted as true and all other characters as false. If word starts with a digit, it is then red as a number and converted appropriately.
* Program will start in non-interactive mode when any input is piped in. For instance `echo | ./comp <file>`
* Programs can be saved with `s` key and loaded by passing their name as parameter.
* If more than one filename is passed, the computers will be chained together (input > comp1 > comp2 > ... > output).
* If folder is passed, all files that have suffix `.cm2` will be loaded in alphabetical order.

Instruction set
---------------
Most of the instructions, together with the highlighted region of the ram that they use/modify, are pretty self-explainatory. Exception are instructions, that start with `-***`, and are a part of **JRI~<>&VX** instruction cluster. They are:  
 * `J` – JUMP_REG,
 * `R` – READ_REG,
 * `I` – INIT,
 * `~` – NOT,
 * `<` – SHIFT_L,
 * `>` – SHIFT_R,
 * `&` – AND,
 * `V` – OR and
 * `X` – XOR.

Detailed descriptions of all instructions can be found [**HERE**](doc/instruction-set.md).

Keys
----
* `Space` – Change bits value,
* `Tab` – Switch address space,
* `Enter` – Start/pause execution,
* `Esc` – Cancel execution,
* `x` – Delete word,
* `Delete` – Delete word and move remaining words up,
* `Insert` – Insert blank word and move remaining words down,
* `s` – Save ram to textfile named `saved-ram-<num>`. To load it, start program with `./comp <file>`

Detailed descriptions of all keys can be found [**HERE**](doc/keys.md).

How to run on…
--------------

### Windows

* Install *Tiny Core Linux* on *VirtualBox* using this [**instructions**](https://github.com/gto76/my-linux-setup/tree/gh-pages/conf-files/tiny-core-linux).
* Run the *UNIX* commands.

### UNIX
```bash
$ git clone https://github.com/gto76/comp-m2.git
$ cd comp-m2
$ ./run
```

Example Programs
--------
#### Multiply
```
$ echo "3 4" | ./comp examples/multiply
----**--  12
```

#### Random Dot
```
$ examples/keyFilter | ./comp examples/randomDot
----*---   8
-----*--   4
----*---   8
---*----  16
...
```
