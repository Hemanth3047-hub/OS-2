#include <iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;
int process_id[1000];
    int period[1000];
    int execution_time[1000];
    int capacity[1000];
    int priority[1000];
    int count_value[1000];
    int waiting_time[1000];
    int n;
    int missing_process=0;
    int total_process=0;
    int mid_process=0;
    ofstream f2;
struct node
{
	struct node* next;
	int ID;
	int time;
	int priority;
}*head=NULL;
void waiting()
{
	struct node *temp;
	temp=head;
   if(temp!=NULL)
   {  temp=temp->next;
      while(temp!=NULL)
      {
      	waiting_time[priority[temp->priority]]++;
      	temp=temp->next;
      }
   }
}
void execution(int z)
{ 
	
	if(head!=NULL)
   {
   	f2<<"\nat time=["<<z<<","<<z+1<<"]\t";	
   	if(mid_process!=total_process)
	    f2<<"Process:"<<head->ID<<" occurs";
   if(head->time>1)
   head->time=head->time-1;
   else
   {
   	f2<<"\nProcess:"<<head->ID<<" execution completed successfully";
   	head=head->next;
   	mid_process++;
   	if(head!=NULL)
   	{

   		waiting_time[priority[head->priority]]++;
   	     f2<<"\nExecution of process-"<<head->ID<<" starts";
   }
}

}
else
{
	f2<<"\nat time=["<<z<<","<<z+1<<"]\t"<<"system is free";	
}

}
void add_into_queue(int a,int z)
{
	struct node *temp,*temp1,*temp2;
	temp=new node();
	temp->ID=process_id[z];
	temp->time=execution_time[z];
	temp->priority=a;
	temp->next=NULL;
	 temp1=head;
	 temp2=head;
	if(head==NULL)
		{
		
			head=temp;
			count_value[z]++;
			f2<<"\nExecution of process-"<<head->ID<<" starts";
			
		}
	else if(temp1->priority > temp->priority)
	{

	  f2<<"\npreemption of process-"<<head->ID<<" takes place";
	  head=temp;
	  head->next=temp1;
	  count_value[z]++;
	  f2<<"\nExecution of process-"<<head->ID<<" starts";
	  
	}
	else if(temp1->priority < temp->priority)
	{
        while(temp1!=NULL && temp1->priority<temp->priority)
        {   
        	temp2=temp1;
        	temp1=temp1->next;
    }
        if(temp1==NULL)
        {
        	temp2->next=temp;
            temp->next=temp1;
            count_value[z]++;
        }
        else if(temp1!=NULL && temp->priority!=temp1->priority)
        {
        	temp2->next=temp;
            temp->next=temp1;
            count_value[z]++;
       }
       else if(temp1!=NULL && temp->priority==temp1->priority)
       {
       	if(temp1->time<temp->time)
       	waiting_time[z]=waiting_time[z]+(temp->time-temp1->time);
       	 f2<<"\nProcess-"<<temp->ID<<" Deadline missed";
         temp2->next=temp;
         temp->next=temp1->next;
         missing_process++;
         mid_process++;
         count_value[z]++;
       }
	}
   else if(temp1->priority == temp->priority)
   {
   
    if(temp->ID == temp1->ID)
    {
       if(temp1->time<temp->time)
       	waiting_time[z]=waiting_time[z]+(temp->time-temp1->time);
   	f2<<"\nProcess-"<<temp->ID<<" Deadline missed";
    missing_process++;
   	temp->next=temp1->next;
   	head=temp;
   	count_value[z]++;
    mid_process++;
   }
   else
   {
      if(temp1->ID>temp->ID)
      {
        temp->next=temp1;
        head=temp;
      }
      else
      { 
        temp->next=temp1->next;
        head->next=temp;
      }
      count_value[z]++;
   }
 }
}
void check_period(int z)
{
	for(int i=0;i<n;i++)
		{
			if(z%(period[priority[i]])==0)
			  {
			  	if(count_value[priority[i]]<capacity[priority[i]])
			  	add_into_queue(i,priority[i]);
              }
        }
}
int main()
{
  ifstream f1;
  ofstream f3;
  f1.open("inp-params.txt");
  f2.open("RMS-logs.txt");
  f3.open("RMS-stats.txt");
    f1>>n;
    for(int i=0;i<n;i++)
    {
      f1>>process_id[i];
      f1>>execution_time[i];
      f1>>period[i];
      f1>>capacity[i];
      count_value[i]=0;
      waiting_time[i]=0;
      total_process=total_process+capacity[i];
    }
     int P[n];
    for(int i=0;i<n;i++)
    {
    	priority[i]=period[i];
    	P[i]=period[i];
    }

    sort(priority,priority+n);
    for(int i=0;i<n;i++)
    {       
    		for(int j=0;j<n;j++)
    		{
    			if(priority[i]==P[j])
    				{
    				   priority[i]=j;
    				   P[j]=0;
    				   break;
    			    }
    		}
    }
    int z=0;
   while(mid_process!=total_process)
   {
   	 
   	 check_period(z);
   	 execution(z);
   	 waiting();
   	 z++;
   }
   float average=0;
   for(int i=0;i<n;i++)  
      average=average+waiting_time[i];
   average=(float)average/total_process;
   f3<<"\nTotal no of process entered:"<<total_process;
   f3<<"\nTotal no of processes that are executing successfully:"<<total_process-missing_process;        
   f3<<"\nTotal no of processes that are missing their deadline:"<<missing_process;
   f3<<"\nAverage Waiting time:"<<average;
   cout<<"Files created and saved successfully";

}