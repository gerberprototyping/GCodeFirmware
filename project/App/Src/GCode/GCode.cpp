#include "GCode.h"

#include "stdlib.h"

using namespace GCode;




//////////////////////////////////////////////////
// Word
//////////////////////////////////////////////////

Word::Word()
    : letter('\0'), number(0)
{}

Word::Word(char letter, double number)
    : letter(letter), number(number)
{}

bool Word::operator==(const Word &w) const {
    return (std::toupper(letter) == std::toupper(w.letter))
        && (number == w.number);
}




//////////////////////////////////////////////////
// Line
//////////////////////////////////////////////////

Line::Line()
    : count(0)
{ }

bool Line::add(const Word &w) {
    bool success = false;
    if (count <= GCODE_MAX_WORDS_PER_LINE) {
        success = true;
        arr[count] = w;
        count++;
    }
    return success;
}

void Line::makeEmpty() {
    count = 0;
}

uint32_t Line::getCount() const {
    return count;
}

Word* Line::find(const char letter) {
    for (uint32_t i=0; i<count; i++) {
        if (letter == arr[i].letter) {
            return &(arr[i]);
        }
    }
    return NULL;
}

Word& Line::operator[](uint32_t i) {
    return arr[i];
}




//////////////////////////////////////////////////
// Response
//////////////////////////////////////////////////

Response::Response(ResponseCode code, uint32_t line_num)
    : code(code), line_num(line_num)
{
    // Intentionally left blank
}

const char* STR_OK = "ok";
const char* STR_ERR = "error:";
// buff must be large enough to hold all response messages
void Response::format(char* const buff) {
    char* curr = buff;

    // add "ok" or "error:"
    const char* str = (this->code == RESPONSE_OK) ? STR_OK : STR_ERR;
    for (const char* i=str; *i!='\0'; ++i) {
        *(curr++) = *i;
    }

    // add error code
    if (this->code != RESPONSE_OK) {
        itoa(int(this->code), curr, 10);
        while (*curr != '\0') { ++curr; }
    }

    // add line number
    if (this->line_num > 0) {
        *(curr++) = ' ';
        *(curr++) = 'N';
        itoa(int(this->line_num), curr, 10);
        // while (*curr != '\0') { ++curr; }
    }
}
