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
  int deadline;
}*head=NULL;
void waiting()
{
  struct node *temp;
  temp=head;
   if(temp!=NULL)
   {  
     temp=temp->next;
      while(temp!=NULL)
      {
        int b;
      for(int i=0;i<n;i++)
      {
        if(temp->ID==process_id[i])
            {
              b=i;
              break;
            }
      }
        waiting_time[priority[b]]++;
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
      f2<<"Process:"<<head->ID<<" occurs ";
   if(head->time>1)
   head->time=head->time-1;
   else
   {
    f2<<"\nProcess:"<<head->ID<<" execution completed successfully";
    head=head->next;
    mid_process++;
    if(head!=NULL)
    {
      int b;
      for(int i=0;i<n;i++)
      {
        if(head->ID==process_id[i])
            {
              b=i;
              break;
            }
      }
      waiting_time[priority[b]]++;
      if(mid_process!=total_process)
      f2<<"\nExecution of process-"<<head->ID<<" starts";
   }
}

}
else
  f2<<"\nat time=["<<z<<","<<z+1<<"]\t"<<"system is free";  
}
void add_into_queue(int a,int z)
{
  struct node *temp,*temp1,*temp2,*temp3;
  temp=new node();
  temp->ID=process_id[z];
  temp->time=execution_time[z];
  temp->priority=a;
  temp->next=NULL;
   temp1=head;
   temp2=head;
  if(head==NULL)
    {
      if(count_value[z]!=capacity[z])
      {
        head=temp;
      count_value[z]++;
      f2<<"\nExecution of process-"<<head->ID<<" starts";
      }
    }
  else if(temp1->priority > temp->priority )
  {
    if(count_value[z]!=capacity[z])
    {f2<<"\npreemption of process-"<<head->ID<<" takes place";
    head=temp;
    head->next=temp1;
    count_value[z]++;
    f2<<"\nExecution of process-"<<head->ID<<" starts";  
  }}
  else if(temp1->priority==temp->priority )
  { if(count_value[z]!=capacity[z])
    {
    if(temp->ID>temp1->ID)
    {
      while(temp1!=NULL && temp->priority==temp1->priority && temp->ID>temp1->ID)
      {   temp2=temp1;
          temp1=temp1->next;
      }
      temp2->next=temp;
      temp->next=temp1;
    }
    else 
    { 
      f2<<"\npreemption of process-"<<head->ID<<" takes place";
       temp->next=temp1;
       head=temp;
       f2<<"\nExecution of process-"<<head->ID<<" starts";  
    }
    count_value[z]++;
  }
}
  else if(temp1->priority<temp->priority)
  {
    if(temp->ID==temp1->ID)
    {
      if(temp1->time<temp->time)
      waiting_time[z]=waiting_time[z]+(temp->time-temp1->time);
      f2<<"\nprocess:"<<head->ID<<" deadline passedmm";
      head=head->next;
      missing_process++;
      mid_process++;
      if(count_value[z]<capacity[z] && head!=NULL)
        { 
          count_value[z]++;
          temp2=head;
          if(temp2->priority<temp->priority)
           {
            while(temp2!=NULL && temp2->priority<temp->priority)
           {
            temp3=temp2;
            temp2=temp2->next;
           }
           if(temp2==NULL)
           {
            temp3->next=temp;
          }
           else if(temp->priority==temp2->priority)
           {
               if(temp2->ID>temp->ID)
               {
                temp->next=temp2->next;
                temp3->next=temp;
               }
               else
               {
                temp->next=temp2->next;
                temp2->next=temp;
               }
           }
           else if(temp2->priority>temp->priority)
           {
            temp->next=temp2;
            temp3->next=temp;
           }
             }
           else if(temp2->priority>=temp->priority)
           {
            if(temp2->priority==temp->priority)
            {
               if(temp2->ID>temp->ID)
               {
                temp->next=temp2;
                head=temp;
               }
               else
               {
                temp->next=temp2->next;
                temp2->next=temp;
               }
            }
            else if(temp2->priority>temp->priority)
            {
               temp->next=temp2;
               head=temp;
            }
           }  
        }
        else if(head==NULL)
        {
          head=temp;
          count_value[z]++;
        }
   }
    else
    {
      while(temp1->next!=NULL && temp1->next->priority<=temp->priority && temp1->next->ID!=temp->ID)
        { 
          temp2=temp1;
          temp1=temp1->next;
        }
      if(temp1->next==NULL && temp1->priority<temp->priority)
        {
          if(count_value[z]!=capacity[z])
          {
            temp1->next=temp;
            count_value[z]++;
          }
        }
        else if(temp1->next==NULL && temp1->priority==temp->priority)
        {
          if(temp1->ID>temp->ID)
          {  if(count_value[z]!=capacity[z])
             {temp2->next=temp;
             temp->next=temp1;
             count_value[z]++;
          }}
          else
          {if(count_value[z]!=capacity[z])
            {temp1->next=temp;
            count_value[z]++;
          }}
        }
      else if(temp1->next->ID==temp->ID)
      {
        temp2=temp1;
        temp1=temp1->next;
        temp2->next=temp1->next;
         if(temp1->time<temp->time)
        waiting_time[z]=waiting_time[z]+(temp->time-temp1->time);
        f2<<"\nprocess:"<<temp1->ID<<" deadline passed";
        missing_process++;
        mid_process++;
      if(count_value[z]<capacity[z])
        {
          temp2=head;
          if(temp2->priority<temp->priority)
           {
            while(temp2!=NULL && temp2->priority<temp->priority)
           {
            temp3=temp2;
            temp2=temp2->next;
           }
           if(temp2==NULL)
            temp3->next=temp;
           else if(temp->priority==temp2->priority)
           {
               if(temp2->ID>temp->ID)
               {
                temp->next=temp2->next;
                temp3->next=temp;
               }
               else
               {
                temp->next=temp2->next;
                temp2->next=temp;
               }
           }
           else if(temp2->priority>temp->priority)
           {
            temp->next=temp2;
            temp3->next=temp;
           }
           count_value[z]++;

             }
           else if(temp2->priority>=temp->priority)
           {
            if(temp2->priority==temp->priority)
            {
               if(temp2->ID>temp->ID)
               {
                temp->next=temp2->next;
                temp3->next=temp;
               }
               else
               {
                temp->next=temp2->next;
                temp2->next=temp;
               }
            }
            else if(temp2->priority>temp->priority)
            {
               temp->next=temp2;
               head=temp;
            }
            count_value[z]++;
           }  
          
        }

      }
      else if(temp1->next->priority>=temp->priority && temp1->next->ID!=temp->ID)
      {
        if(count_value[z]!=capacity[z])
        {
        if(temp1->next->priority==temp->priority)
           {
            if(temp1->next->ID>temp->ID)
            {
              temp->next=temp1->next;
              temp1->next=temp;
              
              count_value[z]++;
            }
            else
            {
              temp1=temp1->next;
              temp->next=temp1->next;
              temp1->next=temp;
              count_value[z]++;
            }
           }
           if(temp1->next->priority>temp->priority)
           {
            temp->next=temp1->next;
            temp1->next=temp;
            count_value[z]++;
           }
      }
    } 

    }
  }
}
void check_period(int z)
{
  for(int i=0;i<n;i++)
    {
      if(z%(period[priority[i]])==0)
        { 
          if(count_value[priority[i]]<=(capacity[priority[i]]))
             {
              add_into_queue(z+period[priority[i]],priority[i]);
             }  
        }
    }
}
int main()
{
  ifstream f1;
  ofstream f3;
  f1.open("inp-params.txt");
  f2.open("EDF-logs.txt");
  f3.open("EDF-stats.txt");
    f1>>n;
    int max=0;
    for(int i=0;i<n;i++)
    {
      f1>>process_id[i];
      f1>>execution_time[i];
      f1>>period[i];
      f1>>capacity[i];
      count_value[i]=0;
      waiting_time[i]=0;
      total_process=total_process+capacity[i];
      if(max<capacity[i]*period[i])
        max=capacity[i]*period[i];
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
   while(mid_process!=total_process && z!=max)
   {
     check_period(z);
     execution(z);
     waiting();
     z++;
   }
   float average=0;
   missing_process=missing_process+(total_process-mid_process);
   for(int i=0;i<n;i++)
      average=average+waiting_time[i];

   average=(float)average/total_process;
   f3<<"\nTotal no of process entered:"<<total_process;
   f3<<"\nTotal no of processes that are executing successfully:"<<total_process-missing_process;        
   f3<<"\nTotal no of processes that are missing their deadline:"<<missing_process;
   f3<<"\nAverage Waiting time:"<<average;
   cout<<"Files created and saved successfully";

}