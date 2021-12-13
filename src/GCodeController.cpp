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

        if ( Word('G',0) == line[0] ) {
            G0(line);
        }
        else if ( Word('G',1) == line[0] ) {
            G1(line);
        }


    }

    StepDriver::stop();
    Stepper::disableAll();
    ostream.print(GCODE_JOB_DONE_MSG);

}


void Controller::G0(Line &line) {
    Point dest = pathEnd;
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
            feedrate = line[i].number * 60;     // convert mm/min to mm/sec
        }
    }
    MotionVector vec = MotionVector(pathEnd, dest, feedrate);
    while(!motionVectorBuffer.add(vec));        // attempt to add to buffer until successfull
}


void Controller::G1(Line &line) {
    G0(line);
}
