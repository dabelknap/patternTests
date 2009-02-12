//****************************************************************************
//
//     Program:  crateInputToPicture
//     Author:   K Grogg
//     Date:     08.10.08
//     Revision:
//     Purpose:  To convert from input format to "picture" format 
//               To improve debugging of problems seen in pattern test results
//
//****************************************************************************
 
#include <iostream>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <list>
#include <vector>
#include <time.h>
 
 
using namespace std;
 
int ecalArray[64][18][7][32],hcalArray[64][18][7][32];  //[event][crate][card][tower_num]
int ehcalArray[18][7][32]; //18 crates, 7 cards, 32 towers  
//will store LUT output: LinearEMET(0:6)+HoEFGVetoBit(7:7)+LinearJetET(8:16)+ActivityBit(17:17)
/*

Tower mapping for the rctTestInputFile --
(tower number are at the bottom right corner of each tower)

Positive eta:
-----------
Cards0 - 5:

        Region 0                                  Region 1
--------------------------------       --------------------------------
|       |       |       |       |      |       |       |       |       |
|      0|      4|      8|     12|      |     16|     20|     24|     28|
--------------------------------       --------------------------------
|       |       |       |       |      |       |       |       |       |
|      1|      5|      9|     13|      |     17|     21|     25|     29|
--------------------------------       --------------------------------
|       |       |       |       |      |       |       |       |       |
|      2|      6|     10|     14|      |     18|     22|     26|     30|
--------------------------------      --------------------------------
|       |       |       |       |      |       |       |       |       |
|      3|      7|     11|     15|      |     19|     23|     27|     31|
--------------------------------       --------------------------------


Card 6:

-------------------------------- 
|       |       |       |       |
|      0|      4|      8|     12|
-------------------------------- 
|       |       |       |       |
|      1|      5|      9|     13|
--------------------------------    Region 0
|       |       |       |       |
|      2|      6|     10|     14|
-------------------------------- 
|       |       |       |       |
|      3|      7|     11|     15|
-------------------------------- 

-------------------------------- 
|       |       |       |       |
|     28|     24|     20|     16|
-------------------------------- 
|       |       |       |       |
|     29|     25|     21|     17|
--------------------------------    Region 1
|       |       |       |       |
|     30|     26|     22|     18|
-------------------------------- 
|       |       |       |       |
|     31|     27|     23|     19|
-------------------------------- 

Negative eta
------------
Mirror image of the above mapping reflected across the eta=0 axis

*/


//___________________________________________________________________________
 

int main(int argc, char **argv) {
  int crate; int card; int event; int EMAddr; int HDAddr; int towernum; int LUTOut; int ret;
  char inputfilename[200], outputfilename[200], line[200];
  vector<int> crates(4); vector<int> crateList(32); vector<int> cardList(224); vector<int> towerList(896);
  if (argc != 3) {
    cout<<"Invalid input file\nFormat: see format in TriDAS/trigger/rct/patternTest/input\n";
    return 1;
  }
  int chooseEvent = argv[2];
  strcpy(inputfilename,argv[1]);
  strcpy(outputfilename, argv[1]);
  strcat(outputfilename, ".txt");
  std::ofstream outstream(outputfilename); 


  FILE *fptr = fopen(inputfilename, "r");
  if (fptr == NULL) {
    cout<<"Error: Could not open cratePlan energies file "<<inputfilename<<"\n";
    return 1;
  }
   for(crate = 0; crate < 18; crate++) {
      for(card = 0; card < 7; card++) {
	for(towernum = 0; towernum < 32; towernum++) {
	  ehcalArray[crate][card][towernum] = 0;
	}
      }
    }

  char line[100];
  //get rid of all the non-relevant information
  while (1) {
    char * oldline = line;
    fgets(line, 100, fptr);
    if(feof(fptr))
      break;
    if(strstr(line,"Event	Crate	Card	Tower	EMAddr	HDAddr	LUTOut")!=NULL)
      //need string version of event chosen, and only compair first digit
      //if(strstr(line,event+"0 0 ")!=null)
      ret = sscanf(line, "%d %d %d %d %d %d %d", &event,&crate, &card, &tower, &EMAddr, &HDAddr, &LUTOut );
    if(event==chooseEvent)
      ehcalArray[crate][card][tower]=LUTOut;
    
  for(int i = 0; i<18;i++){
    if(i<9) // negative eta
      for(int j = 5; j>-1; j--){ //j = 6 hack to avoid card 6 for now
	if(j%2==0) {
      //dump  values for the even cards
	  for(int k = 28; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 29; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 30; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 31; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	}
	else {
      //dump  values for the odd cards
	  for(int k = 28; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 29; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 30; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 31; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	}
      }
    else //positive eta
      for(int j = 0; j<6; j++){ //j = 7 hack to avoid card 6 for now
	if(j%2==0) {
      //dump  values for the even cards
	  for(int k = 28; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 29; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 30; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 31; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	}
	else {
      //dump  values for the odd cards
	  for(int k = 28; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 29; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 30; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	  for(int k = 31; k<=0; k-=4)
	    {
	      outstream << ehcalArray[i][j][k] << "   ";
	    }
	  outstream << "\n";
	}
      }
  }

  fclose(fptr);
  outstream.close();

  return 0;
}

 
