#include "GCodeController.h"


using namespace GCode;




Controller::Controller(InputStream* istream, OutputStream* ostream)
    : pathEnd(Point()),
      offset(Point()),
      feedrate(MAX_SPEED),
      absolutePositioning(true),
      istream(istream),
      ostream(ostream),
      scanner(Scanner(istream,ostream))
{
    StepDriver::initAll();
}


void Controller::run() {

    Stepper::enableAll();
    StepDriver::homeAll();
    StepDriver::start();
    ostream->println(GCODE_JOB_START_MSG);

    Line line = Line();

    while (true) {

        scanner.getNext(line);

        bool success = false;
        if ( Word('G', 0) == line[0] ) {
            success = G0(line);
        }
        else if ( Word('G', 1) == line[0] ) {
            success = G1(line);
        }
        else if ( Word('G', 28) == line[0] ) {
            success = G28(line);
        }
        else if ( Word('G', 90) == line[0] ) {
            success = G90(line);
        }
        else if ( Word('G', 91) == line[0] ) {
            success = G91(line);
        }
        else if ( Word('G', 92) == line[0]) {
            success = G92(line);
        }
        else if ( Word('G', 92.1) == line[0]) {
            success = G92(line);
        }
        else if ( Word('M', 111) == line[0]) {
            success = M111(line);
        } else {
            ostream->println(GCODE_NACK_SYNTAX_MSG);
        }

        if (success) {
            ostream->println(GCODE_ACK_MSG);
        }

    }

    StepDriver::stop();
    Stepper::disableAll();
    ostream->print(GCODE_JOB_DONE_MSG);

}


bool Controller::G0(Line &line) {       // Rapid Positioning
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
            return false;
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
    return true;
}


bool Controller::G1(Line &line) {       // Linear Interpolation
    return G0(line);
}


bool Controller::G28(Line &line) {      // Home
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
                return false;
            }
        }
        if (x) xStepDriver.home();
        if (y) yStepDriver.home();
        if (z) zStepDriver.home();
    }
    pathEnd = Point();
    StepDriver::start();
    return true;
}


bool Controller::G90(Line &line) {      // Absolute Positioning
    // Accepts no arguments
    if (line.getCount() > 1) {
        return false;
    }
    absolutePositioning = true;
    return true;
}


bool Controller::G91(Line &line) {      // Relative Positioning
    // Accepts no arguments
    if (line.getCount() > 1) {
        return false;
    }
    absolutePositioning = false;
    return true;
}


bool Controller::G92(Line &line) {      // Set Position (Workspace Coordinate System)
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
                return false;
            }
        }
        offset = newOffset;     // Assign only after all arguments are parsed error free
    }
    return true;
}


bool Controller::G92_1(Line &line) {
    if (line.getCount() > 1) {
        return false;
    } else {
        offset = Point();
    }
    return true;
}


/**
 * "M111 D0"    Do not echo commands
 * "M111 D1"    Echo all commands
 */
bool Controller::M111(Line &line) {     // Set debug level
    if (2 == line.getCount()) {
        if ('D' == line[1].letter) {
            if (0 == line[1].number || 1 == line[1].number) {
                scanner.setEcho(line[1].number);
                return true;
            }
        }
    }
    return false;
}
