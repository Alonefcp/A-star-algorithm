#include "World.h"
#include "Configuration.h"

///----------------------------------

CWorld::CWorld(unsigned int _uRows, unsigned int _uColumns)
  : m_iHeight(_uRows)
  , m_iWidth(_uColumns)
  , m_tCells(_uColumns)
{
  for (unsigned int uColumn = 0; uColumn < _uColumns; ++uColumn)
  {
    m_tCells[uColumn] = std::vector<CCell>(_uRows);
    for (unsigned int uRow = 0; uRow < _uRows; ++uRow)
    {
      m_tCells[uColumn][uRow] = CCell();
      m_tCells[uColumn][uRow].SetPos(uColumn, uRow);
    }
  }
}

///----------------------------------

void CWorld::Draw()
{
  for (std::vector<CCell>& tColumn : m_tCells)
  {
    for (CCell& rCell : tColumn)
    {
      rCell.Draw();
    }
  }
}

///----------------------------------

CCell* CWorld::GetCell(int _iX, int _iY)
{
  if (_iX >= 0 && _iX < static_cast<int>(m_tCells.size()))
  {
    if (_iY >= 0 && _iY < static_cast<int>(m_tCells[_iX].size()))
    {
      return &m_tCells[_iX][_iY];
    }
  }
  return nullptr;
}

///----------------------------------

bool CWorld::GetCellNeighbors(const CCell* _pCell, std::vector<CCell*>& tNeighbors_)
{
  tNeighbors_.clear();

  // Valid cell?
  if (_pCell != nullptr && _pCell->IsValid() && _pCell->GetPosX() < m_iWidth && _pCell->GetPosY() < m_iHeight)
  {
    if (configuration::s_eNeighborsType == configuration::ENeighborsType::Cross || configuration::s_eNeighborsType == configuration::ENeighborsType::All)
    {
      if (CCell* pCell = GetCell(_pCell->GetPosX() - 1, _pCell->GetPosY()))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
      if (CCell* pCell = GetCell(_pCell->GetPosX() + 1, _pCell->GetPosY()))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
      if (CCell* pCell = GetCell(_pCell->GetPosX(), _pCell->GetPosY() + 1))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
      if (CCell* pCell = GetCell(_pCell->GetPosX(), _pCell->GetPosY() - 1))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
    }

    if (configuration::s_eNeighborsType == configuration::ENeighborsType::All)
    {
      if (CCell* pCell = GetCell(_pCell->GetPosX() - 1, _pCell->GetPosY() - 1))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
      if (CCell* pCell = GetCell(_pCell->GetPosX() + 1, _pCell->GetPosY() + 1))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
      if (CCell* pCell = GetCell(_pCell->GetPosX() - 1, _pCell->GetPosY() + 1))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
      if (CCell* pCell = GetCell(_pCell->GetPosX() + 1, _pCell->GetPosY() - 1))
      {
        if (pCell->GetType() != CCell::EType::Wall)
        {
          tNeighbors_.push_back(pCell);
        }
      }
    }
  }
  return false;
}

///----------------------------------

bool CWorld::GetCellNeighbors(int _iX, int _iY, std::vector<CCell*>& tNeighbors_)
{
  return GetCellNeighbors(GetCell(_iX, _iY), tNeighbors_);
}

// EOF
