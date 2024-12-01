#pragma once
#include <string>
#include <vector>

class Tranzition
{
	std::string m_fromState;
	std::string m_symbol;
	std::string m_toState;
	bool m_isInitial{ false };
	bool m_isFinal{ false };

public:
	Tranzition(std::string fromState, std::string symbol, std::string toState);

	void SetInitial(bool isInitial) { m_isInitial = isInitial; }
	void SetFinal(bool isFinal) { m_isFinal = isFinal; }

	std::string GetFromState() const { return m_fromState; }
	void SetFromState(std::string fromoState) { m_fromState = fromoState; }
	std::string GetToState() const { return m_toState; }
	void SetToState(std::string toState) { m_toState = toState; }
	std::string GetSymbol() const { return m_symbol; }
	bool IsInitial() const { return m_isInitial; }
	bool IsFinal() const { return m_isFinal; }
	void SetSymbol(std::string symbol) { m_symbol = symbol; }

	virtual ~Tranzition() = default;
};

