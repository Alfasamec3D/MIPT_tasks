#include<iostream>
#include<unistd.h>
#include <fstream>
#include <sys/shm.h>
#include<vector>
#include<map>
#include<string>

 int main()
{
    setenv("TABLE_LIMIT", "3",1);
    
    const int N{atoi(std::getenv("TABLE_LIMIT"))};

    int shmid;
    
    std::vector<int> wt{};
    
    std::map<std::string, int> dish_id{};
    
    std::ifstream inf{"wash_time.txt"}; //opening dry_time.txt
    
    std::string tempstr{}, dish{};

    int * Table;
    
    int tempint{};

    int id{0};

    const key_t key{12345};
    
    while (inf>>tempstr)
    {
     
        inf>>tempint;

        dish_id[tempstr]=id;
     
        wt.push_back(tempint);

        id++;
    }
    
    inf.close();              //closing wash_time.txt
    
    if((shmid = shmget(key, (N+1) * sizeof(int), IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(-1);
    }
    
    if((Table = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
    {
        printf("Can't attach shared memory\n");
        
        exit(-1);
    }
    
    for (int i=0;i<=N;i++)
    {
        Table[i]=-1;
    }
   
    inf.open("dishes.txt");

    Table[N]=-2;

    while (inf>>tempstr)
    {
        inf>>tempint;

        for (int i=1;i<=tempint;i++)
        {
            sleep (wt[dish_id[tempstr]]);
               
            std::cout<<"The washer washed the "<<tempstr<<std::endl;
            
            while ((Table[N-1])!=-1){}

            for (int i=0; i<=N-1; i++)
            {
                if (Table[i]==-1)
                {
                    Table[i]=dish_id[tempstr];
                    break;
                }
            }
        }
    }

    Table[N]=-1;

    shmdt(Table);
    return 1;
}