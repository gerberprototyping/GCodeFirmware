#include "GCodeScanner.h"

using namespace GCode;




Scanner::Scanner(InputStream* istream, OutputStream* ostream)
    : istream(istream),
      ostream(ostream),
      foundLineEnd(false),
      echo(true)
{}


void Scanner::setEcho(bool echo) {
    this->echo = echo;
}


Line Scanner::getNext(Line &line) {
    line.makeEmpty();
    bool done = false;
    while (!done) {
        Word word = getNextWord();
        if (word.letter == '\n') {
            done = true;
        } else {
            line.add(word);
        }
    }
    return line;
}


Word Scanner::getNextWord() {
    Word word = Word('\0', 0);

    if (foundLineEnd) {

        if (echo) {
            ostream->print("\n");
        }
        foundLineEnd = false;
        word.letter = '\n';

    } else {

        uint32_t i=0;
        bool whitespace = false;

        while (!whitespace && i<(GCODE_RX_BUFF_SIZE-1)) {
            ibuf[i] = istream->read();
            if (echo) {
                ostream->print(ibuf[i]);
            }
            foundLineEnd = ('\r' == ibuf[i]);
            if ( std::isspace(ibuf[i]) ) {
                whitespace = true;
            }
            else if (std::isalnum(ibuf[i]) || ibuf[i] == '.' || ibuf[i] == '-') {
                i++;
            }
        }

        ibuf[i] = '\0';
        word.number = std::strtod(ibuf+1, NULL);
        if (word.number != 0 || ibuf[1] == '0' || !std::isalpha(ibuf[0])) {
            word.letter = std::toupper(ibuf[0]);
        }

    }

    return word;
}

