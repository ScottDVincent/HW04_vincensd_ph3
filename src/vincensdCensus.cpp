/*****
 * File		: vincensdCensus.cpp	
 * Author   : vincensd
 * Date     : 2012-10-10
 * Sources  : 
 * Purpose  : Implement functionality for HW04: build, buildCensus, getNearest
 
 Some helpful ideas
 http://www.daniweb.com/software-development/cpp/threads/50051/how-to-read-data-from-csv-file-in-an-array-and-parse
 http://stackoverflow.com/questions/5265160/using-getline-to-extract-information-and-store-them-in-a-datatype-in-c?rq=1
 http://stackoverflow.com/questions/12233596/reading-csv-data-from-a-file?rq=1
 */
 
#include "vincensdCensus.h"


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
		//will call insert in BST version
		double threshold = 0.00001;

		//vector<Entry> bucksVec;	
		
		// Convert from array back to vector to remove duplicates and randomize
		for(int i = 0; i <= (n-1); i++)	{	
			bucksVec.push_back(c[i]);		
		}	
		
		// Check for duplicate locations.  (can use .dot operator to check points
		for(int i = 0; i < bucksVec.size(); i++) {	
			for(int j = i + 1; j < bucksVec.size(); j++) {		
				if(abs(bucksVec[i].x - bucksVec[j].x) <= threshold)		//and
					if(abs(bucksVec[i].y - bucksVec[j].y) <= threshold)	
					 // how to erase a single element from aa vector 
                     //http://stackoverflow.com/questions/875103/how-to-erase-element-from-stdvector-by-index			
						bucksVec.erase(bucksVec.begin() + i);	
			}
		}
		
		// console()  << cout << "output = " << bucksVec.size() << endl;
		
		// have to update arraySize to new size of  vector
		starbucksSize =  bucksVec.size();  

		// Shuffles vector to make it random, for use in BST
		//std::random_shuffle(bucksVec.begin(), bucksVec.end());	
		

		/** turn into array and use in getNearest in tree format

		entryArrBld = new Entry[arraySize];	
		// Copies all values from the vector to the array	
		for(int i = 0; i < bucksVec.size(); i++) {		
			entryArrBld[i] = bucksVec[i];
		}
		*/
		
		
		/** Test ranges
		console() << cout << "output = " << &entryArrBld[0] << endl;
		console() << cout << "output = " << &entryArrBld[1] << endl;
		console() << cout << "output = " << &entryArrBld[arraySize-2] << endl;
		console() << cout << "output = " << &entryArrBld[arraySize-1] << endl;
		*/

	} // end build

	////////////////////////////////////////////////////////////
	
	

	 void vincensdCensus::buildCensus(CensusEntry* c, int n) {
		
		// vector<CensusEntry> censusVec;	// : holds our vector from build to use in getNearest
	
		// Convert from array back to vector to remove duplicates and randomize
		for(int i = 0; i <= (n-1); i++)	{	
			censusVec.push_back(c[i]);		
		}	
					
		// have to update arraySize to new size of  vector
		CensusSize =  censusVec.size();  

		// Shuffles vector to make it random, for use in BST
		//std::random_shuffle(bucksVec.begin(), bucksVec.end());	
		

	 } //end buildCensus


	

//////////////////////////////////////////////////////////
	
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
			for (int i = 0; i <= (bucksVec.size()-1); i++) {
				
			double starX = bucksVec[i].x;
			double starY = bucksVec[i].y;

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
					closestBucks = &bucksVec[i]; // make Entry* = to the '&' address of entryArrBld
					} 
		  } // end for

		//check result of shortestDistance
		cout <<  "Identity is: " << closestBucks ;
		
		// return the Entry with the nearest location
		return closestBucks;
		

	} // end getNearest


	/////////////////////////////////////////////////////////////


	void vincensdCensus::drawStarbucks( double x, double y) {
			// turn on alpha blending
		//http://libcinder.org/docs/v0.8.2/namespacecinder_1_1gl.html#a2cb8982a5a007376031745ac074bed4c
		
		
		gl::enableAlphaBlending();
		//activate the alpha channel
		gl::color(ColorA(0.0f,0.0f,0.0f,0.25f));
		
		// draw circle
		//glColor3f (1.0, 0.0, 0.0);
		//Color8u(rand()%256, rand()%256, rand()%256);
		glColor3f (rand()%256, rand()%256, rand()%256);
		// draw a rectangle offset from the primary rect
		gl::drawSolidCircle( Vec2f( ((x)*800), ( (1 -y)*600) ),  1.0f );
		
		
		//gl::color(inColor_);	
		// draw list rectangle
		//gl::drawSolidCircle( Vec2f( ((x)*800)+4, ( (1 -y)*600)+4 ),  2.0f );
		// draw list rectangle
		//gl::drawSolidRect(Rectf (x, y, x+10, y+10) );
		 
	    //gl::drawSolidCircle( Vec2f( 15.0f, 25.0f ), 50.0f );

		//turn off alpha
		gl::disableAlphaBlending();
		
		 // for census stuff
	//	 gl::drawSolidCircle( Vec2f( ((census10_Object.censusVec.at(i).x)*200), ( (1- census10_Object.censusVec.at(i).y)*150) ), 2.0f );
	}


	////////////////////////////////////////////////////////////////
	void vincensdCensus::drawMouseClick( int x, int y) {
		glColor3f (1.0, 1.0, 0.0);
		// draw a rectangle offset from the primary rect
		gl::drawSolidCircle( Vec2f( ((x)), ( (y)) ),  5.0f );
	}




	////////////////////////////////////////////////////////////

	void vincensdCensus::drawCensus(CensusEntry* censusEntry,  int censusYear)  {

		// declare CensuEntry type
		

		
		// turn on alpha blending
		//http://libcinder.org/docs/v0.8.2/namespacecinder_1_1gl.html#a2cb8982a5a007376031745ac074bed4c
		gl::enableAlphaBlending();
		
		//activate the alpha channel
		gl::color(ColorA(0.0f,0.0f,0.0f,0.25f));
		
		
		// loop thru census and send to getNearestCity
		// for (int i=0 i<=

		if (censusYear == 00) {
			glColor3f (0.0, 0.0, 1.0);
		} else {
			glColor3f (1.0, 0.0, 0.0);
		}

		// draw a rectangle offset from the primary rect
		//gl::drawSolidCircle( Vec2f( ((x)*800), ( (1 -y)*600) ),  1.0f );
	
	
	} // edn drawCensus

//////////////////////////////////////////


	/**  */
	// CensusEntry* drawNearestCity(double x, double y, int year) {
	void vincensdCensus::drawNearestCity(double x, double y, int year) {

	// will call search
		
		//Entry* e;
		double qX = x;
		double qY = y;
		double difX, finX ;
		double difY, finY;
		
		// set distance of first point
		double thresholdDistance = 0.1;

		
		// loop thru points
		for (int i = 0; i <= (bucksVec.size()-1); i++) {
				
			double starX = bucksVec[i].x;
			double starY = bucksVec[i].y;

			// find x
			difX = abs(qX - starX);
			finX = pow(difX, 2);
			// find y
			difY = abs(qY - starY);
			finY = pow(difY, 2);


		double distanceTwo = sqrt( finX + finY );
			if  (distanceTwo <= thresholdDistance){
					if (year == 00) {
						glColor3f (0.0, 0.0, 1.0);
					} else {
						glColor3f (1.0, 0.0, 0.0);
					}
				// draw a rectangle offset from the primary rect
				gl::drawSolidCircle( Vec2f( ((starX)*800), ( (1 - starY)*600) ),  2.0f );

					} 
		  } // end for


		
		
	}
	











	///////////////// TREE BASED ATTEMPT ////////////////////////////////////////////
	void vincensdCensus::buildTree(Entry* c, int n){

		root = NULL;
		r = NULL;
		starbucksSize = n;
		bool isXLevel = true;
		int levelCounter = 0;
		
		//don't need to find median, only if sorted
		//int median = n/2; // will give us a whole number
		//need to get middle Entry
		//c = c + median;
		

		// have to create root node with first Entry
		root = insert(c, root, isXLevel);

		// loop through array 'c', start at i=1 because we used '0' for root
		for (int i = 1; i <= (starbucksSize-1); i++ ){ 
			// check level; even = true, odd = false
			//if (i % 2 == 0) // (levelCounter % 2 == 0)
				//isXLevel = true;

			// call insert
			insert (c, r, isXLevel);
					  //levelCounter++;
		     c = c+1; // increase pointer to next Entry object in EntryArr
		}

					
				

	} //end buildTree

	

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

////////////////////////////////////////////////////////////////////////////////////////////////////////


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
		for (int i = 0; i <= starbucksSize; i++) {
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
		


	} // end getNearestNode
	
	////////////////////////////////////////////////////////////////////////////////////////////

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
		 
			return NULL;

	} //end search

