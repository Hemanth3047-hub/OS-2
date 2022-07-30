#include <iostream>
#include <pthread.h>
#include<ctime>
#include<thread>
#include<string>
#include<random>
#include<iomanip>
#include<chrono>
#include<fstream>
#include<time.h>
#include <semaphore.h>
#include <unistd.h>
#include<limits.h>
#include<bits/stdc++.h>//including header files 
std::string time_in_HH_MM_SS_MMM()
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<microseconds>(now.time_since_epoch()) % 1000000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(6) << ms.count();

    return oss.str();
}
using namespace std;
sem_t MUTE;//defining variables globally
sem_t block;
ofstream f2;
int eating=0;
int waiting=0;
int X;
int N;
float L;
string req_enter_time[10000];//array to store request time strap as a string
string  act_enter_time[10000];//array to access  time strap as a string
string  exit_time[10000];//array to exit  time strap as a string
double ENTER_TIME[3][10000];//stores above 3 in terms of milli seconds

default_random_engine generator;
bool must_wait=false;//initialising to false
void testCS(int a,double T)
{
    uint64_t r;
    r=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ENTER_TIME[0][a]=r;//storing in array
	req_enter_time[a]=time_in_HH_MM_SS_MMM();//taking the time strap
	int k;
	sem_wait(&MUTE);//MUTE.wait()
	if(must_wait==true || eating+1>X)//condition for table is full
		{
			waiting++;//increment
			must_wait=true;//assigning it to true
			sem_post(&MUTE);//MUTE.signal()
			sem_wait(&block);//block.wait()
		}
	else
	{
		eating++;//increment
		must_wait=(waiting>0 && eating==X);//condition for table is full
		sem_post(&MUTE);//MUTE.signal()
	}

	r=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ENTER_TIME[1][a]=r;//access time
	act_enter_time[a]=time_in_HH_MM_SS_MMM();//storing access time strap
	
    sleep(T);//eating time

	sem_wait(&MUTE);//MUTE.wait()
	eating--;
	if(eating==0)//table empty
	{
      if(X>waiting)
      	k=waiting;
      else
      	k=X;
      waiting-=k;//decrement by k
      eating+=k;
      must_wait=(eating==X && waiting>0);//table full consition
      	for(int i=0;i<k;i++)       
      		    sem_post(&block);//block.signal()     
}
	sem_post(&MUTE);//MUTE.signal()
	r=std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ENTER_TIME[2][a]=r;//exit time
	exit_time[a]=time_in_HH_MM_SS_MMM();
	
}
void statement(int x,int y)//function to print
{
	 if(x==0)
          f2<<(y+1)<<" th Customer access request at "<<req_enter_time[y]<<endl;
     else if(x==1)
          f2<<(y+1)<<" th Customer given access at "<<act_enter_time[y]<<endl;
     else if(x==2)
          f2<<(y+1)<<" th Customer Exit at "<<exit_time[y]<<endl;
}
void print()
{
   double min;
   
   for(int i=0;i<(3*N);i++)//finding the min one by one  
      {
   	   int x=0;
   	   int y=0;
       min=DBL_MAX;
       for(int j=0;j<3;j++)
         {
       	   for(int l=0;l<N;l++)
       	     {
       		    if(min>ENTER_TIME[j][l] && ENTER_TIME[j][l]>0)
       			  {
       			  	min=ENTER_TIME[j][l];
       				x=j;//storing the indexes of min value time
       				y=l;
       			  }
       	     }
         }
        statement(x,y);//calling statemnet function to print information
        ENTER_TIME[x][y]=-1;//assigning it to -1
    }
}

int main(void)
{
	ifstream f1;
	f1.open("input.txt");//opening file to read
	f2.open("OUTPUT.txt");//output  file
	float lambda,r;
	f1>>N>>X>>lambda>>r>>L;//extracting data from file and storing in variables
	int n=0;
	sem_init(&MUTE,0,1);//assigning to 1
	sem_init(&block,0,0);//assigning to 0
	double t1,t2;
	thread threads[N];//N threads
	exponential_distribution<double> d1(lambda);
	
	srand(time(NULL));
	while(n<N)//condition for creating exactly N threads
	{
		t1=(float)d1(generator)/1000;
		sleep(t1);//delay time 
		int x=r*X;
		x=rand()%(x)+1;//random number group
		for(int i=0;i<x;i++)
		{
			    if(i+n<N)//condition to exactly create N no of threads
			    {
			      exponential_distribution<double> d2((float)1/L);
			      t2=(float)d2(generator)/1000;//eating time for respective thread
		        threads[i+n]=thread(testCS,i+n,t2);//creating and assigning and passing index i+n and eating time t2   
	            }
	    }
	 n=n+x;//incrementing n by x
}
for(int i=0;i<N;i++)   
	    threads[i].join();//joining of threads
	print();

	cout<<"File created and saved successfully";

}