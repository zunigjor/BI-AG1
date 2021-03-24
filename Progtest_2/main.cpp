#ifndef __PROGTEST__

//#include <ctime>
#include "flib.h"
#endif //__PROGTEST__

/**
 * Swaps values of given parameters.
 */
void swap(int32_t *x, int32_t *y){
    int32_t temp = *x;
    *x = *y;
    *y = temp;
}

/**
 * CMinHeap101 class stores 101 elements
 */
class CMinHeap101 {
private:
    int32_t m_HeapArr[101];
    int32_t m_Capacity = 101;
    int32_t m_HeapSize = 0;
public:
    /**
     * Default constructor
     */
    CMinHeap101() = default;
    /**
     * Returns index of the parent of node i
     * @param i Index of selected node
     * @return Index of parent node
     */
    int32_t parent (int32_t i) { return (i-1)/2; }
    /**
     * Returns index of left child of node i
     * @param i Index of selected node
     * @return Index of parent node
     */
    int32_t left   (int32_t i) { return (2*i + 1); }
    /**
     * Returns index of right child of node i
     * @param i Index of selected node
     * @return Index of parent node
     */
    int32_t right  (int32_t i) { return (2*i + 2); }
    /**
     * Inserts a new key into the minheap, if the capacity is full, it doesnt do anything.
     * Maintains the min heap property.
     * @param k Key to insert
     */
    void insertKey(int32_t k){
        if (m_HeapSize == m_Capacity)
            return;
        // First insert the new key at the end
        m_HeapSize++;
        int32_t i = m_HeapSize - 1;
        m_HeapArr[i] = k;
        // Fix the min heap property if it is violated
        while (i != 0 && m_HeapArr[parent(i)] > m_HeapArr[i]){
            swap(&m_HeapArr[i], &m_HeapArr[parent(i)]);
            i = parent(i);
        }
    }
    /**
     * Delete key at index i
     * @param i Index of key to be deleted
     */
    void deleteKey(int i){
        decreaseKey(i, INT32_MIN);
        extractMin();
    }
    /**
     * Applies the heapify algorithm to the subtree at index i
     * @param i Index of the root at which heapify gets called.
     */
    void CMinHeapify(int32_t i){
        int32_t l = left(i);
        int32_t r = right(i);
        int32_t smallest = i;
        if (l < m_HeapSize && m_HeapArr[l] < m_HeapArr[i])
            smallest = l;
        if (r < m_HeapSize && m_HeapArr[r] < m_HeapArr[smallest])
            smallest = r;
        if (smallest != i){
            swap(&m_HeapArr[i], &m_HeapArr[smallest]);
            CMinHeapify(smallest);
        }
    }
    /**
     * Extracts the minimum from the minheap, also removes it
     * @return Minimal value in minheap (root node)
     */
    int extractMin(){
        if (m_HeapSize <= 0)
            return INT32_MAX;
        if (m_HeapSize == 1){
            m_HeapSize--;
            return m_HeapArr[0];
        }
        int root = m_HeapArr[0];
        m_HeapArr[0] = m_HeapArr[m_HeapSize-1];
        m_HeapSize--;
        CMinHeapify(0);
        return root;
    }
    /**
     * Changes val at index i to new_val
     * @param i Index of val
     * @param new_val New value to be placed
     */
    void decreaseKey(int i, int new_val){
        m_HeapArr[i] = new_val;
        while (i != 0 && m_HeapArr[parent(i)] > m_HeapArr[i]){
            swap(&m_HeapArr[i], &m_HeapArr[parent(i)]);
            i = parent(i);
        }
    }
    /**
     * Return minimum key (at root)
     * @return Minimum key from min heap
     */
    int getMin() { return m_HeapArr[0]; }
};

/**
 * Used to pass a test where input's numbers are a maximum of 100 spaces away from their ordered position.
 * @param in_file input file ID
 * @param out_file output file ID
 * @param space Space on the heap for int32_t numbers.
 * @return True if the function succeeded, false otherwise.
 */
bool minheapParkour(int32_t in_file, int32_t out_file, int32_t space){
    if (space/2 <= 101)
        return false;
    CMinHeap101 minHeap101;
    auto * buffer_In  = new int32_t [space/2];
    auto * buffer_Out = new int32_t [space/2];
    int32_t iter_In = 0;
    int32_t iter_Out = 0;
    flib_open(in_file, READ);
    flib_open(out_file, WRITE);
    int32_t read = flib_read(in_file, buffer_In, space/2);
    if (read <= 101){
        flib_close(in_file);
        flib_close(out_file);
        delete [] buffer_In;
        delete [] buffer_Out;
        return false;
    }
    while (iter_In < 101)
        minHeap101.insertKey(buffer_In[iter_In++]);

    while (true){
        buffer_Out[iter_Out] = minHeap101.extractMin();
        minHeap101.insertKey(buffer_In[iter_In]);
        if (buffer_Out[iter_Out] > buffer_In[iter_In]){
            flib_close(in_file);
            flib_close(out_file);
            delete [] buffer_In;
            delete [] buffer_Out;
            return false;
        }
        iter_In++;
        iter_Out++;

        if (iter_Out == space/2){ // outbuffer full, write and start from beginning
            flib_write(out_file, buffer_Out, space/2);
            iter_Out = 0;
        }

        if (iter_In == read){ // inbuffer passed, get new input
            read = flib_read(in_file, buffer_In, space/2);
            iter_In = 0;

            if (read == 0){ // read less than cap of buffer => end of file
                flib_write(out_file, buffer_Out, iter_Out);
                iter_Out = 0;
                int32_t min;
                while ( (min = minHeap101.extractMin()) != INT32_MAX ){
                    buffer_Out[iter_Out++] = min;
                }
                flib_write(out_file, buffer_Out, iter_Out);
                flib_close(in_file);
                flib_close(out_file);
                delete [] buffer_In;
                delete [] buffer_Out;
                return true;
            }
        }
    }
}

/**
 * Qsort compare function.
 */
int int32Compare(const void * a, const void * b){
    if ( *(int32_t*)a > *(int32_t*)b )
        return 1;
    else if ( *(int32_t*)a < *(int32_t*)b )
        return -1;
    else
        return 0;
}

/**
 * Splits the in_file into smaller files of heap size.
 * The smaller files get sorted by qsort.
 * @param in_file ID of the input file with numbers to be sorted
 * @param space Number of int32_t on the heap available for the operation
 * @return Number of temp files
 */
int32_t split( const int32_t in_file, const int32_t space){
    auto * tempArr = new int32_t[space];
    int32_t currentID = 2; // Start of tmp files ID, io files have reserved IDs 1 and 0
    int32_t numOfTmpFiles = 0;
    flib_open(in_file, READ);
    bool eof = false;
    while (!eof) {
        int64_t numsRead;
        numsRead = flib_read(in_file, tempArr, space);
        if (numsRead != space)
            eof = true;
        qsort(tempArr, numsRead, sizeof(int32_t), int32Compare);
        flib_open(currentID, WRITE);
        flib_write(currentID, tempArr, numsRead);
        flib_close(currentID);
        currentID++;
        numOfTmpFiles++;
    }
    flib_close(in_file);
    delete [] tempArr;
    return numOfTmpFiles;
}

/**
 * Merges file A and file B into outFile.
 * @param fileA_ID File A ID
 * @param fileB_ID File B ID
 * @param fileOut_ID OutfileID
 * @param space space available
 */
void fileMerge(const int32_t fileA_ID, const int32_t fileB_ID, const int32_t fileOut_ID, const int32_t space){
    int32_t quarterHeap = space/4;
    auto * buffer_A     = new int32_t[ quarterHeap ];
    auto * buffer_B     = new int32_t[ quarterHeap ];
    auto * buffer_Out   = new int32_t[ quarterHeap * 2 ];
    flib_open(fileA_ID, READ);
    flib_open(fileB_ID, READ);
    flib_open(fileOut_ID, WRITE);
    // Merge file A and B using buffers
    int32_t read_A = 0, read_B = 0;
    int iter_A =0, iter_B = 0, iter_Out = 0;
    while (true){
        // Fill buffers
        if (iter_A == read_A){
            read_A = flib_read(fileA_ID, buffer_A, quarterHeap);
            iter_A = 0;
        }
        if (iter_B == read_B){
            read_B = flib_read(fileB_ID, buffer_B, quarterHeap);
            iter_B = 0;
        }
        // One of the files has ended.
        if (read_A == 0 || read_B == 0)
            break;
        // Compare numbers in buffers and place in buffer_Out
        if (buffer_A[iter_A] < buffer_B[iter_B])
            buffer_Out[iter_Out++] = buffer_A[iter_A++];
        else
            buffer_Out[iter_Out++] = buffer_B[iter_B++];
        // when the output buffer is full, write to file and reset the buffer
        if (iter_Out == quarterHeap * 2){
            flib_write(fileOut_ID, buffer_Out, quarterHeap * 2);
            iter_Out = 0;
        }
    }
    // Dump the rest of the buffer into the output file and reset the buffer
    flib_write(fileOut_ID, buffer_Out, iter_Out);
    // Dump the rest of the file A or file B buffer into the output (one of theese will be empty)
    flib_write(fileOut_ID, buffer_A + iter_A, read_A - iter_A);
    flib_write(fileOut_ID, buffer_B + iter_B, read_B - iter_B);
    // input A or B might still have lots of numbers in them, so keep dumping until the read != 0
    // (one of these will be empty)
    while ( (read_A = flib_read(fileA_ID, buffer_A, quarterHeap)) )
        flib_write(fileOut_ID, buffer_A, read_A);
    while ( (read_B = flib_read(fileB_ID, buffer_B, quarterHeap)) )
        flib_write(fileOut_ID, buffer_B, read_B);
    // Close all files in use
    flib_close(fileA_ID);
    flib_close(fileB_ID);
    flib_close(fileOut_ID);
    delete [] buffer_A;
    delete [] buffer_B;
    delete [] buffer_Out;
}

/**
 * Merges tempFiles into a sorted final file
 * @param out_file ID of the output file, where sorted numbers should be stored
 * @param space Number of int32_t on the heap available for the operation
 * @param numOfTmpFiles Number of temp files
 */
void merge(const int32_t out_file, const int32_t space, int32_t numOfTmpFiles){
    // Solution for single temp file
    if (numOfTmpFiles == 1){
        auto * buffer = new int32_t [space];
        flib_open(2, READ);
        int32_t read = flib_read(2, buffer, space);
        flib_close(2);
        flib_remove(2);
        flib_open(out_file, WRITE);
        flib_write(out_file, buffer, read);
        flib_close(out_file);
        delete [] buffer;
        return;
    }
    // Solution for two and more temp files
    for (int32_t i = 0; i+2 < numOfTmpFiles; i+=2) {
        int32_t fileA_ID = i + 2; // Temp file IDs start from the number 2
        int32_t fileB_ID = i + 3;
        fileMerge(fileA_ID, fileB_ID, numOfTmpFiles+2, space);
        flib_remove(fileA_ID);
        flib_remove(fileB_ID);
        numOfTmpFiles++;
    }
    // Merge last two files into output file
    fileMerge(numOfTmpFiles, numOfTmpFiles + 1, out_file, space);
    flib_remove(numOfTmpFiles);
    flib_remove(numOfTmpFiles + 1);
}

/*!
 * Sorts numbers from input file, into output file, using assigned heap memory.
 * in_file and out_file IDs are always 0 and 1, or 1 and 0.
 * in_file can only be read, out_file can only be written into.
 * @param in_file ID of the input file with numbers to be sorted
 * @param out_file ID of the output file, where sorted numbers should be stored
 * @param bytes Number of bytes on the heap available for the operation
 */
void tarant_allegra ( int32_t in_file, int32_t out_file, int32_t bytes ){
    int32_t space = (bytes - (3*12)) / sizeof(int32_t); // reserve space for 3 open files and divide by sizeof int32
    if ( ! minheapParkour(in_file, out_file, space) ){
        int32_t numOfTmpFiles = split(in_file, space);
        merge(out_file, space, numOfTmpFiles);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __PROGTEST__

uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
    //srand(time(NULL));
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

int main(){
    const uint16_t MAX_FILES = 65535;
    flib_init_files(MAX_FILES);
    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 111;

    create_random(INPUT, SIZE);
    tarant_allegra(INPUT, RESULT, 2000);
    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
