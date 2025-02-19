# Multithreading using the Encoding Method for Sparse Binary Data

Given a multiline input representing the image to decode using the Encoding Method for Sparse Binary Data with the following information per line:
1. ***Image size:*** two positive integer values greater than zero with the size of the image (length and width). The integer values are separated by a single white space.
2. ***Output characters:*** a comma-separated line with the symbols to print. Each symbol will be followed by the range(s) in the x-axis where it will be printed (a symbol can have multiple ranges). The symbol (printable ASCII characters) and its range(s) (at least two positive integer values) are separated by a single white space. The information about each character is separated by a comma.
3. The values for the headPos array (review the paper above). This line has multiple positive integer values (including zero)
separated by a single white space.
4. The values for the dataPos array (review the paper above). This line has multiple positive integer values (including zero) separated by a single white space.

***Process:***
 
Your solution must execute the following steps:
 
* Read the input lines from STDIN.
* Create n POSIX threads (where n is the number of lines in the image (width)). Each child thread executes the following tasks:
  * Receives the line number to decode, the image size, the headPos and dataPos arrays, the information about the symbols to print from the main thread, and the memory location to store the decoding process results.
  * Decodes the assigned line using the Encoding Method for Sparse Binary Data.
  * Stores the decoded line on a memory location accessible by the main thread.
* Print the decoded image into STDOUT.

Input example:
26 7
U 0 10,H 15 25 
0 4 8 12 25 29 33
0 10 15 25 0 10 15 25 0 10 15 25 0 10 15 16 17 18 19 20 21 22 23 24 25 0 10 15 25 1 9 15 25 2 3 4 5 6 7 8 15 25
 
The expected output is:

'''U         U    H         H'''
'''U         U    H         H'''
U         U    H         H
U         U    HHHHHHHHHHH
U         U    H         H
 U       U     H         H
  UUUUUUU      H         H
