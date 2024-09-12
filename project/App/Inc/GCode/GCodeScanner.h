#ifndef __GCODE_SCANNER_H
#define __GCODE_SCANNER_H

#include <cstdlib>
#include <cctype>

#include "main.h"
#include "Config.h"
#include "IOStream.h"
#include "GCode.h"




namespace GCode {


    class Scanner{

        public:

            Scanner(InputStream* istream, OutputStream* ostream, CRC_HandleTypeDef* crc_module);
            void getNext(Line* line, ResponseCode* rcode);

        private:

            InputStream* istream;
            OutputStream* ostream;
            CRC_HandleTypeDef* crc_module;

            char ibuff[GCODE_RX_BUFF_SIZE];

    };


}




#endif
