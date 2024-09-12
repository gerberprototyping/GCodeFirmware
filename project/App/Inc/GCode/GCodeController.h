#ifndef __GCODE_INTERPRETOR_H
#define __GCODE_INTERPRETOR_H

#include "main.h"
#include "IOStream.h"
#include "GCode.h"
#include "GCodeScanner.h"
#include "StepDriver.h"




namespace GCode {


    class Controller {

        public:

            Controller(InputStream* istream, OutputStream* ostream, TIM_HandleTypeDef* timer, CRC_HandleTypeDef* crc_module);
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

            ResponseCode G0(Line &line);        // Rapid Positioning
            ResponseCode G1(Line &line);        // Linear Interpolation
            ResponseCode G28(Line &line);       // Home
            ResponseCode G90(Line &line);       // Absolute Positioning
            ResponseCode G91(Line &line);       // Relative Positioning
            ResponseCode G92(Line &line);       // Set Position (workspace coordinate system)
            ResponseCode G92_1(Line &line);     // Reset Position (i.e. use native machine coordinates)

    };


}




#endif
