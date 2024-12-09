#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
int main(int argc,  char *argv[], char *envp[])
{
    key_t key = ftok("shmfile", 65);
    int shmid;
    if((shmid = shmget(key, 16, 0666 | IPC_CREAT | IPC_EXCL))<0)
    {
        if (errno!=EEXIST)
        {
            std::cout<<"Can't create shared memory"<<std::endl;
            exit(-1);
        }
        else
        {
            if((shmid = shmget(key, 16, 0))<0)
            {
                std::cout<<"Can't find shared memory"<<std::endl;
                exit(-1);
            }
        }

    }
    int *n;
    if ((n = (int *)shmat(shmid, NULL, 0))==(int*)(-1))
    {
        std::cout<<"Can't attach share memory"<<std::endl;
        exit(-1);
    }
    *n=0;
    int N{atoi(argv[1])};
    std::string in{};
    std::cout<<"Enter UNIX-commands: "<<std::endl;
    std::string fin{"fin"};
    pid_t pid{}, pid2{};
    std::getline(std::cin, in);
    while (in!=fin)
    {
        if (*n==N)
        {
            std::cout<<"The amount of working commands is already N="<<N<<" there can not be more!"<<std::endl;
        }
        else
        {
            pid= fork();
            if (pid==0)
            {
                (*n)++;
                system(in.c_str());
                (*n)--;
                shmdt(n);
                exit(1);
            }
        }
        std::getline(std::cin, in);
    }
    shmdt(n);
    return 0;
}