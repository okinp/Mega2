#pragma once
#include "ofVectorMath.h"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

namespace kyra {
	class Ball
	{
	public:
		Ball();
		Ball(int radius);
		~Ball();
		void setup();
		void update();
		void draw();
	//private:
		ofVec2f mPosition;
		ofVec2f mVelocity;
		ofVec2f mAcceleration;
		int mRadius;
	};
}
