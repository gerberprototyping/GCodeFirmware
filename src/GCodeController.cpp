#include "GCodeController.h"


using namespace GCode;




Controller::Controller()
    : atomic_pathEnd(AtomicPoint()), atomic_feedrate(MAX_SPEED)
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
    Point pathEnd = atomic_pathEnd.load();
    Point dest = pathEnd;
    double feedrate = atomic_feedrate.load();
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
            atomic_feedrate.store(feedrate);
        }
    }
    MotionVector vec = MotionVector(pathEnd, dest, feedrate);
    while(!motionVectorBuffer.add(vec));        // attempt to add to buffer until successful
    atomic_pathEnd.store(dest);
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
    atomic_pathEnd.store(Point());
    StepDriver::start();
}
