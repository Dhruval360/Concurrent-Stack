# Concurrent-Stack
This is a concurrent implementation of the data structure STACK using pthreads in C++.

# Objective:
* To implement a parallel stack wherein multiple threads will be performing different actions on the same stack.
* To prevent race conditions, deadlocks and other concurrency bugs by using some kind of synchronization mechanism.

# Operations defined for this implementation of stack:
* Insert at the top (Push).
* Delete at the top (Pop).
* Search (Peek).
* Print (Display the stack from top to bottom).

# Implementation:
I have implemented a class called **STACK** with all the operations mentioned above.
The stack itself is a vector which is a private member of the class. The class also has another private member named top, which is an integer that holds the index of the topmost element.
I have made use of mutexes for synchronization and hence only one thread can access the stack at a time (to prevent the bugs mentioned in the objective).

# Files:
* stack_using_vectors.cpp : It is the main file which has the implementation.
* Thread_logs.txt : This file will get updated with the logs of creation and joining of threads when the main program is run. (Disabled by default. Please uncomment the required lines to enable logging)
* input.txt : This file contains a sample input to the program which can be redirected as command line arguments as discussed below.
* t1.txt : This is an input file containing the edge case when all the threads are trying to **delete (pop)** from an **empty stack**.
* t2.txt : This is an input file containing the edge case when all the threads are trying to **search (seek)** an **empty stack**.
* t3.txt : This is an input file containing the edge case when all the threads are trying to **print** an **empty stack**.
* t4.txt : This is an input file containing the case when all the threads are trying to **insert (push)** to a stack.

# Compilation and running:
Type **g++ concurrent_stack.cpp -pthread** in the terminal to compile the program to give an executable named **a.out**.

This program takes command line arguments which is of the following format:
* **i** followed by the insertion value (without space) for insert (push).
* **d** for delete (pop)
* **s** for search (peek)
* **p** for print (display the stack from top to bottom)

Sample input for LINUX: **./a.out i4 i6 i7 s d d d s p i89 d s p**

Which means **Insert 4, Insert 6, Insert 7, Search, Delete, Delete, Delete, Search, Print, Insert 89, Delete, Search, Print** to the program.

The same input can be given from a text file like input.txt by using **./a.out $(cat input.txt)**

**Note:** The input values for pushing (insertion) to the stack are converted to integers using atoi. Hence any invalid inputs will default to integer 0.
