#pragma once
#include <vector>
#include "Transition.h"

class AFNTransition : public Transition
{
	std::vector<std::string> m_toStates;
public:

	AFNTransition(std::string fromState, std::string symbol, std::vector<std::string> toStates);
	std::vector<std::string> GetToStates() const { return m_toStates; }
	void SetToStates(std::vector<std::string> toStates) { m_toStates = toStates; }

	~AFNTransition() override = default;
};

