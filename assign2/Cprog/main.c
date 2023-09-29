#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

pthread_mutex_t lock;

typedef struct sort_avg_struct
{
    double *sorted_array;
    double average;
    int count;
    int is_threaded;
} sort_struct;

typedef struct merging_struct
{
    double *merge_array;
    double *first_half_arr;
    double *second_half_arr;
    int full_length;
    double first_half_avg;
    double second_half_avg;
    double merged_avg;
} merge_struct;

double *build_rand_array(int len);
void *sorting_avg(void *arg);
void *merging_avg(void *arg);
sort_struct build_sort_struct(double *sortArray, int count, int is_threaded);
merge_struct build_merge_struct(double *first_half, double *second_half, int len);

int main(int argc, char *argv[])
{
    // pthread_mutex_init(&lock, NULL); //---
    pthread_t tid1, tid2, tid3;
    srand(time(NULL));
    struct timespec ts_begin, ts_end;
    double elapsed;
    int n, is_threaded;
    // Validate argument
    if (argc != 2)
    {
        return 1;
    }

    n = atoi(argv[1]);
    // create two arrays of equal length, if n is odd, it is rounded down
    int half_n = n / 2;

    //building the struct for a non-threaded sorting_avg. Using is_threaded as a boolean integer to circumvent pthread_exit
    double *non_threaded_array = (double *)build_rand_array(n);
    is_threaded = 1;
    sort_struct non_thread_struct = build_sort_struct(non_threaded_array, n, is_threaded);
    double *sorted_arr = sorting_avg(&non_thread_struct);
    // non_thread_struct.sorted_array = non_threaded_array;
    // non_thread_struct.average = 0.0;
    // non_thread_struct.count = n;
    // non_thread_struct.is_thread = 1;
    

    double *first_half = (double *)build_rand_array(half_n);
    double *second_half = (double *)build_rand_array(half_n);

    // initialize the structs with info we will need in the thread
    is_threaded = 0;
    sort_struct sort_struct_firsthalf, sort_struct_secondhalf;
    sort_struct_firsthalf = build_sort_struct(first_half, half_n, is_threaded);
    sort_struct_secondhalf = build_sort_struct(second_half, half_n, is_threaded);
    // sort_struct sort_struct_firsthalf, sort_struct_secondhalf;
    // sort_struct_firsthalf.sorted_array = first_half;
    // sort_struct_firsthalf.average = 0;
    // sort_struct_firsthalf.count = half;
    // sort_struct_firsthalf.is_threaded = 0;

    // sort_struct_secondhalf.sorted_array = second_half;
    // sort_struct_secondhalf.average = 0;
    // sort_struct_secondhalf.count = half;
    // sort_struct_secondhalf.is_threaded = 0;


    clock_gettime(CLOCK_MONOTONIC, &ts_begin);

    // create two threads to sort each array
    pthread_create(&tid1, NULL, sorting_avg, (void *)&sort_struct_firsthalf);
    pthread_create(&tid2, NULL, sorting_avg, (void *)&sort_struct_secondhalf);

    // prepping variables to use after pthread_join
    sort_struct *sorted_t1, *sorted_t2;
    // join threads
    pthread_join(tid1, (void **)&sorted_t1);
    pthread_join(tid2, (void **)&sorted_t2);

    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = (ts_end.tv_sec - ts_begin.tv_sec) * 1000.0;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000.0;
    printf("Main elapsed time: %.2f ms\n", elapsed);
    // going to need these arrays for the next thread
    //  double *sortedArr1 = sortedFromThread1->sortArray;
    //  double *sortedArr2 = sortedFromThread2->sortArray;

    // this struct holds both half of the sorted arrays, and an unitialized mergeArray
    merge_struct merge_arr_struct = build_merge_struct(sorted_t1->sorted_array, sorted_t2->sorted_array, n);
    // merge_arr_struct.first_half_arr = sorted_t1->sorted_array;
    // merge_arr_struct.second_half_arr = sorted_t2->sorted_array;

    // merge_arr_struct.merge_array = (double *)malloc(n * sizeof(double));
    // if (merge_arr_struct.merge_array == NULL)
    // {
    //     return 1;
    // }
    // merge_arr_struct.full_length = sorted_t1->count * 2;
    // merge_arr_struct.first_half_avg = sorted_t1->average;
    // merge_arr_struct.second_half_avg = sorted_t2->average;

    pthread_create(&tid3, NULL, merging_avg, (void *)&merge_arr_struct);
    // pthread_create(&tid3, NULL, merging_avg, (void *)&merge_arr_struct);
    merge_struct *merged_t3;
    pthread_join(tid3, (void **)&merged_t3);

    free(first_half);
    free(second_half);
    free(merge_arr_struct.merge_array);
    
    return 0;
}

void *merging_avg(void *arg)
{
    printf("in merge thread\n\n\n");

    merge_struct *local_merge_struct;
    local_merge_struct = (merge_struct *)arg;
    int n = 0;
    int m = 0;
    int length = local_merge_struct->full_length;

    for (int i = 0; i < length; i++)
    {
        if ((local_merge_struct->first_half_arr[n] < local_merge_struct->second_half_arr[m]))
        {
            local_merge_struct->merge_array[i] = local_merge_struct->first_half_arr[n];
            // printf("Merging content n: %f\n", local_merge_struct->merge_array[i]);
            n++;
        }
        else
        {
            local_merge_struct->merge_array[i] = local_merge_struct->second_half_arr[m];
            // printf("Merging content m: %f\n", local_merge_struct->merge_array[i]);
            m++;
        }
    }

    double average = local_merge_struct->first_half_avg + local_merge_struct->second_half_avg / 2;
    local_merge_struct->merged_avg = average;

    pthread_exit((void *)local_merge_struct);
    return NULL;
}

// thread function to sort an array and find the average of its content. Both located inside a struct.
void *sorting_avg(void *arg)
{
    //start clock
    struct timespec ts_begin;
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);
    // make a local ptr of sort_struct type so we can access the struct passed to the thread
    sort_struct *local_struct;
    local_struct = (sort_struct *)arg;

    // pthread_mutex_lock(&lock);
    int arr_len = local_struct->count;

    double total = 0;

    // total the array
    for (int n = 0; n < arr_len; n++)
    {
        total += local_struct->sorted_array[n];
    }

    // avoid division by 0
    if (arr_len == 0)
    {
        return NULL;
    }

    local_struct->average = total / arr_len;

    // insert sort algorithm
    int i;
    for (int j = 1; j < arr_len; j++)
    {
        double insert_value = local_struct->sorted_array[j];
        i = j - 1;

        while (i >= 0 && insert_value < local_struct->sorted_array[i])
        {
            local_struct->sorted_array[i + 1] = local_struct->sorted_array[i];
            i = i - 1;
        }
        local_struct->sorted_array[i + 1] = insert_value;
    }

    for (int i = 0; i < arr_len; i++)
    {
        printf("Array content: %f\n", local_struct->sorted_array[i]);
    }
    struct timespec ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    // Calculate elapsed time in milliseconds
    double elapsed = (ts_end.tv_sec - ts_begin.tv_sec) * 1000.0;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000.0;
    // printf("Sorting time: %.2f ms\n", elapsed);
    // pthread_mutex_unlock(&lock); // --------

    if(local_struct->is_threaded == 0){
        printf("Threaded sorting time: %.2f ms\n", elapsed);
        pthread_exit((void *)local_struct);
    }else{
        printf("Non-threaded sorting time: %.2f ms\n", elapsed);
    }
    return NULL;
}

// allocate an array of size len, keep array in heap so thread can access it
double *build_rand_array(int len)
{
    double *rand_array = (double *)malloc(len * sizeof(double));
    if (rand_array == NULL)
    {
        return NULL;
    }

    // some random generation implementation. Randoms for doubles is more confusing than int.
    double min_range = 0.0;
    double max_range = 1000.0;
    for (int i = 0; i < len; i++)
    {
        double range = (max_range - min_range);
        double div = (double)rand() / RAND_MAX;
        double rand_num = min_range + div * range;
        rand_array[i] = rand_num;
    }
    return rand_array;
}

// sort_struct build_sort_struct(double *sorted_array, int count, int is_threaded) {

//     sort_struct sstruct;
//     sstruct.sorted_array = sorted_array;
//     sstruct.average = 0.0;
//     sstruct.count = count;
//     sstruct.is_threaded = is_threaded;
//     return sstruct;
// }

sort_struct build_sort_struct(double *sorted_array, int count, int is_threaded) {
    
    sort_struct sstruct;
    sstruct.sorted_array = sorted_array;
    sstruct.average = 0.0;
    sstruct.count = count;
    sstruct.is_threaded = is_threaded;
    return sstruct;
}

merge_struct build_merge_struct(double *first_half, double *second_half, int len){
    
    merge_struct mstruct;
    mstruct.first_half_arr = first_half;
    mstruct.second_half_arr = second_half;
    mstruct.merge_array = (double *)malloc(len * sizeof(double));
    // if (mstruct.merge_array == NULL) {
    //     mstruct = nullptr;
    //     return mstruct;
    // }
    mstruct.full_length = len;
    mstruct.first_half_avg = 0.0;
    mstruct.second_half_avg = 0.0;
    return mstruct;
}

// merge_struct *build_merge_struct(double *first_half, double *second_half, int len){
//     merge_struct *mstruct = (merge_struct *)malloc(sizeof(merge_struct));
//     if (mstruct == NULL) {
//         // Handle allocation failure
//         return NULL;
//     }

//     mstruct->first_half_arr = first_half;
//     mstruct->second_half_arr = second_half;
//     mstruct->merge_array = (double *)malloc(len * sizeof(double));
    
//     if (mstruct->merge_array == NULL) {
//         // Handle allocation failure
//         free(mstruct); // Clean up the previously allocated memory
//         return NULL;
//     }

//     mstruct->full_length = len;
//     mstruct->first_half_avg = 0.0;
//     mstruct->second_half_avg = 0.0;
    
//     return mstruct;
// }