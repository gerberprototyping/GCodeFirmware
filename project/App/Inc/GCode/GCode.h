#ifndef __GCODE_H
#define __GCODE_H

#include <cstdint>
#include <cctype>
#include "Config.h"




namespace GCode {


    class Word {

        public:

            char letter;
            double number;

            Word();
            Word(char letter, double number);

            bool operator==(const Word &w) const;

    };


    class Line {

        public:

            Line();

            bool add(const Word &w);
            void makeEmpty();
            uint32_t getCount() const;

            Word* find(const char letter);

            Word& operator[](uint32_t i);

        private:

            uint32_t count;
            Word arr[GCODE_MAX_WORDS_PER_LINE];

    };


    typedef enum {
        RESPONSE_OK                                 = 0,
        RESPONSE_ERR_LINE                           = 1,
        RESPONSE_ERR_CHECKSUM                       = 2,
        RESPONSE_ERR_SYNTAX_OTHER                   = 100,
		RESPONSE_ERR_SYNTAX_UNKOWN_CMD				= 101,
        RESPONSE_ERR_SYNTAX_UNKOWN_ARG              = 102,
        RESPONSE_ERR_SYNTAX_NUMBER_FORMAT           = 103,
        RESPONSE_ERR_SYNTAX_UNKOWN_CHECKSUM         = 108,
        RESPONSE_ERR_SYNTAX_ARG_AFTER_CHECKSUM      = 109
    } ResponseCode;


    class Response {

        public:

            Response(ResponseCode code, uint32_t line_num=0U);

            void format(char* const buff);

            ResponseCode code;
            uint32_t line_num;
    };


}


#endif
