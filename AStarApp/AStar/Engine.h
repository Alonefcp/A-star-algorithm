#pragma once

#include "Singleton.h"
#include <vector>
#include "d2d1.h"

///----------------------------------

class CEngine : public CSingleton<CEngine>
{
public:
  CEngine();
  ~CEngine();

  int Init(int _nCmdShow);

  ID2D1HwndRenderTarget* GetRenderTarget();
  ID2D1SolidColorBrush* GetBrush();
  IDWriteTextFormat* GetTextFormat();
  void ResizeWindow(int _iX, int _iY);

  void DrawTextBalloon(float _fX, float _fY, std::vector<const char*> _tText);
};