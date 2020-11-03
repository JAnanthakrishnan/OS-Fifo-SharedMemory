#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHSIZE 100
int main(int argc, char *argv[])
{
    int shmid;
    key_t key;
    char *shm;
    char *s;

    key = 9876;

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
    char password[10];
    printf("Enter the password of length 10 :");
    scanf("%s", password);
    memcpy(shm, password, strlen(password));
    s = shm;
    s += strlen(password);
    *s = 0;
    printf("Wating for process 2 ....\n");
    while (*shm != '*')
        sleep(1);

    //Getting back the result
    key = 9890;

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
    s = shm;
    printf("Waiting for process 3 ....\n");
    while (*s != '~')
    {
        sleep(1);
    }
    printf("The password is : ");
    for (s = shm + 1; *s != 0; s++)
    {
        printf("%c", *s);
    }
    printf("\n");
    *shm = '*';
    return 0;
}