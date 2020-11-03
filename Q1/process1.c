#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
    float L[5];
    for (int i = 0; i < 5; i++)
    {
        printf("Enter the temperature setting for thermostat at L%d : ", i + 1);
        scanf("%f", &L[i]);
    }
    int fd = open("fifo/temperatures", O_WRONLY);
    if (fd == -1)
    {
        printf("Unable to open temperatures to write \n");
        return 1;
    }
    if (write(fd, L, sizeof(L)) == -1)
    {
        printf("Unable to write temperatures to fifo\n");
        return 2;
    }
    close(fd);
    fd = open("fifo/categories", O_RDONLY);
    char tempCat[5][5];
    if (fd == -1)
    {
        printf("Unable to open categories to print \n");
        return 3;
    }
    if (read(fd, tempCat, sizeof(tempCat)) == -1)
    {
        printf("Unable to catgories from categories\n");
        return 4;
    }
    close(fd);
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("%s\n", tempCat[i]);
    // }
    for (int i = 0; i < 5; i++)
    {
        if (!strcmp(tempCat[i], "Cat0"))
        {
            L[i] = L[i];
        }
        else if (!strcmp(tempCat[i], "Cat1"))
        {
            L[i] -= 3;
        }
        else if (!strcmp(tempCat[i], "Cat2"))
        {
            L[i] -= 1.5;
        }
        else if (!strcmp(tempCat[i], "Cat3"))
        {
            L[i] += 2;
        }
        else if (!strcmp(tempCat[i], "Cat4"))
        {
            L[i] += 2.5;
        }
    }
    printf("The revised temperatures of each location are : \n");
    for (int i = 0; i < 5; i++)
    {
        printf("L%d : %f\n", i + 1, L[i]);
    }
    return 0;
}