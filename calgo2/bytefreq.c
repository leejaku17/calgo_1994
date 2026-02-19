/*                                                                  */
/*   BYTEFREQ.C  :  Check frequency of bytes    	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>

long freq[256];

void clear_freq(void)
{
    int i;
    for (i = 0; i < 256; i++)
        freq[i] = 0;
}

void print_freq(void)
{
    int i;
    long min;
    int mindex;
    min = freq[0];
    mindex = 0;
    for (i = 1; i < 256; i++)
    {
        if (freq[i] < min)
        {
            min = freq[i];
            mindex = i;
        }
    }
    printf("\nLeast frequency is %x : %ld", mindex, min);
}

void main(int argc, char *argv[])
{
    FILE *fp;
    int c;
    if (argc < 2)
    {
        printf("\n Usage : BYTEFREQ [file_name]");
        exit(1);
    }
    clear_freq();
    if ((fp = fopen(argv[1], "rb")) == NULL)
    {
        printf("\n File not found !");
        exit(1);
    }
    while (!feof(fp))
    {
        c = getc(fp);
        freq[c]++;
    }
    fclose(fp);
    print_freq();
}