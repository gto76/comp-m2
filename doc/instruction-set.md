Instruction set
---------------

### Read and Write
 _Name_        | _Code_ | _Description_
:------------- |:--------| ---------------------------------------------------
**READ**       | `····`     | Copies the value at the specified address into register.
**WRITE**      | `···✷`    | Copies value of the register to the specified address.
**READ_***     | `✷···`    | Copies the value that specified address is pointing to into register.
**WRITE_***    | `✷··✷`   | Copies value of the register to the address, that specified address is pointing to.
**PRINT**      | `✷·✷✷`  | Copies the value at the specified address to the last address of data ram, thus sends it to the printer.
**READ_REG**   | `·✷✷✷···✷` | Copies the value at address specified by the last four bits of the register into register.
**INIT**       | `·✷✷✷··✷·` | Overwrites value at the first address of the data ram with value at the second address, and copies it to the register.

### Arithmetic
 _Name_        | _Code_ | _Description_
:------------- |:--------| ---------------------------------------------------
**ADD**        | `··✷·`    | Adds value at the specified address to the value of the register, and writes result to the register. If result is bigger than the maximum possible value of _255_ = '********', then _255_ gets written.
**SUB**        | `··✷✷`   | Subtracts value at the specified address from the value of the register, and writes result to the register. If result is smaller than _0_, then _0_ gets written.
**INC**        | `✷·✷··`  | Adds 1 to the value at the address, specified by the last three bits of the instruction, and copies it to the register. If result is bigger than the maximum possible value of _255_ = '********', then _0_ gets written to both address and the register.
**DEC**        | `✷·✷·✷` | Subtracts 1 from the value at the address, specified by the last three bits of the instruction, and copies it to the register. If result is smaller than _0_, then _255_ = '********' gets written to both address and the register.

### Execution Control
 _Name_        | _Code_ | _Description_
:------------- |:--------| ---------------------------------------------------
**JUMP**       | `·✷··`    | Changes the value of the program counter to the specified address, meaning that in the next cycle execution will continue at that address.
**IF_MAX**     | `·✷·✷`   | Jumps to the specified address if register has value _255_ = '********'.
**IF_MIN**     | `·✷✷·`   | Jumps to the specified address if register has value _0_ = '--------'.
**IF_NOT_MAX** | `✷✷·✷`   | Jumps to the specified address if register does not have value _255_ = '********'.
**IF_NOT_MIN** | `✷✷✷·`  | Jumps to the specified address if register does not have value _0_ = '--------'.
**JUMP_REG**   | `·✷✷✷····` | Jumps to the address, specified by the last four bits of the register.

### Bitwise Logic
 _Name_        |  _Code_     | _Description_
:------------- |:-------------| ---------------------------------------------------
**NOT**        | `·✷✷✷··✷✷`  | Changes value of every bit in the the register.
**SHIFT_L**    | `·✷✷✷·✷··`   | Moves bits in the register one spot to the left.
**SHIFT_R**    | `·✷✷✷·✷·✷` | Moves bits in the register one spot to the right.
**AND**        | `·✷✷✷·✷✷·`  | Bitwise AND is executed between the value of the register and value at the third address of data ram, and result is written to the register.
**OR**         | `·✷✷✷·✷✷✷` | Bitwise OR is executed between the value of the register and value at the fourth address of data ram, and result is written to the register.
**XOR**        | `·✷✷✷✷` | Bitwise XOR is executed between the value of the register and value at the address  specified by last three bits of the instruction. Result is written to the register.
