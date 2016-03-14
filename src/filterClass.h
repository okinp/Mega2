#ifndef FILTERCLASS_H
#define FILTERCLASS_H

#include "ofMain.h"

#define DEFAULT_HISTORY_SIZE    30

//template <class OFObj>
class filterClass
{
    public:
        filterClass( const unsigned int histSize = DEFAULT_HISTORY_SIZE );
        ~filterClass();
        void initFilter(const unsigned int histSize = DEFAULT_HISTORY_SIZE );
        void freeFilter();

        void addElement( const float value );
        float getAverage();

    protected:
    private:
        float updTotal( const int newHistSize , unsigned int erasePtr , const float tot );

        vector<float> historyBuf;
        int currentPtr;
        bool hBufFull;

        float total;
};

#endif // FILTERCLASS_H
