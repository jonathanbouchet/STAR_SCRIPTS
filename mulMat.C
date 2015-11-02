#include "Riostream.h" 
#include "TMatrixT.h"  
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

/*
Sxx Sxy Sxz Syx Syy Syz Szx Szy Szz Sx Sy Sz
           Sxx Sxy Sxz  
rotation = Syx Syy Syz
           Szx Szy Szz
translation = Sx Sy Sz
 */

void matrixMultiplication()
{
  //vectors to hold data from text files
  vector<double> ladderS, ladderC;
  
  // open file    
  ifstream inputFile("surveyTest.txt");
  
  // test file open   
  if (inputFile) {        
    double value;
    
    // read the elements in the file into a vector  
    while ( inputFile >> value ) {
      ladderS.push_back(value);
    }
  }

  ifstream inputFileC("correctionTest.txt");
  
  if (inputFileC) {        
    double valueC;
    
    while ( inputFileC >> valueC ) {
      ladderC.push_back(valueC);
    }
  }

  Double_t inputS[16] = {ladderS[0],ladderS[1],ladderS[2],ladderS[9],ladderS[3],ladderS[4],ladderS[5],ladderS[10],ladderS[6],ladderS[7],ladderS[8],ladderS[11],0.0 ,0.0 ,0.0 ,1.0};
  Double_t inputC[16] = {ladderC[0],ladderC[1],ladderC[2],ladderC[9],ladderC[3],ladderC[4],ladderC[5],ladderC[10],ladderC[6],ladderC[7],ladderC[8],ladderC[11],0.0 ,0.0 ,0.0 ,1.0};
  
  TMatrixT<double> testS(4,4,inputS,"");
  //If option="F", the array fills the matrix column-wise else row-wise
  testS.Print();

  TMatrixT<double> testC(4,4,inputC,"");
  testC.Print();
  testS *=testC;
  testS.Print();
}

void readAll ()
{
  string line;
  ifstream myfile ("correction.txt");
  int counter=0;
  std::map<int, std::vector<double> > myMap;
  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
	{
	  std::vector<double> temp;
	  std::istringstream str(line);
	  double n;
	  while ( str >> n )
	    {
	      temp.push_back(n);
	    }
	   myMap.insert(pair<int,vector<double> >(counter, temp));
	   /*
	     for (unsigned int i=0;i<temp.size(); i++)
	     {
	     std::cout << temp.at(i) ;
	     }
	     std::cout <<" " << std::endl;
	   */
	  ++counter;
	}
      myfile.close();
    }
  else cout << "Unable to open file\n"; 
  
  string lineS;
  ifstream surv ("survey.txt");
  int counterS=0;
  std::map<int, std::vector<double> > myMapS;
  if (surv.is_open())
    {
      while ( getline (surv,lineS) )
	{
	  std::vector<double> tempS;
	  std::istringstream str(lineS);
	  double n;
	  while ( str >> n )
	    {
	      tempS.push_back(n);
	    }
	  myMapS.insert(pair<int,vector<double> >(counterS, tempS));
	  ++counterS;
	}
      surv.close();
    }
  else cout << "Unable to open file\n"; 
  
  Double_t inputC[20][12];
  Double_t inputS[20][12];
  for(int m=0;m<20;m++)
    {
      for(int n=0;n<12;n++)
	{
	  inputC[m][n]=0.0;
	  inputS[m][n]=0.0;
	}
    }

  int tt = 0;
  int cc = 0;
  
  for(auto const &ent1 : myMap) 
    {
      cout <<" current row correction : " << tt << endl;
      // ent1.first is the first key
      cc=0;
      for(auto const &ent2 : ent1.second) 
	{
	  // ent2.first is the second key
	  // ent2.second is the data
	  cout << " " << ent2 ;
	  inputC[tt][cc] = ent2;
	  cc++;
	}
      tt++;
      cout <<" " << endl;
    }

  int rr = 0;
  int qq = 0;
  for(auto const &ens1 : myMapS) 
    {
      cout <<" current row survey : " << rr << endl;
      qq=0;
      for(auto const &ens2 : ens1.second) 
	{
	  cout << " " << ens2 ;
	  inputS[rr][qq] = ens2;
	  qq++;
	}
      rr++;
      cout <<" " << endl;
    }
  ofstream myout;
  myout.open("sstLaddersOnSst.20150101.000002.C");
  for(int i=0;i<20;i++)
    {
      Double_t ladderS[16] = {inputS[i][0],inputS[i][1],inputS[i][2],inputS[i][9],inputS[i][3],inputS[i][4],inputS[i][5],inputS[i][10],inputS[i][6],inputS[i][7],inputS[i][8],inputS[i][11],0.0 ,0.0 ,0.0 ,1.0};
      Double_t ladderC[16] = {inputC[i][0],inputC[i][1],inputC[i][2],inputC[i][9],inputC[i][3],inputC[i][4],inputC[i][5],inputC[i][10],inputC[i][6],inputC[i][7],inputC[i][8],inputC[i][11],0.0 ,0.0 ,0.0 ,1.0};
      
      TMatrixT<double> testS(4,4,ladderS,"");
      cout <<" ****************************************************" << endl;
      cout <<" LADDER " << i+1 <<" SURVEY " << endl;
      testS.Print();
      
      TMatrixT<double> testC(4,4,ladderC,"");
      cout <<" LADDER " << i+1 <<" CORRECTION " << endl;

      testC.Print();

      testS *=testC;
      cout <<" LADDER " << i+1 <<" CORRECTED " << endl;
      testS.Print();
      myout << i+1 <<" ";
      for(int ncol=0;ncol<4;ncol++)
	{
	  for(int nrow=0;nrow<4;nrow++)
	    {
	      //cout <<" " << testS(ncol,nrow);
	      myout << " " << testS(ncol,nrow);
	    }
	}
      myout <<"\n";
      //cout <<" " << endl;
    }
}


// output the values
/*
  typedef map<int, vector<double> >::const_iterator MapIterator;
  for (MapIterator iter = myMap.begin(); iter != myMap.end(); iter++)
  {
  cout << "Key: " << iter->first << endl << "Values:" << endl;
  typedef vector<double>::const_iterator vectIterator;
  for (vectIterator list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
  {
  cout << " " << *list_iter ;
  }
*/

/* 
loop using auto for ( const auto &p : table )
{
   std::cout << p.first << '\t' << p.second << std::endl;
} 
*/

