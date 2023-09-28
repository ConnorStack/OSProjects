#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


//prep
// a_type arr[100];
// pthread_create(&tid, NULL,
// ThFunc,(void *) arr);
// ...

// void *ThFunc(void *arg){
//     a_type *myarr
//     myarr = (a_type)arg;
//     myarr[i] ..... ;
// }

int main(int argc, char *argv[])
{
    // struct timespec ts_begin, ts_end;
    // double elapsed;
    int n;

    if (argc != 2)
    {
        return 1;
    }

    n = atoi(argv[1]);

    if(n % 2 != 0){
        n=n-1;
    }

    n = 100;
    // double entireArray[n];

    //generate random doubles to fill the array
    double minRange = 0.0;
    double maxRange = 100.0;
    int half = n / 2;
    double firstHalf[half];
    double secondHalf[half];

    for (int i = 0; i < n; i++)
    {
        double range = (maxRange - minRange);
        double div = (double)rand() / RAND_MAX;
        double randNum = minRange + div * range;

        if(i < n/2){
            firstHalf[i] = randNum;
        }else{
            secondHalf[i - half] = randNum; 
        }
        
    }

    for(int i = 0; i < 10; i++){
        printf("Firsthalf : %f\n", firstHalf[i]);
        printf("Secondhalf : %f\n", secondHalf[i]);
    }
    


    return 0;
}

void needsToBeThread()
{
    int sampleArray[] = {4, 1, 5, 3, 9, 4};
    int arraySize = sizeof(sampleArray) / sizeof(sampleArray[0]);

    int i;
    for (int j = 1; j < arraySize; j++)
    {
        int insertValue = sampleArray[j];
        i = j - 1;

        while (i >= 0 && insertValue < sampleArray[i])
        {
            sampleArray[i + 1] = sampleArray[i];
            i = i - 1;
        }
        sampleArray[i + 1] = insertValue;
    }

    for (int i = 0; i < arraySize; i++)
    {
        printf("%d\n", sampleArray[i]);
    }
}
