#ifndef FILTERPOINTS_H
#define FILTERPOINTS_H

#include "filterClass.h"

class filterPoints
{
    public:
        filterPoints(const int histSize = DEFAULT_HISTORY_SIZE);
        ~filterPoints();
        void initFilter(const int histSize = DEFAULT_HISTORY_SIZE );
        void freeFilter();

        void addElement( const ofPoint pt );
        void addElement( const float x , const float y , const float z = 0);
        ofPoint getAverage();

    protected:
    private:
        filterClass Xvalues , Yvalues , Zvalues ;
};

#endif // FILTERPOINTS_H
