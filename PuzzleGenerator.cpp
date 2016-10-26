#include "PuzzleGenerator.h"
#include <math.h>
#include <cstdlib>
using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
  timer.StartTimer();
  maxtime = 59.9;	// To make sure we don't exceed a minute

  // return RandomWalk(5);	// Do a random walk for 5 seconds and return the solution.

  // We could also do as many random walks as we can within the given time limit.

  // while(timer.GetElapsedTime() < maxtime - 0.1) {
  // }

  Puzzle best = SimulatedAnnealing(maxtime);

  return best;

  // max solution
  /*Puzzle best = RandomWalk(5);
  while (timer.GetElapsedTime() + 5 < maxtime)
  {
    // Check if p is better than the best puzzle we have found so far.
    Puzzle p = RandomWalk(30);
    cout << "PUZZLE: " << p.GetValue() << endl;
    cout << "SOLUTION?: " << p.HasSolution() << endl;
    if(p.HasSolution() && p.GetValue() > best.GetValue()) {
      best = p;
    }
  }*/

}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that starts at a random configuration and keeps randomly modifying it
	// until the time limit is reached. Returns the best solution found so far.
  cout << "RandomWalk" << endl;

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.

	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;
	int bestValue = p.GetValue();

	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();

	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell
		// (since we are doing a random walk, we just replace p with its successor).
		p = p.GetRandomSuccessor();

		// Update the current best solution.
		if (p.GetValue() > bestValue)	// Calling GetValue() for the first time is costly
										// since the puzzle has to be evaluated first.
		{
			bestValue = p.GetValue();	// Calling it a second time simply returns the value that was computed before.
			bestPuzzle = p;
		}
	}

	return bestPuzzle;

	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
	vector<Puzzle> successors;
	bestPuzzle.GetAllSuccessors(successors);
}

Puzzle PuzzleGenerator::SimulatedAnnealing(double inputTime)
{
  Puzzle best(nRows, nColumns, minVal, maxVal);
  Puzzle current = best;

  while(timer.GetElapsedTime() < inputTime) {
    Puzzle next = current.GetRandomSuccessor();
    int delta = next.GetValue() - current.GetValue();

    // cout << "PUZZLE: " << next.GetValue() << endl;
    // cout << "SOLUTION?: " << next.HasSolution() << endl;

    double T = 1 - (timer.GetElapsedTime() / inputTime);
    double r = ((double) rand() / (RAND_MAX));

    if(delta > 0 || exp(delta / T) > r) {
      current = next;
      if(current.HasSolution() && current.GetValue() > best.GetValue()) {
        best = current;
      }
    }
  }

  return best;
}
