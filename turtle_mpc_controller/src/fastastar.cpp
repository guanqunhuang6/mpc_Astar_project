#include <math.h>
#include "global_planner/fastastar.h"
 
void Astar::InitAstar(std::vector<std::vector<int>> &_maze)
{
	maze=_maze;
}
 
int Astar::calcG(Point *temp_start,Point *point)
{
	int extraG=(abs(point->x-temp_start->x)+abs(point->y-temp_start->y))==1?kCost1:kCost2;
	int parentG=point->parent==NULL?0:point->parent->G; //如果是初始节点，则其父节点是空
	return parentG+extraG;
}
 
int Astar::calcH(Point *point,Point *end)
{
	//用简单的欧几里得距离计算H，这个H的计算是关键，还有很多算法，没深入研究^_^
	return sqrt((double)(end->x-point->x)*(double)(end->x-point->x)+(double)(end->y-point->y)*(double)(end->y-point->y))*kCost1;
}
 
int Astar::calcF(Point *point)
{
	return point->G+point->H;
}
 
Point *Astar::findPath(Point &startPoint,Point &endPoint,bool isIgnoreCorner)
{
	openList.push(new Point(startPoint.x,startPoint.y)); //置入起点,拷贝开辟一个节点，内外隔离
	while(!openList.empty())
	{
		auto curPoint=openList.top(); //找到F值最小的点
		openList.pop(); //从开启列表中删除
		curPoint->is_closed = true;//放到关闭列表
        curPoint->is_open = false;
		//1,找到当前周围八个格中可以通过的格子
		auto surroundPoints=getSurroundPoints(curPoint,isIgnoreCorner);
		for(auto &target:surroundPoints)
		{
			//2,对某一个格子，如果它不在开启列表中，加入到开启列表，设置当前格为其父节点，计算F G H
			if(!target->is_open)
			{
				target->parent=curPoint;
 
				target->G=calcG(curPoint,target);
				target->H=calcH(target,&endPoint);
				target->F=calcF(target);
                target->is_open = true;
 
				openList.push(target);
                if(target->x==endPoint.x && target->y==endPoint.y) return target;//返回列表里的节点指针，不要用原来传入的endpoint指针，因为发生了深拷贝
			}
			//3，对某一个格子，它在开启列表中，计算G值, 如果比原来的大, 就什么都不做, 否则设置它的父节点为当前点,并更新G和F
			else
			{
				int tempG=calcG(curPoint,target);
				if(tempG<target->G)
				{
					target->parent=curPoint;
 
					target->G=tempG;
					target->F=calcF(target);
				}
			}
			//Point *resPoint=isInList(openList,&endPoint);
			//if(endPoint.is_open==true)
			//	return &endPoint; //返回列表里的节点指针，不要用原来传入的endpoint指针，因为发生了深拷贝
		}
	}
 
	return NULL;
}
 
std::list<Point *> Astar::GetPath(Point &startPoint,Point &endPoint,bool isIgnoreCorner)
{
	Point *result=findPath(startPoint,endPoint,isIgnoreCorner);
	std::list<Point *> path;
	//返回路径，如果没找到路径，返回空链表
	while(result)
	{
		path.push_front(result);
		result=result->parent;
	}
 
    // 清空临时开闭列表，防止重复执行GetPath导致结果异常
    //openList.clear();
    while (!openList.empty()) openList.pop();
	return path;
}
 
bool Astar::isCanreach(const Point *point,const Point *target,bool isIgnoreCorner) const
{
	if(target->x<0||target->x>maze.size()-1
		||target->y<0||target->y>maze[0].size()-1
		||maze[target->x][target->y]==1
		||target->x==point->x&&target->y==point->y
		||target->is_closed) //如果点与当前节点重合、超出地图、是障碍物、或者在关闭列表中，返回false
		return false;
	else
	{
		if(abs(point->x-target->x)+abs(point->y-target->y)==1) //非斜角可以
			return true;
		else
		{
			//斜对角要判断是否绊住
			if(maze[point->x][target->y]==0&&maze[target->x][point->y]==0)
				return true;
			else
				return isIgnoreCorner;
		}
	}
}
 
std::vector<Point *> Astar::getSurroundPoints(const Point *point,bool isIgnoreCorner) const
{
	std::vector<Point *> surroundPoints;
 
	for(int x=point->x-1;x<=point->x+1;x++)
		for(int y=point->y-1;y<=point->y+1;y++)
			if(isCanreach(point,new Point(x,y),isIgnoreCorner))
				surroundPoints.push_back(new Point(x,y));
	
	return surroundPoints;
}