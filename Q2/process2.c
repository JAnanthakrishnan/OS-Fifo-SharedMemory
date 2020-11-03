#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>

#define SHSIZE 100
int main(int argc, char *argv[])
{
    int shmid;
    key_t key;
    char *shm;
    int *shm2; //for storing counts
    char *s;
    int *s2; //for shm2

    key = 9876;

    shmid = shmget(key, SHSIZE, 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    shm = shmat(shmid, NULL, 0);

    if (shm == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    int alnum = 0;
    int special = 0;
    for (s = shm; *s != 0; s++)
    {
        // printf("%c", *s);
        if (isalnum(*s))
        {
            alnum++;
        }
        else
        {
            special++;
        }
    }
    printf("%d %d", alnum, special);
    printf("\n");
    *shm = '*';

    //for storing alnum,special
    key = 9898;
    shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget");
        exit(1);
    }

    shm2 = (int *)shmat(shmid, NULL, 0);

    if (shm2 == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }
    s2 = shm2;
    memcpy(s2, &alnum, sizeof(int));
    s2 += sizeof(int);
    memcpy(s2, &special, sizeof(int));
    s2 += sizeof(int);
    *s2 = 0;
    while (*shm2 != -1)
    {
        sleep(1);
    }

    return 0;
}