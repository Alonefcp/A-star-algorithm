#pragma once

#include "Singleton.h"
#include "World.h"
#include "MyAStar.h"

class CGameManager : public CSingleton<CGameManager>
{
public:
  CGameManager();
  ~CGameManager();

  void Update(float _fTimeDiff);
  void Draw();

  void Init();

  CWorld* GetWorld() const { return m_pWorld; }

  void OnMouseLButtonDown(int _iX, int _iY);
  void OnMouseLButtonUp(int _iX, int _iY);
  void OnMouseRButtonDown(int _iX, int _iY);
  void OnMouseRButtonUp(int _iX, int _iY);
  void OnMouseMove(int _iX, int _iY);

  void OnCalculateButtonPressed();
  void OnPlayButtonPressed();
  void OnStepButtonPressed();
  void OnClearButtonPressed();
  void OnNewButtonPressed();
  void OnChangeHeuristicButtonPressed();

private:
  void ResetStartPoint();
  void ResetEndPoint();

  CCell* FindStartPoint();
  CCell* FindEndPoint();

  void Reset();
  void New();

private:
  CWorld* m_pWorld = nullptr;
  CMyAStar m_oAStar;

  bool m_bMouseLButtonPressed = false;
  bool m_bMouseRButtonPressed = false;
  std::vector<CCell*> m_tVisitedCells;

  bool m_bPlaying = false;
  float m_fPlayingStepTime = 0.f;
  int m_iMouseCellPosX = 0;
  int m_iMouseCellPosY = 0;

  float m_fTimeToShowHeuristic = 0.f;
};