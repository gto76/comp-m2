Keys
----

### Main
_Key_           | _Description_
:-------------- | ----------------------------------
Enter           | Start/pause execution.
Esc             | Cancel execution.
s               | Save.
S               | Save as.
q               | Save and quit.
Q               | Quit without saving.
Comma (,)       | Switch to simpler view (character set).
Period (.)      | Switch to more complex view (character set).

### Movement
 _Key_          |  _Vim Key_  | _Description_
:-------------- | ----------- | ----------------------------------
Up Arrow        | k           | Move cursor up.
Down Arrow      | j           | Move cursor down.
Right Arrow     | l           | Move cursor right.
Left Arrow      | h           | Move cursor left.
Tab             | t           | Switch address space.
Shift-Tab       | T, z        | Go to instructions address.
Home            | ^           | Go to beginning of word.
End             | $           | Go to end of word.

#### Movement (Vim only)
 _Vim Key_  | _Description_
:---------- | ---------------------------------------------------
g           | Go to first word.
G           | Go to last word.
e           | Go to end of current word, or end of next if already at end.
b           | Go to beginning of current word, or beginning of last if already at beginning.
w           | Go to beginning of next word.
a           | Go to fourth bit of a word (address part).

### Manipulation
 _Key_          |  _Vim Key_  | _Description_
:-------------- | ----------- | ----------------------------------
Space           |             | Flip bit.
Insert          | ;           | Insert blank word and move all following words one spot lower and update addresses.
Delete, Backspace | x           | Delete word and copy to clipboard if not empty, or move all following words one spot higher if empty. All addresses get updated.
Page Up, Shift–Up Arrow | K           | Switch current and preceding word. All addresses get updated.
Page Down, Shift–Down Arrow | J           | Switch current and following word. All addresses get updated.
c               | y           | Copy word.
v               | p           | Paste word.

#### Manipulation (Vim only)
 _Vim Key_ | _Description_
:--------- | ---------------------------------------------------
f          | Switch current bit on, and move cursor one spot to the right.
d          | Switch current bit off, and move cursor one spot to the right.
o          | Move to next word and insert blank word. Move all following words one spot lower. All addresses get updated.
P          | Paste word and move all following words one spot lower. All addresses get updated.

### Value insertion (works only on DATA)
 _Vim Key_ | _Description_
:--------- | ---------------------------------------------------
i          | Inset character.
I          | Insert number.
