#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Ball.h"

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

		ofPlanePrimitive		mPlane;
		ofxPanel				gui;
		ofxButton				calcHomography;
		ofParameter<ofVec2f>	cameraResolution;
		ofParameter<ofVec2f>	windowSize;
		ofParameter<ofVec2f>	p0, p1, p2, p3;
		ofParameter<float>		videoScaling, pScale;
		ofParameter<int>		cameraIndex;
		ofFbo					mFboMask;
		ofVec2f winSize;

		void cameraResolutionChanged();
		void videoScalingChanged();
		void windowSizeChanged();

		ofVideoGrabber	vidGrabber;
		ofImage mCurrentVideoFrame;

		ofPixels		videoFrame;
		ofTexture		videoTexture;
		int				camWidth;
		int				camHeight;
		int				windowWidth;
		int				windowHeight;
		ofxCvHaarFinder faceFinder;
		float			scalingFactor;
		float			debugCameraScaling;
		bool			showDebug;
		void			setupCamera();
		void			setupGui();
		void			updateCamera();
		void			drawDebugView();
		void			setupHomography();
		void			drawHomographyPoints();
		void			calculateHomography();
		void			applyHomography();
		void			printCvMat(cv::Mat mat);
		void			movePoint(vector<ofVec2f>& points, ofVec2f point);
		ofImage			backImage;
		void			findFaces();
		ofShader		mShader;
		vector<ofVec2f>		calibrationPoints;
		ofImage resizedImage;
		vector<ofRectangle>	faceRects;
		vector<ofVec2f>		facePositions;
		bool			movingPoint;
		ofVec2f*		curPoint;
		bool saveMatrix = false;
		bool homographyReady;
		cv::Mat homography;
		ofImage testImg;
		bool mCameraStarted = false;
		vector<kyra::Ball> mBalls;
		vector<ofRectangle> mFaceRects;
		bool homographyExists = false;
};
