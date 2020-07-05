# Concurrent-Stack
This is a concurrent implementation of the data structure STACK using pthreads in c++.

# Objective:
* To implement a parallel stack wherein multiple threads will be performing different actions on the same stack.
* To prevent race conditions, deadlocks and other concurrency bugs by using some kind of synchronization mechanism.

# Operations defined for this implementation of stack:
* Insert at the top (Push).
* Delete at the top (Pop).
* Search (Peek).
* Print (Display the stack from top to bottom).

# Implementation:
I have implemented a class called STACK with all the operations mentioned above.
The stack itself is a vector which is a private member of the class. The class also has another private member named top, which is an integer that holds the index of the topmost element.
I have made use of mutexes for synchronization and hence only one thread can access the stack at a time (to prevent the bugs mentioned in the objective).

# Files:
* stack_using_vectors.cpp : It is the main file which has the implementation.
* Thread.txt : This file will get updated with the logs of creation and joining of threads when the main program is run.

# Compilation and running:
Type **g++ concurrent_stack.cpp -pthread** in the terminal to compile the program to give an executable named **a.out**.

This program takes command line arguments which is of the following format:
* i followed by the insertion value (without space) for insert (push).
* d means delete (pop)
* s means search (peek)
* p means print (display the stack from top to bottom)

Sample input for LINUX: **./a.out i4 i6 i7 s d d d s p i89 d s p**

Which means **Insert 4, Insert 6, Insert 7, Search, Delete, Delete, Delete, Search, Print, Insert 89, Delete, Search, Print** to the program.
