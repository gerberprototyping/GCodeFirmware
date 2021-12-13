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

            Scanner(InputStream* istream);
            Line getNext(Line &line);
        
        private:

            Word getNextWord();
        
            InputStream* istream;
            bool foundLineEnd;
            char ibuf[GCODE_RX_BUFF_SIZE];

    };


}




#endif