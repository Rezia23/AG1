#ifndef __PROGTEST__

#include <ctime>
#include "flib.h"
#endif //__PROGTEST__


int cmpfunc (const void * a, const void * b) {
    return ( *(int32_t*)a > *(int32_t*)b ) - ( *(int32_t*)a < *(int32_t*)b ) ;
}

void mergeFiles(int id1, int id2, int idOutput, int bufSize){


    int32_t * buffer1 = new int32_t [bufSize/4];
    int32_t * buffer2 = new int32_t [bufSize/4];
    int32_t * bufferOutput = new int32_t [bufSize/2];

    flib_open(id1, READ);
    flib_open(id2, READ);
    flib_open(idOutput, WRITE);

    int count1 = 0;
    int count2 = 0;
    bool eof1 = false;
    bool eof2 = false;
    int index1 = 0;
    int index2 = 0; ///why do i have so many variables
    int indexOut = 0;

    while(1){
        if(count1 == 0 && !eof1){
            count1 = flib_read(id1, buffer1, bufSize/4);
            index1 = 0;
            if(count1 != bufSize/4){
                eof1 = true;
            }
        }
        if(count2==0 && !eof2){
            count2 = flib_read(id2, buffer2, bufSize/4);
            index2 = 0;
            if(count2 != bufSize/4){
                eof2 = true;
            }
        }
        if(count1 == 0 && count2 ==0){
            break;
        }
        if(count1 == 0){
            bufferOutput[indexOut] = buffer2[index2];
            index2++;
            count2--;
        } else if(count2 == 0) {
            bufferOutput[indexOut] = buffer1[index1];
            index1++;
            count1--;
        } else if(buffer1[index1] < buffer2[index2]){
            bufferOutput[indexOut] = buffer1[index1];
            index1++;
            count1--;
        } else{
            bufferOutput[indexOut] = buffer2[index2];
            index2++;
            count2--;
        }
        indexOut++;
        if(indexOut==bufSize/4){
            flib_write(idOutput, bufferOutput, indexOut);
            indexOut = 0;
        }
    }
    if(indexOut != 0){
        flib_write(idOutput, bufferOutput, indexOut);
    }
    flib_close(id1);
    flib_close(id2);
    flib_close(idOutput);

    flib_remove(id1);
    flib_remove(id2);

    delete [] buffer1;
    delete [] buffer2;
    delete [] bufferOutput;
}

void insertElement(int32_t element, int32_t * heap, int heapSize){
    heapSize++;
    heap[heapSize] = element;
    //bubble up
    int pos = heapSize;
    while (heap[pos/2] > element) {
        heap[pos] = heap[pos/2];
        heap[pos/2] = element;
        pos /= 2;
    }
}


void createHeap(int32_t * heap, int32_t * buffer_in, int maxSize){
    int heapSize = 0;
    heap[0] = -INT32_MAX;
    for(int i = 0; i<maxSize;i++){
        insertElement(buffer_in[i],heap, heapSize);
        heapSize++;
    }

}

int smallerChild(int i, int32_t * heap, int heapSize){
    int child = 2*i;
    if(child!=heapSize && heap[child+1]<heap[child]){
        child++;
    }
    return child;
}

int extractMin(int32_t * heap, int heapSize){
    int min = heap[1];
    int child;
    heap[1] = heap[heapSize];
    for(int i = 1; i*2<heapSize;i = child){
        child = smallerChild(i, heap, heapSize);
        if(heap[i]>heap[child]){
            int tmp = heap[i];
            heap[i] = heap[child];
            heap[child] = tmp;
        }else{
            //bubbled down sucessfully
            return min;
        }
    }
    return min;
}


bool try_bonus(int32_t in_file, int32_t out_file, int thirdOfInts){
    flib_open(in_file, READ);
    flib_open(out_file, WRITE);
    
    int32_t * buffer_in = new int32_t [thirdOfInts];
    int32_t * buffer_out = new int32_t [thirdOfInts];
    int32_t * heap = new int32_t [thirdOfInts+1];

    int heapSize = flib_read(in_file, buffer_in, thirdOfInts);
    createHeap(heap, buffer_in, heapSize);

    //todo off by one in heap?

//    printf("debug\n");
//    for(int i = 0; i<thirdOfInts;i++){
//        printf("%d ", heap[i]);
//    }


    int absolute_min = heap[1];
    int index_in = 0;
    int index_out = 0;
    bool readEverything = false;
    int numRead = flib_read(in_file, buffer_in, thirdOfInts);
    do{
        //out is full - write
        if(index_out==thirdOfInts){
            flib_write(out_file, buffer_out, thirdOfInts);
            index_out = 0;
        }
        //in is empty - read
        if(index_in == numRead){
            if(numRead!=thirdOfInts){
                if(heapSize!=0){
                    readEverything = true;
                }else{
                    if(index_out!=0){
                        flib_write(out_file, buffer_out, index_out);
                    }
                    delete [] buffer_out;
                    delete [] buffer_in;
                    delete [] heap;
                    flib_close(in_file);
                    flib_close(out_file);
                    return true;
                }
            } else{
                numRead = flib_read(in_file, buffer_in, thirdOfInts);
                index_in = 0;
            }
        }
        //extract min - if smaller than absolute min, return false
        int min = extractMin(heap, heapSize);
        if(min<absolute_min){
            //delete everything and close files
            delete [] buffer_in;
            delete [] heap;
            delete [] buffer_out;
            flib_close(in_file);
            flib_close(out_file);
            return false;
        }else{
            buffer_out[index_out] = min;
            index_out++;
            //add element from in
            //todo possibly leave out correcting right after extracting min
            if(!readEverything){
                insertElement(buffer_in[index_in], heap, thirdOfInts-1);
                index_in++;
            } else{
                heapSize--;
            }
        }


    }while(1);


}


void tarant_allegra ( int32_t in_file, int32_t out_file, int32_t bytes ){
    
    int numInts = (bytes-250)/4/2;
    int isSorted = try_bonus(in_file, out_file, numInts/3);
    if (!isSorted){
        //extract this somehow
        flib_open(in_file, READ);
        int32_t * buffer = new int32_t [numInts];
        int numRead;
        uint16_t fileCnt = 0;
        do{
            uint16_t fileID = fileCnt + 2;
            numRead = flib_read(in_file,buffer, numInts);
            if(numRead>0){
                qsort(buffer, numRead, sizeof(buffer[0]), cmpfunc);
                flib_open(fileID, WRITE);
                flib_write(fileID, buffer,numRead);
                flib_close(fileID);
                fileCnt++;
            }
        }
        while(numRead == numInts);

        flib_close(in_file);
        delete [] buffer;

        int firstIndex = 2;
        while(fileCnt >2){
            if(fileCnt % 2 == 1){
                mergeFiles(firstIndex, firstIndex+1, firstIndex + fileCnt, numInts);
                firstIndex +=2;
                fileCnt --;
            }
            for(int i = firstIndex ; i<firstIndex + fileCnt/2;i++){
                mergeFiles(i, (firstIndex + fileCnt)-(i-firstIndex)-1, i + fileCnt + 1, numInts);
            }
            firstIndex+=fileCnt + 1;
            fileCnt /=2;

        }
        if(fileCnt == 2){
            mergeFiles(firstIndex, firstIndex+1,out_file, numInts);
        } else{
            //kinda hacky
            flib_open(firstIndex+1, WRITE);
            flib_close(firstIndex +1);
            mergeFiles(firstIndex, firstIndex+1, out_file, numInts);
        }

    }

}


#ifndef __PROGTEST__
int SORTED = 1;
uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
//     srand(time(0));
#define STEP 100ll
    int val[STEP];
    for(int i=0; i<size; i+=STEP){
        for(int j=0; j<STEP && i+j < size; ++j){
            val[j]=-1000 + (rand()%(2*1000+1));
            if(SORTED){
                //my test
                val[j] = i*size + j + rand()%4;
            }
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
                printf("the result file contains numbers %d and %d on position %d in the wrong order!nope\n", last, q[i], i-1);
                exit(1);
            }
            last=q[i];
            current_sum_mod += q[i];
        }
    }
    if(total != SIZE){
        printf("the output contains %d but the input had %d numbers\nnope", total, SIZE); exit(1);
    }
    if(current_sum_mod != total_sum_mod){
        printf("the output numbers are not the same as the input numbers nope\n");
        exit(1);
    }
    flib_close(out_file);
}

int main(int argc, char **argv){
    const uint16_t MAX_FILES = 65535;
    flib_init_files(MAX_FILES);
    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 1000;
    srand(time(0));

    create_random(INPUT, SIZE);
    tarant_allegra(INPUT, RESULT, (1000*2*4)+400);
    check_result(RESULT, SIZE);

    for(SIZE = 550; SIZE<1002;SIZE+=200){
        for(int NB = 1010; NB <=2037;NB+=200){
            SORTED = rand()%2;
            create_random(INPUT, SIZE);
            tarant_allegra(INPUT, RESULT, NB);
            check_result(RESULT, SIZE);
        }
    }

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
