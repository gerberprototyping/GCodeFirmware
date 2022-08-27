#include <iostream>
#include "src\Cartesian.h"

#include "src\Cartesian.cpp"

using namespace std;




int main() {

    Point start = Point::zero();
    Point end   = Point::fromMM(1.5,2,0);

    cout << end << endl;
    cout << end.toMM<CartesianDouble>() << endl;

    CartesianDouble dir = (end-start).toSteps<CartesianDouble>();

    Velocity v = Velocity(50, dir);

    cout << v << endl;

}