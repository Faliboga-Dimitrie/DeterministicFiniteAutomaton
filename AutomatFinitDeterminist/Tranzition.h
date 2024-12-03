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
	Tranzition(const std::string& fromState, const std::string& symbol, const std::string& toState);

	void SetInitial(bool isInitial) { m_isInitial = isInitial; }
	void SetFinal(bool isFinal) { m_isFinal = isFinal; }

	const std::string& GetFromState() const { return m_fromState; }
	const std::string& GetToState() const { return m_toState; }
	const std::string& GetSymbol() const { return m_symbol; }

	void SetFromState(const std::string& fromoState) { m_fromState = fromoState; }
	void SetToState(const std::string& toState) { m_toState = toState; }

	bool IsInitial() const { return m_isInitial; }
	bool IsFinal() const { return m_isFinal; }
	void SetSymbol(const std::string& symbol) { m_symbol = symbol; }

	virtual ~Tranzition() = default;
};

