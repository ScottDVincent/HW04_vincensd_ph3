/*****
 * File: vincensdCensus.h	
 * Author   : vincensd
 * Date     : 2012-10-10
 * Sources  : 
 * Purpose  : 
 */
 
#pragma once
#include "Census.h"
// std includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
#include <algorithm>
#include <functional>
#include <conio.h>



using namespace std;
std::stringstream;


 class Node{
public:		

//constructors
	Node();
	Node(Entry e);

			
	// node pointers in the tree
	Node* left_;
	Node* right_;
	
	// a Entry object to hold data	
	Entry* data;		


   };   // end member declarations

 
 class vincensdCensus : public Census{  // using Starbucks.h as an interface
public:
	
	// Members vars
	// Starbucks stuff
	Entry* entryArrBld;		// starbucks: holds our array from build to use in getNearest 
	vector<Entry> buildVec;	// starbucks: holds our vector from build to use in getNearest
	int arraySize;			// size of array after we remove duplicates
	Entry* closestBucks;	// object of closeest Starbucks
	// Census stuff
	Entry* entry_Census2000ArrBld;		// : holds our array from build to use in getNearest 
	vector<Entry> Census2000_buildVec;	// : holds our vector from build to use in getNearest
	int Census2000Size;					// size of array
	
	Entry* entry_Census2010ArrBld;		// : holds our array from build to use in getNearest 
	vector<Entry> Census2010_buildVec;	// : holds our vector from build to use in getNearest
	int Census2010Size;					// size of array 
	
	// Tree stuff
	Node* newNode;			// new node in tree
	Node* root;				// root node in tree
	Node* r;				// root node of a sub-tree in tree
	/**
	double keyX;
	double keyY;
	bool isXLevel;
	bool isYLevel;
	*/


	/**
	Constructor Method
	*/
	// no arguemnt constructor
	vincensdCensus();
	//Entry();
	
	

	/*
	 * add all entries in the array to your data structure
	 *
	 * The "c" parameter is a pointer to an array of all the entries to be added, and n is the length of  the array.
	 *
	 * Note: If you detect that two items have the same coordinates, then do not add the new item that has the same coordinates as another item. 
	 This is guaranteed to happen, by the way, because some Starbucks locations are listed in the database twice. 
	 We will define two locations to be the "same location" if both |x1 - x2| <= 0.00001 and |y1 - y2| < 0.00001
	 */
	 
	 /**
	* void build
	* @param Entry* c
	* @param int n
	*/
	void build(Entry* c, int n);
	//and
	void buildArray(Entry* c, int n);
	//and
	void buildTree(Entry* c, int n);
	

	/*
	 * Return a pointer to the entry that is closest to the given coordinates. Your
	 *  answer may be approximate, but then you will lose points on the "Accuracy" quality measure
	 */
	 /**
	* Entry* getNearest
	* @param double x
	* @param double y
	*/
	Entry* getNearest(double x, double y);
	//and
	Entry* getNearestArray(double x, double y);
	//and
	Entry* getNearestNode(double x, double y);
	
	/**
	Destructor Method
	*/

	/**
	* Entry* search
	* @param double xQ
	* @param double yQ
	* @param Node* r: Pointer to addy of the node 
	* @param bool isXLevel
	*/
	Entry* search(double xQ, double yQ, Node* r, bool isXLevel);


	/**
	* void insert
	* @param Entry: Entry object we are adding to the tree
	* @param Node* r: Pointer to addy of the node 
	* @paramL bool isXLevel
	*/
	Node* insert (Entry* e, Node* r, bool isXLevel);


};


