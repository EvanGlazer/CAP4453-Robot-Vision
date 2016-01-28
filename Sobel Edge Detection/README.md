# Sobel-Edge-Detection

Zack Shackleton

Sobel.c for Robot Vision (CAP 4453)

Sobel.c inputs a pgm file and computes the edges using the Sobel algorithm. 


How to run:
-Open Terminal/Console
-Locate C Code location 
-Compile the code with the following
    gcc -o sobel Sobel.c
    ./sobel (Input File) (Low Threshold Number) (High Threshold Number)

Example:
    $gcc -o sobel Sobel.c
    $./sobel input.pgm 30 50

Output: 
    magnitude.pgm : Contains the magnitude values of the original image.
    lowThreshold.pgm : Contains the values above the low threshold entered in the command line.
    highThreshold.pgm :  Contains the values above the high threshold entered in the command line.