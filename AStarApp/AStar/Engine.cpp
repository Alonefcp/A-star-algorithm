#include "Engine.h"

#include "framework.h"
#include "Resource.h"

#include <windows.h>
#include <windowsx.h>
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")

#include "basewin.h"

#include "GameManager.h"

///----------------------------------

template <class T> void SafeRelease(T **ppT)
{
  if (*ppT)
  {
    (*ppT)->Release();
    *ppT = NULL;
  }
}

///----------------------------------

class MainWindow : public BaseWindow<MainWindow>
{
  ID2D1Factory            *pFactory;
  ID2D1HwndRenderTarget   *pRenderTarget;
  ID2D1SolidColorBrush    *pBrush;
  IDWriteTextFormat       *pTextFormat;
  IDWriteFactory          *pDWriteFactory;

  HRESULT CreateGraphicsResources();
  void DiscardGraphicsResources();
  void OnPaint();
  void Resize();
  int GetTitleBarSize() const;

public:

  MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
  {
  }

  PCWSTR  ClassName() const { return L"A* Window Class"; }
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  ID2D1HwndRenderTarget* GetRenderTarget() { return pRenderTarget; }
  ID2D1SolidColorBrush* GetBrush() { return pBrush; }
  IDWriteTextFormat* GetTextFormat() { return pTextFormat; }
  void Resize(int _iX, int _iY);
};


#include <windows.h>
#include <CommCtrl.h>

///----------------------------------

HRESULT MainWindow::CreateGraphicsResources()
{
  static const WCHAR msc_fontName[] = L"Consolas";
  static const FLOAT msc_fontSize = 10;

  HRESULT hr = S_OK;
  if (pRenderTarget == NULL)
  {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    hr = pFactory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(m_hwnd, size),
      &pRenderTarget);

    if (SUCCEEDED(hr))
    {
      const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
      hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
    }
    if (SUCCEEDED(hr))
    {

      // Create a DirectWrite factory.
      hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(pDWriteFactory),
        reinterpret_cast<IUnknown **>(&pDWriteFactory)
      );
    }
    if (SUCCEEDED(hr))
    {
      // Create a DirectWrite text format object.
      hr = pDWriteFactory->CreateTextFormat(
        msc_fontName,
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        msc_fontSize,
        L"", //locale
        &pTextFormat
      );
    }
    if (SUCCEEDED(hr))
    {
      // Center the text horizontally and vertically.
      pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

      pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }
  }
  return hr;
}

///----------------------------------

void MainWindow::DiscardGraphicsResources()
{
  SafeRelease(&pRenderTarget);
  SafeRelease(&pBrush);
}

///----------------------------------

void MainWindow::OnPaint()
{
  HRESULT hr = CreateGraphicsResources();
  if (SUCCEEDED(hr))
  {
    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);

    pRenderTarget->BeginDraw();

    pRenderTarget->Clear(D2D1::ColorF(0.9f, 0.9f, 0.9f));

    CGameManager::GetInstance().Draw();

    hr = pRenderTarget->EndDraw();
    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
      DiscardGraphicsResources();
    }
    EndPaint(m_hwnd, &ps);
  }
}

///----------------------------------

void MainWindow::Resize()
{
  if (pRenderTarget != NULL)
  {
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    pRenderTarget->Resize(size);
    InvalidateRect(m_hwnd, NULL, FALSE);
  }
}

///----------------------------------

void MainWindow::Resize(int _iX, int _iY)
{
  HRESULT hr = CreateGraphicsResources();
  if (SUCCEEDED(hr))
  {
    if (pRenderTarget != NULL)
    {
      SetWindowPos(m_hwnd, 0, 0, 0, _iX, _iY + GetTitleBarSize(), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
      InvalidateRect(m_hwnd, NULL, FALSE);
    }
  }
}

///----------------------------------

int MainWindow::GetTitleBarSize() const
{
  RECT wrect;
  GetWindowRect(m_hwnd, &wrect);
  RECT crect;
  GetClientRect(m_hwnd, &crect);
  POINT lefttop = { crect.left, crect.top }; // Practicaly both are 0
  ClientToScreen(m_hwnd, &lefttop);
  POINT rightbottom = { crect.right, crect.bottom };
  ClientToScreen(m_hwnd, &rightbottom);

  //int left_border = lefttop.x - wrect.left; // Windows 10: includes transparent part
  //int right_border = wrect.right - rightbottom.x; // As above
  int bottom_border = wrect.bottom - rightbottom.y; // As above
  int top_border_with_title_bar = lefttop.y - wrect.top; // There is no transparent part

  return top_border_with_title_bar - bottom_border;
}

///----------------------------------

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
    {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}

///----------------------------------

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_LBUTTONDOWN:
  {
    int iX = GET_X_LPARAM(lParam);
    int iY = GET_Y_LPARAM(lParam);
    CGameManager::GetInstance().OnMouseLButtonDown(iX, iY);
  }
  break;
  case WM_LBUTTONUP:
  {
    int iX = GET_X_LPARAM(lParam);
    int iY = GET_Y_LPARAM(lParam);
    CGameManager::GetInstance().OnMouseLButtonUp(iX, iY);
  }
  break;
  case WM_RBUTTONDOWN:
  {
    int iX = GET_X_LPARAM(lParam);
    int iY = GET_Y_LPARAM(lParam);
    CGameManager::GetInstance().OnMouseRButtonDown(iX, iY);
  }
  break;
  case WM_RBUTTONUP:
  {
    int iX = GET_X_LPARAM(lParam);
    int iY = GET_Y_LPARAM(lParam);
    CGameManager::GetInstance().OnMouseRButtonUp(iX, iY);
  }
  break;
  case WM_MOUSEMOVE:
  {
    int iX = GET_X_LPARAM(lParam);
    int iY = GET_Y_LPARAM(lParam);
    CGameManager::GetInstance().OnMouseMove(iX, iY);
  }
  break;
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    // Parse the menu selections:
    switch (wmId)
    {
    case IDM_HELP:
      DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), m_hwnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(m_hwnd);
      break;
    case IDM_CALCULATE:
      CGameManager::GetInstance().OnCalculateButtonPressed();
      break;
    case IDM_STEP:
      CGameManager::GetInstance().OnStepButtonPressed();
      break;
    case IDM_CLEAR:
      CGameManager::GetInstance().OnClearButtonPressed();
      break;
    case IDM_PLAY:
      CGameManager::GetInstance().OnPlayButtonPressed();
      break;
    case IDM_NEW:
      CGameManager::GetInstance().OnNewButtonPressed();
      break;
    case IDM_CHANGE_HEURISTIC:
      CGameManager::GetInstance().OnChangeHeuristicButtonPressed();
      break;
    default:
      return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
  }
  return 0;

  case WM_CREATE:
    if (FAILED(D2D1CreateFactory(
      D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
    {
      return -1;  // Fail CreateWindowEx.
    }
    return 0;

  case WM_DESTROY:
    DiscardGraphicsResources();
    SafeRelease(&pFactory);
    PostQuitMessage(0);
    return 0;

  case WM_PAINT:
    OnPaint();
    return 0;

    // Other messages not shown...

  case WM_SIZE:
    Resize();
    return 0;
  }
  return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

///----------------------------------

namespace internal_CEngine
{
  MainWindow s_oMainWindow;
}

///----------------------------------

CEngine::CEngine()
{
}

///----------------------------------

CEngine::~CEngine()
{
}

///----------------------------------

int CEngine::Init(int _nCmdShow)
{
  if (!internal_CEngine::s_oMainWindow.Create(L"A*", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME))
  {
    return 0;
  }

  ShowWindow(internal_CEngine::s_oMainWindow.Window(), _nCmdShow);

  return 1;
}

///----------------------------------

ID2D1HwndRenderTarget* CEngine::GetRenderTarget()
{
  return internal_CEngine::s_oMainWindow.GetRenderTarget();
}

///----------------------------------

ID2D1SolidColorBrush* CEngine::GetBrush()
{
  return internal_CEngine::s_oMainWindow.GetBrush();
}

///----------------------------------

IDWriteTextFormat* CEngine::GetTextFormat()
{
  return internal_CEngine::s_oMainWindow.GetTextFormat();
}

///----------------------------------

void CEngine::ResizeWindow(int _iX, int _iY)
{
  internal_CEngine::s_oMainWindow.Resize(_iX, _iY);
}

///----------------------------------

void CEngine::DrawTextBalloon(float _fX, float _fY, std::vector<const char*> _tText)
{
  if (_tText.size() == 0) return;

  ID2D1HwndRenderTarget* pRenderTarget = GetRenderTarget();
  ID2D1SolidColorBrush* pBrush = GetBrush();
  IDWriteTextFormat* pTextFormat = GetTextFormat();
  pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(_fX, _fY));

  constexpr float fBorderSize = 5.f;
  constexpr float fTextSeparation = 4.f;
  constexpr float fTextSizeY = 10.f;

  unsigned int uTextLen(0u);
  for (const char* sText : _tText)
  {
    uTextLen = max(uTextLen, static_cast<unsigned int>(strlen(sText)));
  }
  float fRectSizeX = uTextLen * 7.f + fBorderSize * 2.f;
  float fRectSizeY = fBorderSize * 2.f + (fTextSizeY + fTextSeparation) * _tText.size();

  pBrush->SetColor(D2D1::ColorF(0.7f, 0.7f, 0.4f));
  D2D1_RECT_F rect2 = D2D1::RectF(-1, -1, fRectSizeX + 1, fRectSizeY + 1);
  D2D1_ROUNDED_RECT roundedRect2 = D2D1::RoundedRect(rect2, 5, 5);
  pRenderTarget->FillRoundedRectangle(roundedRect2, pBrush);

  pBrush->SetColor(D2D1::ColorF(1.f, 1.f, 0.9f));
  D2D1_RECT_F rect = D2D1::RectF(0, 0, fRectSizeX, fRectSizeY);
  D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, fBorderSize, fBorderSize);
  pRenderTarget->FillRoundedRectangle(roundedRect, pBrush);

  for (unsigned int uIndex = 0; uIndex < _tText.size(); ++uIndex)
  {
    static WCHAR s_sText[256];

    pBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
    rect = D2D1::RectF(fBorderSize * 2.f, fBorderSize + uIndex * (fTextSizeY + fTextSeparation), fRectSizeX, fBorderSize + (uIndex + 1) * (fTextSizeY + fTextSeparation));
    wsprintf(s_sText, L"%S", _tText[uIndex]);
    pRenderTarget->DrawText(
      s_sText,
      static_cast<unsigned int>(wcslen(s_sText)),
      pTextFormat,
      rect,
      pBrush
    );
  }
};

// EOF
