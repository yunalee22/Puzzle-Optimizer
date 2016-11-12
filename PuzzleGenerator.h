#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include "Puzzle.h"
#include "Timer.h"
#include <random>

class PuzzleGenerator{
public:
	PuzzleGenerator(int nRows, int nColumns, int minVal, int maxVal);
	Puzzle GeneratePuzzle(); 
	Puzzle SimulatedAnnealing(double timelimit, int bestValue);	// Time limit given in seconds.
    
    // Simulated annealing
    double getTemperature(double time);
	
private:
	int nRows, nColumns, minVal, maxVal;
	Timer timer;
	double maxtime;
    
    // Simulated annealing
    double temp;
    double coolingRate;
    
};
