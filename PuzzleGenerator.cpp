#include "PuzzleGenerator.h"
#include <cmath>
using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{
    temp = 100000;
    coolingRate = 0.997;
    srand(static_cast<unsigned int>(time(NULL)));
}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
	timer.StartTimer();
	maxtime = 59.9;	// To make sure we don't exceed a minute

	// Do as many random walks as we can within the given time limit.
    Puzzle p(nRows, nColumns, minVal, maxVal);
    Puzzle bestPuzzle = p;
    int bestValue = p.GetValue();
    
	while (timer.GetElapsedTime() + 5 < maxtime)
	{
        std::cout << "STARTING NEW SIMULATED ANNEALING" << std::endl;
        std::cout << "__________________________________________" << std::endl;
        temp = 100000;
        p = SimulatedAnnealing(10, bestValue);
        int pValue = p.GetValue();
        
		// Check if p is better than the best puzzle we have found so far.
        if (pValue > bestValue)
        {
            bestPuzzle = p;
            bestValue = pValue;
        }
        
        std::cout << std::endl << std::endl;
	}
    return bestPuzzle;
}

Puzzle PuzzleGenerator::SimulatedAnnealing(double timelimit, int valueToBeat)
{
    
    // Generate a random puzzle that has a solution.
    Puzzle currPuzzle(nRows, nColumns, minVal, maxVal);
    while ((!currPuzzle.HasSolution()))
    {
        currPuzzle = Puzzle(nRows, nColumns, minVal, maxVal);
    }
    int currValue = currPuzzle.GetValue();
    
    // Keep track of the best puzzle found so far (and its value).
    Puzzle bestPuzzle = currPuzzle;
    int bestValue = currValue;
    std::cout << "Generated initial random puzzle of value " << currValue << std::endl;
	
	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();
	
	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit - 0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
        
        if (temp < 0.0000001 && bestValue < valueToBeat)
        {
            return bestPuzzle;
        }
        
		// Generate a successor by randomly changing the value of a random cell
        Puzzle p = currPuzzle.GetRandomSuccessor();
        for (int i = 0; i < 10; i++)
        {
            Puzzle n = currPuzzle.GetRandomSuccessor();
            if (n.GetValue() > p.GetValue())
            {
                p = n;
            }
        }
        Puzzle newPuzzle = p;
        int newValue = newPuzzle.GetValue();
		
        // New value is better than best value.
        if (newValue > bestValue)
        {
            currPuzzle = newPuzzle;
            currValue = newValue;
            bestPuzzle = newPuzzle;
            bestValue = newValue;
        }
        
        // Otherwise, determine whether to accept the new value.
        else
        {
            double random = (double) rand() / RAND_MAX;
            double acceptanceProbability = pow(2.71828, ((newValue - bestValue) / temp));
            
            if (acceptanceProbability > random) {
                
                std::cout << "Took successor of value " << newValue << std::endl;
                std::cout << "\tAP: " << acceptanceProbability << std::endl;
                std::cout << "\tRandom: " << random << std::endl;
                std::cout << "\tTemperature: " << temp << std::endl;
                
                currPuzzle = newPuzzle;
                currValue = newValue;
            }
        }
        
        // Update temperature
        temp *= coolingRate;
	}
	
	return bestPuzzle;
}
