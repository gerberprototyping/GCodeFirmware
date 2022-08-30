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
            ~Controller() {}

            void run(InputStream &istream, OutputStream &ostream);

        private:

            Point pathEnd;
            double feedrate;

            bool G0(Line &line);
            bool G1(Line &line);
            bool G28(Line &line);

    };


}




#endif