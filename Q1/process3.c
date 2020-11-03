#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    float L[5];
    int fd = open("fifo/analytics", O_RDONLY);
    float mean;
    float SD;
    if (fd == -1)
    {
        printf("Unable to open analytics to read \n");
        return 3;
    }
    if (read(fd, &mean, sizeof(mean)) == -1)
    {
        printf("Unable to read mean\n");
        return 4;
    }
    if (read(fd, &SD, sizeof(SD)) == -1)
    {
        printf("Unable to read SD \n");
        return 5;
    }
    if (read(fd, L, sizeof(L)) == -1)
    {
        printf("Unable to read temperatures\n");
        return 6;
    }
    close(fd);
    char cat[5][5] = {
        "Cat0",
        "Cat1",
        "Cat2",
        "Cat3",
        "Cat4"};
    char tempCat[5][5];
    for (int i = 0; i < 5; i++)
    {
        if (L[i] == mean)
        {
            strcpy(tempCat[i], cat[0]);
        }
        else if (L[i] > (mean + SD))
        {
            strcpy(tempCat[i], cat[1]);
        }
        else if (L[i] > mean && L[i] < (mean + SD))
        {
            strcpy(tempCat[i], cat[2]);
        }
        else if (L[i] < mean && L[i] > (mean - SD))
        {
            strcpy(tempCat[i], cat[3]);
        }
        else if (L[i] < (mean - SD))
        {
            strcpy(tempCat[i], cat[4]);
        }
    }
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("%s\n", tempCat[i]);
    // }
    fd = open("fifo/categories", O_WRONLY);
    if (fd == -1)
    {
        printf("Unable to open categories to write\n");
        return 7;
    }
    if (write(fd, tempCat, sizeof(tempCat)) == -1)
    {
        printf("Unable to write categoies to categories\n");
        return 8;
    }
    close(fd);
    return 0;
}