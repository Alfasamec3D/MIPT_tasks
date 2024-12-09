 #include<iostream>
 #include<unistd.h>
 #include <fstream>
 #include <sys/shm.h>
 #include<vector>
 #include<string>
 int main()
 {
    setenv("TABLE_LIMIT", "3",1);
    
    const int N{atoi(std::getenv("TABLE_LIMIT"))};
    
    int * Table;
    
    const key_t key{12345};

    char pathname[] = "dryer.cpp";
    
    int shmid, tempint, id;

    std::string tempstr;

    std::vector<int> dt{};

    std::vector<std::string> id_dish{};

    std::ifstream inf{"dry_time.txt"}; //opening dry_time.txt

    while (inf>>tempstr)
    {  

        inf>>tempint;
     
        dt.push_back(tempint);

        id_dish.push_back(tempstr);
    }

    inf.close();

    if((shmid = shmget(key, (N+1) * sizeof(int), 0666)) < 0)
    {
        perror("shmget");
        exit(-1);
    }
    
    if((Table = (int *)shmat(shmid, NULL, 0)) ==(int *)(-1))
    {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    
    while((Table[N]!=-1)||(Table[0]!=-1))
    {
        if (Table[0]!=-1)
        {
            id=Table[0];
            
            for (int i=0;i<=N-2;i++)
            {
                Table[i]=Table[i+1];
            }

            Table[N-1]=-1;
    
            sleep(dt[id]);
    
            std::cout<<"The dryer dried the "<<id_dish[id]<<std::endl;
        }
    }

    shmdt(Table);

    return 1;
 }