#include "GCodeScanner.h"

using namespace GCode;




Scanner::Scanner(InputStream* istream, OutputStream* ostream, CRC_HandleTypeDef* crc_module)
    : istream(istream),
      ostream(ostream),
	  crc_module(crc_module)
{}


void Scanner::getNext(Line* line, ResponseCode* rcode) {
    line->makeEmpty();
    uint32_t n = istream->readline((uint8_t*) ibuff, GCODE_RX_BUFF_SIZE);

    char* const end = ibuff + n - 1;
    char* curr = ibuff;
    // Skip any leading whitespace
    while (' ' == *curr || '\t' == *curr) {
        ++curr;
    }

    // Parse words
    *rcode = RESPONSE_OK;
    bool has_checksum = false;
    while (curr <= end) {

        // Get letter
        char letter = std::toupper(*curr);
        if (has_checksum) {
            *rcode = RESPONSE_ERR_SYNTAX_ARG_AFTER_CHECKSUM;
        }
        else if ('*' == letter) {
            has_checksum = true;
        }
        ++curr;
        // Get number
        double number = 0.0;
        if (curr <= end && ' ' != *curr && '\t' != *curr) {
            char* num_end = NULL;
            number = std::strtod(curr, &num_end);
            if (curr == num_end) {
                // invalid number syntax
                *rcode = RESPONSE_ERR_SYNTAX_NUMBER_FORMAT;
                // skip non-whitespace
                while (curr <= end && ' ' != *curr && '\t' != *curr) {
                    ++curr;
                }
            } else {
                curr = num_end;
            }
        }
        // Add word
        line->add(Word(letter, number));
        // Skip whitespace
        while (curr <= end && (' ' == *curr || '\t' == *curr)) {
            ++curr;
        }

    }

    // Validate checksum
    if (has_checksum) {

        // get received checksum
        int32_t src_checksum = -1;
        for (uint32_t i=0; i<line->getCount(); ++i) {
            if ('*' == (*line)[i].letter) {
                src_checksum = (*line)[i].number;
                break;
            }
        }

        // locate checksum char
        curr = ibuff;
        uint32_t len = 0;
        while (curr <= end && '*' != *curr) {
            ++curr;
            ++len;
        }

        // count digits in checksum
        uint32_t digits = 0;
        while (curr <= end && std::isdigit(*curr)) {
            ++curr;
            ++digits;
        }

        // calculate checksum
        bool checksum_pass = false;
        if (digits >= 1 && digits <= 3) {
            // XOR checksum
            int32_t checksum = 0;
            for (curr = ibuff; curr <= end && '*' != *curr; ++curr) {
                checksum = checksum ^ *curr;
            }
            checksum &= 0xff;
            checksum_pass = src_checksum == checksum;
        }
        else if (digits == 5) {
            // CRC
            int32_t checksum = HAL_CRC_Calculate(crc_module, (uint32_t*) ibuff, len);
            checksum_pass = src_checksum == checksum;
        } // else not needed (invalid checksum, pass already = false)

        if (!checksum_pass) {
            *rcode = RESPONSE_ERR_CHECKSUM;
        }
    }
}
