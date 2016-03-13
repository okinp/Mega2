#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetVerticalSync(true);
	calcHomography.addListener(this, &ofApp::calculateHomography);
	setupGui();
	gui.loadFromFile("settings.xml");
	gui.setPosition(680, 20);
	scalingFactor = videoScaling.get();
	ofVec2f camSize = cameraResolution.get();
	camWidth = camSize.x;
	camHeight = camSize.y;
	resizedImage.allocate(camWidth / scalingFactor, camHeight / scalingFactor, OF_IMAGE_COLOR);
	winSize = windowSize.get();
	ofSetWindowShape(winSize.x, winSize.y);

	int w = 1280;
	int h = 720;
	ofSetWindowShape(w, h);
	
	mShader.load("shaderClouds");
	backImage.loadImage("backImage.png");
	backImage.bind(3);
	mPlane = ofPlanePrimitive(w, h, 2, 2);
	mPlane.mapTexCoordsFromTexture(backImage.getTextureReference());
	mPlane.setPosition(w/2, h/2,0);

	debugCameraScaling = 2.f;  // ( 0.5 )
	movingPoint = false;
	homographyReady = false;
	// load the previous homography if it's available
	ofFile previous("homography.yml");
	if (previous.exists()) {
		cv::FileStorage fs(ofToDataPath("homography.yml"), cv::FileStorage::READ);
		fs["homography"] >> homography;
		homographyReady = true;
		ofLogNotice() << "Loaded homography";
		homographyExists = true;
	}



	setupCamera();

	setupHomography();
	faceFinder.setup("haarcascade_frontalface_alt.xml");

	mFboMask.allocate(winSize.x, winSize.y);
	
}

void ofApp::cameraResolutionChanged()
{

}

void ofApp::windowSizeChanged()
{

}

void ofApp::videoScalingChanged()
{

}

void ofApp::setupGui()
{
	int h = 720;
	int w = 1280;

	showDebug = false;
	gui.setup("");
	gui.add(calcHomography.setup("Calculate Homography"));
	gui.add(cameraResolution.set("cameraResolution", ofVec2f(640, 480), ofVec2f(320, 240), ofVec2f(1280, 720)));
	gui.add(windowSize.set("screenSize", ofVec2f(1280, 720), ofVec2f(640, 360), ofVec2f(1920, 1080)));
	gui.add(videoScaling.set("videoScaling", 1, 1, 10));
	gui.add(cameraIndex.set("cameraIndex", 1, 0, 2));
	gui.add(p0.set("p0", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(w,h)));
	gui.add(p1.set("p1", ofVec2f(0, 480), ofVec2f(0, 0), ofVec2f(w,h)));
	gui.add(p2.set("p2", ofVec2f(640, 480), ofVec2f(0, 0), ofVec2f(w,h)));
	gui.add(p3.set("p3", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(w,h)));
	gui.add(pScale.set("CircleScaling", 1, 0, 4));
}

void ofApp::setupCamera()
{
	//we can now get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	for (int i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
			vector<ofVideoFormat> formats = devices[i].formats;
			for (int j = 0; j < formats.size(); j++)
			{
				ofLogNotice() << "Format: " << formats[j].width << " x " << formats[j].height;
			}
		}
		else {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(cameraIndex.get());
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(camWidth, camHeight);
	//videoTexture.allocate(camWidth, camHeight, OF_PIXELS_RGB);
	//ofSetVerticalSync(true);
}

void ofApp::setupHomography()
{
	if (!homographyExists)
	{
		calibrationPoints.push_back(ofVec2f(0, 0));
		calibrationPoints.push_back(ofVec2f(0, camHeight));
		calibrationPoints.push_back(ofVec2f(camWidth, camHeight));
		calibrationPoints.push_back(ofVec2f(camWidth, 0));
	}
	else {
		calibrationPoints.push_back(p0.get());
		calibrationPoints.push_back(p1.get());
		calibrationPoints.push_back(p2.get());
		calibrationPoints.push_back(p3.get());
	}
}
void ofApp::drawHomographyPoints()
{
	ofNoFill();
	ofSetColor(ofColor::red);
	for (int i = 0; i < calibrationPoints.size(); i++) 
	{
		ofDrawCircle(calibrationPoints[i], 10);
		ofDrawCircle(calibrationPoints[i], 1);
	}
	ofDrawLine(calibrationPoints[0], calibrationPoints[1]);
	ofDrawLine(calibrationPoints[1], calibrationPoints[2]);
	ofDrawLine(calibrationPoints[2], calibrationPoints[3]);
	ofDrawLine(calibrationPoints[3], calibrationPoints[0]);
	ofSetColor(ofColor::white);
}

void ofApp::calculateHomography()
{
	ofLogNotice() << "Pressed Calculate Homography";
		vector<cv::Point2f> srcPoints, dstPoints;
		for (int i = 0; i < 4; i++) {
			srcPoints.push_back(cv::Point2f(calibrationPoints[i].x, calibrationPoints[i].y));
		}
		int w = ofGetWindowWidth();
		int h = ofGetWindowHeight();
		dstPoints.push_back(cv::Point2f(0,0));
		dstPoints.push_back(cv::Point2f(0, h));
		dstPoints.push_back(cv::Point2f(w, h));
		dstPoints.push_back(cv::Point2f(w, 0));
		// generate a homography from the two sets of points
		homography  = cv::findHomography(cv::Mat(srcPoints), cv::Mat(dstPoints));
		homographyReady = true;
		ofLogNotice() << "Homography ready";
		//printCvMat(homography);
}
void ofApp::applyHomography()
{
	if (homographyReady)
	{
		std::vector<cv::Point2f> src, dst;
		mFaceRects.clear();
		for (int i = 0; i < faceFinder.blobs.size(); i++)
		{
			mFaceRects.push_back(faceFinder.blobs[i].boundingRect);
			cv::Point2f p(faceFinder.blobs[i].boundingRect.position.x, faceFinder.blobs[i].boundingRect.position.y);
			src.push_back(p);
			dst.push_back(p);
		}
		cv::perspectiveTransform(cv::Mat(src), cv::Mat(dst), homography);
		mBalls.clear();
		for (int i = 0; i < faceFinder.blobs.size(); i++)
		{
			ofRectangle cur = mFaceRects[i];
			cur.position = ofVec2f(dst[i].x, dst[i].y);
			kyra::Ball b(cur.width / pScale.get());
			b.mPosition = cur.position;
			mBalls.push_back(b);
		}
		//ofLogNotice() << "Applied Homography";
		if (saveMatrix) {
			ofLogNotice() << "Saved Homography";
			cv::FileStorage fs(ofToDataPath("homography.yml"), cv::FileStorage::WRITE);
			fs << "homography" << homography;
			saveMatrix = false;
		}
	}
}
void ofApp::updateCamera()
{
	vidGrabber.update();

	if (vidGrabber.isFrameNew()) {
		if (!mCameraStarted)
		{
			mCameraStarted = true;
		}
		findFaces();
	}
}

void ofApp::findFaces()
{
	ofPixels & pixels = vidGrabber.getPixels();
	mCurrentVideoFrame = ofImage(pixels);
	mCurrentVideoFrame.mirror(false, true);
	ofPixels & mirroredPixels = mCurrentVideoFrame.getPixelsRef();
	cv::Mat colorImage = ofxCv::toCv(mirroredPixels);
	cv::Mat grayCameraImage;
	cv::cvtColor(colorImage, grayCameraImage, CV_BGR2GRAY);
	// scale it to half size, as dictated by the calcScale constant
	int scaledWidth = vidGrabber.getWidth() / scalingFactor;
	int scaledHeight = vidGrabber.getHeight() / scalingFactor;
	cv::Mat smallImg(scaledHeight, scaledWidth, CV_8UC1);
	cv::resize(grayCameraImage, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
	// equalize the histogram
	cv::equalizeHist(smallImg, smallImg);
	ofPixels scaled;
	ofxCv::toOf(smallImg, scaled);
	faceFinder.findHaarObjects(scaled);
	//Update Rects
	int numFaces = faceFinder.blobs.size();
	//Scale back face rects
	for (int i = 0; i < numFaces; i++)
	{
		faceFinder.blobs[i].boundingRect.scale(scalingFactor);
		faceFinder.blobs[i].boundingRect.position *= scalingFactor;
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	updateCamera();
	applyHomography();
}
//--------------------------------------------------------------
void ofApp::draw(){
	if (showDebug)
	{
		drawDebugView();
	}
	else {
		//Render circles to FBO
		ofFill();
		mFboMask.begin();
		ofClear(ofColor::black);
		ofSetColor(ofColor::white);
		for (int i = 0; i < mBalls.size(); i++)
		{
			mBalls[i].draw();
		}
		mFboMask.end();
		ofNoFill();
		mFboMask.getTextureReference().bind(4);

		mShader.begin();
		ofClear(ofColor::black);
		mPlane.draw();
		float runningTime = ofGetElapsedTimeMillis();
		mShader.setUniform1f("iGlobalTime", runningTime);
		mShader.setUniform2f("iResolution", ofVec2f(ofGetWindowWidth(), ofGetWindowHeight()));
		mShader.setUniform4f("iMouse", ofVec4f(ofGetMouseX(), ofGetMouseY(), 0, 0));
		mShader.setUniformTexture("iChannel0", backImage.getTextureReference(), 3);
		mShader.setUniformTexture("imageMask", mFboMask.getTextureReference(), 4);
		mShader.end();
	}
}

void ofApp::exit()
{

}

void ofApp::printCvMat(cv::Mat mat)
{
	const float* Mi = mat.ptr<float>(0);
	ofLogNotice() << Mi[0] << "  " << Mi[1] << " " << Mi[2];
	Mi = mat.ptr<float>(1);
	ofLogNotice() << Mi[0] << "  " << Mi[1] << " " << Mi[2];
	Mi = mat.ptr<float>(2);
	ofLogNotice() << Mi[0] << "  " << Mi[1] << " " << Mi[2];
}
void ofApp::drawDebugView()
{
	mCurrentVideoFrame.draw(0, 0);
	drawHomographyPoints();

	//ofVec2f offset(winSize.x - camWidth/debugCameraScaling, 0);
	//videoTexture.draw(20, 20, camWidth / debugCameraScaling, camHeight / debugCameraScaling);
	//videoTexture.draw(offset.x, 0, camWidth/ debugCameraScaling, camHeight/ debugCameraScaling);
	//videoTexture.draw(0, 0, camWidth, camHeight);
	//backImage.draw(640, 360, 640, 360);
	//drawHomographyPoints( offset, debugCameraScaling );
	gui.draw();
}
void ofApp::movePoint(vector<ofVec2f>& points, ofVec2f point)
{
	for (int i = 0; i < points.size(); i++) {
		if (points[i].distance(point) < 20) {
			movingPoint = true;
			curPoint = &points[i];
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'h') {
		showDebug = !showDebug;
	}
	if (key == 's') {
		gui.saveToFile("settings.xml");
		saveMatrix = true;
	}
	if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}
	if (key == 'x')
	{
		screenshot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		screenshot.save("screenshot.png");
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (movingPoint) {
		ofVec2f currentPos(x, y);
		curPoint->set(currentPos);
		calculateHomography();
		p0.set(calibrationPoints[0]);
		p1.set(calibrationPoints[1]);
		p2.set(calibrationPoints[2]);
		p3.set(calibrationPoints[3]);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (showDebug)
	{
		ofVec2f cur(x, y);
		movePoint(calibrationPoints, cur);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	movingPoint = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
