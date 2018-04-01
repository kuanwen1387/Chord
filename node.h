/****************************************************************************************
*											*
*	CSCI319 Assignment 2								*
*	Student Name: Kuan Wen Ng							*
*	Student Number: 5078052								*
*	Email : kwn961@uowmail.edu.au							*
*	Filename: node.h								*
*	Description: Header for class Node						*
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

#include <map>
#include <vector>
#include <string>
using namespace std;

#define successor fingerTable[0].node

class Node;
struct Finger;
typedef map<unsigned int, string> Data;	//Map for data in key and string pair
typedef Data::const_iterator DataIt;	//Iterator for data
typedef vector<Finger> FingerTable;	//Finger table

//Structure for finger
struct Finger
{
	unsigned int start;	//n + 2 ^ (k - 1) mod 2 ^ m, 1 <= k <= m
	Node *node;	//Pointer to node
};

class Node
{
	friend class Chord;

private:
	unsigned short int bitSize;	//Bit size or M
	unsigned int chordSize;	//Size of Chord
	unsigned int id;	//Node id
	Node *predecessor;	//Predecessor of node
	Data data;	//Data
	FingerTable fingerTable;	//Finger table

public:
	Node();
	Node(unsigned short int );
	~Node();
	static void setSize(unsigned short int );
	Node *findSuccessor(unsigned int , bool print = false);
	Node *findPredecessor(unsigned int , bool print = false);
	Node *closestPrecedingFinger(unsigned int );
	void join(Node * , bool empty = false);
	void leave();
	void initFingerTable(Node *);
	void updateOthers(bool leave = false);
	void updateFingerTable(Node * , unsigned short int , bool leave = false);
	void addKey(string );
	void removeKey(string );
	unsigned int Hash(string );
	void noob();
	bool interval(unsigned int num, unsigned int left, unsigned int right, unsigned short int );
	friend ostream &operator << (ostream & , const Node * );
};
