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

            bool G0(Line &line);        // Rapid Positioning
            bool G1(Line &line);        // Linear Interpolation
            bool G28(Line &line);       // Home
            bool G94(Line &line);       // Feedrate

    };


}




#endif