#ifndef __GCODE_SCANNER_H
#define __GCODE_SCANNER_H

#include <cstdlib>
#include <cctype>
#include "Config.h"
#include "GCode.h"
#include "uart.h"




namespace GCode {
    
    
    class Scanner: public OutputStream {

        public:

            Scanner(UART uart);
            Line getNext(Line &line);

            void write(uint8_t x);
            void flush();
        
        private:

            Word getNextWord();
        
            bool foundLineEnd;
            UART uart;
            char ibuf[GCODE_RX_BUFF_SIZE];

    };


}




#endif