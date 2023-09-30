#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

pthread_mutex_t lock;

typedef struct sort_avg_struct
{
    double *sorted_array; // <----- sorted array
    double average; // <----- average
    int count;
    int is_threaded;
} sort_struct;

typedef struct merging_struct
{
    double *merge_array; // <----- merged array
    double *first_half_arr;
    double *second_half_arr;
    int full_length;
    double first_half_avg;
    double second_half_avg;
    double merged_avg; // <-----average
    int is_threaded;
} merge_struct;

double *build_rand_array(int len);
void *sorting_avg(void *arg);
void *merging_avg(void *arg);
sort_struct build_sort_struct(double *sortArray, int count, int is_threaded);
merge_struct build_merge_struct(double *first_half, double *second_half, int len, int is_threaded);

int main(int argc, char *argv[])
{
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
    int half_n = n / 2;
    // -------------------------------------------------------------
    // -------------------- NON THREADED SECTION -------------------
    // -------------------------------------------------------------

    // Building the struct for non-threaded sorting_avg.
    //  is_threaded is a boolean integer used to circumvent pthread_exit while not threading
    double *nont_first_half = (double *)build_rand_array(half_n);
    double *nont_second_half = (double *)build_rand_array(half_n);
    double *non_threaded_array = (double *)build_rand_array(n);
    is_threaded = 1;
    sort_struct nont_first_struct = build_sort_struct(nont_first_half, n / 2, is_threaded);
    sort_struct nont_second_struct = build_sort_struct(nont_second_half, n / 2, is_threaded);

    // Clocking and running the non-threaded implementation
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);
    sorting_avg(&nont_first_struct);
    sorting_avg(&nont_second_struct);

    // Creating merge_struct after sorting_avg is called so it contains the correct values
    merge_struct nont_merge_struct = build_merge_struct(nont_first_struct.sorted_array, nont_second_struct.sorted_array, n, is_threaded);
    merging_avg(&nont_merge_struct);

    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    // Display the results
    elapsed = (ts_end.tv_sec - ts_begin.tv_sec) * 1000.0;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000.0;
    printf("Sorting is done in %.2f ms when one thread is used\n", elapsed);

    // ---------------------------------------------------------
    // -------------------- THREADED SECTION -------------------
    // ---------------------------------------------------------

    // Build two random arrays length n/2
    double *first_half = (double *)build_rand_array(half_n);
    double *second_half = (double *)build_rand_array(half_n);

    // Build the structs to pass to pthread_create, set is_threaded to true
    sort_struct sort_struct_firsthalf, sort_struct_secondhalf;
    is_threaded = 0;
    sort_struct_firsthalf = build_sort_struct(first_half, half_n, is_threaded);
    sort_struct_secondhalf = build_sort_struct(second_half, half_n, is_threaded);

    // Clocking and running sorting_avg threads
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);
    pthread_create(&tid1, NULL, sorting_avg, (void *)&sort_struct_firsthalf);
    pthread_create(&tid2, NULL, sorting_avg, (void *)&sort_struct_secondhalf);

    // Join threads, sorted_t1 and sorted_t2 will contain content needed for the last thread
    sort_struct *sorted_t1, *sorted_t2;
    pthread_join(tid1, (void **)&sorted_t1);
    pthread_join(tid2, (void **)&sorted_t2);

    // Stop the clock, and display values
    //  clock_gettime(CLOCK_MONOTONIC, &ts_end);
    //  elapsed = (ts_end.tv_sec - ts_begin.tv_sec) * 1000.0;
    //  elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000.0;
    //  printf("Sorting is done in %.2f ms when two threads are used\n", elapsed);

    // Create thread to merge the arrays from sorted_t1 and sorted_t2
    merge_struct merge_arr_struct = build_merge_struct(sorted_t1->sorted_array, sorted_t2->sorted_array, n, is_threaded);
    pthread_create(&tid3, NULL, merging_avg, (void *)&merge_arr_struct);
    merge_struct *merged_t3;
    pthread_join(tid3, (void **)&merged_t3);

    // Stop the clock, and display values
    clock_gettime(CLOCK_MONOTONIC, &ts_end);
    elapsed = (ts_end.tv_sec - ts_begin.tv_sec) * 1000.0;
    elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000.0;
    printf("Sorting is done in %.2f ms when two threads are used\n", elapsed);

    // Free allocated memory
    free(first_half);
    free(second_half);
    free(non_threaded_array);
    free(merge_arr_struct.merge_array);

    return 0;
}

void *merging_avg(void *arg)
{
    merge_struct *local_merge_struct;
    local_merge_struct = (merge_struct *)arg;
    int n = 0;
    int m = 0;
    int length = local_merge_struct->full_length;
    int first_half_length = length / 2;
    int second_half_length = length / 2;

    // Taking both n/2 arrays and merging them into a n lengh array.
    // If n is less than firstHalf.length, then we expect more n indexed values to be merged.
    // Now one of two things is possible, either values indexed from m are full (in which case we know all others will be n indexed)
    // Or our standard case, n index value is less than m index value, so it should be placed in the merged array
    // m == second half length means m index is full or firsthalf[n] < secondHalf[m]
    for (int i = 0; i < length; i++)
    {

        if ((n < first_half_length) && ((m == second_half_length) || (local_merge_struct->first_half_arr[n] < local_merge_struct->second_half_arr[m])))
        {
            local_merge_struct->merge_array[i] = local_merge_struct->first_half_arr[n];
            n++;
        }
        else if (m < second_half_length)
        {
            local_merge_struct->merge_array[i] = local_merge_struct->second_half_arr[m];
            m++;
        }
    }
    // Calculate average
    double average = local_merge_struct->first_half_avg + local_merge_struct->second_half_avg / 2;
    local_merge_struct->merged_avg = average;

    if (local_merge_struct->is_threaded == 0)
    {
        pthread_exit((void *)local_merge_struct);
    }

    return NULL;
}

// Thread function to sort an array and find the average of its content. Both located inside a struct.
void *sorting_avg(void *arg)
{
    // Make a local ptr of sort_struct type so we can access the struct passed to the thread
    sort_struct *local_struct;
    local_struct = (sort_struct *)arg;
    int arr_len = local_struct->count;
    double total = 0;

    // Total the array
    for (int n = 0; n < arr_len; n++)
    {
        total += local_struct->sorted_array[n];
    }

    // Avoid division by 0
    if (arr_len == 0)
    {
        return NULL;
    }
    // Calculate average
    local_struct->average = total / arr_len;

    // Insert sort algorithm
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
    // If this is removed, the program halts after the non-threaded function executes this code
    if (local_struct->is_threaded == 0)
    {
        pthread_exit((void *)local_struct);
    }

    return NULL;
}

// Allocate an array of size len, keep array in heap so thread can access it
double *build_rand_array(int len)
{
    double *rand_array = (double *)malloc(len * sizeof(double));
    if (rand_array == NULL)
    {
        return NULL;
    }

    // Some random generation implementation. Randoms for doubles is more complicated than ints
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

sort_struct build_sort_struct(double *sorted_array, int count, int is_threaded)
{
    sort_struct sstruct;
    sstruct.sorted_array = sorted_array;
    sstruct.average = 0.0;
    sstruct.count = count;
    sstruct.is_threaded = is_threaded;

    return sstruct;
}

merge_struct build_merge_struct(double *first_half, double *second_half, int len, int is_threaded)
{
    merge_struct mstruct;
    mstruct.first_half_arr = first_half;
    mstruct.second_half_arr = second_half;
    mstruct.merge_array = (double *)malloc(len * sizeof(double));
    if (mstruct.merge_array == NULL)
    {
        return mstruct;
    }
    mstruct.full_length = len;
    mstruct.first_half_avg = 0.0;
    mstruct.second_half_avg = 0.0;
    mstruct.is_threaded = is_threaded;

    return mstruct;
}