#include "filterPoints.h"

filterPoints::filterPoints( const int histSize )
{
    //ctor
    initFilter(histSize);
}

filterPoints::~filterPoints()
{
    //dtor
    freeFilter();
}

void filterPoints::initFilter(const int histSize ){
    Xvalues.initFilter(histSize);
    Yvalues.initFilter(histSize);
}

void filterPoints::freeFilter(){
    Xvalues.freeFilter();
    Yvalues.freeFilter();
}

void filterPoints::addElement( const float x , const float y , const float z ){
    ofPoint pt(x,y,z);
    addElement(pt);
}

void filterPoints::addElement( const ofPoint pt ){
    Xvalues.addElement(pt.x);
    Yvalues.addElement(pt.y);
    Zvalues.addElement(pt.z);
}

ofPoint filterPoints::getAverage(){
    ofPoint averagedPoint;

    averagedPoint.x = Xvalues.getAverage();
    averagedPoint.y = Yvalues.getAverage();
    averagedPoint.z = Zvalues.getAverage();

    return averagedPoint;
}
