#pragma once

#include "AStar.h"

class CMyAStar : public CAStar
{
public:
  CMyAStar();
  virtual ~CMyAStar();

  // Return the heuristic name (for debug purposes)
  virtual const char* GetHeuristicName() override;
  // Callback to switch the heuristic (button presed in the GUI)
  // Just in case you want to test more than one heuristic
  virtual void SwitchHeuristic() override;

protected:
  // AStar step called until returns true
  virtual bool Step() override;

private:
  enum class Heuristic {Euclidean, Manhattan};

  Heuristic m_heuristic;

  float CalculateH(const CCell* cell) const;
};

