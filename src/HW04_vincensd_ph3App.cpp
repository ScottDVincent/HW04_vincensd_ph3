/*****
 * File: hw04_vincensd_ph3App.cpp	
 * Author   : vincensd
 * Date     : 2012-28-10
 * Purpose  :  This program will perform several duties required for HW04.
 * Sources  : 
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
	void keyDown( KeyEvent event );

	void prepareSettings(Settings* settings);
	
  private:
	Surface* mySurface_; //The Surface object whose pixel array we will modify
	Surface map_pic;

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
		//This is the setup that everyone needs to do
	mySurface_ = new Surface(TextureSize,TextureSize,false);

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
}

CINDER_APP_BASIC( HW04_vincensd_ph3App, RendererGl )
