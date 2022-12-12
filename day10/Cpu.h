#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

class Cpu
{
public:
	Cpu(const std::vector<std::string>& opcodes);
	void Reset();

	void RunTo(int cycle);
	__int64 x() { return m_x; }

	std::string State();

private:
	void DoNoop(const std::vector<std::string>& args);
	void DoAddX(const std::vector<std::string>& args);

	int m_cycle;
	std::vector<std::string> m_opcodes;
	int m_ip;
	__int64 m_x;

	std::map<std::string, int> m_instr_times;
	std::map<std::string, std::function<void(const std::vector<std::string>&)> > m_instr_dispatch;
};

