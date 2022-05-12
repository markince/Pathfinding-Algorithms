// File:	      Search.hpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019
//
// Interface for all search classes
//

#pragma once

#include "Definitions.hpp" // type definitions

// ISearch interface class - cannot be instantiated
// Implementation classes for specific search algorithms should inherit from this interface
class ISearch
{
public:
  // Interfaces don't need a constructor - makes sense really, given they cannot be instantiated.

  // They DO need a virtual destructor if there are other virtual functions in the interface.
  // This compels derived objects destructors to be called as appropriate.
  // However, at the interface level, there is nothing to do, so the body is empty.
  virtual ~ISearch() {}

  // PATH FINDING FUNCTIONS
  // ----------------------

  // Constructs the path from start to goal for the given terrain
  // Pure Virtual function to be implemented in derived class.
  virtual bool FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path, int xMapSize, int yMapSize) = 0;

};