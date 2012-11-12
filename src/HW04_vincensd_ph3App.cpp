/*****
 * File		: hw04_vincensd_ph3App.cpp	
 * Author   : Scott Vincent
 * Date     : 2012-28-10
 * Purpose  : This program will perform several duties required for HW04.
 * Sources  : 
 http://www.creativeapplications.net/tutorials/images-in-cinder-tutorials-cinder/
 http://redpaperheart.com/blog/2012/08/handy-snippets-for-drawing-image-textures-in-cinder/



 * Fulfills * 
 (a) Draw map of the US [20];
 (b) Highlight Nearest Starbucks to mouse click [10],
 (c) Color by region (16 regions) [30];	
 (e) Draw change from 2000 to 2010;  +1000 population:green, -1000 populations:red 
	  Any change within 1000+-: white[30],
 (g) Video [10]: http://www.screenr.com/A677
 
  Total: 100
 
 Notes: The drawStarbucks and drawPopCensus are offset so you can see the difference in 
  

 * @note This file is (c) 2012. It is licensed under the 
 * CC BY 3.0 license (http://creativecommons.org/licenses/by/3.0/),
 * which means you are free to use, share, and remix it as long as you
 * give attribution. Commercial uses are allowed.
 *
 */


//cinder includes
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "cinder/Text.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/Text.h"

//my includes
#include "vincensdCensus.h"

//c++ includes
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

//using
using namespace ci;
using namespace ci::app;
using namespace std;



class HW04_vincensd_ph3App : public AppBasic {
  public:
	void setup();	
	void update();
	void draw();

	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void prepareSettings(Settings* settings);
	void drawMouseClick( int x, int y);
	
  private:
	Surface* mySurface_; //The Surface object whose pixel array we will modify
	//Surface map_pic;
	gl::Texture textureMap;
	//Surface8u regularSurface; // an empty 8 bit surface
   // Surface32f hdrSurface; // an empty 32 bit high dynamic range surface

	vincensdCensus starObject;
	vincensdCensus census00_Object;
	vincensdCensus census10_Object;

	//Track how many frames we have shown, for animation purposes
	int frame_number_;
	boost::posix_time::ptime app_start_time_;

	// declare the menu parameters
	bool hideMenu;  //When true, removes the instructions
	Font* font;		//Part of the cinder drawString method 
	string identLength;


	//Width and height of the screen
	static const int AppWidth=800;
	static const int AppHeight=600;
	static const int TextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions
	
	// try other sizes
	//static const int AppWidth = 1024;
	//static const int AppHeight = 640;
	//static const int SurfaceSize = 1024;

	// mouse - draw stuff
	int xMouseClick;
	int yMouseClick;
	int mouseClicks;
		
	double xMult, yMult;

	bool isStarBucks, isCensus2000, isCensus2010;

};

// Standard prepare settings

void HW04_vincensd_ph3App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(AppWidth,AppHeight);
	(*settings).setResizable(false);
}



void HW04_vincensd_ph3App::setup()
{
	// LOAD IMAGE
	
	//This is the setup that everyone needs to do
	//mySurface_ = new Surface( TextureSize, TextureSize, true, SurfaceChannelOrder::RGBA ); // width, height, alpha?, channel order
	mySurface_ = new Surface( TextureSize, TextureSize, true ); // width, height, alpha?, channel order
	//regularSurface = ( loadImage( "../resources/map1.jpg" ) );
	textureMap = loadImage( "../resources/map1.jpg" );

	xMult = 800.0;
	yMult = 600.0;
	mouseClicks = 0;

	//xMult = 1024.0;
	//yMult  = 640.0;
	
	//	Setup the text menu 
	font = new Font("Arial",14);
	hideMenu = true;

	// Setup who gets calls
	isStarBucks = true;
	isCensus2000 = false;
	isCensus2010 = false;


	////////////////////////// IMPORT DATA ////////////////////////////////////////////////////////

	/////////////////////////// IMPORT Starbucks /////////////////////////////////////////////////
	/** setup vars */
	
	

	std::vector<Entry> entryVec;
	string line;

	//** Read .csv file into a vector /
	//Open file
	ifstream infile ("..\\resources\\Starbucks_2006.csv");			// pg589 create an an input stream
	
	if (infile.fail()){ 
		cout << "Error opening file" << endl;						// %s\n", "Starbucks_2006.csv"); 
	}

	//Fill the vector with entry data
	  do { //while ( !infile.eof() )
		     
		Entry e;		// for each iteration create a new entry variable to hold the input values, don't need a constructed object
		//Entry* e = new Entry();		// Mikes code: would cause a memory leak as we are not destroying the object at end 
		

				 getline(infile, line, ',');
				 e.identifier = line;

				 infile.get();
				 infile >> e.x;

				 infile.get();
				 infile >> e.y;

				 e.pop2000=0;
				 e.pop2010=0;

				 entryVec.push_back(e);	// split the string and add pieces onto back of vector
	}
		while ( !infile.eof() );

	// transform Vector into array
		 const int entrySize = entryVec.size();					// size of new array
		 Entry *entryArr = new Entry [ entryVec.size() ];		// have to create a dynamic array of type Entry
																// http://bytes.com/topic/c/answers/849132-std-vector-c-array
	 // copy vector into array: two approaches
		// std::copy(entryVec.begin(), entryVec.end(), entryArr); 
		//** or ...
		for (int i = 0; i < (entryVec.size()-1); i++ ){
			entryArr[i] = entryVec.at(i);
		}
		
	
		/** test output in Autos window 	 
		cout << "output = " << &entryArr[0] << endl;
		cout << "output = " << &entryArr[1] << endl;
		cout << "output = " << &entryArr[7653] << endl;
		cout << "output = " << &entryArr[7654] << endl;*/


	// BUILD Starbucks DS 
		// call 	
		//vincensdCensus starObject;
		starObject.build( entryArr, entrySize ); 	
	
	// Delete temp Array
		entryVec.clear();
		delete [] entryArr;	
	
	// call getNearest
		//starObject.getNearest( 0.213630099, 0.44826200 );  // flagstaff, az
	//display returned nearest object
		//cout << "Closets neighbor is: " << starObject.closestBucks -> identifier << endl;

		
/////////////////////// Census 2000 /////////////////////////////////////////////////////////////////

	// IMPORT Census 2000 //
	std::vector<CensusEntry> censusVec;
	double latitude, longitude;
	string num, population;

	/** Read .csv file into a vector */
	//Open file
	ifstream infile2 ("..\\resources\\Census_2000.csv");			    // pg589 create an an input stream
	
	if (infile2.fail()){ 
		cout << "Error opening file" << endl;						// %s\n", "Starbucks_2006.csv"); 
	}

	//Fill the vector with entry data
	  do { //while ( !infile.eof() )
		     
		CensusEntry e;		// for each iteration create a new entry variable to hold the input values, don't need a constructed object
		//Entry* e = new Entry();		// Mikes code: would cause a memory leak as we are not destroying the object at end 
		
				// take care of first 6 columns
				for (int i =0; i <=3; i++){
					getline(infile2, num, ',');
					//infile2.get();		// gets a single char
				}

				infile2 >> e.population;	// put the population directly into the object
				// getline(infile, population, ',');
				//e.population = population;

				 infile2.get();					// gets a single char, the comma
				 infile2 >> latitude;
				 e.y = (latitude);
				 //e.y = (latitude - (-125))/((-63) - (-125)); //convert to the proper y coordinate

				 infile2.get();					 // gets a single char,  the comma
				 infile2 >> longitude;
				 e.x = (longitude);
				// e.x = (longitude - 24)/(49-24); //convert to the proper x coordinate


				 censusVec.push_back(e);	// add objects onto back of vector
	}
		while ( !infile2.eof() );

	// transform Vector into array
		 const int Census2000Size = censusVec.size();					// size of new array
		 CensusEntry *entryArr2 = new CensusEntry [ censusVec.size() ];		// have to create a dynamic array of type Entry
																// http://bytes.com/topic/c/answers/849132-std-vector-c-array
	 // copy vector into array: two approaches
		 std::copy(censusVec.begin(), censusVec.end(), entryArr2); 
		/** or ...
		for (int i = 0; i < (entryVec.size()-1); i++ ){
			entryArr2[i] = censusVec.at(i);
		}*/
		
		
		/** test output in Autos window  
		cout << "output = " << &entryArr2[0] << endl;
		cout << "output = " << &entryArr2[1] << endl;
		cout << "output = " << &entryArr2[7653] << endl;
		cout << "output = " << &entryArr2[7654] << endl;*/

	// BUILD Census 2010 DS
		census00_Object.buildCensus( entryArr2, Census2000Size );


	// Delete temp Arrays
		censusVec.clear();
		delete [] entryArr2;	
	
	

////////////////////// Census 2010 ///////////////////////////////////////////////////////////

	// IMPORT Census 2100 //
	std::vector<CensusEntry> censusVec2;
	double latitude2, longitude2;
	string num2, population2;

	/** Read .csv file into a vector */
	//Open file
	ifstream infile3 ("..\\resources\\Census_2010.csv");			    // pg589 create an an input stream
	
	if (infile3.fail()){ 
		cout << "Error opening file" << endl;						   // %s\n", "Starbucks_2006.csv"); 
	}

	//Fill the vector with entry data
	  do { //while ( !infile.eof() )
		     
		CensusEntry f;		// for each iteration create a new entry variable to hold the input values, don't need a constructed object
		//Entry* e = new Entry();		// Mikes code: would cause a memory leak as we are not destroying the object at end 
		
				// take care of first 6 columns
				for (int i =0; i <=3; i++){
					getline(infile3, num2, ',');
					//infile2.get();		// gets a single char
				}

				infile3 >> f.population;	// put the population directly into the object
				// getline(infile, population, ',');
				//e.population = population;

				 infile3.get();					// gets a single char, the comma
				 infile3 >> latitude2;
				 f.y = (latitude2);
				 //f.y = (latitude2 - (-125))/((-63) - (-125)); //convert to the proper y coordinate
				
				 infile3.get();					 // gets a single char,  the comma
				 infile3 >> longitude2;
				 f.x = (longitude2);
				 //f.x = (longitude2 - 24)/(49-24); //convert to the proper x coordinate
			
				 censusVec2.push_back(f);	// add objects onto back of vector
	}
		while ( !infile3.eof() );

	// transform Vector into array
		 const int Census2010Size = censusVec2.size();					// size of new array 
		 CensusEntry *entryArr3 = new CensusEntry [ censusVec2.size() ];		// have to create a dynamic array of type Entry
																// http://bytes.com/topic/c/answers/849132-std-vector-c-array
	 // copy vector into array: two approaches
		 std::copy(censusVec2.begin(), censusVec2.end(), entryArr3); 
		/** or ...
		for (int i = 0; i < (entryVec.size()-1); i++ ){
			entryArr2[i] = censusVec.at(i);
		}*/
		
		
		/** test output in Autos window  
		cout << "output = " << &entryArr3[0] << endl;
		cout << "output = " << &entryArr3[1] << endl;
		cout << "output = " << &entryArr3[7653] << endl;
		cout << "output = " << &entryArr3[7654] << endl; */

	// BUILD Census 2010 DS
		census10_Object.buildCensus( entryArr3, Census2010Size );


	// Delete temp Arrays
		censusVec2.clear();
		delete [] entryArr3;	


	// clear out the window with black
	//gl::clear( Color( 1.0f, 1.0f, 1.0f ) );
	//gl::draw( textureMap ); 

} // end setup

///////////////////////////////////////////////////////////

void HW04_vincensd_ph3App::drawMouseClick( int x, int y) {
		glColor3f (1.0, 1.0, 0.0);
		// draw a rectangle offset from the primary rect
		gl::drawSolidCircle( Vec2f( ((x)), (y) ),  2.0f );
	}


void HW04_vincensd_ph3App::mouseDown( MouseEvent event )
{
//Satisfies ... at the mouse click
//uint8_t* dataArray = (*mySurface_).getData();
	

	// save coordinates and will update when Draw is called
	 xMouseClick = event.getX();
	 yMouseClick = event.getY();
	 //starObject.drawMouseClick (xMouseClick, yMouseClick); // just test
	// drawMouseClick (xMouseClick, yMouseClick); // just test


	 // implementation: send (xMouseClick, yMouseClick) coordinates to getNearest,
	 //return the object and call starObject.drawNearest(entry->x, entry->y);
	 //to show nearest bucks.
	 // in drawNearest make the color green and circle 3f radius.

	
	 // transform y coordinate into Starbucks compatibile
	 double mouseX = xMouseClick/xMult;
	 // transform y coordinate into Starbucks compatibile
	 double mouseY = 1-(yMouseClick/yMult);

	 //identLength = starObject.closestBucks -> identifier;
	 // clear old text
	 // if (mouseClicks != 0){
	//	 gl::drawString(starObject.closestBucks -> identifier, Vec2f(20.0f,575.0f), Color(0.0f,0.0f,0.0f), *font);	
	//	 }

	  // call getNearest
	 starObject.getNearest ( (mouseX), (mouseY) ); 

	//display returned nearest object
	// gl::draw(textureMap);
	 glColor3f (1.0, 1.0, 0.0);
	 //transform returned coordinates into proper values for screen
	 gl::drawSolidCircle( Vec2f( ((starObject.closestBucks -> x * xMult)), ( (1-(starObject.closestBucks -> y)) * yMult) ),  2.0f );

	// glColor3f (0.0, 0.0, 0.0);
		
	 gl::drawString(starObject.closestBucks -> identifier, Vec2f(20.0f,575.0f), Color(1.0f,0.0f,0.0f), *font);	
	 mouseClicks +=1;
	//  gl::drawString(starObject.closestBucks -> identifier, Vec2f(starObject.closestBucks ->x *800, (1-(starObject.closestBucks ->y)) * yMult),Color(1.0f,0.0f,0.0f), *font);
	 // 

			
 }


void  HW04_vincensd_ph3App::keyDown( KeyEvent event ) {
	
	//CensusEntry censusObject;
	int year;
	Color8u colorIn;
	int popDifference;

    if( event.getChar() == 'q' ){
		 //call census_2000: 		year = 2000;
		//censusObject = &census00_Object;
		//census00_Object.drawCensus(&census00_Object, year);

		 for(int i = 0; i <=( ((census00_Object.censusVec.size() -1) ) ); i++){ // (census00_Object.censusVec.size() -1)
			// call getNearest
			starObject.getNearest( census00_Object.censusVec.at(i).x, census00_Object.censusVec.at(i).y );
			//add population to returned object returned nearest object
				starObject.closestBucks -> pop2000 += census00_Object.censusVec.at(i).population; 
			 
			//census00_Object.drawNearestCity(starObject, census00_Object.censusVec.at(i).x, census00_Object.censusVec.at(i).y, year);	 	 
		} // end for


	//} else if( event.getChar() == 'e' ){
		 //call census_2010: 	year = 2010;
	
		for(int i = 0; i <= ( (census10_Object.censusVec.size() -1 ) ); i++){  // (census10_Object.censusVec.size() -1 )
			// call getNearest
				starObject.getNearest( census10_Object.censusVec.at(i).x, census10_Object.censusVec.at(i).y );
			//add population to returned object returned nearest object
				starObject.closestBucks -> pop2010 += census10_Object.censusVec.at(i).population; 
			
			//census10_Object.drawNearestCity(starObject, census10_Object.censusVec.at(i).y, census10_Object.censusVec.at(i).x, year);	 	 
		}

		 for(int i = 0; i <=(starObject.starbucksSize-1) ; i++){ // (starObject.starbucksSize-1)
			
			// if (abs((starObject.bucksVec.at(i).pop2000 - starObject.bucksVec.at(i).pop2010) < 1000)) {
			 
			  popDifference = (((starObject.bucksVec.at(i).pop2000 - starObject.bucksVec.at(i).pop2010)));
			  cout << popDifference << endl;

			 if (popDifference < -1000) {
				 colorIn = Color8u(255,0,0);
			 } else if (popDifference >= 1000) {
				colorIn = Color8u(0,255,0);
			 } else {
				 colorIn = Color8u(255,255,255);
			 } // end if
			 starObject.drawPopCensus(starObject.bucksVec.at(i).x, starObject.bucksVec.at(i).y, colorIn );	 	 
	} // end for


	
	} else if ((event.getChar() == '/') && hideMenu == false) {
			hideMenu = true;
			//gl::color(1,1,1);
	
	} else if ((event.getChar() == '/') && hideMenu == true) {
			hideMenu = false;

	}// end main if
	

} // end keyDown


////////////////////////////////////////////////////////////////////////////

void HW04_vincensd_ph3App::update()
{
}

void HW04_vincensd_ph3App::draw(){
	


	if(hideMenu)                         // draw menu 
	{	
		// libcinder.org/docs/v0.8.2/namespacecinder_1_1gl.html#a8715d619df092110ac326e7a4ab08098
		gl::drawString("Menu Operations: q = Show Population Difference.", Vec2f(20.0f,525.0f),Color(1.0f,0.5f,0.0f), *font);		
		//gl::drawString("Press ? to toggle menu.", Vec2f(20.0f,550.0f),Color(1.0f,0.5f,0.0f),*font);	
	    // } else	{	
		// gl::clear(Color( 1, 1, 1 )); //Clear out text and makes screen  
	}
		
 

	if (isStarBucks){
	
	//gl::clear(Color( 0, 0, 0 ));
	//gl::enableAlphaBlending();
	
	//Draw our texture to the screen, using graphics library
	//gl::draw(*mySurface_);
	 //gl::draw(regularSurface);

	// DRAW ORDER: 1st: so that it's always on bottom
	//gl::draw( textureMap );  
   
	// DRAW ORDER: 2nd: loop thru SB points to be ontop of map
	/** draw colors/ rectangles like I did in proj 2 */
	//randomze color, rndColor
	//Color8u(rand()%256,rand()%256,rand()%256), 3);
	 //gl::color (200,200,200); 
	 //glColor3f (1.0, 0.0, 0.0);

///////////////////////////////////////////////////////////
	 /** loop thru and draw starObject points */

	 for(int i = 0; i <=(starObject.starbucksSize-1) ; i++){ // (starObject.starbucksSize-1)
	//	
		 starObject.drawStarbucks(starObject.bucksVec.at(i).x, starObject.bucksVec.at(i).y);	 	 
	 }
	

} // end isStarBucks
/////////////////////////////////////////////////////////////////////////
	
	
	

	if (isCensus2000){
	
	for(int i = 0; i <=(census00_Object.CensusSize-1) ; i++){ 
		
		 census00_Object.drawCensus(census00_Object.censusVec.at(i).x, census00_Object.censusVec.at(i).y);	 	 
		 }
	} // end isCensus2000


	if (isCensus2010){
	  for(int i = 0; i <=(census10_Object.CensusSize-1) ; i++){ 
		
		 census10_Object.drawCensus(census10_Object.censusVec.at(i).x, census10_Object.censusVec.at(i).y);	 	 
		}
	} // end isCensus2010
	
	
	

} //end Draw()

CINDER_APP_BASIC( HW04_vincensd_ph3App, RendererGl )
