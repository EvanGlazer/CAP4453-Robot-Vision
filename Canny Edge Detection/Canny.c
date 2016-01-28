#include <stdio.h>                  /*  Canny.c  (or canny.c) */
#include <math.h>
#include <stdlib.h>
#define  PICSIZE 256
#define  MAXMASK 100

     int    pic[PICSIZE][PICSIZE];
     int    edgeflag[PICSIZE][PICSIZE];
     double xoutpic1[PICSIZE][PICSIZE];
     double youtpic1[PICSIZE][PICSIZE];
     double final[PICSIZE][PICSIZE];
     double xmask[MAXMASK][MAXMASK];
     double ymask[MAXMASK][MAXMASK];
     double xconv[PICSIZE][PICSIZE];
     double yconv[PICSIZE][PICSIZE];
     double magnitude[PICSIZE][PICSIZE];
     int histogram[PICSIZE][PICSIZE];

int main(int argc, char **argv) {

    int     i, j, p, q, mr, centx, centy, moretodobool;
    double  xmaskval, ymaskval, xsum, ysum, sig, maxival, minival, maxval, high, low, slope;
    FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
    char    *foobar;
    
    // Scanning in input pgm file
    argc--; argv++;
    foobar = *argv;
    fp1=fopen(foobar,"rb");

    // Scanning in the sigma value
    argc--; argv++;
    foobar = *argv;
    sig = atof(foobar);

    // Scanning in the high value
    argc--; argv++;
    foobar = *argv;
    high = atof(foobar);

    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);

    // Removes the first 15 characters of the pgm
    // These characters are the pgm info specifications
    for (i = 0; i < 15; i++) {
        getc (fp1);
    }

    // Scanning each pixel value into the pic array
    for (i = 0; i < PICSIZE; i++) {   
        for (j = 0; j < PICSIZE; j++) {
                pic[i][j]  =  getc (fp1);
        }
    }

    // Creating the flexible mask with the first
    //   derivative of both the x and y direction
    for (p = -mr; p <= mr; p++) {   
        for (q = -mr; q <= mr; q++) {
            xmaskval = -(p * (exp(-(1/2) * (p/sig) * (p/sig))));
            ymaskval = -(q * (exp(-(1/2) * (q/sig) * (q/sig))));
            
            xmask[p+centy][q+centx] = xmaskval;
            ymask[p+centy][q+centx] = ymaskval;
        }
    }

    // Convolution of the file with the masks created above
    for (i = mr; i <= 255-mr; i++) {   
        for (j= mr;j <= 255-mr; j++) {
            xsum = 0;
            ysum = 0;
            
            for (p = -mr; p <= mr; p++){
                for (q = -mr; q <= mr; q++){
                    xsum += pic[i+p][j+q] * xmask[p+centy][q+centx];
                    ysum += pic[i+p][j+q] * ymask[p+centy][q+centx];
                }
            }
         
            xoutpic1[i][j] = xsum;
            youtpic1[i][j] = ysum;
            
            xconv[i][j] = xsum;
            yconv[i][j] = ysum;
        }
    }
    
    // Combining the values of x and y convoltions
    // Maxival is the largest value in the magnitude array
    maxival = 0;
    for (i = mr; i < PICSIZE-mr; i++) {   
        for (j = mr; j < PICSIZE-mr; j++) {
            magnitude[i][j]=sqrt((double)((xoutpic1[i][j]*xoutpic1[i][j]) +
                                  (youtpic1[i][j]*youtpic1[i][j])));

            if (magnitude[i][j] > maxival)
                maxival = magnitude[i][j];
        }
    }

    // Creating and printing the magnitude to the pgm file
    fo1=fopen("magnitude.pgm", "wb");
    fprintf(fo1, "P5\n%d %d\n255\n", PICSIZE, PICSIZE);

    for (i = 0; i < PICSIZE; i++) { 
        for (j = 0; j < PICSIZE; j++) {
            magnitude[i][j] = (magnitude[i][j] / maxival) * 255;            
            fprintf(fo1,"%c",(char)((int)(magnitude[i][j])));
        }
    }

    fclose(fo1);


    // Finds and marks the peaks in the magnitude array
    for(i = mr; i < PICSIZE-mr; i++) {   
        for(j = mr;j < PICSIZE-mr; j++) {
            if((xconv[i][j]) == 0.0) {
                xconv[i][j] = .00001;
            }
            
            slope = yconv[i][j]/xconv[i][j];
            
            if( (slope <= .4142)&&(slope > -.4142)){   
                if((magnitude[i][j] > magnitude[i][j-1])&&(magnitude[i][j] > magnitude[i][j+1])) {
                    edgeflag[i][j] = 255;
                }
            }
      
            else if( (slope <= 2.4142)&&(slope > .4142)) {
                if((magnitude[i][j] > magnitude[i-1][j-1])&&(magnitude[i][j] > magnitude[i+1][j+1])) {
                    edgeflag[i][j] = 255;
                }
            }

            else if( (slope <= -.4142)&&(slope > -2.4142)) {
                if((magnitude[i][j] > magnitude[i+1][j-1])&&(magnitude[i][j] > magnitude[i-1][j+1])) {
                    edgeflag[i][j] = 255;
                }
            }

            else {
                if((magnitude[i][j] > magnitude[i-1][j])&&(magnitude[i][j] > magnitude[i+1][j])) {
                    edgeflag[i][j] = 255;
                }
            }
        }
    }

    // Creating and printing the peaks to the pgm file
    fo2=fopen("peaks.pgm","wb");
    fprintf(fo2, "P5\n%d %d\n255\n", PICSIZE, PICSIZE);

    for (i = 0; i < PICSIZE; i++) {   
        for (j = 0; j < PICSIZE; j++) {
            fprintf(fo2, "%c", (char)((int)edgeflag[i][j]));
        }
    }

    fclose(fo2);


    // Automatically calculating the hi and low 
    //   threshold that will be used in the double
    //   threshold calculation below
    // for (i = 0; i < PICSIZE; i++)
    // {   for (j = 0; j < PICSIZE; j++)
    //     {
    //         histogram[i][j] = magnitude[i][j] ++
    //     }
    // }
    low = 0.35 * high;


    // Applying the double threshold and creating
    //   the final array
    for (i = 0; i < PICSIZE; i++) {   
        for (j = 0; j < PICSIZE; j++) {
            if (edgeflag[i][j] == 255) {
                if (magnitude[i][j] > high) {
                    edgeflag[i][j] = 0;
                    final[i][j] = 255;
                } 

                else if (magnitude[i][j] < low) {
                    edgeflag[i][j] = 0;
                    final[i][j] = 0;
                }
            }
        }
    }


    // Choosing what values will stay in the final
    //   array based upon their neighbors
    moretodobool = 1;

    while (moretodobool == 1) {
        moretodobool = 0;
        
        for (i = 0; i < PICSIZE; i++) {   
            for (j = 0; j < PICSIZE; j++) {
                if (edgeflag[i][j] == 255) {
                    for (p=-1;p<=1;p++) {
                        for (q=-1;q<=1;q++) {
                            if (final[i+p][j+p] == 255) {
                                edgeflag[i][j] = 0;
                                final[i][j] = 255;
                                moretodobool = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    // Creating and printing the double threshold to the pgm file
    fo3 = fopen("doubleThreshold.pgm", "wb");
    fprintf(fo3, "P5\n%d %d\n255\n", PICSIZE, PICSIZE);
    
    for (i = 0; i < PICSIZE; i++) {   
        for (j = 0; j < PICSIZE; j++) {
            fprintf(fo3, "%c", (char)((int)final[i][j]));
        }
    }
    
    fclose(fo3);

    return 0;
}
