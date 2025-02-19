# Multithreading using the Encoding Method for Sparse Binary Data

Given a multiline input representing the image to decode using the Encoding Method for Sparse Binary Data with the following information per line:
1. Image size: two positive integer values greater than zero with the size of the image (length and width). The integer values are separated by a single white space.
2. Output characters: a comma-separated line with the symbols to print. Each symbol will be followed by the range(s) in the x-axis where it will be printed (a symbol can have multiple ranges). The symbol (printable ASCII characters) and its range(s) (at least two positive integer values) are separated by a single white space. The information about each character is separated by a comma.
3. The values for the headPos array (review the paper above). This line has multiple positive integer values (including zero)
separated by a single white space.
4. The values for the dataPos array (review the paper above). This line has multiple positive integer values (including zero) separated by a single white space.
