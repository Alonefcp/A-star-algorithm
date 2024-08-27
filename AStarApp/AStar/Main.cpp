// AStar.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "basewin.h"
#include "Engine.h"
#include "GameManager.h"
#include "Stopwatch.h"

///----------------------------------

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR    lpCmdLine,
  _In_ int       nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // Init - Engine
  CEngine::CreateSingleton();
  if (!CEngine::GetInstance().Init(nCmdShow))
  { 
    return false;
  }
  // Init - GameManager
  CGameManager::CreateSingleton();
  CGameManager::GetInstance().Init();

  win32::Stopwatch sw;
  sw.Start();
  double dPrevElapsedTime(0.0);

  // Main message loop:
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    double dElapsedTime(sw.ElapsedMilliseconds());
    float fTimeDiff(static_cast<float>(dElapsedTime - dPrevElapsedTime) / 1000.f);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTAR));
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      CGameManager::GetInstance().Update(fTimeDiff);

      InvalidateRect(msg.hwnd, NULL, FALSE);
    }

    dPrevElapsedTime = dElapsedTime;
  }

  // End
  CGameManager::DestroySingleton();
  CEngine::DestroySingleton();

  return (int)msg.wParam;

}

// EOF
