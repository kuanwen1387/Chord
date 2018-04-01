/****************************************************************************************
*											*
*	CSCI319 Assignment 2								*
*	Student Name: Kuan Wen Ng							*
*	Student Number: 5078052								*
*	Email : kwn961@uowmail.edu.au							*
*	Filename: node.cpp								*
*	Description: Functions for class Node						*
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
#include <cmath>
#include <map>
#include <string>
#include "node.h"
#include "chord.h"
using namespace std;

Node::Node()
{
	id = 0;
	predecessor = NULL;
}

//Constructor to initialize node for size M
Node::Node(unsigned short int newBitSize)
{
	bitSize = newBitSize;
	chordSize = pow(2.0, double(bitSize));
	id = 0;
	fingerTable.resize(bitSize);
	predecessor = NULL;
	successor = NULL;
}

Node::~Node()
{
	//No memory allocated for member
}

Node *Node::findSuccessor(unsigned int targetId, bool print)
{
	Node *nodePtr = NULL;

	//Print switch to print nodes traversed
	if (print)
	{
		nodePtr = findPredecessor(targetId, true);
		cout << nodePtr->successor->id << endl;
	}

	else
		nodePtr = findPredecessor(targetId);

	return nodePtr->successor;
}

Node *Node::findPredecessor(unsigned int targetId, bool print)
{
	Node *nodePtr = this;

	if (nodePtr->id == targetId)
		return nodePtr->predecessor;

	if (print)
		cout << nodePtr->id << " > ";

	while (!interval(targetId, nodePtr->id, nodePtr->successor->id, 3))
	{
		//Find closest preceeding finger
		nodePtr = nodePtr->closestPrecedingFinger(targetId);

		if (nodePtr->id == nodePtr->successor->id)
			return nodePtr;

		//Print node traversed along the way
		if (print)
			cout << nodePtr->id << " > ";
		
	}

	return nodePtr;
}

Node *Node::closestPrecedingFinger(unsigned int targetId)
{
	//Find the closest finger to target id
	for (int fingerIndex = bitSize - 1; fingerIndex >= 0; fingerIndex--)
	{
		if (interval(fingerTable[fingerIndex].node->id, id, targetId, 1))
		{
			return fingerTable[fingerIndex].node;
		}
	}
	return this;
}

void Node::join(Node *nodePtr, bool empty)
{
	//Initialise start value for fingers
	for (int fingerIndex = 0; fingerIndex < bitSize; fingerIndex++)
	{
		fingerTable[fingerIndex].start = fmod((id + pow(2.0, fingerIndex)), pow(2.0, double(bitSize)));

		//Point to self if first node
		if (empty)		
			fingerTable[fingerIndex].node = this;
	}

	//Initialise finger table and update others
	if (!empty)
	{
		initFingerTable(nodePtr);
		updateOthers();
	}

	//Point to self if first node
	else
		predecessor = this;
}

void Node::leave()
{
	Node *nodePtr = successor;

	//Predecessor's successor point to successor after removal
	nodePtr->predecessor = predecessor;

	//Update others
	updateOthers(true);
}

void Node::initFingerTable(Node *nodePtr)
{
	//Insert new node before successor
	successor = nodePtr->findSuccessor(fingerTable[0].start, true);
	predecessor = successor->predecessor;
	successor->predecessor = this;


	//Get data and remove from successor
	for (DataIt dataIt = successor->data.begin(); dataIt != successor->data.end() && dataIt->first <= id; dataIt++)
	{
		data[dataIt->first] = dataIt->second;
		successor->data.erase(dataIt->first);
	}

	//Initialise finger table
	for (int fingerIndex = 1; fingerIndex < bitSize; fingerIndex++)
	{
		if (interval(fingerTable[fingerIndex].start, id, fingerTable[fingerIndex - 1].node->id, 2))
			fingerTable[fingerIndex].node = fingerTable[fingerIndex - 1].node;

		else
			fingerTable[fingerIndex].node = nodePtr->findSuccessor(fingerTable[fingerIndex].start);
	}
}

void Node::updateOthers(bool leave)
{
	long long int targetId;
	Node *nodePtr1 = NULL;

	//Transfer keys to successor if leaving
	if (leave)
	{
		for (DataIt dataIt = data.begin(); dataIt != data.end(); dataIt++)
			successor->data[dataIt->first] = dataIt->second;
	}

	//Update preceeding nodes
	for (int fingerIndex = 0; fingerIndex < bitSize; fingerIndex++)
	{
		targetId = id + 1 - pow(2.0, double(fingerIndex));

		if (targetId < 0)
			targetId += chordSize;

		//Find preceeding node to update
		nodePtr1 = findPredecessor(targetId);

		if (nodePtr1->id != id)
		{
			if (leave)
				nodePtr1->updateFingerTable(this, fingerIndex, leave);

			else
				nodePtr1->updateFingerTable(this, fingerIndex);
		}
	}
}

void Node::updateFingerTable(Node *nodePtr, unsigned short int fingerIndex, bool leave)
{
	Node *tempPtr = NULL;

	//Leave switch
	if (leave)
	{
		if (fingerTable[fingerIndex].node->id == nodePtr->id)
		{
			fingerTable[fingerIndex].node = nodePtr->successor;
			
			if (nodePtr->id != predecessor->id)
			{
				tempPtr = predecessor;
				tempPtr->updateFingerTable(nodePtr, fingerIndex, leave);
			}
		}
	}

	else
	{
		if (interval(nodePtr->id, id, fingerTable[fingerIndex].node->id, 2) || id == fingerTable[fingerIndex].node->id)
		{
			fingerTable[fingerIndex].node = nodePtr;

			if (nodePtr->id != predecessor->id)
			{
				tempPtr = predecessor;
				tempPtr->updateFingerTable(nodePtr, fingerIndex);
			}
		}
	}
}

void Node::addKey(string input)
{
	unsigned int key;
	Node *nodePtr = NULL;

	//Hash the string
	key = Hash(input);

	//Find successor node for key
	nodePtr = findSuccessor(key, true);

	//Insert key at node
	nodePtr->data[key] = input;

	cout << "INSERTED " << input << " (key = " << key << ") AT " << nodePtr->id << endl;
}

void Node::removeKey(string input)
{
	unsigned int key;
	Node *nodePtr = NULL;

	//Hash the string
	key = Hash(input);

	//Find successor to key
	nodePtr = findSuccessor(key, true);

	//Remove key from node
	nodePtr->data.erase(key);

	cout << "REMOVED " << input << " (key = " << key << ") FROM " << nodePtr->id << endl;
}

unsigned int Node::Hash(string input)
{
	unsigned int key = 0;

	for (int index = 0; index < input.length(); index++)
	{
		key = ((key << 5) + key) ^ input[index];
	}

	key %= chordSize;

	return key;
}

//Interval function to validate id within interval
bool Node::interval(unsigned int num, unsigned int left, unsigned int right, unsigned short int mode = 1)
{
	bool increment = false;

	if (left == right)
		return false;

	if (right < left)
	{
		right += chordSize;
		increment = true;
	}

	if (num <= right - chordSize && increment)
		num += chordSize;

	//if (num == left && num == right)
		//return true;
	//cout << "num: " << num << " left: " << left << " right: " << right << " Mode: " << mode << endl;
	switch (mode)
	{
		case 1: return (num > left && num < right);
			break;

		case 2: return ((num >= left && num < right) || num == left);
			break;

		case 3: return ((num > left && num <= right) || num == right);
			break;

		default: return false;
	}
}

//Ostream operator overloading for Node
ostream &operator << (ostream &strm, const Node *node)
{
	strm << "DATA AT INDEX NODE " << node->id << ";" << endl;
	for (DataIt dataIt = node->data.begin(); dataIt != node->data.end(); dataIt++)
		strm << dataIt->second << endl;

	strm << "FINGER TABLE OF NODE " << node->id << ";" << endl;
	for (int fingerIndex = 0; fingerIndex < node->fingerTable.size(); fingerIndex++)
		strm << node->fingerTable[fingerIndex].node->id << " ";

	strm << endl;

	return strm;
}
