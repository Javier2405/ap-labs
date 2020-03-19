#include <stdio.h>
#include <string.h>

void merge(char array[],int beg, int mid, int end){

    char leftPart[mid-beg+1],rightPart[end-mid];

    int arr_size_left = mid-beg+1;
    int arr_size_right = end-mid;

    //Separating the array into two different auxiliary arrays
    
    for(int i = 0; i<arr_size_left; i++){
        leftPart[i] = array[beg+i];
    }

    for(int i = 0; i<arr_size_right; i++){
        rightPart[i] = array[mid+i+1];
    }

    int i = 0; //pointer of leftpart
    int j = 0; //pointer of rightpart
    int global = beg; //pointer of final array

    //Merging both arrays

    while(i < arr_size_left && j < arr_size_right){
        if(leftPart[i]<=rightPart[j]){
            array[global]=leftPart[i];
            i++;
        }else{
            array[global]=rightPart[j];
            j++;
        }
        global++;
    }

    //Complete leftside
    while(i < arr_size_left){
        array[global]=leftPart[i];
        i++;
        global++;
    }

    //complete rightside
    while(j < arr_size_right){
        array[global]=rightPart[j];
        j++;
        global++;
    }
}

void mergeSort(char array[], int beg, int end){
    if(beg < end){
        int mid = (beg+end)/2;
        mergeSort(array, beg, mid);
        mergeSort(array, mid+1, end);
        merge(array, beg, mid, end);
    }

}

void printArray(char A[], int size) 
{ 
    int i;
    for (i=0; i < size; i++) {
        printf("%c",A[i]); 
    }
    printf("\n"); 
} 

int main()
{ 
    char arr[]="hola como estas";
    int arr_size =strlen(arr);
    printArray(arr, arr_size);
    mergeSort(arr, 0, arr_size - 1); 
    printArray(arr, arr_size); 
    return 0; 
}
