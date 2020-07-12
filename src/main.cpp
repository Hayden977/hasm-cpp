#include <iostream>
#include <fstream>
#include <string>

bool bChars     = false;
bool bFlagShift = false;
bool bLogFile   = false;
bool bReadOnly  = false;
bool bSilent    = false;

void vLog(std::string const &sPath, std::string const &sLine);
void vProcess();
void vPrint();
void vReadFile(std::string const &sPath);
void vTokenize(std::string const &s, std::string &sDelim);

std::string sLogPath;

std::string sInput;

std::string sTokens[3];
std::string sDelim = " ";

const int nMemorySize = 32;
const int nStackSize = 16;

typedef int ADDRESS;

int nMemory[nMemorySize] = {0};
ADDRESS jmp      = 0;
ADDRESS greater  = 1; 
ADDRESS less     = 2; 
ADDRESS neg      = 3; 
ADDRESS zero     = 4;
ADDRESS mem_none = 5; 
ADDRESS mem_out  = 6;
ADDRESS mem_used = 7;

ADDRESS nStack_ptr = nStackSize;
int nStack[nStackSize] = {0};

void mov(ADDRESS s, ADDRESS d);
void psh(ADDRESS s);
void pop(ADDRESS d);
void add(ADDRESS s, ADDRESS d);
void sub(ADDRESS s, ADDRESS d);
void inc(ADDRESS d);
void dec(ADDRESS d);

int main(int argc, char* argv[])
{
	if (argc > 1) 
	{
		for (int i = 1; i < argc; i++)
		{
			std::string sArg = argv[i];
			if (sArg == "-c")
			{
				bChars = true;
			} else
			if (sArg == "-e")
			{
				vReadFile(argv[++i]);
			} else
			if (sArg == "-f")
			{
				bFlagShift = true;
			} else
			if (sArg == "-h")
			{
				return 0;
			} else 
			if (sArg == "-l")
			{
				bLogFile = true;
				sLogPath = argv[++i];
			} else
			if (sArg == "-r")
			{
				bReadOnly = true;
			} else
			if (sArg == "-s")
			{
				bSilent = true;
			} else 
			if (sArg == "-v")
			{
				std::cout << "HASM Version 1.2, 7/5/2020" << std::endl;
				return 0;
			}
		}
	}
	while (true)
	{	
		if (!bSilent) { std::cout << "[HASM]: "; }

		std::getline(std::cin, sInput);
		if (sInput == "quit" || sInput == "q") 
		{
			break;
		}
		if (bLogFile) { vLog(sLogPath, sInput); }
		vTokenize(sInput, sDelim);
		vProcess();

		if (!bSilent) { vPrint(); }
	}
	return 0;
}

void vLog(std::string const &sPath, std::string const &sLine)
{
	std::fstream fsOut {sPath, std::ios_base::out | std::ios_base::app};
	if (fsOut.is_open())
	{
		fsOut << sLine << std::endl;
		fsOut.close();
	}
}

void vReadFile(std::string const &sPath)
{
	std::fstream fsIn(sPath);
	if (fsIn.is_open())
	{
		std::string sLine;
		while (getline(fsIn, sLine))
		{
			if (char(sLine[0]) != ';')
			{
				vTokenize(sLine, sDelim);
				vProcess();
			}
		}
		fsIn.close();
	} 
}

void vTokenize(std::string const &s, std::string &sDelim)
{
	int nI = 0;
	auto aStart = 0U;
	auto aEnd = s.find(sDelim);
	while (aEnd != std::string::npos)
	{
		std::string sCurrent = s.substr(aStart, aEnd - aStart);
		sTokens[nI] = sCurrent;
		nI++;
		aStart = aEnd + sDelim.length();
		aEnd = s.find(sDelim, aStart);
	}
	sTokens[nI] = s.substr(aStart, aEnd);
}

void vProcess()
{
	if (sTokens[0] == "mov" || sTokens[0] == "m")
	{
		mov(stoi(sTokens[1]), stoi(sTokens[2]));
	} else
	if (sTokens[0] == "psh" || sTokens[0] == "ps")
	{
		psh(stoi(sTokens[1]));
	} else
	if (sTokens[0] == "pop" || sTokens[0] == "pp")
	{
		pop(stoi(sTokens[1]));
	} else
	if (sTokens[0] == "add" || sTokens[0] == "a")
	{
		add(stoi(sTokens[1]), stoi(sTokens[2]));
	} else
	if (sTokens[0] == "sub" || sTokens[0] == "s")
	{
		sub(stoi(sTokens[1]), stoi(sTokens[2]));
	} else
	if (sTokens[0] == "inc" || sTokens[0] == "i")
	{
		inc(stoi(sTokens[1]));
	} else
	if (sTokens[0] == "dec" || sTokens[0] == "d")
	{
		dec(stoi(sTokens[1]));
	} else
	if (sTokens[0] == "place" || sTokens[0] == "p")
	{
		nMemory[stoi(sTokens[2])] = stoi(sTokens[1]);
	} else
	if (sTokens[0] == "peek" || sTokens[0] == "pe")
	{
		if (bSilent) { vPrint(); }
	}
}

void vPrint()
{
	std::cout << "Stack*: " << nStack_ptr << std::endl;
	std::cout << "Stack:  |";
	for (auto aItem : nStack)
	{
		if (bChars)
		{
			std::cout << char(aItem) << "|";
		} else
		{
			std::cout << aItem << "|";
		}
	}
	std::cout << std::endl << "Memory: |";
	for (auto aItem : nMemory)
	{
		if (bChars)
		{
			std::cout << char(aItem) << "|";
		} else
		{
			std::cout << aItem << "|";
		}
	}
	std::cout << std::endl;
}

void mov(ADDRESS s, ADDRESS d)
{
	nMemory[d] = nMemory[s];
	nMemory[s] = 0;
}

void psh(ADDRESS s)
{
	nStack_ptr--;
	if (nStack_ptr > 0)
	{
		nStack[nStack_ptr] = nMemory[s];
	} else
	{
		nMemory[mem_out] = 1;
		nStack_ptr++;
	}
}

void pop(ADDRESS d)
{
	nMemory[d] = nStack[nStack_ptr];
	nStack[nStack_ptr] = 0;
	if (nStack_ptr < nStackSize)
	{
		nStack_ptr++;
	}
}

void add(ADDRESS s, ADDRESS d)
{
	nMemory[d] += nMemory[s];
	if (nMemory[d] < 0)
	{
		nMemory[neg] = 1;
	}
}

void sub(ADDRESS s, ADDRESS d)
{
	nMemory[d] -= nMemory[s];
	if (nMemory[d] < 0)
	{
		nMemory[neg] = 1;
	}
}

void inc(ADDRESS d)
{
	nMemory[d] += 1;
	if (nMemory[d] < 0)
	{
		nMemory[neg] = 1;
	}
}

void dec(ADDRESS d)
{
	nMemory[d] -= 1;
	if (nMemory[d] < 0)
	{
		nMemory[neg] = 1;
	}
}
