#ifndef __PROGTEST__
#include "flib.h"
#endif //__PROGTEST__


int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void mergeFiles(int id1, int id2, int idOutput, int bufSize){
    int * buffer1 = new int [bufSize/4];
    int * buffer2 = new int [bufSize/4];
    flib_open(id1, READ);
    flib_open(id2, READ);
    flib_open(idOutput, WRITE);

    int * bufferOutput = new int [bufSize/2];

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
}


void tarant_allegra ( int32_t in_file, int32_t out_file, int32_t bytes ){
    flib_open(in_file, READ);
    int numInts = bytes/4 - 9;
    int * buffer = new int [numInts];
    int numRead;
    int fileCnt = 0;
    do{
        int fileID = fileCnt + 2;
        numRead = flib_read(READ,buffer, numInts);

        qsort(buffer, numRead, sizeof(int), cmpfunc);
        flib_open(fileID, WRITE);
        flib_write(fileID, buffer,numRead);
        flib_close(fileID);
        fileCnt++;
    }
    while(numRead == numInts);


//    //DEBUG
//    int * arr = new int [2000];
//    flib_open(2, READ);
//    int numNums = flib_read(2, arr, 2000);
//    int index = 0;
//    while(index<numNums){
//        printf("next: %d \n", arr[index]);
//        index++;
//    }
//    flib_close(2);
//
//
//    //end of debug

    flib_close(in_file);
    delete [] buffer;
    int firstIndex = 2;
    while(fileCnt >2){
        if(fileCnt % 2 == 1){
            mergeFiles(fileCnt/2+1, firstIndex, firstIndex + fileCnt + 1, numInts);
        }
        firstIndex ++;
        fileCnt --;
        for(int i = firstIndex ; i<firstIndex + fileCnt/2;i++){
            mergeFiles(i, fileCnt-i, firstIndex + fileCnt + 1, numInts);
        }
        fileCnt /=2;
        firstIndex+=fileCnt + 1;
    }
    if(fileCnt == 2){
        //todo test merging multiple files
        mergeFiles(firstIndex, firstIndex+1,out_file, numInts);
    } else{
        //kinda hacky
        flib_open(firstIndex+1, WRITE);
        flib_close(firstIndex +1);
        mergeFiles(firstIndex, firstIndex+1, out_file, numInts);
    }



}


#ifndef __PROGTEST__

uint64_t total_sum_mod;
void create_random(int output, int size){
    total_sum_mod=0;
    flib_open(output, WRITE);
    /* srand(time(NULL)); */
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
    int INPUT = 0;
    int RESULT = 1;
    int SIZE = 140;

    create_random(INPUT, SIZE);
    tarant_allegra(INPUT, RESULT, 1000);
    check_result(RESULT, SIZE);

    flib_free_files();
    return 0;
}
#endif //__PROGTEST__
