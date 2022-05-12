// File:	      SearchFactory.cpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019

// Factory function implementation to create CSearchXXX objects 
//

#include "CSearchBreadthFirst.hpp"  // Breadth-first search class
#include "CSearchDepthFirst.hpp"    // Depth-first search class
#include "CSearchHillClimbing.hpp"  // Hill-climbing search class
#include "CSearchBestFirst.hpp"     // Best-first search class
#include "CSearchAStar.hpp"         // A* search class
#include "SearchFactory.hpp"        // Factory declarations


// Create new search object of the given type and return a pointer to it.
// Note the returned pointer type is the base class. This is how we implement polymorphism.
ISearch* NewSearch(ESearchType search)
{
  switch (search)
  {
	case BreadthFirst:
	{
		return new CSearchBreadthFirst();
	}
	case DepthFirst:
	{
		return new CSearchDepthFirst();
	}
	case HillClimbing:
	{
		return new CSearchHillClimbing();
	}
	case BestFirst:
	{
		return new CSearchBestFirst();
	}
	case AStar:
	{
		return new CSearchAStar();
	}
  } // End of switch statment

} // End of ISearch
