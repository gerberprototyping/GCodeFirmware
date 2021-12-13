#include "GCode.h"

using namespace GCode;




Word::Word()
    : letter('\0')
{}


Word::Word(char letter, double number)
    : letter(letter), number(number)
{}


Word::Word(const Word &w)
    : letter(w.letter), number(w.number)
{}


bool Word::operator==(const Word &w) const {
    return (std::toupper(letter) == std::toupper(w.letter))
        && (number == w.number);
}








Line::Line()
    : count(0)
{ }


Line::Line(const Word &w) {
    arr[0] = w;
    count = 1;
}


Line::Line(const Line &l) {
    count = l.count;
    for (uint32_t i=0; i<count; i++) {
        arr[i] = l.arr[i];
    }
}


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


Word& Line::operator[](uint32_t i) {
    return arr[i];
}

