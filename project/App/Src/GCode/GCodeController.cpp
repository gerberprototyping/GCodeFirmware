#include "GCodeController.h"


using namespace GCode;

char response_buff[RESPONSE_BUFF_SIZE];




Controller::Controller(InputStream* istream, OutputStream* ostream, TIM_HandleTypeDef* timer, CRC_HandleTypeDef* crc_module)
    : pathEnd(Point()),
      offset(Point()),
      feedrate(MAX_SPEED),
      absolutePositioning(true),
      istream(istream),
      ostream(ostream),
      scanner(Scanner(istream,ostream, crc_module))
{
    StepDriver::initAll(timer);
}


void Controller::run() {

    Stepper::enableAll();
    StepDriver::homeAll();
    StepDriver::start();
    ostream->println(GCODE_JOB_START_MSG);

    Line line = Line();
    Response response = Response(RESPONSE_OK);
    uint32_t prev_line = 0;

    while (true) {

        scanner.getNext(&line, &response.code);

        // Check for line number, even if error already occurred
        uint32_t curr_line = 0;
        Word line_word = *(line.find('N'));
        if ('\0' != line_word.letter) {
            curr_line = (uint32_t) line_word.number;
            response.line_num = curr_line;
            if (Word('G', 110) == line[0]) {
                // set current line number
                if (line.getCount() != 2 || 'N' != line[1].letter) {
                	response.code = RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
                } else {
                	curr_line = line[1].number;
                }
            }
            else if ((prev_line != 0 && curr_line != prev_line+1)
                    || (double) curr_line != line_word.number)
            {
                if (RESPONSE_OK == response.code) {
                    response.code = RESPONSE_ERR_LINE;
                }
            }
        }

        if (RESPONSE_OK == response.code) {
            if (Word('G', 0) == line[0] ) {
                response.code = G0(line);
            }
            else if (Word('G', 1) == line[0] ) {
                response.code = G1(line);
            }
            else if (Word('G', 28) == line[0] ) {
                response.code = G28(line);
            }
            else if (Word('G', 90) == line[0] ) {
                response.code = G90(line);
            }
            else if (Word('G', 91) == line[0] ) {
                response.code = G91(line);
            }
            else if (Word('G', 92) == line[0]) {
                response.code = G92(line);
            }
            else if (Word('G', 92.1) == line[0]) {
                response.code = G92(line);
            }
            else if (Word('M', 110) == line[0]) {
                // handled above, do nothing
            } else {
                response.code = RESPONSE_ERR_SYNTAX_UNKOWN_CMD;
            }
        }

        response.format(response_buff);
        ostream->println(response_buff);

        // If error, flush rx buff (wait ~100 ms, then flush?)
        if (response.code != RESPONSE_OK) {
            osDelay(100);
            istream->discardall();
        }
        else if (curr_line > 0) {
            prev_line = curr_line;
        }

    }

    StepDriver::stop();
    Stepper::disableAll();
    ostream->println(GCODE_JOB_DONE_MSG);

}


ResponseCode Controller::G0(Line &line) {       // Rapid Positioning
    Point dest = pathEnd;
    for (uint32_t i=1; i<line.getCount(); i++) {
        if ( 'X' == line[i].letter ) {
            int64_t xOffset = absolutePositioning ? offset.getXSteps() : pathEnd.getXSteps();
            dest.setXSteps(line[i].number*X_STEPS_PER_MM + xOffset);
        }
        else if ( 'Y' == line[i].letter ) {
            int64_t yOffset = absolutePositioning ? offset.getYSteps() : pathEnd.getYSteps();
            dest.setYSteps(line[i].number*Y_STEPS_PER_MM + yOffset);
        }
        else if ( 'Z' == line[i].letter ) {
            int64_t zOffset = absolutePositioning ? offset.getZSteps() : pathEnd.getZSteps();
            dest.setZSteps(line[i].number*Z_STEPS_PER_MM + zOffset);
        }
        else if ( 'F' == line[i].letter ) {
            feedrate = line[i].number;
        }
        else {
            // unrecognized arg
            return RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
        }
    }
    // Constrain destination
    if (dest.getXSteps() < X_MIN_STEPS ) {
        dest.setXSteps(X_MIN_STEPS+1);
    }
    else if (dest.getXSteps() > X_MAX_STEPS) {
        dest.setXSteps(X_MAX_STEPS-1);
    }
    if (dest.getYSteps() < Y_MIN_STEPS ) {
        dest.setYSteps(Y_MIN_STEPS+1);
    }
    else if (dest.getYSteps() > Y_MAX_STEPS) {
        dest.setYSteps(Y_MAX_STEPS-1);
    }
    if (dest.getZSteps() < Z_MIN_STEPS ) {
        dest.setZSteps(Z_MIN_STEPS+1);
    }
    else if (dest.getZSteps() > Z_MAX_STEPS) {
        dest.setZSteps(Z_MAX_STEPS-1);
    }
    MotionVector vec = MotionVector(pathEnd, dest, feedrate);
    while(!motionVectorBuffer.add(vec));        // attempt to add to buffer until successful
    pathEnd = dest;
    return RESPONSE_OK;
}


ResponseCode Controller::G1(Line &line) {       // Linear Interpolation
    return G0(line);
}


ResponseCode Controller::G28(Line &line) {      // Home
    while(!motionVectorBuffer.isEmpty());
    StepDriver::stop();
    if (line.getCount() < 2) {              // No arguments
        StepDriver::homeAll();
    } else {
        bool x, y, z = false;
        for (uint32_t i=1; i<line.getCount(); i++) {
            if ( 'Z' == line[i].letter ) {
                z = true;
            }
            else if ( 'X' == line[i].letter ) {
                x = true;
            }
            else if ( 'Y' == line[i].letter ) {
                y = true;
            }
            else {
                // unrecognized arg
                return RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
            }
        }
        if (x) xStepDriver.home();
        if (y) yStepDriver.home();
        if (z) zStepDriver.home();
    }
    pathEnd = Point();
    StepDriver::start();
    return RESPONSE_OK;
}


ResponseCode Controller::G90(Line &line) {      // Absolute Positioning
    // Accepts no arguments
    if (line.getCount() > 1) {
        return RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
    }
    absolutePositioning = true;
    return RESPONSE_OK;
}


ResponseCode Controller::G91(Line &line) {      // Relative Positioning
    // Accepts no arguments
    if (line.getCount() > 1) {
        return RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
    }
    absolutePositioning = false;
    return RESPONSE_OK;
}


ResponseCode Controller::G92(Line &line) {      // Set Position (Workspace Coordinate System)
    if (line.getCount() < 2) {              // No arguments
        offset = Point::fromSteps(
                pathEnd.getXSteps(),
                pathEnd.getYSteps(),
                pathEnd.getZSteps()
        );
    } else {
        Point newOffset = offset;
        for (uint32_t i=1; i<line.getCount(); i++) {
            if ( 'X' == line[i].letter ) {
                newOffset.setXSteps(pathEnd.getXSteps() - line[i].number*X_STEPS_PER_MM);
            }
            else if ( 'Y' == line[i].letter ) {
                newOffset.setYSteps(pathEnd.getYSteps() - line[i].number*Y_STEPS_PER_MM);
            }
            else if ( 'Z' == line[i].letter ) {
                newOffset.setZSteps(pathEnd.getZSteps() - line[i].number*Z_STEPS_PER_MM);
            }
            else {
                // unrecognized arg
                return RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
            }
        }
        offset = newOffset;     // Assign only after all arguments are parsed error free
    }
    return RESPONSE_OK;
}


ResponseCode Controller::G92_1(Line &line) {    // Reset Position (i.e. user native machine coordinates)
    // Accepts no arguments
    if (line.getCount() > 1) {
        return RESPONSE_ERR_SYNTAX_UNKOWN_ARG;
    } else {
        offset = Point();
    }
    return RESPONSE_OK;
}
