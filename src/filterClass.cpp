#include "filterClass.h"

filterClass::filterClass( const unsigned int histSize )
{
    //ctor
    initFilter(histSize);
    currentPtr = 0 ;

}

filterClass::~filterClass()
{
    //dtor
    freeFilter();
}

void filterClass::initFilter(const unsigned int newHistSize){

    //if new size larger, keep total, keep current pointer, but falsify hBufFull
    //if new size smaller, remove (historyBuf.size() - newHistSize) entries from total
    //  and update total by removing the oldest entries from curPtr and increment
    //  then if curPtr is >  ( newHistSize), set to 0, else leave as is
    freeFilter();
    historyBuf.assign( newHistSize , 0 );
    /*if ( historyBuf.size() != newHistSize ){
        if ( historyBuf.size() < newHistSize ){
            hBufFull = false ;
            freeFilter();
        } else if ( historyBuf.size() > newHistSize ){

            total = updTotal( newHistSize , currentPtr , total ); //remove oldest entries from total

            //update currentPtr if outside new boundary
            if ( currentPtr >= newHistSize){
                currentPtr = 0 ;
            }
            freeFilter();

        }

        //finally resize the history buffer
        if ( !historyBuf.empty() ){
            historyBuf.resize( newHistSize );
        } else {
            historyBuf.assign( newHistSize , 0 );
        }
    }*/

}

void filterClass::freeFilter(){

    if ( !historyBuf.empty() )
        historyBuf.clear();

    hBufFull = false ;
    currentPtr = 0 ;
    total = 0 ;
}


float filterClass::updTotal( const int newHistSize , unsigned int erasePtr , const float tot ){
    int hDiff = historyBuf.size() - newHistSize ;
    int countDelItems = 0 ;
    float newTotal = tot ;

    while ( countDelItems < hDiff ){
        newTotal -= historyBuf[erasePtr];
        if ( erasePtr != historyBuf.size() ){
            erasePtr++;
        } else {
            erasePtr = 0 ;
        }
        countDelItems++;
    } //end while


    return newTotal;
}

void filterClass::addElement(const float value)
{
    //if filled buffer, remove last value
    if ( hBufFull ){
        total -= historyBuf[currentPtr];
    }

    //we only keep the history to know what to subtract from the buffer later on.
    historyBuf[currentPtr] = value ;
    total += value ;

    //circular buffer - so update pointer to next current value
    if ( currentPtr >= historyBuf.size()-1 ){
        currentPtr = 0 ;
        hBufFull = true ;
    } else {
        currentPtr++;
    }
}

float filterClass::getAverage()
{
    float average;

    if ( !hBufFull )
    {
        if ( currentPtr == 0 )
            average = 0 ;
        else
            average = total / currentPtr ;
    }
    else
        average = total / (historyBuf.size()) ;

    return average;

}


