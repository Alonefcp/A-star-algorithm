#include "GameManager.h"

#include <string>
#include "Engine.h"
#include "Configuration.h"

///----------------------------------

CGameManager::CGameManager()
{
}

///----------------------------------

CGameManager::~CGameManager()
{
  if (m_pWorld)
  {
    delete m_pWorld;
    m_pWorld = nullptr;
  }
}

///----------------------------------

void CGameManager::Update(float _fTimeDiff)
{
  if (m_bPlaying)
  {
    CCell* pStart = FindStartPoint();
    CCell* pEnd = FindEndPoint();
    if (pStart && pEnd)
    {
      m_fPlayingStepTime += _fTimeDiff;
      if (m_fPlayingStepTime > 0.08f)
      {
        m_fPlayingStepTime = 0.f;
        m_oAStar.ProcessStepByStep(pStart, pEnd);
      }
    }
    else
    {
      m_bPlaying = false;
    }
  }

  if (m_fTimeToShowHeuristic > 0.f)
  {
    m_fTimeToShowHeuristic -= _fTimeDiff;
    if (m_fTimeToShowHeuristic < 0.f)
    {
      m_fTimeToShowHeuristic = 0.f;
    }
  }
}

///----------------------------------

void CGameManager::Draw()
{
  if (m_pWorld)
  {
    m_pWorld->Draw();
  }

  m_oAStar.Draw();

  // Tooltip
  bool const bShiftPressed = GetAsyncKeyState(VK_SHIFT) != 0;
  if (bShiftPressed)
  {
    if (m_pWorld)
    {
      CCell* pTooltipCell = m_pWorld->GetCell(m_iMouseCellPosX, m_iMouseCellPosY);
      if (pTooltipCell)
      {
        m_oAStar.DrawTooltip(pTooltipCell);
      }
    }
  }

  if (bShiftPressed || m_fTimeToShowHeuristic > 0.f)
  {
    std::vector<const char*> tText;

    const char* sHeuristic = m_oAStar.GetHeuristicName();
    if (strlen(sHeuristic) != 0)
    {
      static char s_sHeuristic[64];
      sprintf_s(s_sHeuristic, "Heuristic: %s", sHeuristic);
      tText.push_back(s_sHeuristic);
    }

    if (m_oAStar.GetOpenedSize() > 0 || m_oAStar.GetClosedSize() > 0)
    {
      static char s_sOpened[64];
      sprintf_s(s_sOpened, "Opened = %d", static_cast<int>(m_oAStar.GetOpenedSize()));
      tText.push_back(s_sOpened);

      static char s_sClosed[64];
      sprintf_s(s_sClosed, "Closed = %d", static_cast<int>(m_oAStar.GetClosedSize()));
      tText.push_back(s_sClosed);
    }

    CEngine::GetInstance().DrawTextBalloon(10.f, 10.f, tText);
  }
}

///----------------------------------

void CGameManager::Init()
{
  if (m_pWorld)
  {
    delete m_pWorld;
  }

  constexpr int s_iNumRows = 30;
  constexpr int s_iNumColums = 60;

  m_pWorld = new CWorld(s_iNumRows, s_iNumColums);

  CEngine::GetInstance().ResizeWindow(static_cast<int>(configuration::s_fCellSize * (s_iNumColums + 1)), static_cast<int>(configuration::s_fCellSize * (s_iNumRows + 1)));
}

///----------------------------------

void CGameManager::Reset()
{
  m_bMouseRButtonPressed = false;
  m_bMouseLButtonPressed = false;
  m_tVisitedCells.clear();
  m_bPlaying = false;
}

///----------------------------------

void CGameManager::New()
{
  Reset();
  Init();
}

///----------------------------------

void CGameManager::OnMouseLButtonDown(int _iX, int _iY)
{
  Reset();
  m_bMouseLButtonPressed = true;

  if (m_pWorld)
  {
    // Convert X,Y to cell coordinates
    int iX = _iX / static_cast<int>(configuration::s_fCellSize);
    int iY = _iY / static_cast<int>(configuration::s_fCellSize);
    CCell* pCell = m_pWorld->GetCell(iX, iY);
    if (pCell)
    {
      m_oAStar.Clean();

      bool const bControlPressed = GetAsyncKeyState(VK_CONTROL) != 0;
      if (bControlPressed)
      {
        ResetStartPoint();
        pCell->SetType(CCell::EType::Start);
      }
      else
      {
        pCell->SetType(CCell::EType::Wall);
        m_tVisitedCells.push_back(pCell);
      }
    }
  }
}

///----------------------------------

void CGameManager::OnMouseLButtonUp(int _iX, int _iY)
{
  if (m_bMouseLButtonPressed)
  {
    Reset();
  }
}

///----------------------------------

void CGameManager::OnMouseRButtonDown(int _iX, int _iY)
{
  Reset();
  m_bMouseRButtonPressed = true;

  if (m_pWorld)
  {
    // Convert X,Y to cell coordinates
    int iX = _iX / static_cast<int>(configuration::s_fCellSize);
    int iY = _iY / static_cast<int>(configuration::s_fCellSize);
    CCell* pCell = m_pWorld->GetCell(iX, iY);
    if (pCell)
    {
      m_oAStar.Clean();

      bool const bControlPressed = GetAsyncKeyState(VK_CONTROL) != 0;
      if (bControlPressed)
      {
        ResetEndPoint();
        pCell->SetType(CCell::EType::End);
      }
      else
      {
        pCell->SetType(CCell::EType::Empty);
        m_tVisitedCells.push_back(pCell);
      }
    }
  }
}

///----------------------------------

void CGameManager::OnMouseRButtonUp(int _iX, int _iY)
{
  if (m_bMouseRButtonPressed)
  {
    Reset();
  }
}

///----------------------------------

void CGameManager::OnMouseMove(int _iX, int _iY)
{
  bool const bControlPressed = GetAsyncKeyState(VK_CONTROL) != 0;
  if (bControlPressed)
  {
    Reset();
  }

  if (m_pWorld)
  {
    // Convert X,Y to cell coordinates
    m_iMouseCellPosX = _iX / static_cast<int>(configuration::s_fCellSize);
    m_iMouseCellPosY = _iY / static_cast<int>(configuration::s_fCellSize);
    CCell* pCell = m_pWorld->GetCell(m_iMouseCellPosX, m_iMouseCellPosY);
    if (pCell)
    {
      // New cell found do the behavior
      if (std::find(m_tVisitedCells.begin(), m_tVisitedCells.end(), pCell) == m_tVisitedCells.end())
      {
        if (m_bMouseLButtonPressed)
        {
          m_tVisitedCells.push_back(pCell);
          pCell->SetType(CCell::EType::Wall);
          m_oAStar.Clean();
        }
        else if (m_bMouseRButtonPressed)
        {
          m_tVisitedCells.push_back(pCell);
          pCell->SetType(CCell::EType::Empty);
          m_oAStar.Clean();
        }
      }
    }
  }
}

///----------------------------------

void CGameManager::OnCalculateButtonPressed()
{
  Reset();

  CCell* pStart = FindStartPoint();
  CCell* pEnd = FindEndPoint();
  if (pStart && pEnd)
  {
    m_oAStar.Calculate(pStart, pEnd);
  }
}

///----------------------------------

void CGameManager::OnPlayButtonPressed()
{
  if (m_bPlaying)
  {
    m_bPlaying = false;
  }
  else
  {
    CCell* pStart = FindStartPoint();
    CCell* pEnd = FindEndPoint();
    if (pStart && pEnd)
    {
      m_bPlaying = true;
      m_fPlayingStepTime = 0.f;
      m_oAStar.ProcessStepByStep(pStart, pEnd);
    }
  }
}

///----------------------------------

void CGameManager::OnStepButtonPressed()
{
  if (m_bPlaying)
  {
    m_bPlaying = false;
  }
  else
  {
    CCell* pStart = FindStartPoint();
    CCell* pEnd = FindEndPoint();
    if (pStart && pEnd)
    {
      m_oAStar.ProcessStepByStep(pStart, pEnd);
    }
  }
}

///----------------------------------

void CGameManager::OnClearButtonPressed()
{
  m_oAStar.Clean();
  Reset();
}

///----------------------------------

void CGameManager::OnNewButtonPressed()
{
  m_oAStar.Clean();
  New();
}

///----------------------------------

void CGameManager::OnChangeHeuristicButtonPressed()
{
  m_oAStar.Clean();
  Reset();
  m_oAStar.SwitchHeuristic();
  m_fTimeToShowHeuristic = 3.f;
}

///----------------------------------

void CGameManager::ResetStartPoint()
{
  for (int iX = 0; iX < m_pWorld->GetWidth(); ++iX)
  {
    for (int iY = 0; iY < m_pWorld->GetHeight(); ++iY)
    {
      CCell* pCell = m_pWorld->GetCell(iX, iY);
      if (pCell && pCell->GetType() == CCell::EType::Start)
      {
        pCell->SetType(CCell::EType::Empty);
        return;
      }
    }
  }
}

///----------------------------------

void CGameManager::ResetEndPoint()
{
  for (int iX = 0; iX < m_pWorld->GetWidth(); ++iX)
  {
    for (int iY = 0; iY < m_pWorld->GetHeight(); ++iY)
    {
      CCell* pCell = m_pWorld->GetCell(iX, iY);
      if (pCell && pCell->GetType() == CCell::EType::End)
      {
        pCell->SetType(CCell::EType::Empty);
        return;
      }
    }
  }
}

///----------------------------------

CCell* CGameManager::FindStartPoint()
{
  for (int iX = 0; iX < m_pWorld->GetWidth(); ++iX)
  {
    for (int iY = 0; iY < m_pWorld->GetHeight(); ++iY)
    {
      CCell* pCell = m_pWorld->GetCell(iX, iY);
      if (pCell && pCell->GetType() == CCell::EType::Start)
      {
        return pCell;
      }
    }
  }
  return nullptr;
}

///----------------------------------

CCell* CGameManager::FindEndPoint()
{
  for (int iX = 0; iX < m_pWorld->GetWidth(); ++iX)
  {
    for (int iY = 0; iY < m_pWorld->GetHeight(); ++iY)
    {
      CCell* pCell = m_pWorld->GetCell(iX, iY);
      if (pCell && pCell->GetType() == CCell::EType::End)
      {
        return pCell;
      }
    }
  }
  return nullptr;
}

// EOF
