#ifndef __GCODE_INTERPRETOR_H
#define __GCODE_INTERPRETOR_H


#include "IOStream.h"
#include "GCode.h"
#include "GCodeScanner.h"
#include "StepDriver.h"




namespace GCode {


    class Controller {

        public:

            Controller();

            void run(InputStream &istream, OutputStream &ostream);

        private:

            volatile Point pathEnd;
            volatile double feedrate;

            void G0(Line &line);
            void G1(Line &line);
            void G28(Line &line);

    };


}




#endif