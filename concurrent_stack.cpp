#include<iostream>
#include<stdio.h>
#include<vector>
#include<pthread.h>
#include<fstream>
#define Max_Threads 15 // Change this to change the number of threads being used (keep it a multiple of 3)

pthread_mutex_t stackmutex; // For atomic pushing and poping
 
using namespace std;
 
class STACK{
	private:
		vector <int>num; // This holds the stack
		int top; // Top of the stack
    
	public:
		STACK(){    
			top = -1;
		}
		
		int insert(int n){ // Pushing elements onto the stack
			top++;
			num.push_back(n);
			return n;
		}
		
		int isEmpty(){ // For preventing poping empty stacks
			if(top == -1)
				return 1;
			else
				return 0;   
		}
		
		int del(){ // Poping elements from the top of a stack
	 		if(isEmpty()) //Checking if the stack is empty
				return -1;
			else{
				int temp = num[top]; // For returning the popped element
				num.pop_back();
				top -= 1;
				return temp;     
			}
		}
		
		void print(){ // For displaying the elements of a stack from top to bottom
			if (top != -1){
				cout << "Stack is: ";
				for(int i = (top); i >= 0; i--)
					cout << num[i] << " ";
				cout << '\n';
			}
			
			else 
				cout << "Stack is empty\n";
		}
		
		int search(){ // For displaying the element at the top of the stack (peek)
			if (top != -1)
				return  num[top];
			else 
				return -1;
		}
};

typedef struct thread_data{ // For the thread arguments
	int id; // Thread id given to the thread during its creation
	STACK *stack; // A pointer to the stack object
}thread_data;


// For the threads 
void *insert(void *thread_arg){
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock (&stackmutex);
	(*(mydata->stack)).insert(mydata->id);
	//cout << "Thread " << mydata->id << " Pushed " << mydata->id << " onto the stack\n"; 
	printf("Thread %d inseted (pushed) %d onto the stack\n", mydata->id, mydata->id);
	pthread_mutex_unlock (&stackmutex);
	
	pthread_exit(NULL);
}

void *search(void *thread_arg){
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock (&stackmutex);
	int topmost_element = (*(mydata->stack)).search();
	//cout << "Thread " << mydata->id << " Pushed " << mydata->id << " onto the stack\n"; 
	if (topmost_element != -1)
		printf("Thread %d found that the topmost element in the stack is %d\n", mydata->id, topmost_element);
	else
		printf("Thread %d found the stack to be empty\n", mydata->id);
	pthread_mutex_unlock (&stackmutex);
	
	pthread_exit(NULL);
}

void *del(void *thread_arg){ 
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock (&stackmutex);
	int deleted_data = (*(mydata->stack)).del(); // Poped data
	
	if(deleted_data != -1) 
		//cout << "Thread " << mydata->id << " Popped " << popped_data << " from the stack\n";
		printf("Thread %d deleted (poped) %d from the stack\n", mydata->id, deleted_data);
	else
		//cout << "Thread " << mydata->id << " cannot del from empty stack\n";
		printf("Thread %d cannot delete (pop) from empty stack\n", mydata->id);
	
	pthread_mutex_unlock (&stackmutex);
	pthread_exit(NULL);
}
 
// Learnt that cout sends data as streams and mixing of data may occur while printing, hence I have used printf instead of cout
 
int main(){
	if(Max_Threads % 3){
		printf("The number of threads is not a mltiple of 3.\nPlease give it a value that is a multiple of 3\n");
		exit(-1);
	}	
	
	ofstream output; 
/* This file maintains the thread creation record. It will hold any errors that occur while the threads are created as well.
 I have put those into a separate file instead of printing it onto the terminal as I did'nt want it to interfere with the thread outputs */
	output.open("Thread_creations.txt");

	STACK stack; // Creates the stack object
	void *status; // For the exit status of threads
	pthread_mutex_init(&stackmutex, NULL); // Initializing the mutex
	pthread_t threads[Max_Threads]; // Creates an array of threads 
	thread_data data[Max_Threads]; // Holds arguments for the threads 
	
	// Creating the threads
	pthread_attr_t attr; 
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	// Needed to make the threads created to be joinable as implementations don't do it by default)
	
	int err; // Holds return value of the pthread_create function	
	for(unsigned int i = 0; i < Max_Threads - 1; i += 3){
		// Setting the arguments
		data[i].id = i; 
		data[i+1].id = i + 1;
		data[i+2].id = i + 2; 
		
		data[i].stack = &stack;
		data[i+1].stack = &stack;
		data[i+2].stack = &stack;
		
		output << "In main: creating thread " << i << '\n';
		//printf("In main: creating thread %d\n", i);
		err = pthread_create(&threads[i], &attr, insert, (void *) &data[i]);
		if (err){ // Creation of thread failed
			cout << "ERROR!! Return code from pthread_create() is " << err << '\n';
			exit(-1);
		}
		
		output << "In main: creating thread " << i+1 << '\n';
		//printf("In main: creating thread %d\n", i+1);
		err = pthread_create(&threads[i+1], &attr, del, (void *) &data[i+1]);
		if (err){ // Creation of thread failed
			cout << "ERROR!! Return code from pthread_create() is " << err << '\n';
			exit(-1);
		}
		
		output << "In main: creating thread " << i+2 << '\n';
		//printf("In main: creating thread %d\n", i+2);
		err = pthread_create(&threads[i+2], &attr, search, (void *) &data[i+2]);
		if (err){ // Creation of thread failed
			cout << "ERROR!! Return code from pthread_create() is " << err << '\n';
			exit(-1);
		}
	}
	
	pthread_attr_destroy(&attr);

	/* Wait on the other threads */	
	for(unsigned int i = 0; i < Max_Threads; i++){
		err = pthread_join(threads[i], &status);
		if (err){
			printf("ERROR; return code from pthread_join() is %u\n", err);
			exit(-1);
		}
		output << "Main: completed join with thread " << i << " having a status of " << status << '\n';
		//printf("Main: completed join with thread %u having a status of %ld\n",i,(long)status);
		// This shows the joining of the main thread with the created threads	
	}
	
	stack.print();
	
	pthread_mutex_destroy(&stackmutex);
	pthread_exit(NULL);	     
	return 0;     
}
