#include "global_planner/global_planner.h"
#include <stdlib.h>
#include <iostream>


GlobalPlanner::GlobalPlanner(int w, int h, int l, int r, int noise): obstacleGrid(w, std::vector<int>(h, 0)){
    for(int i=(h-2*(l+r+noise))/2;i<(h+2*(l+r+noise))/2;i++){
        for(int j=(h-2*(l+r+noise))/2;j<(h+2*(l+r+noise))/2;j++){
            obstacleGrid[i][j] = 1;
        }
    }
}

GlobalPlanner::~GlobalPlanner(){
    //delete obstacleGrid;
}

std::list<Point*> GlobalPlanner::solve(Point start_point, Point end_point){
    astar.InitAstar(obstacleGrid);
    std::list<Point*> path=astar.GetPath(start_point, end_point, false);
	//打印
	//for(auto &p:path) std::cout<<'('<<p->x<<','<<p->y<<')'<< std::endl;

	return path;
}