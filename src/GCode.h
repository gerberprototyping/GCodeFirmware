#ifndef __GCODE_H
#define __GCODE_H

#include <cstdint>
#include "Config.h"




namespace GCode {


    struct Word {
        char letter;
        double number;
        Word();
        Word(char letter, double number);
        Word(const Word &w);
    };




    class Line {

        public:

            Line();
            Line(const Word &w);
            Line(const Line &l);

            bool add(const Word &w);
            void makeEmpty();
            uint32_t getCount() const;

            Word& operator[](uint32_t i);
        
        private:

            uint32_t count;
            Word arr[GCODE_MAX_WORDS_PER_LINE];

    };


}


#endif