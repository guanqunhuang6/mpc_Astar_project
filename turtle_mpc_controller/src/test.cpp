#include "global_planner/global_planner.hpp"

int main(int argc, char** argv){
    GlobalPlanner globalPlanner(20,20,2,2,1);
    Point start(0,0);
    Point end(19,19);
    list<Point*> path = globalPlanner.solve(start, end);
    return 0;
}