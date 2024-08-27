#pragma once

#include <list>

class CCell;
class CAStarNode;

class CAStar
{
public:
  CAStar();
  virtual ~CAStar();

  void Draw();
  void DrawTooltip(const CCell* _pCell);

  bool Calculate(const CCell* _pFrom, const CCell* _pTo);
  void Clean();

  void ProcessStepByStep(const CCell* _pFrom, const CCell* _pTo);

  size_t GetOpenedSize() const { return m_tOpened.size(); }
  size_t GetClosedSize() const { return m_tClosed.size(); }

  // Return the heuristic name (for debug purposes)
  virtual const char* GetHeuristicName() = 0;
  // Callback to switch the heuristic (button presed in the GUI)
  // Just in case you want to test more than one heuristic
  virtual void SwitchHeuristic() = 0;

protected:
  // AStar step called until returns true
  virtual bool Step() = 0;

protected:
  const CCell* m_pStart = nullptr;
  const CCell* m_pEnd = nullptr;
  std::list<CAStarNode*> m_tOpened;
  std::list<CAStarNode*> m_tClosed;
};

