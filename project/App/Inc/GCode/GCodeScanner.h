#ifndef __GCODE_SCANNER_H
#define __GCODE_SCANNER_H

#include <cstdlib>
#include <cctype>
#include "Config.h"
#include "IOStream.h"
#include "GCode.h"




namespace GCode {
    
    
    class Scanner{

        public:

            Scanner(InputStream* istream, OutputStream* ostream);
            Line getNext(Line& line);
            void setEcho(bool echo);
        
        private:

            Word getNextWord();
        
            InputStream* istream;
            OutputStream* ostream;
            bool foundLineEnd;
            volatile bool echo;
            char ibuf[GCODE_RX_BUFF_SIZE];

    };


}




#endif