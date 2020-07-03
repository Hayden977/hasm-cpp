#include <iostream>
#include <string>

bool silent = false;
bool chars = false;

void print();
void tokenize(std::string const &str, std::string &delim);

std::string input;

std::string tokens[3];
std::string delim = " ";

typedef int ADDRESS;

int memory[32] = {0};
// Jump register
ADDRESS jmp      = 0;
// Comparison flags   // Set if
ADDRESS greater  = 1; // The last jump comparison resulted in greater than
ADDRESS less     = 2; // The last jump comparison resulted in less than
ADDRESS neg      = 3; // The last math operation resulted in a negative number
ADDRESS zero     = 4; // The last jump comparison resulted in zero
// Memory flags       // Set if
ADDRESS mem_none = 5; // A move had no effect
ADDRESS mem_out  = 6; // The stack is full
ADDRESS mem_used = 7; // The jump address is being used

ADDRESS stack_ptr = 16;
int stack[16] = {0};

void mov(ADDRESS s, ADDRESS d);
void psh(ADDRESS s);
void pop(ADDRESS d);
void add(ADDRESS s, ADDRESS d);
void sub(ADDRESS s, ADDRESS d);
void inc(ADDRESS d);
void dec(ADDRESS d);
// void jmp(ADDRESS s);
// void jmpz(ADDRESS s, ADDRESS d);
// void jmpl(ADDRESS s, ADDRESS d);
// void jmpg(ADDRESS s, ADDRESS d);


int main(int argc, char* argv[])
{
    if (argc > 1) 
    {
        for (int i = 1; i < argc; i++)
	{
	    std::string arg = argv[i];
	    if (arg == "-c")
	    {
		chars = true;
	    }
	    if (arg == "-h")
	    {
		return 0;
	    } else 
	    if (arg == "-s")
	    {
	    	silent = true;
	    } else 
            if (arg == "-v")
	    {
		std::cout << "HASM Version 1.0, 6/29/2020" << std::endl;
		return 0;
	    }
	}
    }
    while (input != "quit" && input != "q")
    {	
	if (!silent) { std::cout << "[HASM]: "; }

	std::getline(std::cin, input);
	tokenize(input, delim);

	if (tokens[0] == "mov" || tokens[0] == "m")
	{
	    mov(stoi(tokens[1]), stoi(tokens[2]));
	} else
	if (tokens[0] == "psh" || tokens[0] == "ps")
	{
	    psh(stoi(tokens[1]));
	} else
	if (tokens[0] == "pop" || tokens[0] == "pp")
	{
	    pop(stoi(tokens[1]));
	} else
	if (tokens[0] == "add" || tokens[0] == "a")
	{
	    add(stoi(tokens[1]), stoi(tokens[2]));
	} else
	if (tokens[0] == "sub" || tokens[0] == "s")
	{
	    sub(stoi(tokens[1]), stoi(tokens[2]));
	} else
	if (tokens[0] == "inc" || tokens[0] == "i")
	{
	    inc(stoi(tokens[1]));
	} else
	if (tokens[0] == "dec" || tokens[0] == "d")
	{
	    dec(stoi(tokens[1]));
	} else
	if (tokens[0] == "place" || tokens[0] == "p")
	{
	    memory[stoi(tokens[2])] = stoi(tokens[1]);
	} else
	if (tokens[0] == "peek" || tokens[0] == "pe")
	{
	    if (silent) { print(); }
	}

	if (!silent) { print(); }
    }
    return 0;
}

void print()
{
    std::cout << "Stack*: " << stack_ptr << std::endl;
    std::cout << "Stack:  |";
    for (auto item : stack)
    {
	if (chars)
	{
	    std::cout << char(item) << "|";
	} else
	{
	    std::cout << item << "|";
	}
    }
    std::cout << std::endl << "Memory: |";
    for (auto item : memory)
    {
	if (chars)
	{
    	    std::cout << char(item) << "|";
	} else
	{
	    std::cout << item << "|";
	}
    }
    std::cout << std::endl;
}

void tokenize(std::string const &str, std::string &delim)
{
    int i = 0;
    auto start = 0U;
    auto end = str.find(delim);
    while (end != std::string::npos)
    {
	std::string current = str.substr(start, end - start);
	tokens[i] = current;
	i++;
	start = end + delim.length();
	end = str.find(delim, start);
    }
    tokens[i] = str.substr(start, end);
}

void mov(ADDRESS s, ADDRESS d)
{
    memory[d] = memory[s];
    memory[s] = 0;
    return;
}

void psh(ADDRESS s)
{
    stack_ptr--;
    if (stack_ptr > 0)
    {
	stack[stack_ptr] = memory[s];
	return;
    }
    else
    {
	memory[mem_out] = 1;
	stack_ptr++;
	return;
    }
}

void pop(ADDRESS d)
{
    memory[d] = stack[stack_ptr];
    stack[stack_ptr] = 0;
    if (stack_ptr < 16)
    {
	stack_ptr++;
	return;
    }
    else
    {
	return;
    }
}

void add(ADDRESS s, ADDRESS d)
{
    memory[d] += memory[s];
    if (memory[d] < 0)
    {
	memory[neg] = 1;
    }
    return;
}

void sub(ADDRESS s, ADDRESS d)
{
    memory[d] -= memory[s];
    if (memory[d] < 0)
    {
	memory[neg] = 1;
    }
    return;
}

void inc(ADDRESS d)
{
    memory[d] += 1;
    if (memory[d] < 0)
    {
    	memory[neg] = 1;
    }
    return;
}

void dec(ADDRESS d)
{
    memory[d] -= 1;
    if (memory[d] < 0)
    {
    	memory[neg] = 1;
    }
    return; 
}
