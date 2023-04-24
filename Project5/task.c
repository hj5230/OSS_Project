/*
Project 5 Simulate the page frame replacement
Complete the LRU and OPT algorithm code
below is code for this project
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define VM_PAGE 7       /*Number of virtual pages*/
#define PM_PAGE 4       /* Number of memory blocks allocated to the job */
#define TOTAL_INSERT 18 /*Number of virtual page replacements*/

typedef struct
{
    int vmn;
    int pmn;
    int exist;
    int time;
} vpage_item;
vpage_item page_table[VM_PAGE];
vpage_item *ppage_bitmap[PM_PAGE];
int vpage_arr[TOTAL_INSERT] = {1, 2, 3, 4, 2, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6}; // The access order virtual pages

void init_data() // initialize data
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

// Implemented LRU algorithm below
void LRU() {
    int idx, physMemIdx = 0, totalCount = 0, currTime = 0, missedPageCount = 0;
    bool hasFreeSpace = true; // is there are remaining physical blocks
    while (totalCount < TOTAL_INSERT) // until all pages have been inserted
    {
        if (page_table[vpage_arr[totalCount] - 1].exist == 0) // if the page is not present in physical memory
        {
            ++missedPageCount;
            if (physMemIdx < 4 && ppage_bitmap[physMemIdx] == NULL) // check if there is free space in physical memory
            {
                /* add the page to physical memory */
                ppage_bitmap[physMemIdx] = &page_table[vpage_arr[totalCount] - 1];
                ppage_bitmap[physMemIdx]->exist = 1;
                ppage_bitmap[physMemIdx]->pmn = physMemIdx;
                ppage_bitmap[physMemIdx]->time = currTime;
                ++physMemIdx;
            }
            else
            {
                /* replace the least recently used page in physical memory */
                int leastRecentTime;
                leastRecentTime = ppage_bitmap[0]->time;
                int j = 0;
                for (idx = 0; idx < PM_PAGE; ++idx) // find the page with least recently used time
                {
                    if (ppage_bitmap[idx]->time < leastRecentTime)
                    {
                        leastRecentTime = ppage_bitmap[idx]->time;
                        j = idx;
                    }
                }
                ppage_bitmap[j]->exist = 0; // remove the page from physical memory
                ppage_bitmap[j] = &page_table[vpage_arr[totalCount] - 1]; // replace with new page
                ppage_bitmap[j]->exist = 1;
                ppage_bitmap[j]->pmn = j;
                ppage_bitmap[j]->time = currTime;
            }
        }
        else // the page is present in physical memory
        {
            // Update the time of the page
            for (idx = 0; idx < PM_PAGE; ++idx)
            {
                if (ppage_bitmap[idx]->vmn == page_table[vpage_arr[totalCount] - 1].vmn)
                {
                    ppage_bitmap[idx]->time = currTime;
                    break;
                }
            }
        }
        ++currTime; // Increment current time
        ++totalCount; // Increment total page count
    }
    printf("LRU:\nPage fault:%d\tPage fault rate:%f\nReplacement:%d\tReplacement rate:%f\n", missedPageCount, missedPageCount / (float)TOTAL_INSERT, missedPageCount - 4, (missedPageCount - 4) / (float)TOTAL_INSERT);
}

// Implemented OPT algorithm below
void OPT()
{
    int physMemIdx = 0; // index for physical memory
    int idx; // index for loop
    int totalCount = 0; // total number of pages inserted
    int missedPageCount = 0; // number of page faults
    int currTime = 0; // current time
    bool hasFreeSpace = true; /* whether there are remaining physical blocks */
    while (totalCount < TOTAL_INSERT) // loop until all pages have been inserted
    {
        if (page_table[vpage_arr[totalCount] - 1].exist == 0) // check if the page is not present in physical memory
        {
            ++missedPageCount; // increment page fault count
            if (physMemIdx < 4 && ppage_bitmap[physMemIdx] == NULL) // check if there is free space in physical memory
            {
                // add the page to physical memory
                ppage_bitmap[physMemIdx] = &page_table[vpage_arr[totalCount] - 1];
                ppage_bitmap[physMemIdx]->exist = 1;
                ppage_bitmap[physMemIdx]->pmn = physMemIdx;
                ppage_bitmap[physMemIdx]->time = currTime;
                ++physMemIdx; // increment physical memory index
            }
            else
            {
                // find the page with the maximum distance to next reference
                int maxDistance = 0;
                int j = 0;
                for (idx = 0; idx < PM_PAGE; ++idx)
                {
                    int tempCount;
                    tempCount = totalCount;
                    while (tempCount < TOTAL_INSERT && page_table[vpage_arr[tempCount] - 1].vmn != ppage_bitmap[idx]->vmn)
                    {
                        ++tempCount;
                    }
                    if (tempCount == TOTAL_INSERT)
                    {
                        j = idx;
                        break;
                    }
                    else
                    {
                        if (tempCount - totalCount > maxDistance)
                        {
                            maxDistance = tempCount - totalCount;
                            j = idx;
                        }
                    }
                }
                ppage_bitmap[j]->exist = 0; // remove the page from physical memory
                ppage_bitmap[j] = &page_table[vpage_arr[totalCount] - 1]; // replace with new page
                ppage_bitmap[j]->exist = 1;
                ppage_bitmap[j]->pmn = j;
                ppage_bitmap[j]->time = currTime;
            }
        }
        ++currTime; // increment current time
        ++totalCount; // increment total page count
    }
    // Print statistics
    printf("OPT:\nPage fault:%d\tPage fault rate:%f\nReplacement:%d\tReplacement rate:%f\n",
    missedPageCount, missedPageCount / (float)TOTAL_INSERT,
    missedPageCount - 4, (missedPageCount - 4) / (float)TOTAL_INSERT);
}

int main()
{
    int a;
    printf("Please choose page replacement algorithm: 1.FIFO 2.LRU 3.OPT 0. quit\n");
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
            LRU(); // present the LRU solution
            break;
        case 3:
            init_data();
            OPT(); // present the OPT solution
            break;
        }
    } while (a != 0);
    return 0;
}