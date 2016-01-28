Zack Shackleton

Canny.c for Robot Vision (CAP 4453)

Canny.c inputs a pgm file and computes the edges using the Canny algorithm. 


How to run:
-Open Terminal/Console
-Locate C Code location 
-Compile the code with the following
	gcc -o canny Canny.c
	./Canny (Input File) (Sigma Value)

Example:
	$gcc -o canny Canny.c
	$./canny input.pgm 3

Output: 
	magnitude.pgm : Contains the magnitude values of the original image.
	peaks.pgm : Contains only the peaks found from the magnitude values
	doubleThreshold.pgm :  Applies a double threshold to provide the most clear final clear image of the edges