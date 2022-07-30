#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<stdlib.h>
#include<limits.h>
long arr[10000000];//defining array globally
int start=0;//start vertex of array
int end=0;//end vertex of array
FILE *f1,*f3;//defining file pointer
// Merge two subarrays L and M into arr
void MERGE(int p, int q, int r) {

  // Create X ← A[p..q] and Y ← A[q+1..r]
  int x = q - p + 1;
  int y = r - q;

  int X[x], Y[y];

  for (int i = 0; i < x; i++)
    X[i] = arr[p + i];
  for (int j = 0; j < y; j++)
    Y[j] = arr[q + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i, j, k;
  i = 0;
  j = 0;
  k = p;

  // Until we reach either end of either X or Y, pick larger among
  // elements X and Y and place them in the correct position at A[p..r]
  while (i < x && j < y) {
    if (X[i] <= Y[j]) {
      arr[k] = X[i];
      i++;
    } else {
      arr[k] = Y[j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either X or Y,
  // pick up the remaining elements and put in A[p..r]
  while (i < x) {
    arr[k] = X[i];
    i++;
    k++;
  }

  while (j < y) {
    arr[k] = Y[j];
    j++;
    k++;
  }
}

// Divide the array into two subarrays, sort them and merge them
void merge( int l, int r) {
  if (l < r) {

    // m is the point where the array is divided into two subarrays
    int m=(r + l) / 2;

    merge(l, m);
    merge(m + 1, r);

    // Merge the sorted subarrays
    MERGE(l, m, r);
  }
}
void* mergesort(void* p)
{
	merge(start,end);
	return NULL;
	pthread_exit(NULL);
}
// Print the array
void printArray(int size) 
{
	fprintf(f3,"\n");
  for (int i = 0; i < size; i++)
    fprintf(f3,"%ld ", arr[i]);
  
}

// Driver program
int main() 
{
	f1=fopen("inp.txt","r");//opening the file named ass6.txt and assigning it to read mode and pointing to f1
    f3=fopen("output.txt","w");//creating the output file to print the output
 int n,p;
 fscanf(f1,"%d %d",&n,&p);//scanning from inp.txt file

 n=pow(2,n);//n=2^n
 p=pow(2,p);//p=2^p
 
 double time_taken;

 for(int i=0;i<10000000;i++)//assigning all the elements of array to 0
      arr[i]=0;
 srand(time(NULL));
  int i=0;
  //printf("Initial array before sorting:");
  while(i<n)
   {
     int t=rand();//randomly allocationg numbers to array
         arr[i]=t;
         //if(P==9)
         fprintf(f3,"%ld ", arr[i]);//printing in output.txt file
         i++;
   }
clock_t T;//beginning time  
  T = clock();//starting the clock
pthread_t  p_thread[p];
for(int i=0;i<p;i++)//creating 2^p threads and assigning them to sort array
   {
    start=i*(n/p);
    end=(i+1)*(n/p)-1;
	pthread_create(&p_thread[i],NULL,mergesort,NULL);
	//pthread_join(p_thread[i],NULL);
   }
for(int i=0;i<p;i++)
{
	pthread_join(p_thread[i],NULL);
}
for(int i=0;i<p;i++)//merging 2^p threads and assigning them to sort array
{
  start=0;
  end=(n/p)*(i+1)-1;
  merge(start,end);
}
 T = clock() - T; 
      time_taken = ((double)T)/CLOCKS_PER_SEC; // in seconds 

  printArray(n);//printing sorted array in output.txt file
  fprintf(f3,"Time taken: %f micro seconds \n", time_taken*1000000);//printing the time taken for execution in output.txt
  printf("File created and saved successfully");//prompt
}