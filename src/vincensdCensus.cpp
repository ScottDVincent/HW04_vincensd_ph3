/*****
 * File: vincensdCensus.cpp	
 * Author   : vincensd
 * Date     : 2012-10-10
 * Sources  : 
 * Purpose  : 
 
 Some helpful ideas
 http://www.daniweb.com/software-development/cpp/threads/50051/how-to-read-data-from-csv-file-in-an-array-and-parse
 http://stackoverflow.com/questions/5265160/using-getline-to-extract-information-and-store-them-in-a-datatype-in-c?rq=1
 /http://stackoverflow.com/questions/12233596/reading-csv-data-from-a-file?rq=1
 */
 
#include "vincensdCensus.h"
using namespace std;

	/** 
	 * Constructs Node object which points to itself and has a data_ member
	 */  

	Node::Node(){
		left_ = right_ = NULL;		// create node; the assignments go in reverse order
		data = new Entry();			
	}


	Node::Node(Entry e){
		left_ = right_ = NULL;		// create node; the assignments go in reverse order
		data = new Entry();			
	}

	/** 
	*Default vincensdCensus constructor
	*/  
	vincensdCensus::vincensdCensus(){
	}



	/*
	 * add all entries in the array to your data structure
	 *
	 * The "c" parameter is a pointer to an array of all the entries to be added, 
	 and n is the length of the array.
	 *
	 * Note: If you detect that two items have the same coordinates, 
	 then do not add the new item that has the same coordinates as another item. 
	 This is guaranteed to happen, by the way, because some Starbucks locations are listed in the database twice. We will define two locations to be the "same location" if both |x1 - x2| <= 0.00001 and |y1 - y2| < 0.00001
	 */

	///////////////////////////////////////////////////////


	void vincensdCensus::build(Entry* c, int n){
		//will call insert
		double threshold = 0.00001;

		//vector<Entry> buildVec;	
		//storage.push_back(n);	
		// Convert from array back to vector to remove duplicates and randomize
		for(int i = 0; i <= (n-1); i++)	{	
			buildVec.push_back(c[i]);		
		}	
		
		// Check for duplicate locations.  (can use .dot operator to check points
		for(int i = 0; i < buildVec.size(); i++) {	
			for(int j = i + 1; j < buildVec.size(); j++) {		
				if(abs(buildVec[i].x - buildVec[j].x) <= threshold)		//and
					if(abs(buildVec[i].y - buildVec[j].y) <= threshold)	
					 // how to erase a single element from aa vector 
                     //http://stackoverflow.com/questions/875103/how-to-erase-element-from-stdvector-by-index			
						buildVec.erase(buildVec.begin() + i);	
			}
		}
		
		// console()  << cout << "output = " << buildVec.size() << endl;
		
		// have to update arraySize to new size of  vector
		arraySize =  buildVec.size();  

		// Shuffles vector to make it random	
		std::random_shuffle(buildVec.begin(), buildVec.end());	
		

		/** turn into array and use in getNearest in tree format

		entryArrBld = new Entry[arraySize];	
		// Copies all values from the vector to the array	
		for(int i = 0; i < buildVec.size(); i++) {		
			entryArrBld[i] = buildVec[i];
		}
		*/
		
		
		/** Test ranges
		console() << cout << "output = " << &entryArrBld[0] << endl;
		console() << cout << "output = " << &entryArrBld[1] << endl;
		console() << cout << "output = " << &entryArrBld[arraySize-2] << endl;
		console() << cout << "output = " << &entryArrBld[arraySize-1] << endl;
		*/

	} // end build
	
	/** Return a pointer to the entry that is closest to the given coordinates. Your
	 *  answer may be approximate, but then you will lose points on the "Accuracy" quality measure
	 */
	
	Entry* vincensdCensus::getNearest(double x, double y) {
		// will call search
		
		//Entry* e;
		double qX = x;
		double qY = y;
		double difX, finX ;
		double difY, finY;
		
		// set distance of first point
		double distanceSmallest = 1.1;

		
		// loop thru points
			for (int i = 0; i <= (buildVec.size()-1); i++) {
				
			double starX = buildVec[i].x;
			double starY = buildVec[i].y;

			// find x
			difX = abs(qX - starX);
			finX = pow(difX, 2);
			// find y
			difY = abs(qY - starY);
			finY = pow(difY, 2);


		double distanceTwo = sqrt( finX + finY );
			if  (distanceTwo < distanceSmallest){
					distanceSmallest = distanceTwo;
					//  equate the Entry w/ the smallest (x,y) coords
					closestBucks = &buildVec[i]; // make Entry* = to the '&' address of entryArrBld
					} 
		  } // end for

		//check result of shortestDistance
		cout <<  "Identity is: " << closestBucks ;
		
		// return the Entry with the nearest location
		return closestBucks;
		

	} // end getNearest



	///////////////// TREE BASED ATTEMPT ////////////////////////////////////////////
	void vincensdCensus::buildTree(Entry* c, int n){

		root = NULL;
		r = NULL;
		arraySize = n;
		bool isXLevel = true;
		int levelCounter = 0;
		
		//don't need to find median, only if sorted
		//int median = n/2; // will give us a whole number
		//need to get middle Entry
		//c = c + median;
		

		// have to create root node with first Entry
		root = insert(c, root, isXLevel);

		// loop through array 'c', start at i=1 because we used '0' for root
		for (int i = 1; i <= (arraySize-1); i++ ){ 
			// check level; even = true, odd = false
			//if (i % 2 == 0) // (levelCounter % 2 == 0)
				//isXLevel = true;

			// call insert
			insert (c, r, isXLevel);
					  //levelCounter++;
		     c = c+1; // increase pointer to next Entry object in EntryArr
		}

					
				

	} //end build

	

	/**
	* void insert
	* @param Entry: Entry object we are adding to the tree
	* @param Node* r: Pointer to addy of the node 
	* @paramL bool isXLevel
	*/
	Node* vincensdCensus::insert(Entry* e, Node* r, bool isXLevel){ 

		 double xDist, yDist; 
		 double threshold = 0.00001;

		if ( r == NULL)				// base case
			newNode = new Node(*e);
			return newNode;

			// base case
		/** this is the place to check and see if our distances are within bound, if not then don't insert,
			//  if (r -> key == e -> key) //original base case for equals
			 our trial entry e has an x coord associated with it and the node, which is already in place has a data var which is an entry which of course has a x coord. 
			 This is what we are comparing, to see if the new Entry is "the same" as the Node which position we have arrived on in our tree placement
			*/
		 xDist = abs((e -> x) - (r -> data -> x));  // |x1 - x2|<= 0.00001
		 yDist = abs((e -> y) - (r -> data -> y));  // |y1 - y2|  <= 0.00001  
		 if ( (xDist <= threshold) && ( yDist <= threshold) )
			return r;

		if (isXLevel) {
		// for X Level : 

			if (e -> x < r -> data -> x){
				r -> left_ = insert (e, r-> left_, !isXLevel);      //
			} else {
				r -> right_ = insert (e, r -> right_, !isXLevel);	//
			} 
		
		//for Y Level
		} else {

			if (e -> y < r -> data -> y){
				r -> left_ = insert (e, r-> left_, !isXLevel);      //
			} else {
				r -> right_ = insert (e, r -> right_, !isXLevel);	//
			}

		} // end if (isXLevel)

		return r;
	} //end insert




	/*
	 * Return a pointer to the entry that is closest to the given coordinates. Your
	 *  answer may be approximate, but then you will lose points on the "Accuracy" quality measure
	 */
	Entry* vincensdCensus::getNearestNode(double x, double y) {
		// vars
		//Entry* e;
			
		
		// set distance of first point
		double distanceSmallest = 1.1;
		bool isXLevel;
	 
		// loop thru points
		for (int i = 0; i <= arraySize; i++) {
			// check level; even = true, odd = false
			//if (i % 2 == 0) // (levelCounter % 2 == 0)
				//isXLevel = true;
			// call search 
			search(x, y, r, isXLevel); 
				//levelCounter++;	
		
		} // end for

	
		//check result of shortestDistance
		cout << "Identity is: " << closestBucks ;
		
		// return the Entry with the nearest location
		return closestBucks;
		


	} // end getNearest
	


	/**
	* Entry* search
	* @param double xQ
	* @param double yQ
	* @param Node* r: Pointer to addy of the node 
	* @param bool isXLevel
	*/
	Entry* vincensdCensus::search(double x, double y, Node* r, bool isXLevel){ 

		//vars
		double qX = x;
		double qY = y;
		double difX, finX ;
		double difY, finY;
		// set distance of first point
		double distanceSmallest = 1.1;



		if (r == NULL)								//base case
			return NULL;

			// !tip  r -> data.x is incorrect will get a "Expression must have class type" error
		if ( (qX  == r -> data-> x) && (qY  == r -> data -> y) )   // this would never happen that q would be the exact location
			return r -> data;						


			double starX = r ->data ->x;
			double starY = r ->data ->y;

		Entry* best_left  = search (qX, qY, r -> left_, !isXLevel);
		Entry* best_right = search (qX, qY, r -> right_, !isXLevel);
		Entry* current_entry = r -> data;
		// have to check distance here to see how far you are away from the Node you find in the tree


		// ?? have to do something to keep track of our depth/level ??

			// find x
			difX = abs(qX - starX);
			finX = pow(difX, 2);
			// find y
			difY = abs(qY - starY);
			finY = pow(difY, 2);

			// compare distance
		double distanceTwo = sqrt( finX + finY );
			if  (distanceTwo < distanceSmallest){
					distanceSmallest = distanceTwo;
					closestBucks = current_entry; //
					} 
		 

	} //end search

