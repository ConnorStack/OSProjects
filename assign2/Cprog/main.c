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
pthread_mutex_t lock;

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
    int fullLength;
    double firstHalfAvg;
    double secondHalfAvg;
    double mergedAvg;
} merge_struct;

double *buildRandArrayOfLength(int len);
void *sortThreadFunc(void *arg);
void *mergeThreadFunc(void *arg);

int main(int argc, char *argv[])
{
    pthread_mutex_init(&lock, NULL); //---
    pthread_t tid1, tid2, tid3;
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
    // I will use n later, I want to make sure it is consistent if we had to round down, so mult by 2
    n = half * 2;
    double *firstHalf = (double *)buildRandArrayOfLength(half);
    double *secondHalf = (double *)buildRandArrayOfLength(half);

    // initialize the structs with info we will need in the thread
    sort_struct sort_struct_firsthalf, sort_struct_secondhalf;
    sort_struct_firsthalf.sortArray = firstHalf;
    sort_struct_firsthalf.average = 0;
    sort_struct_firsthalf.count = half;

    sort_struct_secondhalf.sortArray = secondHalf;
    sort_struct_secondhalf.average = 0;
    sort_struct_secondhalf.count = half;

    // create two threads to sort each array
    pthread_create(&tid1, NULL, sortThreadFunc, (void *)&sort_struct_firsthalf);
    pthread_create(&tid2, NULL, sortThreadFunc, (void *)&sort_struct_secondhalf);

    // prepping variables to use after pthread_join
    sort_struct *sortedFromThread1;
    sort_struct *sortedFromThread2;
    // join threads
    pthread_join(tid1, (void **)&sortedFromThread1);
    pthread_join(tid2, (void **)&sortedFromThread2);
    // going to need these arrays for the next thread
    //  double *sortedArr1 = sortedFromThread1->sortArray;
    //  double *sortedArr2 = sortedFromThread2->sortArray;

    // this struct holds both half of the sorted arrays, and an unitialized mergeArray
    merge_struct mergeArr_struct;
    mergeArr_struct.firstHalfArr = sortedFromThread1->sortArray;
    mergeArr_struct.secondHalfArr = sortedFromThread2->sortArray;

    mergeArr_struct.mergeArray = (double *)malloc(n * sizeof(double));
    if (mergeArr_struct.mergeArray == NULL)
    {
        return 1;
    }
    mergeArr_struct.fullLength = sortedFromThread1->count * 2;
    mergeArr_struct.firstHalfAvg = sortedFromThread1->average;
    mergeArr_struct.secondHalfAvg = sortedFromThread2->average;

    pthread_create(&tid3, NULL, mergeThreadFunc, (void *)&mergeArr_struct);
    merge_struct *mergedFromThread;
    pthread_join(tid3, (void **)&mergedFromThread);

    free(firstHalf);
    free(secondHalf);
    free(mergeArr_struct.mergeArray);

    return 0;
}

void *mergeThreadFunc(void *arg)
{
    printf("in merge thread\n\n\n");

    merge_struct *localMergeStruct;
    localMergeStruct = (merge_struct *)arg;
    int n = 0;
    int m = 0;
    int length = localMergeStruct->fullLength;

    for (int i = 0; i < length; i++)
    {
        if ((localMergeStruct->firstHalfArr[n] < localMergeStruct->secondHalfArr[m]))
        {
            localMergeStruct->mergeArray[i] = localMergeStruct->firstHalfArr[n];
            printf("Merging content n: %f\n", localMergeStruct->mergeArray[i]);
            n++;
        }
        else
        {
            localMergeStruct->mergeArray[i] = localMergeStruct->secondHalfArr[m];
            printf("Merging content m: %f\n", localMergeStruct->mergeArray[i]);
            m++;
        }
    }
    
    double average = localMergeStruct->firstHalfAvg + localMergeStruct->secondHalfAvg / 2;
    localMergeStruct->mergedAvg = average;

    pthread_exit((void *)localMergeStruct);
    return NULL;
}

// thread function to sort an array and find the average of its content. Both located inside a struct.
void *sortThreadFunc(void *arg)
{
    // make a local ptr of sort_struct type so we can access the struct passed to the thread
    sort_struct *localStruct;
    localStruct = (sort_struct *)arg;
    pthread_mutex_lock(&lock);
    int arrLen = localStruct->count;

    double total = 0;

    // total the array
    for (int n = 0; n < arrLen; n++)
    {
        total += localStruct->sortArray[n];
    }

    // avoid division by 0
    if (arrLen >= 0)
    {
        localStruct->average = total / arrLen;
    }
    else
    {
        return NULL;
    }

    // insert sort algorithm
    int i;
    for (int j = 1; j < arrLen; j++)
    {
        double insertValue = localStruct->sortArray[j];
        i = j - 1;

        while (i >= 0 && insertValue < localStruct->sortArray[i])
        {
            localStruct->sortArray[i + 1] = localStruct->sortArray[i];
            i = i - 1;
        }
        localStruct->sortArray[i + 1] = insertValue;
    }

    for (int i = 0; i < arrLen; i++)
    {
        printf("Array content: %f\n", localStruct->sortArray[i]);
    }
    pthread_mutex_unlock(&lock); // --------
    printf("avg : %f\n", localStruct->average);
    printf("count : %d\n", localStruct->count);
    pthread_exit((void *)localStruct);

    return NULL;
}

// allocate an array of size len, keep array in heap so thread can access it
double *buildRandArrayOfLength(int len)
{
    double *randArray = (double *)malloc(len * sizeof(double));
    if (randArray == NULL)
    {
        return NULL;
    }

    // some random generation implementation. Randoms for doubles is more confusing than int.
    double minRange = 0.0;
    double maxRange = 1000.0;
    for (int i = 0; i < len; i++)
    {
        double range = (maxRange - minRange);
        double div = (double)rand() / RAND_MAX;
        double randNum = minRange + div * range;
        randArray[i] = randNum;
    }
    return randArray;
}
