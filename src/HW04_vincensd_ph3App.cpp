/*****
 * File		: hw04_vincensd_ph3App.cpp	
 * Author   : vincensd
 * Date     : 2012-28-10
 * Purpose  : This program will perform several duties required for HW04.
 * Sources  : 
 http://www.creativeapplications.net/tutorials/images-in-cinder-tutorials-cinder/


 * Fulfills : (a) Draw map of the US

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


using namespace ci;
using namespace ci::app;
using namespace std;



class HW04_vincensd_ph3App : public AppBasic {
  public:
	void setup();	
	void update();
	void draw();

	void mouseDown( MouseEvent event );
	//void keyDown( KeyEvent event );

	void prepareSettings(Settings* settings);
	
  private:
	Surface* mySurface_; //The Surface object whose pixel array we will modify
	Surface map_pic;
	gl::Texture textureMap;
	Surface8u regularSurface; // an empty 8 bit surface
    Surface32f hdrSurface; // an empty 32 bit high dynamic range surface

	vincensdCensus starObject;
	vincensdCensus census00_Object;
	vincensdCensus census10_Object;

	//Track how many frames we have shown, for animation purposes
	int frame_number_;
	boost::posix_time::ptime app_start_time_;


	//Width and height of the screen
	static const int AppWidth=800;
	static const int AppHeight=600;
	static const int TextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions
	

	// mouse - draw stuff
	int xMouseClick;
	int yMouseClick;
		

};

void HW04_vincensd_ph3App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(AppWidth,AppHeight);
	(*settings).setResizable(false);
}



void HW04_vincensd_ph3App::setup()
{
	// LOAD IMAGE
	//This is the setup that everyone needs to do
	//mySurface_ = new Surface(TextureSize,TextureSize,false);
	mySurface_ = new Surface( TextureSize, TextureSize, true, SurfaceChannelOrder::RGBA ); // width, height, alpha?, channel order
	textureMap = loadImage( "../resources/map1_1024.jpg" );


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
		
	
		/** test output in Autos window */	 
		cout << "output = " << &entryArr[0] << endl;
		cout << "output = " << &entryArr[1] << endl;
		cout << "output = " << &entryArr[7653] << endl;
		cout << "output = " << &entryArr[7654] << endl;


	// BUILD Starbucks DS 
		// call 	
		//vincensdCensus starObject;
		starObject.build( entryArr, entrySize ); 	
	
	// Delete temp Array
		entryVec.clear();
		delete [] entryArr;	
	
	// call getNearest
		starObject.getNearest( 0.213630099, 0.44826200 );  // flagstaff, az
	//display returned nearest object
		cout << "Closets neighbor is: " << starObject.closestBucks -> identifier << endl;

		
/////////////////////// Census 200 /////////////////////////////////////////////////////////////////

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
				 e.y = (latitude - (-125))/((-63) - (-125)); //convert to the proper y coordinate

				 infile2.get();					 // gets a single char,  the comma
				 infile2 >> longitude;
				 e.x = (longitude - 24)/(49-24); //convert to the proper x coordinate


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
		
		
		/** test output in Autos window  */
		cout << "output = " << &entryArr2[0] << endl;
		cout << "output = " << &entryArr2[1] << endl;
		cout << "output = " << &entryArr2[7653] << endl;
		cout << "output = " << &entryArr2[7654] << endl;

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
		cout << "Error opening file" << endl;						// %s\n", "Starbucks_2006.csv"); 
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
				 f.y = (latitude2 - (-125))/((-63) - (-125)); //convert to the proper y coordinate
				
				 infile3.get();					 // gets a single char,  the comma
				 infile3 >> longitude2;
				 f.x = (longitude2 - 24)/(49-24); //convert to the proper x coordinate
			
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
		
		
		/** test output in Autos window  */
		cout << "output = " << &entryArr3[0] << endl;
		cout << "output = " << &entryArr3[1] << endl;
		cout << "output = " << &entryArr3[7653] << endl;
		cout << "output = " << &entryArr3[7654] << endl;

	// BUILD Census 2010 DS
		census10_Object.buildCensus( entryArr3, Census2010Size );


	// Delete temp Arrays
		censusVec2.clear();
		delete [] entryArr3;	

	
}




void HW04_vincensd_ph3App::mouseDown( MouseEvent event )
{
//Satisfies ... at the mouse click
//uint8_t* dataArray = (*mySurface_).getData();

// save coordinates and will update when Draw is called
 xMouseClick = event.getX();
 yMouseClick = event.getY();
 
 }



void HW04_vincensd_ph3App::update()
{
}

void HW04_vincensd_ph3App::draw()
{
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) ); 

	//Draw our texture to the screen, using graphics library

	//gl::draw(*mySurface_);

	// DRAW ORDER: 1st: so that it's always on bottom
	gl::draw( textureMap );  


	// DRAW ORDER: 2nd: loop thru SB points to be ontop of map
	/** draw colors/ rectangles like I did in proj 2 */
	//randomze color, rndColor
		//Color8u(rand()%256,rand()%256,rand()%256), 3);
	 //gl::color (200,200,200); 
	// glColor3f (1.0, 0.0, 0.0);

	 /** loop thru and draw starObject points */

	 for(int i = 0; i <=5 ; i++){ //starObject.starbucksSize
		glColor3f (1.0, 1.0, 0.0);

		//http://libcinder.org/docs/dev/namespacecinder_1_1gl.html#a225b1296ceeaa8b2393339fc39debf7a
	
		gl::drawSolidCircle( Vec2f( ((starObject.bucksVec.at(i).x)*800), ( (1 - starObject.bucksVec.at(i).y)*600) ), 2.0f );
		 // for census stuff
	//	 gl::drawSolidCircle( Vec2f( ((census10_Object.censusVec.at(i).x)*200), ( (1- census10_Object.censusVec.at(i).y)*150) ), 2.0f );
	 }

	 gl::drawSolidCircle( Vec2f( 15.0f, 25.0f ), 50.0f );

	 //gl::drawSolidCircle( Vec2f( 15.0f, 25.0f ), 50.0f, 7 ); 
	// gl::drawSolidRect(Rectf (.5*800, .5*600, .6*800, .6*600) );

	/**
		int shadowOffset = 3;
		int shake = rand()%shakeFactor_;
		shadowOffset += shake;


		// turn on alpha blending
		//http://libcinder.org/docs/v0.8.2/namespacecinder_1_1gl.html#a2cb8982a5a007376031745ac074bed4c
		gl::enableAlphaBlending();
		//activate the alpha channel
		gl::color(ColorA(0.0f,0.0f,0.0f,0.25f));
		// draw a rectangle offset from the primary rect
		//gl::drawSolidRect(Rectf (x1_+shadowOffset, y1_+shadowOffset, x2_+shadowOffset, y2_+shadowOffset), 6.0f);
		//turn off alpha
		gl::disableAlphaBlending();	
		// set the color of the list rectangle
		gl::color(inColor_);	
		// draw list rectangle
		gl::drawSolidRect(Rectf (x1_+ shake, y1_+ shake, x2_+shake, y2_+shake) );




	*/
	

}

CINDER_APP_BASIC( HW04_vincensd_ph3App, RendererGl )
