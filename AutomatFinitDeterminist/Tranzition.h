#pragma once
#include <string>
#include <vector>

class Tranzition
{
	std::string m_fromState;
	std::string m_symbol;
	bool m_isInitial{ false };
	bool m_isFinal{ false };

public:
	Tranzition(std::string fromState, std::string symbol);

	void SetInitial(bool isInitial) { m_isInitial = isInitial; }
	void SetFinal(bool isFinal) { m_isFinal = isFinal; }

	std::string GetFromState() const { return m_fromState; }
	std::string GetSymbol() const { return m_symbol; }
	bool IsInitial() const { return m_isInitial; }
	bool IsFinal() const { return m_isFinal; }

	virtual ~Tranzition() = default;
};

