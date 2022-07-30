#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<stdlib.h>
long arr[100000000];//defining array globally
int start=0;//defining start and end globally
int end=0;
int mid=0;
FILE *f1,*f3;//defining file pointer
// Merge two subarrays L and M into arr
void MERGE(int p, int q, int r) {

  // Create X← A[p..q] and Y ← A[q+1..r]
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
    int m = l + (r - l) / 2;

    merge(l, m);
    merge(m + 1, r);

    // Merge the sorted subarrays
    MERGE(l, m, r);
  }
}
void* MERGESORT(void* p)
{
  MERGE(start,mid,end);
}
void* mergesort(void* p)
{
  merge(start,end);
  pthread_exit(NULL);
  return NULL;
}
// Print the array
void printArray(int size)//function to print whole array 
{
  fprintf(f3,"\n");
  for (int i = 0; i < size; i++)
      fprintf(f3,"%ld ", arr[i]);
  
}

// Driver program
int main() 
{
 
    f1=fopen("inp.txt","r");//opening the file named inp.txt and assigning it to read mode and pointing to f1
    f3=fopen("output.txt","w");//creating new file to write the output 
 int n,p;
 fscanf(f1,"%d %d",&n,&p);

 n=pow(2,n);
 p=pow(2,p);
 double time_taken;
 
 for(int i=0;i<100000000;i++)
      arr[i]=0;
 srand(time(NULL));
  int i=0;

    fprintf(f3,"Initial array before sorting:");
  while(i<n)
   {
     double t=rand();
         arr[i]=t;
         fprintf(f3,"%ld ", arr[i]);//randomly assigning and printing in output.txt file
         i++;
   }

    i=p;
   int count=0;
 while(i>0)//counting total no of threads
 {
   count+=i;
   i=i/2;
 }
pthread_t  p_thread[count];
clock_t T;//beginning time  
  T = clock();
for(int i=0;i<p;i++)//loop for creating first 2^p threads
   {
    start=i*(n/p);
    end=(i+1)*(n/p)-1;
    pthread_create(&p_thread[i],NULL,mergesort,NULL);
    pthread_join(p_thread[i],NULL);
   }
int j=p;
int k=p;

while(k != 1)
   {
    for(int i=0;i<k;i=i+2)//creating the other threads to sort after joining to arrays assigned to initial threads 
      {
        
        start=i*(n/k);
        end=(i+2)*(n/k)-1;
         mid=start+(end-start)/2;
        pthread_create(&p_thread[j],NULL,MERGESORT,NULL);
        pthread_join(p_thread[j],NULL);
        
        j++;

      }
      k=k/2;
  }

  
  T = clock() - T; //stopping the clock to calculate time took until now from the start time
   time_taken= ((double)T)/CLOCKS_PER_SEC; // in seconds

  printArray(n);//prints the sorted array in output.txt file

  fprintf(f3,"Time taken= %f micro seconds ", time_taken*1000000);//prints the time took for execution in output.txt file
  printf("File created and saved successfully");//prompt
}