#include "AStar.h"

#include <algorithm>
#include "Configuration.h"
#include "AStarNode.h"
#include "GameManager.h"
#include "Cell.h"
#include "World.h"
#include "Engine.h"

///----------------------------------

CAStar::CAStar()
{
}

///----------------------------------

CAStar::~CAStar()
{
}

///----------------------------------

void CAStar::Draw()
{
  int iNumOpened = static_cast<int>(m_tOpened.size());
  if (iNumOpened > 0)
  {
    constexpr float s_fColorFactor(1.f);
    float fInc = 1.f / static_cast<float>(iNumOpened);
    float fG(1.f);
    for (CAStarNode* pNode : m_tOpened)
    {
      pNode->Draw(0.f, fG * s_fColorFactor, s_fColorFactor);

      // Calculate color for next node
      fG -= fInc;
      if (fG < 0.f)
      {
        fG = 0.f;
      }
    }
  }
  for (CAStarNode* pNode : m_tClosed)
  {
    pNode->Draw(0.6f, 0.6f, 0.6f);
  }

  if (m_tOpened.size() > 0)
  {
    CAStarNode* pCurrentNode = *m_tOpened.begin();
    if (pCurrentNode)
    {
      pCurrentNode->Draw(1.f, 1.f, 0.f);

      pCurrentNode = pCurrentNode->GetPrevNode();
      while (pCurrentNode)
      {
        pCurrentNode->Draw(0.3f, 1.f, 0.3f);
        pCurrentNode = pCurrentNode->GetPrevNode();
      }
    }
  }
}

///----------------------------------

void CAStar::DrawTooltip(const CCell* _pCell)
{
  CAStarNode* pTooltipNode(nullptr);
  for (CAStarNode* pNode : m_tOpened)
  {
    if (pNode->GetCell() == _pCell)
    {
      pTooltipNode = pNode;
      break;
    }
  }
  if (pTooltipNode == nullptr)
  {
    for (CAStarNode* pNode : m_tClosed)
    {
      if (pNode->GetCell() == _pCell)
      {
        pTooltipNode = pNode;
        break;
      }
    }
  }  

  if (pTooltipNode)
  {
    static char s_sG[32];
    static char s_sH[32];
    static char s_sEval[32];
    sprintf_s(s_sG, "G = %.2f", pTooltipNode->GetG());
    sprintf_s(s_sH, "H = %.2f", pTooltipNode->GetH());
    sprintf_s(s_sEval, "E = %.2f", pTooltipNode->GetEvaluation());

    std::vector<const char*> tText;
    tText.push_back(s_sG);
    tText.push_back(s_sH);
    tText.push_back(s_sEval);

    CEngine::GetInstance().DrawTextBalloon((_pCell->GetPosX() + 1) * configuration::s_fCellSize + configuration::s_fCellMargin * 2.f, (_pCell->GetPosY() + 1) * configuration::s_fCellSize + configuration::s_fCellMargin, tText);
  }
}

///----------------------------------

void CAStar::Clean()
{
  for (CAStarNode* pNode : m_tOpened)
  {
    delete pNode;
  }
  m_tOpened.clear();
  for (CAStarNode* pNode : m_tClosed)
  {
    delete pNode;
  }
  m_tClosed.clear();
}

///----------------------------------

bool CAStar::Calculate(const CCell* _pFrom, const CCell* _pTo)
{
  Clean();

  m_pStart = _pFrom;
  m_pEnd = _pTo;

  while (Step());

  if (m_tOpened.size() > 0)
  {
    return (*m_tOpened.begin())->GetCell() == _pTo;
  }
  return false;
}

///----------------------------------

void CAStar::ProcessStepByStep(const CCell* _pFrom, const CCell* _pTo)
{
  if ((m_tOpened.size() == 0 && m_tClosed.size() == 0) || m_pStart != _pFrom || m_pEnd != _pTo)
  {
    Clean();

    m_pStart = _pFrom;
    m_pEnd = _pTo;
  }

  Step();
}

// EOF
