#include <iostream>
#include <sstream>
#include "Cpu.h"
#include "../shared/Split.h"

using namespace std;

Cpu::Cpu(const vector<string>& opcodes) :
	m_opcodes(opcodes)
{
	m_instr_times["noop"] = 1;
	m_instr_times["addx"] = 2;

	m_instr_dispatch["noop"] = [&](auto& args) {DoNoop(args); };
	m_instr_dispatch["addx"] = [&](auto& args) {DoAddX(args); };

	Reset();
}

void Cpu::Reset()
{
	m_x = 1;
	m_ip = 0;
	m_cycle = 0;
}

void Cpu::RunTo(int cycle)
{
	while (true) {
		string instr = m_opcodes[m_ip];
		vector<string> args = Split<string>(instr, ' ');
		const string& opcode = args[0];

		int t = m_instr_times[opcode];
		if (m_cycle + t >= cycle) {
			return;
		}

		++m_ip;
		m_cycle += t;

		//cout << State() << " " << instr;

		m_instr_dispatch[opcode](args);

		//cout << " " << State() << endl;
	}
}

void Cpu::DoNoop(const std::vector<std::string>& args)
{
}

void Cpu::DoAddX(const std::vector<std::string>& args)
{
	m_x += stol(args[1]);
}

string Cpu::State()
{
	stringstream ss;
	ss << "CPU[ x = " << m_x << "; cyc = " << m_cycle << "]";
	return ss.str();
}