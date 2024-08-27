#pragma once

#include <assert.h>

template < class T >
class CSingleton
{
public:
  virtual ~CSingleton() {}

  static T& CreateSingleton()
  {
    assert(!s_bDestroyed && "The singleton is already destroyed, this will possibly create a memory leak!");
    assert(!s_pSingleton);
    if (!s_pSingleton)
    {
      s_pSingleton = new T();
    }
    return *s_pSingleton;
  }

  static void DestroySingleton()
  {
    assert(s_pSingleton);
    if (s_pSingleton)
    {
      delete s_pSingleton;
      s_pSingleton = nullptr;
    }

    s_bDestroyed = true;
  }

  static bool HasInstance()
  {
    return s_pSingleton != nullptr;
  }

  static T& GetInstance() 
  {
    assert(s_pSingleton);
    return *s_pSingleton;
  }

  static T* GetInstancePtr() 
  { 
    assert(s_pSingleton);
    return s_pSingleton;
  }

  static T* TryGetInstancePtr()
  {
    return s_pSingleton;
  }

private:
  static T *s_pSingleton;
  // Indicates if the singleton is already destroyed (call to DestroySingleton)
  static bool s_bDestroyed;
};

template < class T >
T *CSingleton<T>::s_pSingleton = nullptr;
template < class T >
bool CSingleton<T>::s_bDestroyed = false;
