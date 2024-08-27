#pragma once

#include <vector>
#include "Cell.h"

class CWorld
{
public:
  CWorld(unsigned int _uRows, unsigned int _uColumns);

  void Draw();

  CCell* GetCell(int _iX, int _iY);
  int GetHeight() const { return m_iHeight; }
  int GetWidth() const { return m_iWidth; }

  bool GetCellNeighbors(const CCell* _pCell, std::vector<CCell*>& tNeighbors_);
  bool GetCellNeighbors(int _iX, int _iY, std::vector<CCell*>& tNeighbors_);

private:
  int m_iHeight = -1;
  int m_iWidth = -1;
  std::vector<std::vector<CCell> > m_tCells;
};

