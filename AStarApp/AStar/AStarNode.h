#pragma once

class CCell;

class CAStarNode
{
public:
  CAStarNode(const CCell* _pCell);
  ~CAStarNode();

  const CCell* GetCell() const { return m_pCell; }

  void Draw(float _fColorR, float _fColorG, float _fColorB);

  CAStarNode* GetPrevNode() const { return m_pPrevNode; }
  void SetPrevNode(CAStarNode* _value) { m_pPrevNode = _value; }

  void SetEvaluation(float _fG, float _fH);
  float GetEvaluation() const { return m_fG + m_fH; }
  float GetG() const { return m_fG; }
  float GetH() const { return m_fH; }

private:
  const CCell* m_pCell;
  CAStarNode* m_pPrevNode = nullptr;

  float m_fG = 0.f;
  float m_fH = 0.f;
};

