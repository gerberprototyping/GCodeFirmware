#ifndef __GCODE_INTERPRETOR_H
#define __GCODE_INTERPRETOR_H

#include "IOStream.h"
#include "GCode.h"
#include "GCodeScanner.h"
#include "StepDriver.h"




namespace GCode {


    class Controller {

        public:

            Controller(InputStream* istream, OutputStream* ostream);
            ~Controller() {}

            void run();

        private:

            Point pathEnd;
            Point offset;
            double feedrate;
            bool absolutePositioning;

            InputStream* istream;
            OutputStream* ostream;
            Scanner scanner;

            bool G0(Line &line);        // Rapid Positioning
            bool G1(Line &line);        // Linear Interpolation
            bool G28(Line &line);       // Home
            bool G90(Line &line);       // Absolute Positioning
            bool G91(Line &line);       // Relative Positioning
            bool G92(Line &line);       // Set Position (workspace coordinate system)
            bool G92_1(Line &line);     // Reset Position (use native machine coordinates)

            bool M111(Line &line);      // Set debug level (see source for details)

    };


}




#endif