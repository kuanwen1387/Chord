/****************************************************************************************
*											*
*	CSCI319 Assignment 2								*
*	Student Name: Kuan Wen Ng							*
*	Student Number: 5078052								*
*	Email : kwn961@uowmail.edu.au							*
*	Filename: chord.cpp								*
*	Description: Functions for class Chord						*
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
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <fstream>
#include "node.h"
#include "chord.h"
using namespace std;

Chord::Chord()
{
	first = NULL;
}

Chord::~Chord()
{
	ClearChord();
}

void Chord::InitChord(unsigned short int newBitSize)
{
	//Return if m is invalid
	if (newBitSize < 1 || newBitSize > 32)
		return;

	//Clear chord if Chord is not empty
	if (first != NULL)
		ClearChord();

	//Assign m and chordSize
	bitSize = newBitSize;
	chordSize = pow(2.0, double(bitSize));

	//Initialise first node with lowest index
	first = new Node(bitSize);
	first->id = 0;
	first->join(first, true);

	//Output student name and number
	cout << "Kuan Wen Ng\n5078052" << endl;
}

//Function to clear the Chord
void Chord::ClearChord()
{
	unsigned int breakPoint = first->predecessor->id;
	Node *nodePtr1 = first, *nodePtr2 = NULL;

	//Traverse the Chord clockwise
	while (nodePtr1->id != breakPoint)
	{
		nodePtr2 = nodePtr1->successor;
		delete nodePtr1;
		nodePtr1 = nodePtr2;

		if (nodePtr1->id == breakPoint)
		{
			delete nodePtr1;
			break;
		}
	}
	first = NULL;
}

void Chord::AddPeer(unsigned int newId)
{
	//Initialise new node
	Node *nodePtr = new Node(bitSize);

	nodePtr->id = newId;

	//New node joining the first node
	nodePtr->join(first);

	cout << "PEER " << newId << " ADDED" << endl;
}

void Chord::RemovePeer(unsigned int removeId)
{
	//Find the node to remove
	Node *nodePtr = first->findSuccessor(removeId, true);

	//Remove the node
	nodePtr->leave();

	cout << "PEER " << nodePtr->id << " REMOVED" << endl;

	if (nodePtr->id == first->id)
	{
		//Last node removed
		if (first->id == first->successor->id)
		{
			delete first;
			cout << "Last of the peers removed. CHORD terminated." << endl;
			exit(0);
		}

		else
			first = first->successor;
	}
	//Free allocated memory
	delete nodePtr;
}

void Chord::FindKey(unsigned int key)
{	
	first->findSuccessor(key, true);
}

void Chord::Insert(string input)
{
	first->addKey(input);
}

void Chord::Delete(string input)
{
	first->removeKey(input);
}

void Chord::Print(unsigned int key)
{
	Node *nodePtr = NULL;

	//Find the node
	nodePtr = first->findSuccessor(key, true);

	//Print details of the node
	cout << nodePtr;
}

void Chord::Read(char *filename)
{
	ifstream inputFile;
	size_t found;
	unsigned short int size = 100, input;
	char readString[size], *strPtr = NULL;
	string inputString, comment;

	//Open file
	inputFile.open(filename);

	while (inputFile.good() && !inputFile.eof())
	{
		if (inputFile.getline(readString, size, '\n'))
		{
			//Remove comments
			inputString = readString;
			found = inputString.find("#");

			if (found != string::npos)
				inputString.erase(found, inputString.length() - found);

			switch (inputString[0])
			{
				case 'i': if (inputString[2] == 'i')
					{
						inputString.erase(0, 10);
						InitChord(atoi(inputString.c_str()));
					}

					else
					{
						inputString.erase(0, 7);
						Insert(inputString);
					}
					break;

				case 'a': inputString.erase(0, 8);
					AddPeer(atoi(inputString.c_str()));
					break;

				case 'r': inputString.erase(0, 11);
					RemovePeer(atoi(inputString.c_str()));
					break;

				case 'f': inputString.erase(0, 8);
					FindKey(atoi(inputString.c_str()));
					break;

				case 'd': inputString.erase(0, 7);
					Delete(inputString);
					break;

				case 'p': inputString.erase(0, 6);
					Print(atoi(inputString.c_str()));
					break;

				default: ;
			}
		}
	}
	//Close file
	inputFile.close();
}

void Chord::noobc()
{
	unsigned int num = first->predecessor->id;
	Node *nodePtr = first;

	while (nodePtr->id != num)
	{
		cout << nodePtr;
		nodePtr = nodePtr->successor;

		if (nodePtr->id == num)
			cout << nodePtr;
	}
}
