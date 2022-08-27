#ifndef __GCODE_INTERPRETOR_H
#define __GCODE_INTERPRETOR_H

#include "IOStream.h"
#include "GCode.h"
#include "GCodeScanner.h"
#include "StepDriver.h"
#include "Concurrent.h"




namespace GCode {


    class Controller {

        public:

            Controller();
            ~Controller() {}

            void run(InputStream &istream, OutputStream &ostream);

        private:

            AtomicPoint atomic_pathEnd;
            Atomic<double> atomic_feedrate;

            void G0(Line &line);
            void G1(Line &line);
            void G28(Line &line);

    };


}




#endif