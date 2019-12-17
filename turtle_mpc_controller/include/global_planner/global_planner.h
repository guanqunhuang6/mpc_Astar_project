#ifndef GLOBAL_PLANNER_H
#define GLOBAL_PLANNER_H

#include <vector>
#include <stdlib.h>
#include <list>
#include <global_planner/fastastar.h>

class GlobalPlanner{
public:
    GlobalPlanner(int w, int h, int l, int r, int noise);
    ~GlobalPlanner();
    std::list<Point*> solve(const Point start_point, const Point end_point);
private:
	std::vector<std::vector<int >> obstacleGrid;
    Astar astar;
};

#endif