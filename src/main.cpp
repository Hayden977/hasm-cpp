#include <iostream>
#include <fstream>
#include <string>

bool bSilent = false;
bool bChars = false;

void vReadfile(std::string const &path);
void vTokenize(std::string const &str, std::string &delim);
void vProcess();
void vPrint();

std::string sInput;

std::string sTokens[3];
std::string sDelim = " ";

typedef int ADDRESS;

int nMemory[32] = {0};
ADDRESS jmp      = 0;
ADDRESS greater  = 1; 
ADDRESS less     = 2; 
ADDRESS neg      = 3; 
ADDRESS zero     = 4;
ADDRESS mem_none = 5; 
ADDRESS mem_out  = 6;
ADDRESS mem_used = 7;

ADDRESS nStack_ptr = 16;
int nStack[16] = {0};

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
		vReadfile(argv[++i]);
	    }
	    if (sArg == "-h")
	    {
		return 0;
	    } else 
	    if (sArg == "-s")
	    {
	    	bSilent = true;
	    } else 
            if (sArg == "-v")
	    {
		std::cout << "HASM Version 1.0, 6/29/2020" << std::endl;
		return 0;
	    }
	}
    }
    while (sInput != "quit" && sInput != "q")
    {	
	if (!bSilent) { std::cout << "[HASM]: "; }

	std::getline(std::cin, sInput);
	vTokenize(sInput, sDelim);

	vProcess();

	if (!bSilent) { vPrint(); }
    }
    return 0;
}

void vReadfile(std::string const &sPath)
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

void vTokenize(std::string const &sStr, std::string &sDelim)
{
    int nI = 0;
    auto aStart = 0U;
    auto aEnd = sStr.find(sDelim);
    while (aEnd != std::string::npos)
    {
	std::string sCurrent = sStr.substr(aStart, aEnd - aStart);
	sTokens[nI] = sCurrent;
	nI++;
	aStart = aEnd + sDelim.length();
	aEnd = sStr.find(sDelim, aStart);
    }
    sTokens[nI] = sStr.substr(aStart, aEnd);
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
    return;
}

void psh(ADDRESS s)
{
    nStack_ptr--;
    if (nStack_ptr > 0)
    {
	nStack[nStack_ptr] = nMemory[s];
	return;
    }
    else
    {
	nMemory[mem_out] = 1;
	nStack_ptr++;
	return;
    }
}

void pop(ADDRESS d)
{
    nMemory[d] = nStack[nStack_ptr];
    nStack[nStack_ptr] = 0;
    if (nStack_ptr < 16)
    {
	nStack_ptr++;
	return;
    }
    else
    {
	return;
    }
}

void add(ADDRESS s, ADDRESS d)
{
    nMemory[d] += nMemory[s];
    if (nMemory[d] < 0)
    {
	nMemory[neg] = 1;
    }
    return;
}

void sub(ADDRESS s, ADDRESS d)
{
    nMemory[d] -= nMemory[s];
    if (nMemory[d] < 0)
    {
	nMemory[neg] = 1;
    }
    return;
}

void inc(ADDRESS d)
{
    nMemory[d] += 1;
    if (nMemory[d] < 0)
    {
    	nMemory[neg] = 1;
    }
    return;
}

void dec(ADDRESS d)
{
    nMemory[d] -= 1;
    if (nMemory[d] < 0)
    {
    	nMemory[neg] = 1;
    }
    return; 
}
