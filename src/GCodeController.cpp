#include "GCodeController.h"


using namespace GCode;




Controller::Controller()
    : pathEnd(Point::zero()), feedrate(MAX_SPEED)
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
        ostream.println(GCODE_ACK_MSG);

        if ( Word('G', 0) == line[0] ) {
            G0(line);
        }
        else if ( Word('G', 1) == line[0] ) {
            G1(line);
        }
        else if ( Word('G', 28) == line[0] ) {
            G28(line);
        }


    }

    StepDriver::stop();
    Stepper::disableAll();
    ostream.print(GCODE_JOB_DONE_MSG);

}


void Controller::G0(Line &line) {
    Point _pathEnd = pathEnd;
    Point dest = _pathEnd;
    for (uint32_t i=1; i<line.getCount(); i++) {
        if ( 'X' == line[i].letter ) {
            dest.x = Point::fromMM(line[i].number);
        }
        else if ( 'Y' == line[i].letter ) {
            dest.y = Point::fromMM(line[i].number);
        }
        else if ( 'Z' == line[i].letter ) {
            dest.z = Point::fromMM(line[i].number);
        }
        else if ( 'F' == line[i].letter ) {
            feedrate = line[i].number / 60;     // convert mm/min to mm/sec
        }
    }
    double _feedrate = feedrate;
    MotionVector vec = MotionVector(_pathEnd, dest, _feedrate);
    while(!motionVectorBuffer.add(vec));        // attempt to add to buffer until successfull
    pathEnd = dest;
}


void Controller::G1(Line &line) {
    G0(line);
}


void Controller::G28(Line &line) {
    while(!motionVectorBuffer.isEmpty());
    StepDriver::stop();
    if (line.getCount() < 2) {
        StepDriver::homeAll();
    } else {
        for (uint32_t i=1; i<line.getCount(); i++) {
            if ( 'Z' == line[i].letter ) {
                zStepDriver.home();
            }
            else if ( 'X' == line[i].letter ) {
                xStepDriver.home();
            }
            else if ( 'Y' == line[i].letter ) {
                yStepDriver.home();
            }
        }
    }
    Point pathEnd = Point::zero();
    StepDriver::start();
}
