/*
10/17/07
Cam.cpp
gplv2, please see gplv2 licence if you plan to use this code, share people!
Hunter Davis
*/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>


//a helper function to convert from color to gray
IplImage* convert_grayscale(IplImage *img){
	IplImage *gray_image,*gray_img0;
	gray_image=cvCloneImage(img);
          //check image is it gray or not if nor convert it to the gray
	if(img->nChannels!=1){
                      //convert original image to gray_scale image
		gray_img0 = cvCreateImage(cvSize( gray_image->width,
					  gray_image->height), 8, 1 );
		cvCvtColor(gray_image, gray_img0, CV_RGB2GRAY );
		gray_image = cvCloneImage( gray_img0 );
	}
	return gray_image;
}

//a helper function to return the average value of a chunk of pixels

int average_pixel_value(IplImage *img, int x, int y, int boxsize)
{
	int total = 0;
	int pixels = boxsize * boxsize;
	for(int xa = x;xa < x+boxsize;xa++)
	{
		for(int ya = y;ya < y+boxsize;ya++)
		{
			total += ((uchar*)(img->imageData + img->widthStep*y))[x];
		}
	}
	return total/pixels;
}



// A Simple Camera Capture Framework
int main(int argc, char* argv[]) {
	
 //let's set the size of our image bin (nxn)
 int imagebinsize;	
 
 if(argc > 1)
 {
	imagebinsize = atoi(argv[1]);	 
 }
 else
 {
	 std::cout << "Error, usage is 'cam <binsize>' i.e. 'cam 2' or 'cam 3'" << std::endl;
	 return 0;
 }

	
  CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
  if( !capture ) {
    fprintf( stderr, "ERROR: capture is NULL \n" );
    getchar();
    return -1;
  }

  //first we should let the camera driver autofocus
  for(int i = 1;i<5;i++)
  {
	// Get one frame
	  IplImage* frame = cvQueryFrame( capture );
	  if( !frame ) {
		  fprintf( stderr, "ERROR: frame is null...\n" );
		  getchar();
		  break;
	  }  
  }

  
  // Create a window in which the captured images will be presented
  cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
  
  // Capture our primary scene in a frame
  IplImage* primarySceneFrame = cvQueryFrame( capture );
  
  //convert our primary image to black and white for processing if needed
  IplImage* grayPrimarySceneFrame = convert_grayscale(primarySceneFrame);
   
  //let's get the size of our image
  CvSize imsize = cvGetSize(grayPrimarySceneFrame);
  int width = imsize.width;
  int height = imsize.height;

  
  // capture images
  while(1)
  {
    // Get one frame
    IplImage* frame = cvQueryFrame( capture );
    if( !frame ) {
      fprintf( stderr, "ERROR: frame is null...\n" );
      getchar();
      break;
    }
    //convert our new image to black and white for processing
    frame = convert_grayscale(frame);


    //Now we do the following
    //1.  Loop through frame with a boxsize box
    //2.  if luminence value in the box is average > maybe 200
    //3.  then primarySceneFrame[] = 255(bright);
    //4.  display the primarysceneframe, not the 'frame'
    for(int x = 0;x<width-imagebinsize;x+=1)
    {
	    for(int y = 0;y<height-imagebinsize;y+=1)
	    {
		    if(average_pixel_value(frame, x, y, imagebinsize) > 250)
		    {
			    ((uchar*)(grayPrimarySceneFrame->imageData + grayPrimarySceneFrame->widthStep*y))[x] = 255;
		    }
	    }
    }
    
    //display
    cvShowImage( "mywindow", grayPrimarySceneFrame );
    //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
    //remove higher bits using AND operator
    if( (cvWaitKey(10) & 255) == 27 ) break;
  }
    
  // Release the capture device housekeeping
  cvReleaseCapture( &capture );
  cvDestroyWindow( "mywindow" );
  return 0;
}
