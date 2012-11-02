/*****
 * File: hw04_vincensd_ph3App.cpp	
 * Author   : vincensd
 * Date     : 2012-28-10
 * Purpose  :  This program will perform several duties required for HW04.
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

	//Track how many frames we have shown, for animation purposes
	int frame_number_;
	boost::posix_time::ptime app_start_time_;


	//Width and height of the screen
	static const int AppWidth=800;
	static const int AppHeight=600;
	static const int TextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions
	
		

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
	textureMap = loadImage( "../resources/map1.jpg" );


	// IMPORT DATA //

	// IMPORT Starbucks //
	/** setup vars */
	std::vector<Entry> entryVec;
	string line;

	/** Read .csv file into a vector */
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
		/** or ...*/
		for (int i = 0; i < (entryVec.size()-1); i++ ){
			entryArr[i] = entryVec.at(i);
		}
		
		/** test output in Autos window 
		cout << "output = " << &entryArr[0] << endl;
		cout << "output = " << &entryArr[1] << endl;
		cout << "output = " << &entryArr[7653] << endl;
		cout << "output = " << &entryArr[7654] << endl;

	// BUILD Starbucks DS 
		// call 	
		vincensdCensus starObject;
		starObject.build( entryArr, entrySize ); 	
	// Delete temp Array
		delete [] entryArr;	
	// call getNearest
		starObject.getNearest( 0.213630099, 0.44826200 );  // flagstaff, az
	//display returned nearest object
		cout << "Closets neighbor is: " << starObject.closestBucks -> identifier << endl;


////////////////////////////////////////////////////////////////////////////////

	// IMPORT Census 2000 //
	// BUILD Census 2010 DS 
	// Delete temp Array
	
	/**
	x = (longitude - 24)/(49-24)
 
	y = (latitude - (-125))/((-63) - (-125))
	*/

	// IMPORT Census 2010 //
	// BUILD Census 2010 DS 
	// Delete temp Array

}

void HW04_vincensd_ph3App::mouseDown( MouseEvent event )
{
}

void HW04_vincensd_ph3App::update()
{
}

void HW04_vincensd_ph3App::draw()
{
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) ); 

	//Draw our texture to the screen, using graphics library

	gl::draw(*mySurface_);
	gl::draw( textureMap );
}

CINDER_APP_BASIC( HW04_vincensd_ph3App, RendererGl )
