# betterSed

This C program is a simple text editor that reads a series of commands from a file and applies those commands to a given text input. It allows you to perform various operations on the text, such as appending content, inserting content, deleting lines, and replacing text patterns. resembling Unix Sed tool

## Features
Append to the End of Line: You can append content to the end of specific lines in the text.

Insert Content at the Beginning: Content can be inserted at the beginning of specific lines in the text.

Insert Title Before Line: A line containing a title can be inserted before specific lines in the text.

Delete Lines: You can delete specific lines from the text.

Replace Text Patterns: You can replace specific text patterns with other text patterns in the text.

Line Filtering: Commands can be applied to specific lines based on line numbers, content patterns, or applied to all lines.

## Usage
Compile the Program: You need a C compiler to compile the program. Use the following command to compile it:

```
gcc text_editor.c -o text_editor
```
Run the Program: Execute the program with the name of the command file as an argument:


```
./text_editor commands.txt
```
Replace commands.txt with the path to your command file.

Input Text: Provide the text input through standard input (usually by typing or piping from a file). The program will read and process this text based on the commands in the command file.

View Output: The program will apply the specified commands to the input text and display the modified text on the standard output.

Command File Format
The command file contains a series of commands, where each command specifies the action to be performed on the text. Each command should be on a separate line.

```
Command Format: <line_range>/<command>/<arguments>

<line_range> can be a line number or a pattern to match lines.
<command> is one of: I, A, O, d, s (for insert, append, append title, delete, and replace, respectively).
<arguments> are specific to the command type.
```
Examples
Here are some example command file entries:

5,10/I/Insert this at the beginning of lines 5 to 10
/pattern/O/Insert title before lines containing 'pattern'
15/s/old_pattern/new_pattern/Replace 'old_pattern' with 'new_pattern' on line 15
/another_pattern/d/Delete lines containing 'another_pattern'
## Dependencies
This program relies on standard C libraries and does not require any external dependencies.
License
This project is open-source and available under the MIT License.

