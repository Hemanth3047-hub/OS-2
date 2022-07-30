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
#include<fstream>
#include<bits/stdc++.h>//header files
std::string time_in_HH_MM_SS_MMM()//function returns exact time string
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}
using namespace std;

default_random_engine generator;//global variables
double lambda1,lambda2;
int  n,k;
ofstream f2;
int L=0;
double worst=0;
int expected=0;
int desired=1;
bool waiting[1000];
string req_enter_time[1000][1000];
string  act_enter_time[1000][1000];
string  exit_time[1000][1000];
int compare_and_swap(int *temp,int a,int b)//CAS algorithm
{ 

   if(*temp==0)
      {
        *temp=1;
        return 0;
      }
      else
    return 1;
 
}
void testCS(int a,double t1,double t2)//test Cs 
 {
   int p;
  thread::id id=this_thread::get_id();
 for(p=0;p<k;p++)
 {
  int key;
   int j;
  
   req_enter_time[a][p]=time_in_HH_MM_SS_MMM();//ctime(&reqEnterTime);
   waiting[a]=true;
   key=1;
   while(waiting[a] && key==1)
    key=compare_and_swap(&L,expected,desired);
  waiting[a]=false;
   //entry sec ends
   act_enter_time[a][p]=time_in_HH_MM_SS_MMM();//ctime(&actEnterTime);
   sleep(t1);// Simulation of critical-section
   j=(a+1)%n;
   while((j!=a) && !waiting[j])
    j=(j+1)%n;
  if(j==a)
    L=0;
  else
    waiting[j]=false;
   exit_time[a][p]=time_in_HH_MM_SS_MMM();
   sleep(t2);//Simulation of Reminder Section
   }
    pthread_exit(NULL);
}


void print()//function to print
{
   for(int i=0;i<k;i++)
   {
      for(int j=0;j<n;j++)
      {
       f2<<(i+1)<<" th CS Request at "<<req_enter_time[j][i]<<" by thread_"<<(j+1)<<endl;
       f2<<(i+1)<<" th CS Entry at "<<act_enter_time[j][i]<<" by thread_"<<(j+1)<<endl;
       f2<<(i+1)<<" th CS Exit at "<<exit_time[j][i]<<" by thread_"<<(j+1)<<endl;
      }
   }
}
int main() //main function
{
   ifstream f1;
   f1.open("inp-params.txt");//opening file to read
   f2.open("Bounded ME with CAS.txt");
   int rc;
   int i;
   double t1,t2;
   f1>>n>>k>>lambda1>>lambda2;//taking input from file
   exponential_distribution<double> d1((float)1/lambda1);
   exponential_distribution<double> d2((float)1/lambda2);
   
   thread threads[n];

   for(int i=0;i<n;i++)
   {
    t1=(float)d1(generator)/1000;
    t2=(float)d2(generator)/1000;
    threads[i]=thread(testCS,i,t1,t2);//creating and assignng 
   }
   for(int i=0;i<n;i++)
    threads[i].join();//joining of threads
   print();//calling print

  cout << "Main: completed";
}
