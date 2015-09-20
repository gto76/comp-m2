Keys
----

### Common
_Key_           | _Description_
:-------------- | ----------------------------------
Enter           | Start/pause execution.
Esc             | Cancel execution.
s               | Save to new file.
S               | Save.
Period (.)      | Change scene (character set).

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

### Movement (Vim only)
 _Vim Key_  | _Description_
:---------- | ---------------------------------------------------
g           | Go to first word.
G           | Go to last word.
e           | Go to end of current word, or end of next if already at end.
b           | Go to beginning of current word, or beginning of last if already at beginning.
w           | Go to beginning of next word.
a           | Go to fourth bit of instruction.

### Manipulation
 _Key_          |  _Vim Key_  | _Description_
:-------------- | ----------- | ----------------------------------
Space           |             | Change bits value.
Insert          |             | Insert blank word and move all following words one spot lower.
Delete          | X           | Delete word, and move all following words one spot higher. Word gets copied to clipboard.
Page Up, Shift–Up Arrow | K           | Move current word up, and the preceding word down.
Page Down, Shift–Down Arrow | J           | Move current word down, and the following word up.
c               | y           | Copy word.
v               | p           | Paste word.

### Manipulation (Vim only)
 _Vim Key_ | _Description_
:--------- | ---------------------------------------------------
f          | Switch current bit on, and move cursor one spot to the right.
d          | Switch current bit off, and move cursor one spot to the right.
x          | Delete word and copy it to clipboard.
o          | Move to next word and insert blank word. Move all following words one spot lower.
P          | Paste word and move all following words one spot lower.

### Value insertion (works only on DATA)
 _Vim Key_ | _Description_
:--------- | ---------------------------------------------------
i          | Inset character.
I          | Insert number.
