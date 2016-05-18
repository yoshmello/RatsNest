#include <iostream>
#include <fstream>
#include "Position.h"
#include "Maze.h"

Maze* initialize_maze(char* argv[])
{
    Maze *maze;
    int r, c;
    fstream input;
    input.open(argv[1], std::fstream::in);

    if(input.is_open())
    {
        //get maze size
        int maze_size;
        input >> maze_size;
        input.ignore(256, '\n');

        //Get start position
        input >> r >> c;
        input.ignore(256, '\n');
        Position startpos(r,c);

        //Get exit position
        input >> r >> c;
        input.ignore(256, '\n');
        Position exitpos(r,c);

        maze = new Maze(startpos, exitpos, maze_size + 2);

        //Get maze strucure
        //For each row r, get columns that are open
        for (r = 1; r < maze_size + 1; r++) {
            input >> c;
            while (c > 0){
                maze->setState(Position(r, c), OPEN);
                input >> c;
            };
            input.ignore(256,'\n'); //ignore remainder of line
        }

        if (maze->getState(startpos) != OPEN) {
            cout << "Start position must be open. Opening.";
            maze->setState(startpos, OPEN);
        }
        if (maze->getState(exitpos) != OPEN) {
            cout << "Exit position must be open. Opening.";
            maze->setState(exitpos, OPEN);
        }
        cout << "\nMaze entry complete" << endl;
    }
    else
    {
        cout << "Could not open input file " << argv[1] << ", exiting." << endl;
        cin.ignore();
        exit(-1);
    }

    return maze;
}



int main(int argc, char* argv[])
{
    fstream output;

    if(argc < 3)
    {
        cout << "Expects two program arguments: <input_file>.txt and <output_file>.txt\n";
        cout << endl << "Press any key to continue..." << endl;
        cin.ignore();
        exit(1);
    }

    //may need to add fstream::binary if diff tool thinks they are different (line endings don't match)
    output.open(argv[2], fstream::out | fstream::trunc);
    if(output.is_open())
    {
        cout << "RAT IN MAZE" << endl;
        cout << endl;

        Maze *RatHaus = initialize_maze(argv);

        cout << "Your maze:\n";
        RatHaus->display(cout);

        int choice;
        cout << "Enter 1 to use DFS." << endl;
        cout << "Enter 2 to use BFS." << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(256, '\n');

        if (choice == 1) {
            if (!RatHaus->findExitPath())
                cout << "No path found. Check your maze." << endl;
            else {
                cout << "Path found:" << endl;
                RatHaus->printExitPath(cout);
                RatHaus->printExitPath(output);
                cout << "\n\n";
            }
        } else {
            if (!RatHaus->findShortestPath())
                cout << "No path found. Check your maze." << endl;
            else {
                cout << "Path found:" << endl;
                RatHaus->printShortestPath(cout);
                RatHaus->printShortestPath(output);
            }
        }
        cout << endl << "Press any key to continue..." << endl;
        cin.ignore();
    }
    else
    {
        cout << "Could not open output file " << argv[2] << ", exiting." << endl;
        cout << endl << "Press any key to continue..." << endl;
        cin.ignore();
        exit(-1);
    }

    return 0;
}
