#pragma once
#include <vector>
#include "Tranzition.h"

class AFNTranzition : public Tranzition
{
	std::vector<std::string> m_toStates;
public:

	AFNTranzition(std::string fromState, std::string symbol, std::vector<std::string> toStates);
	std::vector<std::string> GetToStates() const { return m_toStates; }
	void SetToStates(std::vector<std::string> toStates) { m_toStates = toStates; }

	~AFNTranzition() override = default;
};

