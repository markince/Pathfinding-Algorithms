// File:	      SearchFactory.hpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019
//
// Factory function declaration to create CSearchXXX objects 
//

#pragma once

#include "Search.hpp" // Search interface class

// List of implemented seach algorithms
enum ESearchType
{
	BreadthFirst,
	DepthFirst,
	HillClimbing,
	BestFirst,
    AStar
};

// Factory function to create CSearchXXX object where XXX is the given search type
ISearch* NewSearch(ESearchType search);
