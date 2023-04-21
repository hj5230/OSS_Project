#include<stdio.h>
#include<stdlib.h>
#define VM_PAGE 7 /*Number of virtual pages*/
#define PM_PAGE 4 /* Number of memory blocks allocated to the job */
#define TOTAL_INSERT 18 /*Number of virtual page replacements*/
typedef struct
{
    int vmn;
    int pmn;
    int exist;
    int time;
} vpage_item;
vpage_item page_table[VM_PAGE];
vpage_item* ppage_bitmap[PM_PAGE];
int vpage_arr[TOTAL_INSERT] = { 1, 2, 3, 4, 2, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6 }; // The access order virtual pages

void init_data() //initialize data
{
    for (int i = 0; i < VM_PAGE; i++)
    {
        page_table[i].vmn = i + 1;
        page_table[i].pmn = -1;
        page_table[i].exist = 0;
        page_table[i].time = -1;
    }
    for (int i = 0; i < PM_PAGE; i++) /*initialize the physical page map*/
    {
        ppage_bitmap[i] = NULL;
    }
}

void FIFO() /*FIFO page replacement algorithem*/
{
    int k = 0;
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    while (sum < TOTAL_INSERT)
    {
        if (page_table[vpage_arr[sum] - 1].exist == 0)
        {
            missing_page_count++;
            if (k < PM_PAGE)
            {
                if (ppage_bitmap[k] == NULL) /*find a free block*/
                {
                    ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[k]->exist = 1;
                    ppage_bitmap[k]->pmn = k;
                    ppage_bitmap[k]->time = current_time;
                    k++;
                }
            }
            else
            {
                int temp = ppage_bitmap[0]->time;
                int j = 0;
                for (int i = 0; i < PM_PAGE; i++)
                {
                    if (ppage_bitmap[i]->time < temp)
                    {
                        temp = ppage_bitmap[i]->time;
                        j = i;
                    }
                }
                ppage_bitmap[j]->exist = 0;
                ppage_bitmap[j] = &page_table[vpage_arr[sum] - 1]; /*update page table*/
                ppage_bitmap[j]->exist = 1;
                ppage_bitmap[j]->pmn = j;
                ppage_bitmap[j]->time = current_time;
            }
        }
        current_time++;
        sum++;
    }
    printf("The number of page faults of FIFO is:%d\t Page fault rate:%f\t The number of replacement:%d\tReplacement rate:%f", missing_page_count, missing_page_count / (float)TOTAL_INSERT, missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

void LRU()
{
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    while (sum < TOTAL_INSERT)
    {
        if (page_table[vpage_arr[sum] - 1].exist == 0)
        {
            missing_page_count++;
            int min_time = -1;
            int min_time_index = -1;
            for (int i = 0; i < PM_PAGE; i++)
            {
                if (ppage_bitmap[i] == NULL) // find a free block
                {
                    ppage_bitmap[i] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[i]->exist = 1;
                    ppage_bitmap[i]->pmn = i;
                    ppage_bitmap[i]->time = current_time;
                    break;
                }
                else
                {
                    if (min_time == -1 || ppage_bitmap[i]->time < min_time)
                    {
                        min_time = ppage_bitmap[i]->time;
                        min_time_index = i;
                    }
                }
            }

            if (min_time_index != -1) // need to replace a page
            {
                ppage_bitmap[min_time_index]->exist = 0;
                ppage_bitmap[min_time_index] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[min_time_index]->exist = 1;
                ppage_bitmap[min_time_index]->pmn = min_time_index;
                ppage_bitmap[min_time_index]->time = current_time;
            }
        }
        else // update access time
        {
            page_table[vpage_arr[sum] - 1].time = current_time;
        }

        current_time++;
        sum++;
    }
    printf("The number of page faults of LRU is:%d\t Page fault rate:%f\t The number of replacement:%d\tReplacement rate:%f", missing_page_count, missing_page_count / (float)TOTAL_INSERT, missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

void OPT()
{
    int sum = 0;
    int missing_page_count = 0;
    while (sum < TOTAL_INSERT)
    {
        if (page_table[vpage_arr[sum] - 1].exist == 0)
        {
            missing_page_count++;
            int max_distance = -1;
            int max_distance_index = -1;

            for (int i = 0; i < PM_PAGE; i++)
            {
                if (ppage_bitmap[i] == NULL) // find a free block
                {
                    ppage_bitmap[i] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[i]->exist = 1;
                    ppage_bitmap[i]->pmn = i;
                    break;
                }
                else
                {
                    int distance = 0;
                    for (int j = sum + 1; j < TOTAL_INSERT; j++)
                    {
                        if (ppage_bitmap[i]->vmn == vpage_arr[j])
                            break;
                        distance++;
                    }

                    if (distance > max_distance)
                    {
                        max_distance = distance;
                        max_distance_index = i;
                    }
                }
            }

            if (max_distance_index != -1) // need to replace a page
            {
                ppage_bitmap[max_distance_index]->exist = 0;
                ppage_bitmap[max_distance_index] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[max_distance_index]->exist = 1;
                ppage_bitmap[max_distance_index]->pmn = max_distance_index;
            }
        }

        sum++;
    }
    printf("The number of page faults of OPT is:%d\t Page fault rate:%f\t The number of replacement:%d\tReplacement rate:%f", missing_page_count, missing_page_count / (float)TOTAL_INSERT, missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

int main()
{
    int a;
    printf("Please choose page replacement algorithm：1.FIFO\t2.LRU\t3.OPT\t0. quit\n");
    do
    {
        scanf("%d", &a);
        switch (a)
        {
        case 1:
            init_data();
            FIFO();
            break;
        case 2:
            init_data();
            LRU();
            break;
        case 3:
            init_data();
            OPT();
            break;
        }
    } while (a != 0);
    return 0;
}