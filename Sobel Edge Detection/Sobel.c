#include <stdio.h>                          /* Sobel.c */
#include <math.h>
#include <stdlib.h>
#define PICSIZE 256

        int pic[PICSIZE][PICSIZE];
        int outpicx[PICSIZE][PICSIZE];
        int outpicy[PICSIZE][PICSIZE];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        double ival[PICSIZE][PICSIZE],maxival;

int main(int argc, char** argv)
{
    int i,j,p,q,mr,sum1,sum2;
    double bigThreshold, smallThreshold;
    FILE *fo1, *fo2, *fo3, *fp1;
    char *foobar;

    // Scanning in the input file
    argc--; argv++;
    foobar = *argv;
    fp1 = fopen(foobar,"rb");

    // Scanning in the small threshold value
    argc--; argv++;
	foobar = *argv;
	smallThreshold = atof(foobar);

    // Scanning in the big threshold value
    argc--; argv++;
    foobar = *argv;
    bigThreshold = atof(foobar);


    // Removes the first 15 characters of the pgm
    // These characters are the pgm info specifications
    for (i = 0; i < 15; i++) {
        getc(fp1);
    }

    // Scanning each pixel value into the pic array
    for (i = 0; i < PICSIZE; i++)
    {   for (j = 0; j < PICSIZE; j++)
        {
            pic[i][j] = getc(fp1);
        }
    }

    // Using a mask radius and convoluting in both the
    //   x and y directions
    mr = 1;
    for (i = mr; i < PICSIZE-mr; i++)
    {   for (j = mr; j < PICSIZE-mr; j++)
        {
            sum1 = 0;
            sum2 = 0;
            for (p = -mr;p <= mr; p++)
            {
               for (q = -mr;q <= mr; q++)
               {
                  sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                  sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
               }
            }
            
            outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
        }
    }

    // Combining the x and y convolutions
    maxival = 0;
    for (i = mr; i < PICSIZE-mr; i++)
    { for (j = mr; j < PICSIZE-mr; j++)
      {
         ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                  (outpicy[i][j]*outpicy[i][j])));

         if (ival[i][j] > maxival)
            maxival = ival[i][j];
       }
    }

    

    // Creating the pgm files for output
    fo1 = fopen("magnitude.pgm", "wb");
    fo2 = fopen("lowThreshold.pgm", "wb"); 
    fo3 = fopen("highThreshold.pgm", "wb");

    // Adding header to pgm files
    fprintf(fo1, "P5\n%d %d\n255\n", PICSIZE, PICSIZE);
    fprintf(fo2, "P5\n%d %d\n255\n", PICSIZE, PICSIZE);
    fprintf(fo3, "P5\n%d %d\n255\n", PICSIZE, PICSIZE);

    // Printing values to pgm files
    for (i=0;i<PICSIZE;i++)
    {   for (j=0;j<PICSIZE;j++)
        {
            // Adds shades of grey to the output with no threshold
            ival[i][j] = (ival[i][j] / maxival) * 255;            
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));

            // If the value is above the small threshold, 
            //   white will be added to the output fo3
            if (ival[i][j] > smallThreshold) 
            {
                fprintf(fo2, "%c", (char)255);
            } 

            else 
            {
                fprintf(fo2, "%c", (char)0);
            }

            // If the value is above the larger threshold, 
            //  white will be added to the output fo2
            if (ival[i][j] > bigThreshold) 
            {
               fprintf(fo3, "%c", (char)255);
            } 

            else 
            {
               fprintf(fo3, "%c", (char)0);
            } 
        }
    }

    // Closing the files
    fclose(fo1);
    fclose(fo2);
    fclose(fo3);

    return 0;
}