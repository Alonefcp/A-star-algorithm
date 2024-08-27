#include "Cell.h"
#include "Engine.h"
#include "Configuration.h"

///----------------------------------

void CCell::SetPos(int _iX, int _iY)
{
  m_iPosX = _iX;
  m_iPosY = _iY;
}

///----------------------------------

void CCell::Draw()
{
  if (!IsValid()) return;

  ID2D1HwndRenderTarget* pRenderTarget = CEngine::GetInstance().GetRenderTarget();
  ID2D1SolidColorBrush* pBrush = CEngine::GetInstance().GetBrush();

  pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(m_iPosX * configuration::s_fCellSize + configuration::s_fCellMargin * 2.f, m_iPosY * configuration::s_fCellSize + configuration::s_fCellMargin));

  constexpr float s_fNoHiglightColor = 0.8f;

  if (m_eType == CCell::EType::Start ||
    m_eType == CCell::EType::End)
  {
  }

  if (m_eType == CCell::EType::Empty ||
    m_eType == CCell::EType::Wall)
  {
    switch (m_eType)
    {
    case CCell::EType::Empty:
      pBrush->SetColor(D2D1::ColorF(1.f, 1.f, 1.f));
      break;
    case CCell::EType::Wall:
      pBrush->SetColor(D2D1::ColorF(0.3f, 0.3f, 0.3f));
      break;
    }

    // Draw the rectangle.
    D2D1_RECT_F rect = D2D1::RectF(0.f, 0.f, configuration::s_fCellFill, configuration::s_fCellFill);
    pRenderTarget->FillRectangle(rect, pBrush);
  }
  else
  {
    pBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
    // Draw the border.
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(configuration::s_fCellFill * 0.5f, configuration::s_fCellFill * 0.5f), configuration::s_fCellFill * 0.5f, configuration::s_fCellFill * 0.5f);
    pRenderTarget->FillEllipse(ellipse, pBrush);

    switch (m_eType)
    {
    case CCell::EType::Start:
      pBrush->SetColor(D2D1::ColorF(0.f, s_fNoHiglightColor, 0.f));
      break;
    case CCell::EType::End:
      pBrush->SetColor(D2D1::ColorF(s_fNoHiglightColor, 0.f, 0.f));
      break;
    }
    // Draw the circle.
    ellipse = D2D1::Ellipse(D2D1::Point2F(configuration::s_fCellFill * 0.5f, configuration::s_fCellFill * 0.5f), configuration::s_fCellFill * 0.5f - 2.f, configuration::s_fCellFill * 0.5f - 2.f);
    pRenderTarget->FillEllipse(ellipse, pBrush);
  }
}

// EOF
