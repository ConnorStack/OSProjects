#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

// pass array
//  a_type arr[100];
//  pthread_create(&tid, NULL, ThFunc,(void *) arr);
//  ...

// void *ThFunc(void *arg){
//     a_type *myarr
//     myarr = (a_type)arg;
//     myarr[i] ..... ;
// }

// return array
//  a_type arr[size], *ret;
//  pthread_create(&tid, NULL, TheFunc, (void*) arr);
//  ...
//  pthread_join(tid, (void **), &ret);
//  ret[i]...

// void* ThFunc(void * arg){
//     a_type *myarr, myret[len], *myptrrt;
//     myarr[i] ....
//     myret[i] = ...

//     myptrret = (a_type *),alloc(len * sizeof(a_type));
//     check NULL
//     myptrret[i] = myret[i]; ...
//     pthread_exit((void*) myptrret);
// }

typedef struct sortAndAvgStruct
{
    double *sortArray;
    double average;
    int count;
} sort_struct;

typedef struct mergeStruct
{
    double *mergeArray;
    double *firstHalfArr;
    double *secondHalfArr;
} merge_struct;

void *buildRandArrayOfLength(int len);
void *sortThreadFunc(void *arg1);

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;
    srand(time(NULL));
    // struct timespec ts_begin, ts_end;
    // double elapsed;
    int n;
    // Validate argument
    if (argc != 2)
    {
        return 1;
    }

    n = atoi(argv[1]);
    // create two arrays of equal length, if n is odd, it is rounded down
    int half = n / 2;
    double *firstHalf = (double *)buildRandArrayOfLength(half);
    double *secondHalf = (double *)buildRandArrayOfLength(half);

    sort_struct sort_struct_firsthalf, sort_struct_secondhalf;
    sort_struct_firsthalf.sortArray = firstHalf;
    sort_struct_firsthalf.average = 0;
    sort_struct_firsthalf.count = half;

    sort_struct_secondhalf.sortArray = secondHalf;
    sort_struct_secondhalf.average = 0;
    sort_struct_secondhalf.count = half;

    // sort_struct *sort_struct_firsthalf, *sort_struct_secondhalf;
    // sort_struct_firsthalf->sortArray = firstHalf;
    // sort_struct_firsthalf->average = 0;
    // sort_struct_firsthalf->count = half;

    // sort_struct_secondhalf->sortArray = secondHalf;
    // sort_struct_secondhalf->average = 0;
    // sort_struct_secondhalf->count = half;

    // create two threads to sort each array
    pthread_create(&tid1, NULL, sortThreadFunc, (void *)&sort_struct_firsthalf);
    pthread_create(&tid2, NULL, sortThreadFunc, (void *)&sort_struct_secondhalf);

    // void *result1;
    // void *result2;
    // pthread_join(tid1, &result1);
    // pthread_join(tid2, &result2);
    // sort_struct *ret1 = (sort_struct *)result1;
    // sort_struct *ret2 = (sort_struct *)result2;


    printf("test : %f\n", sort_struct_firsthalf.average);
    printf("test : %d\n", sort_struct_firsthalf.count);
    // for (int i = 0; i < half; i++)
    // {
    //     printf("Firsthalf : %f\n", firstHalf[i]);
    //     printf("Secondhalf : %f\n", secondHalf[i]);
    // }

    free(firstHalf);
    free(secondHalf);
    return 0;
}

void *buildRandArrayOfLength(int len)
{
    double *randArray = (double *)malloc(len * sizeof(double));
    if (randArray == NULL)
    {
        return NULL;
    }

    double minRange = 0.0;
    double maxRange = 100.0;

    for (int i = 0; i < len; i++)
    {
        double range = (maxRange - minRange);
        double div = (double)rand() / RAND_MAX;
        double randNum = minRange + div * range;

        randArray[i] = randNum;
    }

    return randArray;
}

void *sortThreadFunc(void *arg)
{
    sort_struct *localStruct = (sort_struct *)malloc(sizeof(localStruct));
    if (localStruct == NULL)
    {
        pthread_exit(NULL);
    }
    memcpy(localStruct, arg, sizeof(sort_struct));

    // sort_struct *localStruct;
    // localStruct = (sort_struct *)arg;

    int argLen = 0;
    double total = 0;

    while (localStruct->sortArray[argLen] != -1)
    {
        total += localStruct->sortArray[argLen];
        argLen++;
    }

    if (argLen >= 0)
    {
        localStruct->average = total / argLen;
    }

    int i;
    for (int j = 1; j < argLen; j++)
    {
        int insertValue = localStruct->sortArray[j];
        i = j - 1;

        while (i >= 0 && insertValue < localStruct->sortArray[i])
        {
            localStruct->sortArray[i + 1] = localStruct->sortArray[i];
            i = i - 1;
        }
        localStruct->sortArray[i + 1] = insertValue;
    }

    for (int i = 0; i < argLen; i++)
    {
        printf("thread called %f\n", localStruct->sortArray[i]);
    }
    pthread_exit((void *)localStruct);
    free(localStruct);
    return NULL;
}
