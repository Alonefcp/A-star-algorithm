#include "MyAStar.h"

#include <algorithm>
#include "Configuration.h"
#include "AStarNode.h"
#include "GameManager.h"
#include "Cell.h"
#include "World.h"
#include "Engine.h"

///----------------------------------

CMyAStar::CMyAStar():m_heuristic(Heuristic::Euclidean)
{
}

///----------------------------------

CMyAStar::~CMyAStar()
{
}

///----------------------------------

void CMyAStar::SwitchHeuristic()
{
	if (m_heuristic == Heuristic::Euclidean)
	{
		m_heuristic = Heuristic::Manhattan;
	}
	else if (m_heuristic == Heuristic::Manhattan)
	{
		m_heuristic = Heuristic::Euclidean;
	}
	else
	{
		m_heuristic = Heuristic::Euclidean;
	}
}

///----------------------------------

const char* CMyAStar::GetHeuristicName()
{
	if (m_heuristic == Heuristic::Euclidean)
	{
		return "Euclidean";
	}
	else if (m_heuristic == Heuristic::Manhattan)
	{
		return "Manhattan";
	}
	else
	{
		return "Euclidean";
	}
}

///----------------------------------

bool CMyAStar::Step()
{
	CWorld* world = CGameManager::GetInstance().GetWorld();

	if (world && m_pEnd && m_pStart)
	{
		//We get the start node and add it to the opened list
		if (m_tOpened.size() == 0 && m_tClosed.size() == 0)
		{
			CAStarNode* startNode = new CAStarNode(m_pStart);
			startNode->SetEvaluation(0.0f, CalculateH(startNode->GetCell()));
			m_tOpened.push_back(startNode);
		}
		else if(m_tOpened.size() > 0)
		{
			CAStarNode* currentNode = *m_tOpened.begin();

			//Path found
			if (currentNode->GetCell() == m_pEnd)
			{
				return false;
			}
			else
			{
				//m_tOpened.remove(currentNode);
				m_tOpened.pop_front();
				m_tClosed.push_back(currentNode);

				if (currentNode && currentNode->GetCell())
				{
					std::vector<CCell*> neighbors(4);
					world->GetCellNeighbors(currentNode->GetCell(), neighbors);

					for (CCell* neighborCell : neighbors)
					{
						auto lambdaCompareNode = [neighborCell](CAStarNode* _pNodeA)
						{
							if (_pNodeA->GetCell() == neighborCell)
							{
								return true;
							}
							return false;
						};

						//If the neighbor node is not in the closed list and we add it to the opened list
						if (std::find_if(m_tClosed.begin(), m_tClosed.end(), lambdaCompareNode) == m_tClosed.end())
						{
							//Calculate H and G
							float hCost = CalculateH(neighborCell);
							float gCost = currentNode->GetG();

							//Add the distance to the node
							float posX = std::abs(currentNode->GetCell()->GetPosX() - neighborCell->GetPosX());
							float posY = std::abs(currentNode->GetCell()->GetPosY() - neighborCell->GetPosY());
							float distance = sqrt(posX * posX + posY * posY);

							gCost += distance;

							auto it = std::find_if(m_tOpened.begin(), m_tOpened.end(), lambdaCompareNode);

							//The node is not in the opened list
							if (it == m_tOpened.end())
							{
								CAStarNode* newNode = new CAStarNode(neighborCell);
								newNode->SetPrevNode(currentNode);
								newNode->SetEvaluation(gCost, hCost);
								m_tOpened.push_front(newNode);
							}
							else //The node is in the opened list
							{
								CAStarNode* foundNode = new CAStarNode((*it)->GetCell());

								if (foundNode->GetEvaluation() > gCost + hCost)
								{
									foundNode->SetPrevNode(currentNode);
									foundNode->SetEvaluation(gCost, hCost);
								}
							}
						}

						// Sort opened list to select the better the next step
						m_tOpened.sort([](auto& _pNodeA, auto& _pNodeB) -> bool
						{
							if (_pNodeA->GetEvaluation() < _pNodeB->GetEvaluation())
							{
								return true;
							}
							else if (_pNodeA->GetEvaluation() == _pNodeB->GetEvaluation() && _pNodeA->GetG() > _pNodeB->GetG())
							{
								return true;
							}
							return false;
						});
					}
				}
			}

		}

		return m_tOpened.size() > 0;
	}


  return false;
}

float CMyAStar::CalculateH(const CCell* cell) const
{
	float distance = 0.0f;

	switch (m_heuristic)
	{
	case Heuristic::Euclidean:
	{

		float posX = std::abs(cell->GetPosX() - m_pEnd->GetPosX());
		float posY = std::abs(cell->GetPosY() - m_pEnd->GetPosY());

		distance = sqrt(posX * posX + posY * posY);

	}
		break;

	case Heuristic::Manhattan:
	{
		float posX = std::abs(cell->GetPosX() - m_pEnd->GetPosX());
		float posY = std::abs(cell->GetPosY() - m_pEnd->GetPosY());

		distance = posX + posY;
	}

		break;
	default:
		break;
	}

	return distance;
}

// EOF
