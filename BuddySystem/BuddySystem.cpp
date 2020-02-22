// BuddySystem.cpp : Defines the entry point for the console application.
//
/***************************************************************************
copyright            : (C) Asad Ali
email                : asad_82@yahoo.com
website		         : sites.google.com/site/asad82
***************************************************************************/
// Programmed by Asad Ali

#include "stdafx.h"
#include <conio.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//******************* Structures Declaration *************************//
struct Buddy
{
	int size;
	int address;
	Buddy *parent;
	Buddy *right;
	Buddy *left;
	Buddy *next;
	Buddy *previous;
	bool allocated;
	int freeSpace;
	int level;
	bool deleteThis;
};


//************************* Functions declaration **********************//

void buddySystem();
void allocateMemoryToProcess();
void deallocateMemoryOfProcess();
void printStatistics();
Buddy * splitMemory(Buddy *tree);
void randomSearchAndDelete(Buddy * root);
void recursiveCheck(Buddy * root);
void computeInternalFragmentation(Buddy *root);


//*********************** Variables Declarations ***********************//
Buddy *root,*newnodeLeft,*newnodeRight;
Buddy * freelist[7],*allocateThisNode;
Buddy * rootCopy;

int allocations=0,internalFragmentation;
int processSize=0,level=0;
int value=1,allocationDenied=0;
int luck=0,processValue=0;
bool deleteSuccess=false,nodeFound=false;

//************************* Main Function ******************************//

int main()
{
	
	int number;
	while(number!=0)
	{
		cout<<"\n Press 1 to Simulate Buddy System."
			<<"\n Press 0 to Exit."
			<<"\n\n Enter Your Choice = ";
		cin>>number;

		if(number==1)
			buddySystem();
		else
			break;

	}


	
	return 0;
}// end of main


//********************* Loop 10,000 Times ****************************//

void buddySystem()
{
	// to support multiple runs in one execution
	// clear all pointers
	root=NULL;
	rootCopy=NULL;
	newnodeLeft=NULL;
	newnodeRight=NULL;
	allocations=0;
	allocationDenied=0;
	internalFragmentation=0;
	

	for(int clear=0;clear<7;clear++)
	{
		freelist[clear]=NULL;
	}

	// create the first node in the tree, i.e root with
	// 64K memory which is all free at the moment
	root = new Buddy;
	root->size=64;
	root->address=0;
	root->freeSpace=64;
	root->left=NULL;
	root->right=NULL;
	root->parent=NULL;
	root->allocated=false;
	root->deleteThis=false;
	root->next=NULL;
	root->previous=NULL;
	
	rootCopy=root;
	freelist[6]=root;

	for(int i=0;i<10100;i++)
	{
		if(allocations > 0)
		{
			srand(time(NULL));
			if(rand()%3!=0) // if rem 0 then allocate memory to process
			{
				// pass the value of select Algo to allocate memory
				// function which will then select the appropriate
				// while loop for finding the Hole.

				allocateMemoryToProcess();
			} // end of if
			else	// if rem !=0 then deallocate process memory randomly
			{
				deallocateMemoryOfProcess();
			}// end of else

		}// end of if
		else
		{	// no allocation has so far been done , so first 
			// allocate some memory to a process and then start
			// random allocation and deallocation process		
			allocateMemoryToProcess();
		}
		
	}// end of for loop


	printStatistics();


}// end of function




//******************* Allocate Memory By Splitting Tree **********//

void allocateMemoryToProcess()
{
	srand(time(NULL));
	processSize = 1 + rand() % 17;
	processValue=1;
	level=0;
	while(processValue < processSize)
	{
		processValue = processValue * 2;
		level++;
	}// end of while

	allocateThisNode = NULL;
	// search free node in the list 
	if(freelist[level]!=NULL)
	{	
		if(freelist[level]->size==value)
		{
			allocateThisNode = freelist[level];
			if(freelist[level]->next!=NULL)
			{
				freelist[level]=freelist[level]->next;
				freelist[level]->previous=NULL;
			}
			else
				freelist[level]=NULL;
			
			allocateThisNode->next=NULL;
			allocateThisNode->previous=NULL;
		}
			
	}// end if
	else
	{	nodeFound=false;
		root=rootCopy;
		splitMemory(root);		
		if(allocateThisNode!=NULL)
			allocateThisNode->deleteThis=true;
		else
		{
			allocationDenied++;
			return;
		}
	}

	// we have the node where we'll insert the process
	// but first remove it from the free list
	if(allocateThisNode==NULL)
		return;

	if(allocateThisNode->deleteThis==true)
	{
		level=0;
		value=1;
		while(allocateThisNode->size > value)
		{
			value = value * 2;
			level++;
		}// end of while
		
		while(freelist[level]->deleteThis!=true)
		{
			freelist[level]=freelist[level]->next;
		}// end of while

		if(freelist[level]->previous!=NULL)
		{
			freelist[level]->previous->next=freelist[level]->next;
			if(freelist[level]->next!=NULL)
				freelist[level]->next->previous=freelist[level]->previous;
		}// end of if
		else
		{
			if(freelist[level]->next!=NULL)
			{
				freelist[level]=freelist[level]->next;
				freelist[level]->previous=NULL;
			}
			else
				freelist[level]=NULL;
		}// end of else
		
	}// end of if

	allocateThisNode->allocated=true;
	allocateThisNode->freeSpace=allocateThisNode->size - processSize;
	allocations++;
	cout<<"\n Allocated Size= "<<allocateThisNode->size<<"--- Process Size = "<<processSize;

}// end of process


//******** Split the tree to accomodate process of smaller size **********//

Buddy * splitMemory(Buddy *tree)
{
	if(tree==NULL)
		return NULL;

	if(tree->right==NULL && tree->left==NULL && tree->size > processValue && tree->allocated==false && nodeFound==false)
	{
		newnodeRight = new Buddy;
		newnodeLeft = new Buddy;

		newnodeRight->parent=tree;
		newnodeLeft->parent=tree;
		newnodeRight->size=tree->size/2;
		newnodeLeft->size=tree->size/2;
		newnodeRight->freeSpace=tree->size/2;
		newnodeLeft->freeSpace=tree->size/2;
		newnodeRight->allocated=false;
		newnodeLeft->allocated=false;
		newnodeRight->deleteThis=false;
		newnodeLeft->deleteThis=false;
		newnodeLeft->address=tree->address;
		newnodeRight->address=(tree->address+tree->size/2);
		newnodeRight->left=NULL;
		newnodeRight->right=NULL;
		newnodeLeft->left=NULL;
		newnodeLeft->right=NULL;

		// link with the tree
		tree->left=newnodeLeft;
		tree->right=newnodeRight;
		tree->deleteThis=true;
		// delete the parent from the free list
		value=1;
		level=0;
		while(value < tree->size)
		{
			value = value * 2;
			level++;
		}

		while(freelist[level]->deleteThis!=true)
		{
			freelist[level]=freelist[level]->next;
		}// end of while
		
		// set the delete bit to false again for future use
		freelist[level]->deleteThis=false;
		if(freelist[level]->previous!=NULL)
		{
			freelist[level]->previous->next=freelist[level]->next;
			if(freelist[level]->next!=NULL)
				freelist[level]->next->previous=freelist[level]->previous;
		}// end of if
		else
		{
			if(freelist[level]->next!=NULL)
			{
				freelist[level]=freelist[level]->next;
				freelist[level]->previous=NULL;
			}
			else
				freelist[level]=NULL;
		}
		// end of delete the parent node

		// insert the two nodes in the free list
		value=1;
		level=0;
		while(value < newnodeLeft->size)
		{
			value = value * 2;
			level++;
		}
		
		if(freelist[level]!=NULL)
		{
			while(freelist[level]->next!=NULL)
			{
				freelist[level]=freelist[level]->next;
			}

			freelist[level]->next=newnodeLeft;
			newnodeLeft->previous=freelist[level];
			newnodeLeft->next=newnodeRight;
			newnodeRight->previous=newnodeLeft;
			newnodeRight->next=NULL;
		}// end of if

		else
		{
			freelist[level]=newnodeLeft;
			freelist[level]->previous=NULL;
			freelist[level]->next=newnodeRight;
			newnodeRight->previous=newnodeLeft;
			newnodeRight->next=NULL;
		}// end of else


		if(newnodeLeft->size==processValue)
		{
			nodeFound=true;
			allocateThisNode = tree->left;		
		}
		
	}// end of if

	if(nodeFound==false)
	{
		splitMemory(tree->left);
		splitMemory(tree->right);
	}

}// end of function


//**************** Deallocate Memory and merge buddies *****************// 
void deallocateMemoryOfProcess()
{	
	root = rootCopy;
	deleteSuccess = false;
	if(root->left!=NULL && root->right!=NULL)
	{
		randomSearchAndDelete(root);		
	}// end of if
	
	//if(deleteSuccess==false)
	//	cout<<"\n Delete Failed.";

}// end of function


//************** Randomly identify the process to delete ****************//
void randomSearchAndDelete(Buddy * root)
{
	if(root==NULL)
		return;

	if(root->left==NULL && root->right==NULL)
	{
		srand(time(NULL));
		luck = rand()%10;
		if(luck!=0)
		{
			root->allocated=false;
			root->freeSpace=root->size;
			allocations--;
			cout<<"\n Deallocated.";
			deleteSuccess=true;
			if(root->parent!=NULL)
			{
				if(root->parent->left->allocated==false && root->parent->right->allocated==false && root->parent->right->right==NULL && root->parent->left->left==NULL)
				{ // merge the two nodes.
					
					if(root->parent->left==root)
					{
						// remove the right portion from the 
						// free list
						root->parent->right->deleteThis=true;
						value=1;
						level=0;
						while(value < root->parent->right->size)
						{
							value = value * 2;
							level++;
						}
						
						while(freelist[level]->deleteThis!=true)
						{
							freelist[level]=freelist[level]->next;
						}// end of while

						// set the delete bit to false again for future use
						freelist[level]->deleteThis=false;

						if(freelist[level]->previous!=NULL)
						{
							freelist[level]->previous->next=freelist[level]->next;
							if(freelist[level]->next!=NULL)
								freelist[level]->next->previous=freelist[level]->previous;
						}// end of if
						else
						{
							if(freelist[level]->next!=NULL)
							{
								freelist[level]=freelist[level]->next;
								freelist[level]->previous=NULL;
							}
							else
								freelist[level]=NULL;
						}
						
					}// end of if

					else
					{
						// remove the left portion from the free list
						root->parent->left->deleteThis=true;
						value=1;
						level=0;
						while(value < root->parent->left->size)
						{
							value = value * 2;
							level++;
						}
						
						while(freelist[level]->deleteThis!=true)
						{
							freelist[level]=freelist[level]->next;
						}// end of while

						// set the delete bit to false again for future use
						freelist[level]->deleteThis=false;

						if(freelist[level]->previous!=NULL)
						{
							freelist[level]->previous->next=freelist[level]->next;
							if(freelist[level]->next!=NULL)
								freelist[level]->next->previous=freelist[level]->previous;
						}// end of if
						else
						{
							if(freelist[level]->next!=NULL)
							{
								freelist[level]=freelist[level]->next;
								freelist[level]->previous=NULL;
							}
							else
								freelist[level]=NULL;
						}
						

					}// end of else
					
					root->parent->left=NULL;
					root->parent->right=NULL;
					
					// recursively check till the root if merge is possible
					// of the all the budds or not
					if(root->parent!=NULL)
						recursiveCheck(root->parent);
				}
				else
				{	// compute the level so that the node can be inserted
					// into the free list
					value=1;
					level=0;
					while(value < root->size)
					{
						value = value * 2;
						level++;
					}
					
					if(freelist[level]!=NULL)
					{
						while(freelist[level]->next!=NULL)
						{
							freelist[level]=freelist[level]->next;
						}
						freelist[level]->next=root;
						root->previous=freelist[level];

					}// end if
					else
					{
						freelist[level]=root;
						freelist[level]->previous=NULL;
						freelist[level]->next=NULL;
					}	
					
				}// end of else

			}
		}
	}// end of if
	if(deleteSuccess==false)
	{
		randomSearchAndDelete(root->left);
		randomSearchAndDelete(root->right);
	}


}// end of function


//***** Recursively check till the root if buddies can be merged*********//
void recursiveCheck(Buddy * root)
{
	if( root->parent!=NULL && root->parent->left->left==NULL && root->parent->left->right==NULL && root->parent->right->left==NULL && root->parent->right->right==NULL)
	{
		if(root->parent->left->allocated==false && root->parent->right->allocated==false)
		{ // merge the two nodes.
					
			if(root->parent->left==root)
			{
				// remove the right portion from the 
				// free list
				root->parent->right->deleteThis=true;
				value=1;
				level=0;
				while(value < root->parent->right->size)
				{
					value = value * 2;
					level++;
				}
						
				while(freelist[level]->deleteThis!=true)
				{
					freelist[level]=freelist[level]->next;
				}// end of while

				// set the delete bit to false again for future use
				freelist[level]->deleteThis=false;

				if(freelist[level]->previous!=NULL)
				{
					freelist[level]->previous->next=freelist[level]->next;
					if(freelist[level]->next!=NULL)
						freelist[level]->next->previous=freelist[level]->previous;
				}// end of if
				else
				{
					if(freelist[level]->next!=NULL)
					{
						freelist[level]=freelist[level]->next;
						freelist[level]->previous=NULL;
					}
					else
						freelist[level]=NULL;
				}
				
			}// end of if

			else
			{
				// remove the left portion from the free list
				root->parent->left->deleteThis=true;
				value=1;
				level=0;
				while(value < root->parent->left->size)
				{
					value = value * 2;
					level++;
				}
						
				while(freelist[level]->deleteThis!=true)
				{
					freelist[level]=freelist[level]->next;
				}// end of while

				// set the delete bit to false again for future use
				freelist[level]->deleteThis=false;
							
				if(freelist[level]->previous!=NULL)
				{
					freelist[level]->previous->next=freelist[level]->next;
					if(freelist[level]->next!=NULL)
						freelist[level]->next->previous=freelist[level]->previous;
				}// end of if
				else
				{
					if(freelist[level]->next!=NULL)
					{
						freelist[level]=freelist[level]->next;
						freelist[level]->previous=NULL;
					}
					else
						freelist[level]=NULL;
				}


			}// end of else
					
			root->parent->left=NULL;
			root->parent->right=NULL;
					
			// recursively check till the root if merge is possible
			// of the all the budds or not
			recursiveCheck(root->parent);
		}// end of if on allocated or not
		else
		{
			// insert into the free list
			if(root->parent->left->allocated==true)
			{ // insert the right child in the freelist

					value=1;
					level=0;
					while(value < root->parent->right->size)
					{
						value = value * 2;
						level++;
					}
					
					if(freelist[level]!=NULL)
					{
						while(freelist[level]->next!=NULL)
						{
							freelist[level]=freelist[level]->next;
						}
						freelist[level]->next=root->parent->right;
						root->parent->right->previous=freelist[level];

					}// end if
					else
					{
						freelist[level]=root->parent->right;
						freelist[level]->previous=NULL;
						freelist[level]->next=NULL;
					}	
				

			}// end of if on left
			else 
			{
					value=1;
					level=0;
					while(value < root->parent->left->size)
					{
						value = value * 2;
						level++;
					}
					
					if(freelist[level]!=NULL)
					{
						while(freelist[level]->next!=NULL)
						{
							freelist[level]=freelist[level]->next;
						}
						freelist[level]->next=root->parent->left;
						root->parent->left->previous=freelist[level];

					}// end if
					else
					{
						freelist[level]=root->parent->left;
						freelist[level]->previous=NULL;
						freelist[level]->next=NULL;
					}// end of else	


			}// end of else



		}// end of else on allocated or not


	}// end of outer if on root->left == NULL ....
	else
	{ // we are at the root node so insert the 64k chunk
	  // in the free list.

		if(root->parent==NULL)
		{
			value=1;
			level=0;
			while(value < root->size)
			{
				value = value * 2;
				level++;
			}
					
			if(freelist[level]!=NULL)
			{
				while(freelist[level]->next!=NULL)
				{
					freelist[level]=freelist[level]->next;
				}
				freelist[level]->next=root;
				root->previous=freelist[level];

			}// end if
			else
			{
				// it is the first node
				freelist[level]=root;
				freelist[level]->previous=NULL;
				freelist[level]->next=NULL;
			}// end of else	
			

		}// end of if


	}// end of else
	
	

}// end of function


//************ Print the collected Statistics ************************//
void printStatistics()
{
	root = rootCopy;
	computeInternalFragmentation(root);
	cout<<"\n Internal Fragmentation (KB)= "<<internalFragmentation <<" -----% "<<((float)internalFragmentation/64 );
	cout<<"\n # of Allocation Requests Denied = "<<allocationDenied;
	cout<<"\n External Fragmentation = 0";

}

//************* Calculate Internal Fragmentation ********************//
void computeInternalFragmentation(Buddy * root)
{
	if(root!=NULL && root->freeSpace < root->size)
		internalFragmentation += root->freeSpace;
	else if(root==NULL)
		return;

	computeInternalFragmentation(root->left);
	computeInternalFragmentation(root->right);

	
}// end of function
