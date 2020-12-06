#ifndef __PROGTEST__
#include "flib.h"
//#include <iostream>
#endif //__PROGTEST__

void merge_parts(int32_t * buffer, int32_t * tmp_buf, int32_t start, int32_t end) {
    int32_t left_end = (start + end)/2;
    int32_t left = start;
    int32_t right = left_end+1;
    int32_t index = start;

    while(left <= left_end && right <= end){
        if(buffer[left] <= buffer[right])
            tmp_buf[index] = buffer[left++];
        else
            tmp_buf[index] = buffer[right++];
        index++;
    }

    if(left <= left_end)
        while(left <= left_end)
            tmp_buf[index++] = buffer[left++];
    else if(right <= end)
        while(right <= end)
            tmp_buf[index++] = buffer[right++];

}

int16_t mergesortFiles(int16_t file1, int16_t file2, int32_t num, int32_t location, int32_t * buffer_in, int32_t * tmp_buffer_in) {
    int32_t *buffer = buffer_in;
    int32_t *answer = tmp_buffer_in;
    int32_t count=0;
    int32_t left = 0;
    int32_t right = 0;
    int32_t end_r = 0;
    int32_t end_l = 0;
    int32_t index = 0;
    int32_t left_point = 0;
    int32_t right_point = 0;
    int16_t answer_file = location+1;
    bool miss, looped;

    flib_open(file1, READ);
    flib_open(file2, READ);
    flib_open(answer_file, WRITE);

    while(1) {
        miss=false;
        looped=false;
        count = flib_read(file1, buffer+left_point,(num/2)-left_point);
        count+=left_point;

        if(count==0) miss = true;
        end_l = count;

        count = flib_read(file2, buffer+(num/2)+right_point, (num/2)-right_point);
        count+=right_point;
        end_r = (num/2) + count;
        if(count==0 && miss) break;
        else if(!count) miss = true;

        left = 0;
        right = (num/2);
        index=0;
        while(right < end_r && left < end_l){
            if(buffer[left] <= buffer[right])
                answer[index++] = buffer[left++];
            else
                answer[index++] = buffer[right++];
            looped=true;
        }
        if(miss && !looped && right < end_r) {
            while ( right < end_r ) {
                answer[index++] = buffer[right++];
            }
        }
        else if(miss && !looped && left < end_l){
            while(left < end_l)
                answer[index++] = buffer[left++];
        }
        flib_write(answer_file, answer, index);


        left_point = 0;
        right_point=0;

        if(left < end_l){
            int32_t i=0;
            while(left < end_l)
                buffer[i++] = buffer[left++];
            left_point = i;
        }
        else if(right < end_r) {
            int32_t i=0;
            while ( right < end_r ) {
                int n=( num / 2 ) + i++;
                buffer[n] = buffer[right++];
            }
            right_point=i;
        }
    }

    flib_close(file1);
    flib_close(file2);
    flib_close(answer_file);

    flib_remove(file1);
    flib_remove(file2);
    return answer_file;
}

void mergesort(int32_t * buffer, int32_t * tmp_buf, int32_t start, int32_t end){
    if(start >= end) return;

    bool tmp=false;

    for(int32_t i=start; i < end; i++){
        if(buffer[i] > buffer[i+1]){
            tmp= true;
            break;
        }
    }
    if(tmp) {
        int32_t mid = ( start + end ) / 2;
        mergesort(buffer, tmp_buf, start, mid);
        mergesort(buffer, tmp_buf, mid + 1, end);
        merge_parts(buffer, tmp_buf, start, end);

        for(int32_t i=0; i < end-start+1; i++)
            buffer[start+i] = tmp_buf[start+i];
    }
}

/*void selectSort(int32_t * buffer, int32_t end){
    int32_t m=0;
    int32_t tmp=0;
    for(int32_t i=0; i < end; i++){
        m=i;
        for(int32_t j=i+1; j < end; j++){
            if(buffer[j] < buffer[m])
                m = j;
        }
        tmp = buffer[i];
        buffer[i] = buffer[m];
        buffer[m] = tmp;
    }

}*/

void swap(int *x, int *y);

// A class for Min Heap
class MinHeap
{
    int *harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    MinHeap(int capacity);

    // to heapify a subtree with the root at given index
    void MinHeapify(int );

    int parent(int i) { return (i-1)/2; }

    // to get index of left child of node at index i
    int left(int i) { return (2*i + 1); }

    // to get index of right child of node at index i
    int right(int i) { return (2*i + 2); }

    // to extract the root which is the minimum element
    int extractMin();

    // Inserts a new key 'k'
    void insertKey(int k);
};

// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new int[cap];
}

// Inserts a new key 'k'
void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)] > harr[i])
    {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
}

// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return 2147483647;
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }

    // Store the minimum value, and remove it from heap
    int root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);

    return root;
}

// A recursive method to heapify a subtree with the root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}

// A utility function to swap two elements
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}



void tarant_allegra ( int32_t in_file, int32_t out_file, int32_t bytes ) {
    int32_t num_int = ( bytes / ( 4 * 2 ));            /// add -12 because we have opened new file on heap
    int32_t *buffer = new int32_t[num_int];
    int32_t *tmp_buffer = new int32_t[num_int];
    int64_t count = 0;
    int16_t file = out_file + in_file;
    int32_t point = 1;
    int32_t fi_point = 1;
    bool problem = false;
    bool first = false;

    flib_open(in_file, READ);

//    int32_t stackMem[100];
    MinHeap heap(100);

    count = flib_read(in_file, buffer, 100);
    flib_open(out_file, WRITE);
    int64_t index = 0;

    while(index < count)
        heap.insertKey(buffer[index++]);

//    buffer[0] = NULL;
    int32_t globalmin=0;
    while(count != 0 && !problem){
        count = flib_read(in_file, buffer, num_int);
        if(count == 0)
            break;
//        int32_t globalmin=0;
        index=0;
        while(index < count) {
            int32_t min = heap.extractMin();
            heap.insertKey(buffer[index]);
            if ( first && min < globalmin ) {
                problem = true;
                break;
            }
            first = true;
            globalmin = min;
            tmp_buffer[index++] = min;
        }
        flib_write(out_file, tmp_buffer, index);
    }
    if(!problem){
        tmp_buffer[0] = heap.extractMin();
        index=0;
        while(tmp_buffer[0] != 2147483647){
            flib_write(out_file, tmp_buffer, 1);
            tmp_buffer[0] = heap.extractMin();
            index++;
        }
    }
    flib_close(out_file);
    flib_close(in_file);

    if(problem) {
        flib_open(in_file, READ);
        while ( 1 ) {
            flib_open(file + point, WRITE);
            count = flib_read(in_file, buffer, num_int);
            if ( count == 0 ) {
                flib_close(file + point);
                break;
            }
            mergesort(buffer, tmp_buffer, 0, count - 1);
//        selectSort(buffer, count);
            flib_write(file + point, buffer, count);
            flib_close(file + point);
            point++;
            if ( count < num_int ) break;
        }
        point--;

        flib_close(in_file);

        if ( fi_point == point ) {
            flib_open(file + point, READ);
            flib_open(out_file, WRITE);

            while ( 1 ) {
                count = flib_read(file + point, buffer, num_int);
                flib_write(out_file, buffer, count);
                if ( count < num_int ) break;
            }
            flib_close(file + point);
            flib_close(out_file);
        }

        while ( fi_point < point ) {
            if ( fi_point + 2 >= point + 1 ) {
                mergesortFiles(file + fi_point, file + fi_point + 1, num_int, out_file - 1, buffer, tmp_buffer);
                break;
            }
            mergesortFiles(file + fi_point, file + fi_point + 1, num_int, file + point, buffer, tmp_buffer);
            ++point;
            fi_point += 2;

        }
    }
    delete[] buffer;
    delete[] tmp_buffer;

}
#ifndef __PROGTEST__

uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
//     srand(time(NULL));
#define STEP 100ll
    int val[STEP];
    for(int i=0; i<size; i+=STEP){
        for(int j=0; j<STEP && i+j < size; ++j){
            val[j]=-1000 + (rand()%(2*1000+1));
            total_sum_mod += val[j];
        }
        flib_write(output, val, (STEP < size-i) ? STEP : size-i);
    }
    flib_close(output);
}

void tarant_allegra ( int in_file, int out_file, int bytes );

void check_result ( int out_file, int SIZE ){
    flib_open(out_file, READ);
    int q[30], loaded, last=-(1<<30), total=0;
    uint64_t current_sum_mod=0;
    while(loaded = flib_read(out_file, q, 30), loaded != 0){
        total += loaded;
        for(int i=0; i<loaded; ++i){
            if(last > q[i]){
                printf("the result file contains numbers %d and %d on position %d in the wrong order!\n", last, q[i], i-1);
                exit(1);
            }
            last=q[i];
            current_sum_mod += q[i];
        }
    }
    if(total != SIZE){
        printf("the output contains %d but the input had %d numbers\n", total, SIZE); exit(1);
    }
    if(current_sum_mod != total_sum_mod){
        printf("the output numbers are not the same as the input numbers\n");
        exit(1);
    }
    flib_close(out_file);
}

int main(int argc, char **argv){
    const uint16_t MAX_FILES = 65535;
    flib_init_files(MAX_FILES);
    int INPUT = 10;
    int RESULT = 0;
    int SIZE = 125;

    create_random(INPUT, SIZE);
//    create_random(INPUT, 1000);
    tarant_allegra(INPUT, RESULT, 1000);
//    check_result(RESULT, 1000);
    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
