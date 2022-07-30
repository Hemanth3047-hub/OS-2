#include <iostream>
#include <cstdlib>
#include<cstdint>
#include <pthread.h>
#include <unistd.h>
#include<ctime>
#include<thread>
#include<string>
#include<random>
#include<atomic>
#include<chrono>
#include<time.h>
#include <semaphore.h>
#include<fstream>
#include<bits/stdc++.h>//including all the required header files
using namespace std;
int N,K;//defining these 2 variables globally
int TH[100][2];//partition 2-D array stores the start and end of the respective index numberTH partition
int Matrix[10000][10000];//stores the adjacency matrix
ifstream f1;//file
ofstream f2;
sem_t block;//semaphore for locking
 int elements[10000];//storing the vertoices shuffled
 int colour[10000];//colour of respective vertex

void partition(int TH[][2])//function to partition the given graph
{
	int y=0;//ininitialising
  int z=0;
  srand(time(NULL));//NULL
  for(int i=0;i<K;i++)
  {
  	if(i==K-1)//making all the remaining vertices to last thread
            {
            	TH[i][0]=z;
  	            TH[i][1]=N-1;
  	        }
  	else
  	{
  	  y=rand()%((N-z)-(K-i-1))+1;//assigning the vertices to a partition randomly
  	  TH[i][0]=z;//start index of vertices in the partition
  	  z=z+y;
  	  TH[i][1]=z-1;//end index
    }
  }
}
void input_Matrix()//function to store matrix given
{
	int n=0;
	for(int i=0;i<N;i++)
  	f1>>n;
  for(int i=0;i<N;i++)
  {
  	f1>>n;
  	if(n==i+1)
  	   {
  		for(int j=0;j<N;j++)
  			f1>>Matrix[i][j];//storing in the respective position		        
       }
       elements[i]=i;//assigning the vertex id to element array
  }
}

int find(int l,int m)//function to find whether given element is in the partition or not
{
	for(int i=TH[l][0];i<=TH[l][1];i++)//starting from start vertex of partition to end vertex ofm the partition
	{
		if(elements[i]==m)//internal
		return 1;
	}
	return 0;//external vertex
}
int find_LM(int m,int l)
{
	int c=0,c1=0;
		for(int j=0;j<N;j++)
		if(Matrix[l][j]==1)
          { 
          	c++;
            if(find(m,j)==1)
            c1++;
          }
          if(c1==c)//if all are internal
          	return 1;
          else//if anyone verytex linked to l is external
          	return 0;

}
void testCS(int a)//thread function to work on each partition
{
  int k;
	bool assign[N];//array to find whether given acolour can be used or not 
	for(int i=TH[a][0];i<=TH[a][1];i++)
		colour[elements[i]]=-1;//initialising to -1
		
	for(int i=0;i<N;i++)
		assign[i]=false;//ionitialising to false

	for(int i=TH[a][0];i<=TH[a][1];i++)//starting the loop
	{
		if(find_LM(a,elements[i])==0)//calling find_LM
			sem_wait(&block);//block.wait()
		for(int j=0;j<N;j++)
		{
			if(Matrix[elements[i]][j]==1)//checking whether the vertex has a edge
			{
				if(colour[j]!=-1)//if condition
					assign[colour[j]]=true;
			}
		}
		  if(find_LM(a,elements[i])==0)//calling find_LM
          sem_post(&block);//block.signal()    
		for(k=0;k<N;k++)
		{
			if(assign[k]!=true)
				break;
		}
		if(find_LM(a,elements[i])==0)//calling find_LM
		sem_wait(&block);//block.wait()
		colour[elements[i]]=k;//assigning colour to vertex
	
		for(int j=0;j<N;j++)//again re-initialising the assign array as previous
		{
			if(Matrix[elements[i]][j]==1)//edge
			{ 
				if(colour[j]!=-1)
					assign[colour[j]]=false;//false
         
		    }

		}
	if(find_LM(a,elements[i])==0)//calling find_LM
	sem_post(&block);//block.signal()  
   }
}
void no_of_colours()
{
	int col=0;
	for(int i=0;i<N;i++)
	{
       if(colour[i]>=col)
       	col=colour[i];//max value in the colour array denotes the no fo colours used
	}
	f2<<"No of colours used:"<<col+1<<endl;//print statement
}
void print()
{
	for (int u = 0; u < N; u++)
        f2<< "V" << u+1 << " - "<< colour[u] <<", ";
    f2<<endl;
}
int main()
{
	
	f1.open("input_params.txt");//opening file to read
	f2.open("output.txt");
	f1>>K>>N;
  sem_init(&block,0,1);//assigning to 1
  partition(TH);//calling partition function
  input_Matrix();//input_matrix
  random_shuffle(elements,elements+N);//shuffling all the vertices with in the elements array
   thread threads[K];
   uint64_t A1,A2;
   A1=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();//time in micro seconds
   for(int i=0;i<K;i++)
    threads[i]=thread(testCS,i);//create
   
   for(int i=0;i<K;i++)
    threads[i].join();//join
  A2=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();//time in micro seconds
f2<<"Time taken by the algorithm:"<<A2-A1<<"milli seconds"<<endl;//time taken 
no_of_colours();//no of colours
print();//print function
cout<<"Files created and saved successsfully";
}