// Concurrent implementation of stack using pthreads

// Sample input: i4 i6 i7 d d p d p s s i1
/* Here: 
	i means insert (push)
	d means delete (pop)
	s means search (peek)
	p means print (display the stack from top to bottom)
*/ 

//#include<iostream>
// Learnt that cout sends data as streams and mixing of data may occur while printing, hence I have used printf instead of cout
#include<stdio.h>
#include<vector>
#include<pthread.h>
#include<stdlib.h>
//#include<fstream> // This is for the thread creation and joining logs

#define Max_Threads argc-1 // The number of arguments is the number of threads	

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
		
		void insert(int n){ // Pushing elements onto the stack
			top++;
			num.push_back(n);
		}
		
		/*
		int isEmpty(){ // For preventing poping empty stacks
			if(top == -1)
				return 1;
			else
				return 0;   
		}*/
		
		int del(){ // Poping elements from the top of a stack
	 		//if(isEmpty()) //Checking if the stack is empty
	 		if(top == -1) //Checking if the stack is empty
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
				//cout << "Stack is: ";
				printf("Stack is: ");
				for(int i = (top); i >= 0; i--)
					//cout << num[i] << " ";
					printf("%d ", num[i]);
				//cout << '\n';
				printf("\n");
			}
			
			else 
				//cout << "Stack is empty\n";
				printf("Stack is empty\n");
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
	int value; // If the thread has to push some value to the stack
	STACK *stack; // A pointer to the stack object
}thread_data;


// For the threads 
void *insert(void *thread_arg){
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock(&stackmutex); // Locking the mutex (to prevent other threads from accessing the stack)
	
	(*(mydata->stack)).insert(mydata->value);
	printf("Thread %d inserted (pushed) %d onto the stack\n", mydata->id, mydata->value);

	pthread_mutex_unlock(&stackmutex); // Unlocking the mutex (Now another thread can access the stack)
	pthread_exit(NULL);
}

void *search(void *thread_arg){
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock(&stackmutex);
	
	int topmost_element = (*(mydata->stack)).search();
	
	if (topmost_element != -1)
		printf("Thread %d found that the topmost element in the stack is %d\n", mydata->id, topmost_element);
	else
		printf("Thread %d found the stack to be empty\n", mydata->id);
	
	pthread_mutex_unlock(&stackmutex);	
	pthread_exit(NULL);
}

void *print(void *thread_arg){
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock(&stackmutex);
	
	printf("Thread %d is trying to print the stack: ", mydata->id);
	(*(mydata->stack)).print();
	
	pthread_mutex_unlock(&stackmutex);	
	pthread_exit(NULL);
}

void *del(void *thread_arg){ 
	thread_data *mydata = (thread_data*)thread_arg;
	
	pthread_mutex_lock (&stackmutex);
	int deleted_data = (*(mydata->stack)).del(); // Popped data
	
	if(deleted_data != -1) 
		printf("Thread %d deleted (popped) %d from the stack\n", mydata->id, deleted_data);
	else
		printf("Thread %d cannot delete (pop) from empty stack\n", mydata->id);
	
	pthread_mutex_unlock (&stackmutex);
	pthread_exit(NULL);
}
 
int main(int argc, char **argv){			
	printf("The number threads is: %d\n\n", Max_Threads);
	
	STACK stack; // Creates the stack object
	void *status; // For the exit status of threads
	pthread_mutex_init(&stackmutex, NULL); // Initializing the mutex
	pthread_t threads[Max_Threads]; // Creates an array of threads 
	thread_data data[Max_Threads]; // Holds arguments for each thread as per user's command line input 
	
	char first; // To check the first character of each argument
	int value; // To hold the value in case of a push argument
	
	printf("The given input is:\n");
	for (unsigned int i = 1; i < argc; i++){
		data[i-1].id = i;
		data[i-1].stack = &stack; // Setting the stack pointer for the thread_data structure
		
		printf("Thread %u: ", i);
		
		first = argv[i][0];
		
		if(first == 'i'){ // Insertion
			value = atoi(argv[i]+1);
			printf(" Insert (Push) %d\n", value);
			data[i-1].value = value;
		}
		
		else if(argv[i][1] != '\0'){ // As other operations need to have a single letter
			printf(" Invalid operation %s\n\nProgram terminated\n", argv[i]);
			exit(-1);
		}
		
		else{
			switch(first){			
				case 'd':{ // Deletion
					printf(" Delete (pop)\n");
					break;
				}
				
				case 's':{ // Search
					printf(" Search (peek)\n");
					break;
				}
				
				case 'p':{ // Print
					printf(" Print (Display the stack from top to bottom)\n");
					break;
				}		
				
				default:{
					printf(" Invalid operation %s\n\nProgram terminated\n", argv[i]);
					exit(-1);
				}	 
			}
		}	
	}	
	printf("\n\n");
	
	//ofstream output; 
/* This file maintains the thread creation record. It will hold any errors that occur while the threads are created as well.
 I have put those into a separate file instead of printing it onto the terminal as I didn't want it to interfere with the thread outputs */
	//output.open("Thread_logs.txt");
	
	// Creating the threads
	pthread_attr_t attr; 
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	// Needed to make the threads created to be joinable as some implementations of pthreads don't do it by default)
	
	int err; // Holds return value of the pthread_create function	
	for(unsigned int i = 0; i < Max_Threads; i++){			
		//output << "In main: creating thread " << i << '\n'; // For updating the log file		
		//printf("In main: creating thread %d\n", i+1); // For obtaining the thread creation log on the standard output
		
		switch(argv[i+1][0]){
			case 'i':{
				err = pthread_create(&threads[i], &attr, insert, (void *) &data[i]);
				break;
			}
			
			case 'd':{
				err = pthread_create(&threads[i], &attr, del, (void *) &data[i]);
				break;
			}
			
			case 's':{
				err = pthread_create(&threads[i], &attr, search, (void *) &data[i]);
				break;
			}
			
			case 'p':{
				err = pthread_create(&threads[i], &attr, print, (void *) &data[i]);
				break;
			}		
			
			default:{
				printf("Invalid operation %s\nProgram will terminate\n", argv[i]);
				exit(-1);
			}
		}
		
		if (err){ // Creation of thread failed
			//cout << "ERROR!! Return code from pthread_create() is " << err << '\n';
			printf("ERROR!! Return code from pthread_create() is %d\n\nProgram terminated\n", err);
			exit(-1);
		}
		
	}
	
	pthread_attr_destroy(&attr);

	/* Wait on the other threads */	
	for(unsigned int i = 0; i < Max_Threads; i++){
		err = pthread_join(threads[i], &status);
		if (err){
			printf("ERROR!! Return code from pthread_join() is %d\n\nProgram terminated\n", err);
			exit(-1);
		}
		//output << "Main: completed join with thread " << i << " having a status of " << status << '\n';
		
		//printf("Main: completed join with thread %u having a status of %ld\n",i,(long)status);
		// These show the joining of the main thread with the created threads	
	}
	
	printf("\nAfter all the threads have completed their tasks, the ");
	stack.print();
	
	pthread_mutex_destroy(&stackmutex);
	pthread_exit(NULL);	     
	return 0;     
}
