#include "Nucleo-L476RG.h"
#include "Nema.h"
#include "LimitSwitch.h"


int main() {

    initClock40();

    
    LimitSwitch::initAll();
    Nema::initAll();

    volatile bool running = false;
    while(1) {
        if (xLimitSw.isActive()) {
            if (running) {
                xNema.stepCW();
                yNema1.stepCW();
                yNema2.stepCW();
                zNema.stepCW();
                delay_microseconds(200);
            } else {
                running = true;
                //for (int i=1000; i>500; i-=500) {
                    xNema.stepCW();
                    yNema1.stepCW();
                    yNema2.stepCW();
                    zNema.stepCW();
                    //delay_microseconds(i);
                    delay_microseconds(1000);
                //}
            }
        } else {
            running = false;
        }
    }
    

}