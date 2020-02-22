Buddy System
-------------	

a. Implementation Details
--------------------------

I.	Tree data structure was used for the implementation of buddy system. 

II.	First of all create a single tree node of size 64 K, and then enter into a loop which will run 10,100 times.

III.	Start random allocation and de-allocation in this loop by calling respective functions based on the remainder of rand function.

IV.	When ever the allocation function is called a process of random size is generated and the nearest value in power of 2 is calculated which helps in the splitting of the tree if required.

V.	Based on the computed value a block of required size is first searched in the free list of that power of 2 only.

VI.	If the block is found then the process is inserted into that block, free space left after the allocation of process in that block is computed for later use.

VII.	If the block of required size is not found in the free list then a block of larger size is split into two, and the process of splitting continues until a match with the nearest power of 2 occurs.

VIII.	When the de-allocation function is called a block which has been allocated to a process is selected randomly while parsing the tree.

IX.	The space allocated to the process is freed and a recursive function is called which checks if merging of buddies is possible or not.

X.	The recursive function moves from the leaves to the root and tries to merge two buddies if they are free. Free list is also kept updated for each level of the power of 2

b. Sample Output  
----------------

The context below shows output from several runs of the program.

Sample Run 1
------------
Internal Fragmentation (KB)= 15 ----- % 0.234375

# of Allocation Requests Denied = 9087

External Fragmentation = 0

Sample Run 2
-------------
Internal Fragmentation (KB)= 8 -----% 0.125

# of Allocation Requests Denied = 5971

External Fragmentation = 0

Sample Run 3
-------------
Internal Fragmentation (KB)= 2 -----% 0.03125

# of Allocation Requests Denied = 0

External Fragmentation = 0

Sample Run 4
------------
Internal Fragmentation (KB)= 16 -----% 0.25

# of Allocation Requests Denied = 6422

External Fragmentation = 0

Sample Run 5
------------
Internal Fragmentation (KB)= 28 -----% 0.4375

# of Allocation Requests Denied = 4990

External Fragmentation = 0


c. Observations
---------------

I.	The amount of space wasted as a result of internal fragmentation is the function of process size. 

II.	If the buddy system is feeded with process of larger size then the space wasted as a result of internal fragmentation is greater because larger blocks are allocated to those processes.

III.	Also the number of allocation requests denied is greater, but usually it is dependent on the way allocations and de-allocations over lap each other

IV.	External fragmentation in buddy system will always be zero.

V.	When ever a process is deleted there is an extra over head of checking buddies till the root node for merging which makes this technique slower.
