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
#include<bits/stdc++.h>//including standard libraries
std::string time_in_HH_MM_SS_MMM()//function whch return exact time
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

default_random_engine generator;//defining variables globally
double lambda1,lambda2;
int  n,k;
ofstream f2;
atomic<int> L;
int expected=0;
int desired=1;
string req_enter_time[1000][1000];
string  act_enter_time[1000][1000];
string  exit_time[1000][1000];
void entry_sec()//entry sec
{
  
   while(L.compare_exchange_strong(expected,desired))
      ;
}
void exit_sec()//exit sec function
{
   L=0;
}
void testCS(int a,double t1,double t2)//test CS function
 {
   int p;

  thread::id id=this_thread::get_id();
 for(p=0;p<k;p++)
 {
   req_enter_time[a][p]=time_in_HH_MM_SS_MMM();//ctime(&reqEnterTime);
   entry_sec(); // Entry Section
   act_enter_time[a][p]=time_in_HH_MM_SS_MMM();//ctime(&actEnterTime);
   sleep(t1);// Simulation of critical-section
   exit_sec();//Exit Section
   exit_time[a][p]=time_in_HH_MM_SS_MMM();//ctime(&exitTime);
   sleep(t2);//Simulation of Reminder Section
   }
    pthread_exit(NULL);
}
void print()
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
   f2.open("CAS.txt");
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
