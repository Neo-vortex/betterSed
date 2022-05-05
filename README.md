# betterSed

Edit Commands

Each edit command consists of an optional line range specification, an edit operation specifier,

and data required by the edit operation. I assume that all commands in command files

are correctly formatted.

There are three types of line range specifications:

/<text>/
  
This format specifies all lines that contain the <text>. The <text> can be maximally 80
  
characters.
  
<1st line number>,<last line number>/
  
This format specifies all lines in the range <1st line number> to <last line number>,
  
inclusive. Lines in the input file are considered to be numbered from 1, and this
  
numbering is not changed in the editing process.
  
No line range specification specifies all lines in the file.
  
There are five edit operations:
  
A<text>
  
Appends the <text> at the end of the line. For example:
  
Ahello jim
  
appends hello jim to all lines in the file. The <text> can be maximally 80 characters.
  
I<text>
  
Inserts the <text> at the start of the line. For example:
  
/blah/Ineedle noddle noo
  
inserts needle noddle noo at the start of all lines that contain blah. The <text>
  
can be maximally 80 characters.
  
O<text>
  
Inserts the <text> on a new line before the current line. For example:
  
1,1/OThe Title
  
Inserts a new line before the first line in the file, containing the text The Title. The
  
<text> can be maximally 80 characters. The new line is not put through the editing
  
process.
  
d
Deletes the line from the file. For example:
  
3,6/d
  
deletes lines 3 to 6 inclusive.
  
s/<old text>/<new text>/
  
Replaces the first occurence of <old text>, in the line, with <new text>. For
  
example:
  
/filename/s/.pas/.c/
  
replaces the first occurrence of .pas with .c, in all lines containing filename. The
  
<old text> and <new text> can be maximally 80 characters.
