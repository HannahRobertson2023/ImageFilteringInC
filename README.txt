assuming you have the permissions to execute the file on your machine

To Build:
gcc -c RobertsonFilters.c -o <desiredProgramName> -lm -pthread

To Run:
./<desiredProgramName> -i <intputfile>.bmp -o <outputfile>.bmp -f s
(of course, you put 'b' instead of the 's' to blur)