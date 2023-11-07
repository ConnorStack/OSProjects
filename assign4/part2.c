#include <stdlib.h>
#include <stdio.h>

typedef struct page_table_cell{
    int f_num;
    int vi_bit;
}page_table_cell;

int main(int argc, char *argv[])
{
    unsigned long LA, PA;

    unsigned int p=5, f=3, d=7;
    unsigned int pnum, fnum, dnum;
    int CLK = 0;
    int freeframes[8] = {0, 1, 1, 1, 1, 1, 1, 1};
    int LRUcount[8] = {0};
    int revmap[8] = {-1};
    
    printf("working\n");
    return 0;
}