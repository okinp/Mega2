#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxCv.h"
#include "ofxGui.h"

/*----*/
#include "ofxOpenCv.h"
#include "filterPoints.h"

#define PROJECTOR_WIDTH 1920
#define PROJECTOR_HEIGHT 1080
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

#define HIST_SIZE 5 //larger means smoother

#define MAX_CIRCLES 1


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void cameraResolutionChanged();
		void videoScalingChanged();
		void windowSizeChanged();

		ofPlanePrimitive			mPlane;
		ofxPanel					gui;
		ofxButton					calcHomography;
		ofParameter<ofVec2f>		cameraResolution;
		ofParameter<ofVec2f>		p0, p1, p2, p3;
		ofParameter<float>			videoScaling, pScale;
		ofParameter<int>			cameraIndex;
		ofFbo						mFboMask;
		ofVec2f						winSize;
		ofVideoGrabber				vidGrabber;
		ofImage						mCurrentVideoFrame;
		ofPixels					videoFrame;
		ofTexture					videoTexture;
		//
		int							camWidth;
		int							camHeight;
		int							windowWidth;
		int							windowHeight;
		ofxCvHaarFinder				faceFinder;
		float						scalingFactor;
		float						debugCameraScaling;
		bool						showDebug;
		
		ofShader					mShader;
		vector<ofVec2f>				calibrationPoints;
		ofImage						resizedImage;
		vector<ofRectangle>			faceRects;
		vector<ofVec2f>				facePositions;
		bool						movingPoint;
		ofVec2f*					curPoint;
		bool						saveMatrix = false;
		bool						homographyReady;
		cv::Mat						homography;
		ofImage						testImg;
		bool						mCameraStarted = false;

		vector<ofRectangle>			 mFaceRects;
		bool homographyExists = false;
		ofImage						screenshot;

		ofPixels resized;
		ofImage			backImage;

		void			setupCamera();
		bool			updateCamera();

		void			setupGui();
		void			drawDebugView();
		void			movePoint(vector<ofVec2f>& points, ofVec2f point);

		void			setupHomography();
		void			drawHomographyPoints();
		void			calculateHomography();
		void			applyHomography();
		void			printCvMat(cv::Mat mat);

		void			findFaces();
		void			drawRects();
		void			makeCircles();
		void			loadHomography();
		void			saveHomography();
		void			growCircle();

		//ofImage			mBackground;
		float			circleRadius, minRadius, curRadius;
		bool            hasNewCircle;

		ofVec2f circles[MAX_CIRCLES];
		int circlesArraySize;

		filterPoints filtCircles;
};
