/****************************************************************************************
*											*
*	CSCI319 Assignment 2								*
*	Student Name: Kuan Wen Ng							*
*	Student Number: 5078052								*
*	Email : kwn961@uowmail.edu.au							*
*	Filename: chord.h								*
*	Description: Header for class Chord						*
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
using namespace std;

class Chord
{
	friend class Node;

private:
	unsigned short int bitSize;	//Bit size or M
	unsigned int chordSize;	//Size of the Chord
	Node *first;	//Pointer to the first node

public:
	Chord();
	~Chord();
	void InitChord(unsigned short int );
	void ClearChord();
	void AddPeer(unsigned int );
	void RemovePeer(unsigned int );
	void FindKey(unsigned int );
	void Insert(string );
	void Delete(string );
	void Print(unsigned int );
	void Read(char * );
	void noobc();
};


