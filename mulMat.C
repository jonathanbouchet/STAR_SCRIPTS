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

void allMatrixMultiplication()
{
  //vectors to hold data from text files
  vector<double> ladderS, ladderC;
  
  // open file    
  ifstream inputFile("survey.txt");
  
  // test file open   
  if (inputFile) {        
    double value;
    
    // read the elements in the file into a vector  
    while ( inputFile >> value ) {
      ladderS.push_back(value);
    }
  }

  ifstream inputFileC("allCorrection.txt");
  
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

void matrixMultiplication()
{
  //vectors to hold data from text files
  vector<double> ladderS, ladderC;
  
  // open file    
  ifstream inputFile("survey.txt");
  
  // test file open   
  if (inputFile) {        
    double value;
    
    // read the elements in the file into a vector  
    while ( inputFile >> value ) {
      ladderS.push_back(value);
    }
  }

  ifstream inputFileC("correction.txt");
  
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
  ifstream myfile ("allCorrection.txt");
  vector<double> ladderC;
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
	  //myMap[counter].push_back(temp);
	   myMap.insert(pair<int,vector<double> >(counter, temp));
	  //cout << line << '\n';
	  for (unsigned int i=0;i<temp.size(); i++)
	    {
	      std::cout << temp.at(i) ;
	    }
	  std::cout <<" " << std::endl;
	  ++counter;
	}

      // output the values
      typedef map<int, vector<double> >::const_iterator MapIterator;
      for (MapIterator iter = myMap.begin(); iter != myMap.end(); iter++)
	{
	  cout << "Key: " << iter->first << endl << "Values:" << endl;
	  typedef vector<double>::const_iterator vectIterator;
	  for (vectIterator list_iter = iter->second.begin(); list_iter != iter->second.end(); list_iter++)
	    cout << " " << *list_iter << endl;
	}
    
      myfile.close();
    }
  
  else cout << "Unable to open file\n"; 
}
