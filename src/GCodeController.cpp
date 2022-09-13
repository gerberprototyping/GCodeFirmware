#include "GCodeController.h"


using namespace GCode;




Controller::Controller()
    : pathEnd(Point()), feedrate(MAX_SPEED)
{
    StepDriver::initAll();
}


void Controller::run(InputStream &istream, OutputStream &ostream) {

    Stepper::enableAll();
    StepDriver::homeAll();
    StepDriver::start();
    ostream.print(GCODE_JOB_START_MSG);

    Scanner scanner = Scanner(&istream);
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
        else if ( Word('G', 94) == line[0]) {
            success = G94(line);
        } else {
            ostream.println(GCODE_NACK_SYNTAX_MSG);
        }

        if (success) {
            ostream.println(GCODE_ACK_MSG);
        }

    }

    StepDriver::stop();
    Stepper::disableAll();
    ostream.print(GCODE_JOB_DONE_MSG);

}


bool Controller::G0(Line &line) {       // Rapid Positioning
    Point dest = pathEnd;
    for (uint32_t i=1; i<line.getCount(); i++) {
        if ( 'X' == line[i].letter ) {
            dest.setXMM(line[i].number);
        }
        else if ( 'Y' == line[i].letter ) {
            dest.setYMM(line[i].number);
        }
        else if ( 'Z' == line[i].letter ) {
            dest.setZMM(line[i].number);
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
    if (line.getCount() < 2) {
        StepDriver::homeAll();
    } else {
        bool x, y, z;
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
        if (z) zStepDriver.home();
        if (x) xStepDriver.home();
        if (y) yStepDriver.home();
    }
    pathEnd = Point();
    StepDriver::start();
    return true;
}


bool Controller::G94(Line &line) {      // Feedrate
    if ('F' == line[1].letter) {
        feedrate = line[1].number;
        return true;
    }
    return false;
}
