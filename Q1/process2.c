#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h> /*IMPORTANT ---Should be compiled with -lm flag--- */

float calculateSD(float data[], float mean, int size)
{
    float SD = 0.0;
    int i;
    for (i = 0; i < size; ++i)
        SD += pow(data[i] - mean, 2);
    return sqrt(SD / size);
}
float calculateSum(float data[], int size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += data[i];
    }
    return sum;
}

int main(int argc, char *argv[])
{
    float L[5];
    int fd = open("fifo/temperatures", O_RDONLY);
    if (fd == -1)
    {
        printf("Unable to open temperatures to read \n");
        return 1;
    }
    if (read(fd, L, sizeof(L)) == -1)
    {
        printf("Unable to read temperatures from fifo\n");
        return 2;
    }
    close(fd);
    float mean = calculateSum(L, 5) / 5;
    float SD = calculateSD(L, mean, 5);
    // printf("The mean is %f\n", mean);
    // printf("The SD is %f\n", SD);
    fd = open("fifo/analytics", O_WRONLY);
    if (fd == -1)
    {
        printf("Unable to open analytics to write\n");
        return 3;
    }
    if (write(fd, &mean, sizeof(mean)) == -1)
    {
        printf("Unable to write mean to analytics\n");
        return 4;
    }
    if (write(fd, &SD, sizeof(SD)) == -1)
    {
        printf("Unable to write SD to analytics\n");
        return 5;
    }
    if (write(fd, L, sizeof(L)) == -1)
    {
        printf("Unable to write temperatures to anaytics\n");
        return 6;
    }
    close(fd);
    return 0;
}