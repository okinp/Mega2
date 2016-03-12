#include "Ball.h"
#include "ofMath.h"
#include "ofGraphics.h"

namespace kyra {
	Ball::Ball(int radius )
		:mRadius(radius)
	{
		setup();
	}
	Ball::Ball()
		:mRadius(10)
	{
		setup();
	}

	Ball::~Ball()
	{
	}

	void Ball::setup()
	{
		mVelocity = ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1));
		mPosition = ofVec2f(ofRandom(0, WINDOW_WIDTH), ofRandom(0, WINDOW_HEIGHT));
	}

	void Ball::update() {
		mPosition += mVelocity;
		if (mPosition.x > WINDOW_WIDTH || mPosition.x <= 0)
		{
			mVelocity *= -1;
		}
		else if ( mPosition.y > WINDOW_HEIGHT || mPosition.y <= 0 )
		{
			mVelocity *= -1;
		}
	}
	void Ball::draw() {
		ofDrawCircle(mPosition.x, mPosition.y, mRadius);
	}
}


