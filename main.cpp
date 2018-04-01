/****************************************************************************************
*											*
*	CSCI319 Assignment 2								*
*	Student Name: Kuan Wen Ng							*
*	Student Number: 5078052								*
*	Email : kwn961@uowmail.edu.au							*
*	Filename: main.cpp								*
*	Description: Main for Chord program						*
*											*
*	The program could be compiled with the following command on Ubuntu or Banshee.	*
*	g++ -o CHORD ass2Main.cpp chord.cpp node.cpp					*
*											*
*	The program will take an argument from the command line of the filename		*
*	containing the instructions. The following command shows an example of running	*
*	the program where myFile.dat is the filename.					*
*	./CHORD myFile.dat								*
*											*
****************************************************************************************/

#include <iostream>
#include "node.h"
#include "chord.h"
using namespace std;

int main(int argc, char *argv[])
{
	Chord chord;
	chord.Read(argv[1]);

	return 0;
}
