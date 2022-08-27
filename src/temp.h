#ifndef __TEMP_H
#define __TEMP_H

#include "Nucleo-L476RG.h"


class Temp {

    public:
        Temp();
        float read();

    private:
        void wakeADC1();
        void calibrateADC1();

};


#endif