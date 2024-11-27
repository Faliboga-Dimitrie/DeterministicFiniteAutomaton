#pragma once
#include "Transition.h"

class AFDTransition : public Transition
{
	std::string m_toState;
public:
	
	AFDTransition(std::string fromState, std::string symbol, std::string toState);

	std::string GetToState() const { return m_toState; }
	void SetToState(std::string toState) { m_toState = toState; }

	~AFDTransition() override = default;
};

