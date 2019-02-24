/*
The Piece of code was designed by Giorgi Gvimradze, for his
Final Bachelor Thesis/Project MaggyTone.

Maggytone Translates USB inputs out of MIDI into Matrix va-
lues. Starts and Ends are places where button gets pressed
and took off.

Summer 2018
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>

#define MAX_NOTE_NUMBER 61
#define LENGTH	16
#define DEBUG


using namespace std;


bool busy()
{
    //check it timer is >0.01 s
    //if yes return true
    return 0; //demo
}

/*
Input value is the needed row of the matrix which represents
exact time of notes played.
*/
void add_value_to_matrix(
	int matrix[][MAX_NOTE_NUMBER],				//the row to be updated
	vector <int> starts,	//start positions
	int st_n,
	vector <int> ends,	//end positions
	int ed_n,
	int counter,
	int force){
	
	//use matrix value as the address of 61 set array
	//Mutex starts
	while(st_n>0)
	{
		st_n--;
		matrix[counter][starts[st_n]]=force;
	}
	
	while(ed_n>0)
	{
		ed_n--;
		matrix[counter][ends[ed_n]]=-1;
	}
	//Mutex ends
}

void print_matrix(
	int matrix[LENGTH][MAX_NOTE_NUMBER])
{
	int i,j;

    for(i=0; i<LENGTH; i++)
    {
        for(j=0; j<MAX_NOTE_NUMBER; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char byte;

    int note,force,matrix[1000][MAX_NOTE_NUMBER]={0},to_int=0, counter=0,fd = open("/dev/midi1", O_RDWR);
    short state=-1;

    vector <int> starts;	//start notes
    vector <int> ends;		//end notes

    while(counter<LENGTH){
	    ssize_t size = read(fd, &byte, 1);
	    to_int=byte;

//	    printf("%d\n", to_int);
#ifdef DEBUG
	    if(to_int==-112)
		{
	    	printf("START\n");
	    	state =1;
	    }
	    else if(to_int==-128)
	    {
	    	printf("END\n");
	    	state =0;
	    }
	    else
	    {	if(state==2)
	    	{
	    		state=3;
	    		if(!busy())
   		  		{	
   		  			force=to_int;
   		  			add_value_to_matrix(matrix,starts,starts.size(),ends,ends.size(),counter,force);
   		  			counter++;
   		  			starts.clear();
   		  			ends.clear();
   		  		}
	    	}
	    	else
	    	if(state==1)
	    	{
	    		starts.push_back(to_int-36);
	    		state=2;   		  		
	    	}
	    	else 
	    	if(state==0){
	    		ends.push_back(to_int-36);
	    		state=3;
	    		if(!busy())
	    		{
   		  			add_value_to_matrix(matrix,starts,starts.size(),ends,ends.size(),counter,0);
	    			counter++;
	    			starts.clear();
   		  			ends.clear();
				}
	    	}
	    	else if(state==-1){
	    		printf("Not connected!\n");
	    	}
	    	printf("%d                 %d\n", to_int,counter);
	    }
#endif
    }
    print_matrix(matrix);
    return 0;
}
