// MemSimulator.cpp : Defines the entry point for the console application.
//
/***************************************************************************
copyright            : (C) Asad Ali
email                : asad_82@yahoo.com
website		     : sites.google.com/site/asad82
***************************************************************************/
// Programmed by Asad Ali

#include "stdafx.h"
#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//*************************** Structures ********************************//
// For linklist of holes
struct Hole
{	int size;
	int startAddress;
	Hole *next;
	Hole *previous;
	bool mark;
};

// structure which will help maintain a linklist of processes
// that has been allocated memory
struct Process
{	
	int size;
	int startAddress;
	Process *next;
	Process *previous;
};

//************************ Functions Declaration **********************//
void SelectAlgorithm(int selectAlgorithm);
void makePartitionsInMemory();
void allocateMemoryToProcess(int selectAlgorithm);
void deallocateMemoryToProcess();
void mergeHoles();
void printStatistics();
Hole * FirstFit(Hole *hole);
Hole * NextFit(Hole *hole);
Hole * BestFit(Hole *hole);
Hole * WorstFit(Hole *hole);

//************************ Declarations *********************************//

	Hole *newnodeHole,*firstHole,*lastHole,*tempHole,*wHole;
	Hole *remLastPosition, *bestNode, *worstNode;
	Process *newnodeProcess,*firstProcess,*lastProcess,*tempProcess;
	int allocations=0,allocIndex=0,alterBtHoleProcess=0;
	int firstTime=0,firstTimeProcess=0;
	int deleteThis=0,processCount=0;
	int memPartitionSize=0, spaceLeft=0;
	int processSize=0,holesCount=0;
	int memoryAllocationFailed=0,initAllocations=0;
	int maxHolesCount=0,maxHolesSizeCount=0,holesSizeCount=0;
	bool noHole=false;

//********************* End of Declarations ****************************//

//******************** Start of Main Function **************************//
int main()
{
	// declarations
	int input;
	do
	{	
		cout<<"\n Press 1 to Simulate First Fit."
			<<"\n Press 2 to Simulate Next Fit."
			<<"\n Press 3 to Simulate Best Fit."
			<<"\n Press 4 to Simulate Worst Fit."
			<<"\n Press 0 to Exit"
			<<"\n\n Enter Your Choice = ";
		
		cin>>input;
		// First Fit
		if(input==1)
		{
			cout<<"\n************* First Fit Stats **********************";
			SelectAlgorithm(1);
		}
		// Next Fit
		else if(input==2)
		{
			cout<<"\n************* Next Fit Stats **********************";
			SelectAlgorithm(2);
		}
		// Best Fit
		else if(input==3)
		{
			cout<<"\n************* Best Fit Stats **********************";
			SelectAlgorithm(3);
		}
		// Worst Fit
		else if(input==4)
		{
			cout<<"\n************* Worst Fit Stats **********************";
			SelectAlgorithm(4);
		}

			cout<<"\n****************************************************";

	}
	while(input!=0);
	
	
	return 0;
}


//******************* Create Holes In Memory ***************************//
void makePartitionsInMemory()
{
	//STEP 1
	// first of all divide the 64KB of memory in partitions
	// of random size between 1 and 16 KB
	firstTime=0;
	allocIndex=0;
	firstHole = NULL;
	lastHole = NULL;
	tempHole=NULL;
	wHole=NULL;
	processCount=0;
	memoryAllocationFailed=0;
	allocations=0;
	alterBtHoleProcess=0;
	initAllocations=0;
	noHole = false;

	while(allocIndex!=63)
	{
		
		
		if(alterBtHoleProcess==0)
		{
				srand(time(NULL));
				memPartitionSize = (rand()%16)+1;
			LastPartition:		
				if((allocIndex+memPartitionSize) <=63)
				{
					newnodeHole = new Hole;
					newnodeHole->size=memPartitionSize;
					newnodeHole->startAddress = allocIndex;
					newnodeHole->next = NULL;
					newnodeHole->previous=NULL;
					holesCount++;
					if(firstTime==0)
					{
						firstHole = newnodeHole;
						lastHole = newnodeHole;
						firstHole->previous=NULL;
						firstTime =1;
					}
					else
					{
						lastHole->next = newnodeHole;
						newnodeHole->previous=lastHole;
						lastHole = newnodeHole;
					}

					allocIndex+=memPartitionSize;
					alterBtHoleProcess=1;

				}
				else // for last partition
				{	// this code will execute only once
					memPartitionSize=63-allocIndex;
					goto LastPartition;
				}
		}// end of if on alter
		else
		{	
			srand(time(NULL));
			processSize = 1+rand()%5;
		processLast:
			if((allocIndex+processSize) <=63)			
			{	
				initAllocations++;
				newnodeProcess = new Process;
				newnodeProcess->size = processSize;
				newnodeProcess->startAddress=allocIndex;
				newnodeProcess->next=NULL;
				newnodeProcess->previous=NULL;
				if(firstTimeProcess ==0 )
				{
					//cout<<"\n First Allocation";
					firstProcess = newnodeProcess;
					lastProcess = newnodeProcess;
					firstProcess->previous=NULL;
					firstTimeProcess=1;
				}
				else
				{
					newnodeProcess->previous=lastProcess;
					lastProcess->next = newnodeProcess;
					lastProcess = newnodeProcess;
				}
				allocIndex+=processSize;
				allocations++;
				alterBtHoleProcess=0;
			}
			else
			{		// at the end of memory
					processSize=63-allocIndex;
					goto processLast;
			}
		}// end of else on alter


	}// end of while

cout<<"\n STEP 1: Memory Partioned.";
}// end of functions



//********* For Loop of 10,000 + allocations/deallocations***************//
void SelectAlgorithm(int selectAlgorithm)
{

	firstProcess=NULL;
	lastProcess=NULL;
	firstTimeProcess=0;
	tempProcess=NULL;

	// STEP 1
	makePartitionsInMemory();
	cout<<"\n STEP 2: Random Allocations / Deallocations In Progress.";
	remLastPosition = firstHole;
	// STEP 2
	// more than 10000 allocation and deallocation requests are
	// generated in this loop randomly
	for(int i=0;i<10100;i++)
	{	
		if(allocations > 0)
		{
			srand(time(NULL));
			if(rand()%2!=0 && noHole==false) // if rem ! 0 then allocate memory to process
			{
				// pass the value of select Algo to allocate memory
				// function which will then select the appropriate
				// while loop for finding the Hole.
				allocateMemoryToProcess(selectAlgorithm);
			} // end of if
			else	// if rem ==0 then deallocate process memory randomly
			{
				deallocateMemoryToProcess();
			}// end of else

		}// end of if
		else
		{	// no allocation has so far been done , so first 
			// allocate some memory to a process and then start
			// random allocation and deallocation process		
			allocateMemoryToProcess(selectAlgorithm);
		}

		// count the number of hole in the list each time
		holesCount=0;
		holesSizeCount=0;
		tempHole = firstHole;
		while(tempHole!=NULL)
		{
			tempHole = tempHole->next;
			holesCount++;

		}// end of while

		if(maxHolesCount < holesCount)
			maxHolesCount = holesCount;
		
	}// end of for loop


	// display the algorithm statistics to the user.
	printStatistics();		


}// end of function


//*************************** First Fit *********************************//
Hole * FirstFit(Hole * tempHole)
{
	while(tempHole!=NULL)
	{
		if(tempHole->size >= processSize)
			break;
		tempHole = tempHole->next;
	}// end of while


	return tempHole;
}

//*************************** Next Fit *********************************//
Hole * NextFit(Hole * tempHole)
{
	int cnt = 0, checkEnd=0;
	int holeFound=0;
	// count the number of holes
	while(tempHole!=NULL)
	{
		cnt++;
		tempHole = tempHole->next;
	}

	// start searching hole from the last position 
	// where we found a hole.
	if(remLastPosition!=NULL)
		tempHole = remLastPosition;
	else
		tempHole = firstHole;

	while(tempHole!=NULL)
	{
		if(tempHole->size >=processSize)
		{	holeFound=1;
			break;
		}
		tempHole = tempHole->next;
		checkEnd++;
	}// end of while
	if(checkEnd!=cnt && holeFound==0)
	{ // holes count is not equal
		tempHole = firstHole;
		while(tempHole!=remLastPosition)
		{
			if(tempHole->size >= processSize)
				break;
			tempHole = tempHole->next;
		}
						
	}

	if(tempHole!=NULL && tempHole->next!=NULL)
		remLastPosition = tempHole->next;
	else
		remLastPosition = firstHole;

return tempHole;
}

//*************************** Best Fit *********************************//
Hole * BestFit(Hole * tempHole)
{

	int minDifference = 17;
	int difference =0; 
	bestNode=NULL;
	tempHole = firstHole;
	while(tempHole!=NULL)
	{
		difference = (tempHole->size - processSize);
		if(difference >=0 && difference < minDifference)
		{
			minDifference = difference;//tempHole->size;
			bestNode = tempHole;
		}
		tempHole = tempHole->next;
	}
	
	tempHole = bestNode;

return tempHole;
}

//*************************** Worst Fit *********************************//
Hole * WorstFit(Hole * tempHole)
{
	int minDifference = 0;
	int difference =0; 
	worstNode=NULL;
	tempHole = firstHole;
	while(tempHole!=NULL)
	{
		difference = (tempHole->size - processSize);
		if(difference >=0 && difference > minDifference)
		{
			minDifference = difference;
			worstNode = tempHole;
		}
		tempHole = tempHole->next;
	}
	tempHole = worstNode;

return tempHole;
}


//******************** Allocate Memory To Process ************************//
void allocateMemoryToProcess(int selectAlgorithm)
{
			int hCount=0;
			
			if(firstHole==NULL)
			{	
				//cout<<"\n No Hole Available";
				memoryAllocationFailed++;
				noHole=true;
				return;
			}
			tempHole = firstHole;
			while(tempHole!=NULL)
			{
				tempHole = tempHole->next;
				hCount++;
			}// end of while
				
			if(hCount > 0 )
			{
				srand(time(NULL));
				processSize = (rand()%14)+1;
				tempHole = firstHole;
				//*************** FIRST FIT *************************//
				if(selectAlgorithm == 1)
				{
					tempHole = FirstFit(tempHole);
				}// end of if

				//*************** NEXT FIT *************************//
				else if(selectAlgorithm == 2)
				{
					tempHole = NextFit(tempHole);	
				}

				//*************** BEST FIT *************************//
				else if(selectAlgorithm == 3)
				{
					tempHole = BestFit(tempHole);
				}

				//*************** WORST FIT *************************//
				else if(selectAlgorithm == 4)
				{
					tempHole = WorstFit(tempHole);
				}

				if(tempHole!=NULL)
				{
					spaceLeft = tempHole->size - processSize;

					newnodeProcess = new Process;
					newnodeProcess->size = processSize;
					newnodeProcess->startAddress=tempHole->startAddress;
					newnodeProcess->next=NULL;
					newnodeProcess->previous=NULL;
					if(firstTimeProcess ==0 )
					{
						//cout<<"\n First Allocation";
						firstProcess = newnodeProcess;
						lastProcess = newnodeProcess;
						firstProcess->previous=NULL;
						firstTimeProcess=1;
					}
					else
					{
						newnodeProcess->previous=lastProcess;
						lastProcess->next = newnodeProcess;
						lastProcess = newnodeProcess;
					}

					if(spaceLeft > 0)
					{
						tempHole->startAddress+=processSize;
						tempHole->size-=processSize;
					}// end of inner if
					else if(spaceLeft==0)
					{ // remove the hole completely
						if(tempHole->previous!=NULL)
						{
							tempHole->previous->next=tempHole->next;
							tempHole=NULL;
						}
						else
						{
							if(tempHole->next!=NULL)
							{
								firstHole = tempHole->next;
								firstHole->previous=NULL;
							}
							else
							{
								// no more hole left
								firstHole = NULL;
								firstTime = 0;
							}
						}
					}
					allocations++;
					cout<<"\n Allocations = "<<allocations;

				}// end of if after while
				else
				{
					// memory allocation request failed
					// because of non-availability of memory
					// count them here.
					memoryAllocationFailed++;
					//cout<<"\n Allocation Failed ="<<memoryAllocationFailed;
				}

			}// end of if on hCount
}// end of function




//********************** Deallocate Process Memory **********************//
void deallocateMemoryToProcess()
{

				int added=0;
				processCount=0;
				if(firstProcess==NULL)
					return;

				tempProcess=firstProcess;
				while(tempProcess!=NULL)
				{
					tempProcess=tempProcess->next;
					processCount++;
				}// end of while

				srand(time(NULL));
				deleteThis=(rand()%processCount)+1;

				tempProcess = firstProcess;
				processCount=0;
				while(tempProcess!=NULL)
				{
					processCount++;
					if(processCount==deleteThis)
						break;				
					tempProcess = tempProcess->next;
					
				}
				
				// returning memory to the holes list
				if(firstHole!=NULL)
				{
					wHole = firstHole;
					while(wHole!=NULL)
					{
						if(wHole->startAddress>tempProcess->startAddress)
						{
							if(wHole->previous!=NULL)
							{
								if((wHole->previous->startAddress + wHole->previous->size) ==tempProcess->startAddress)
								{	// merge the two blocks
									wHole->previous->size+=tempProcess->size;
									break;
								}// end of if
								else if(wHole->startAddress == (tempProcess->startAddress + tempProcess->size))
								{
									// merge with forward blocks
									wHole->startAddress=tempProcess->startAddress;
									wHole->size+=tempProcess->size;
									break;
								}
								else
								{	// add a new hole to the list
									newnodeHole = new Hole;							
									newnodeHole->next=wHole;//->previous->next;
									newnodeHole->previous=wHole->previous;
									newnodeHole->startAddress=tempProcess->startAddress;
									newnodeHole->size=tempProcess->size;
									newnodeHole->mark=true;
									wHole->previous->next = newnodeHole;
									wHole->previous = newnodeHole;
									added=1;
									break;
								}// end of else
							}
							else
							{	// it is the first node
								if( (wHole->startAddress + wHole->size) == tempProcess->startAddress)
								{
									wHole->size+=tempProcess->size;
									wHole->startAddress=tempProcess->startAddress;
									break;
								}// end of if
								else
								{
									//cout<<"\n***************************************";
									newnodeHole = new Hole;
									newnodeHole->mark=true;
									newnodeHole->next= wHole;
									newnodeHole->previous=NULL;
									newnodeHole->startAddress=tempProcess->startAddress;
									newnodeHole->size=tempProcess->size;
									wHole->previous=newnodeHole;
									firstHole=newnodeHole;
									added=1;
									break;
								}// end of else

							}// end of else on previous ! = NULL

						}
					
						wHole = wHole->next;
					}// end of while loop on wHole
				}// end of if
				else
				{
					firstHole = new Hole;
					firstHole->next=NULL;
					firstHole->previous=NULL;
					firstHole->size=tempProcess->size;
					firstHole->startAddress=tempProcess->startAddress;
					firstHole->mark=true;
					lastHole=firstHole;

				}
				// process deleted
				if(tempProcess->previous!=NULL)
					tempProcess->previous->next=tempProcess->next;
				else
				{
					if(tempProcess->next!=NULL)
					{
						firstProcess = tempProcess->next;
						firstProcess->previous=NULL;
					}
					else
					{
						firstProcess=NULL;
						firstTimeProcess=0;
					}
				}
				allocations--;
				cout<<"\n Deallocation = "<<allocations;
				if(noHole==true)
					noHole=false;

				if(added==1)
					mergeHoles();
				// scanning.
				tempHole = firstHole;
				while(tempHole!=NULL)
				{
					if(newnodeHole->startAddress > tempHole->startAddress && newnodeHole->startAddress < (tempHole->startAddress + tempHole->size))
					{
						if((newnodeHole->size+newnodeHole->startAddress) > (tempHole->size+tempHole->startAddress))
						{
							tempHole->size+=(newnodeHole->size+newnodeHole->startAddress)-(tempHole->size+tempHole->startAddress);
							if(newnodeHole->previous!=NULL)
								newnodeHole->previous->next=newnodeHole->next;
							else
							{
								firstHole = newnodeHole->next;
								firstHole->previous=NULL;
							}// end of else
							if(newnodeHole->next!=NULL)
								newnodeHole->next->previous=newnodeHole->previous;
							else
								newnodeHole->previous->next=NULL;


						}// end if
					}// end of if

					if(newnodeHole->startAddress==tempHole->startAddress && newnodeHole->size==tempHole->size)
					{
							if(newnodeHole->previous!=NULL)
								newnodeHole->previous->next=newnodeHole->next;
							else
							{
								if(newnodeHole->next!=NULL)
								{
									firstHole = newnodeHole->next;
									firstHole->previous=NULL;
								}
								else
									firstHole=NULL;
							}// end of else
						
					}// end of else if

					if(newnodeHole->startAddress==tempHole->startAddress )
					{
							if(newnodeHole->previous!=NULL)
								newnodeHole->previous->next=newnodeHole->next;
							else
							{
								if(newnodeHole->next!=NULL)
								{
									firstHole = newnodeHole->next;
									firstHole->previous=NULL;
								}
								else
									firstHole=NULL;
							}// end of else
						
					}// end of else if

				
					tempHole = tempHole->next;
				}// end of while

				mergeHoles();

}// end of function





//************************** Print Collected Stats *********************//
void printStatistics()
{

		mergeHoles();
		if(tempHole!=NULL)
			cout<<"\n#### Holes List ####";		
		// sum the sizes of holes to calculate the external fragmentation
		tempHole = firstHole;
		holesCount=0;
		holesSizeCount=0;
		while(tempHole!=NULL)
		{
			cout<<"\nHole Number #"<<holesCount;
			cout<<"\n Start Address ="<<tempHole->startAddress;
			cout<<"\n Size of Hole ="<<tempHole->size;
			holesSizeCount+=tempHole->size;
			tempHole = tempHole->next;
			holesCount++;

		}// end of while
	cout<<"\n-----------------------------------------------------";
	cout<<"\n External Fragmentation (Max) = "<<((float)holesSizeCount/(float)64);
	cout<<"\n # of Allocation Requests Denied = "<<memoryAllocationFailed<<" out of "<<(10100+initAllocations);
	cout<<"\n Max # of Holes in the LinkList = "<<maxHolesCount;
	cout<<"\n Average Internal Fragmentation = "<<"0";


}// end of function


//********************** Merge Contigious Memory Slots *******************//
void mergeHoles()
{
		// merging holes 
		tempHole=firstHole;
		while(tempHole!=NULL)
			{
				if(/*tempHole->mark==true &&*/ tempHole->next!=NULL)
				{
					if(tempHole->next->next!=NULL)
					{
						if((tempHole->startAddress+tempHole->size)==tempHole->next->startAddress)
						{
							tempHole->mark=false;
							tempHole->size+=tempHole->next->size;
							tempHole->next=tempHole->next->next;
							if(tempHole->next->next!=NULL)
								tempHole->next->next->previous=tempHole;
							else
								tempHole->next=NULL;

						}// end if
					}
				}

				tempHole=tempHole->next;
			}


}// end of function

