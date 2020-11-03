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

    key = 9898;

    shmid = shmget(key, SHSIZE, 0666);
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
    int alnum = 0;
    int special = 0;
    s2 = shm2;
    alnum = *s2;
    s2 += sizeof(int);
    special = *s2;
    *shm2 = -1;

    //Storing back the result
    key = 9890;

    shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
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
    char str[10];
    if (alnum < special)
    {
        strcpy(str, "Strong");
    }
    else
    {
        strcpy(str, "Weak");
    }
    s = shm;
    *s = '~';
    s++;
    memcpy(s, str, strlen(str));
    s += strlen(str);
    *s = 0;
    while (*shm != '*')
        sleep(1);
    return 0;
}