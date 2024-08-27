#include "AStarNode.h"

#include "Configuration.h"
#include "Engine.h"
#include "Cell.h"

///----------------------------------

CAStarNode::CAStarNode(const CCell* _pCell)
  : m_pCell(_pCell)
{
}

///----------------------------------

CAStarNode::~CAStarNode()
{
}

///----------------------------------

void CAStarNode::SetEvaluation(float _fG, float _fH)
{
  m_fG = _fG;
  m_fH = _fH;
}

///----------------------------------

void CAStarNode::Draw(float _fColorR, float _fColorG, float _fColorB)
{
  if (m_pCell)
  {
    ID2D1HwndRenderTarget* pRenderTarget = CEngine::GetInstance().GetRenderTarget();
    ID2D1SolidColorBrush* pBrush = CEngine::GetInstance().GetBrush();

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(m_pCell->GetPosX() * configuration::s_fCellSize + configuration::s_fCellMargin * 2.f, m_pCell->GetPosY() * configuration::s_fCellSize + configuration::s_fCellMargin));

    if (m_pCell->GetType() == CCell::EType::Empty)
    {
      pBrush->SetColor(D2D1::ColorF(_fColorR, _fColorG, _fColorB));
      // Draw the circle.
      D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(configuration::s_fCellFill * 0.5f, configuration::s_fCellFill * 0.5f), configuration::s_fCellFill * 0.3f, configuration::s_fCellFill * 0.3f);
      pRenderTarget->FillEllipse(ellipse, pBrush);
    }
    else if (m_pCell->GetType() == CCell::EType::Wall)
    {
      pBrush->SetColor(D2D1::ColorF(1.f, 0.f, 0.f));
      // Draw the rectangle.
      D2D1_RECT_F rect = D2D1::RectF(0.f, 0.f, configuration::s_fCellFill, configuration::s_fCellFill);
      pRenderTarget->FillRectangle(rect, pBrush);
    }
  }
}

// EOF
