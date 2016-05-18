/***********************************************************************************
 *	Purpose: The Maze class implements a maze represented as a square grid of rooms.
 *           Implementation of depth-first search to findExitPath() and implementation   
 *			 of breadth-first search to findShortestPath().
 *
 *	Author:  Jenny Nguyen and Yoshitaka Meguro
 *  Date:    April 25, 2015
 **********************************************************************************/

#include "Maze.h"
#include "Position.h"
#include <queue>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <cstdlib>

using namespace std;

Maze::Maze(Position s, Position e, int n)
{
    size = n;
    start = s;
    exitpos = e;
    try {
        M = new State *[size];
    }
    catch(bad_alloc) {
        cerr << "Unable to allocate array of state pointers";
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        try {
            M[i] = new State[size];
        }
        catch (bad_alloc) {
            cerr << "Unable to allocate row of state values";
            exit(1);
        }
        for (int j = 0; j < size; j++)
            M[i][j] = WALL;
    }
    try {
        Pred = new Position *[size];
    }
    catch(bad_alloc) {
        cerr << "Unable to allocate array of predecessor positions";
        exit(1);
    }
    for(int i = 0; i < size; i++) {
        try {
            Pred[i] = new Position[size];
        }
        catch (bad_alloc) {
            cerr << "Unable to allocate row of predecessor positions";
            exit(1);
        }
        for (int j = 0; j < size; j++)
            Pred[i][j] = Position(i, j);
    }
}

Maze::~Maze()
{
    for(int i = 0; i < size; ++i) {
        delete [] M[i];
        delete [] Pred[i];
    }
    delete [] M;
    delete [] Pred;
}

State
Maze::getState(const Position &P) const
{
    return M[P.getRow()][P.getCol()];
}

void Maze::display(ostream &out) const
{
    out << '\n';
    for (int i=0; i < size;i++) {
        for (int j=0; j < size; j++)
            if (Position(i,j) == start)
                cout << 's';
            else if (Position(i,j) == exitpos)
                cout << 'e';
            else if (M[i][j] == WALL)
                out << '*';
            else
                out << ' ';
        out << '\n';
    }
    out << '\n';
}

void Maze::setState(const Position &P, State s)
{
    int i = P.getRow();
    int j = P.getCol();
    assert(1 <= i && i <= size && 1 <= j && j <= size);
    M[i][j] = s;
}

bool Maze::findExitPath()
{
    // Fill in the missing code for DFS
    // Returns true if a start-to-exit path has been found
    // and the contents of the path stack from to bottom
    // to top will be the exit path

	Position current = start; //Set the current position to start position
	Position next, neighbor; 

	path.push(current); // Push the current position into stack

	while(!path.empty()){  //While path is not empty
		current = path.top();
		setState(current, VISITED);
		
		// Returns true if a start-to-exit path has been found
		if (current == exitpos){
			return true;
		}
		
		else{
			next = current;
			
			// Check if the current position is unvisited and have neighbor
			for (direction i = DOWN; i != NONE; i++){
				neighbor = current.Neighbor(i);

				//If neighbor is open, set it to next
				if (getState(neighbor) == OPEN){
					next = neighbor;
					break;
				}
			}
			// If next position is not current position then
			if (!(next == current)){
				//Mark the next position to visited and push to stack 
				setState(next, VISITED);
				path.push(next);
			}

			else{
				// If current position is not start position,
				if (!(current == start)){ 
					path.pop();  // Pop the current position from stack
				}
				else{
					return false;
				}
			}
		}
	}
	return false;
}
	


bool Maze::findShortestPath()
{
    // Fill in the missing code for BFS
    // Returns true if a start-to-exit path has been found
    // and this path will be a shortest possible escape path
    // This function will fill in the values of the Pred array,
    // which is used to find the shortest path

    queue<Position> q;
	Position current = this->start; //Set current position to start 
	Position neighbor;

	Pred[current.getRow()][current.getCol()] = current; //Set the current to previous current
	q.push(current);

	while (!(q.empty())){ // While q is not empty
		current = q.front(); // Set current to start
		q.pop();			 // Pop start position from queue

		//Returns true if a start-to-exit path has been found
		if (current == this->exitpos)
		{
			return true;
		}

		// BTS: Find the shortest path 
		for (direction i = DOWN; i != NONE; i++){
			neighbor = current.Neighbor(i);

			// If neighbor is open, mark neighbor to visit and push it to stack
			if (getState(neighbor) == OPEN){
				setState(neighbor, VISITED);
				Pred[neighbor.getRow()][neighbor.getCol()] = current;
				q.push(neighbor);
			}
		}
	}

	return false;
}

void Maze::printExitPath(ostream& out)
{
    //make a copy so we can print this path to both cout and file
    stack<Position> pathCopy(path);

    //Reverse the copied list
    stack<Position> reversedCopy;
    while(!pathCopy.empty())
    {
        reversedCopy.push(pathCopy.top());
        pathCopy.pop();
    }

    //output to stream
    while(!reversedCopy.empty())
    {
        out << reversedCopy.top() << endl;
        reversedCopy.pop();
    }
}

void Maze::printShortestPath(ostream& out)
{
    Position P = exitpos;
    while(!(P == start)) {
        path.push(P);
        P = Pred[P.getRow()][P.getCol()];
    }
    path.push(start);

    while(!path.empty()) {
        out << path.top() << endl;
        path.pop();
    }
}

