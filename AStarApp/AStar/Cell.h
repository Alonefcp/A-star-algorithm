#pragma once

class CCell
{
public:
  enum class EType
  {
    Empty,
    Wall,
    Start,
    End
  };

public:
  void Draw();

  bool IsValid() const { return m_iPosX >= 0 && m_iPosY >= 0; }

  void SetPos(int _iX, int _iY);
  int GetPosX() const { return m_iPosX; }
  int GetPosY() const { return m_iPosY; }
  void GetPos(int& iX_, int& iY_) const { iX_ = m_iPosX; iY_ = m_iPosY; }

  EType GetType() const { return m_eType; }
  void SetType(EType _eType) { m_eType = _eType; }

private:
  int m_iPosX = -1;
  int m_iPosY = -1;
  EType m_eType = EType::Empty;
};
