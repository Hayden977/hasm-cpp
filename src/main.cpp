#include <iostream>
#include <string>

// Switch for showing output of commands
bool silent = false;

// Utility functions
void print();
// https://stackoverflow.com/a/14267455
void tokenize(std::string const &str, std::string &delim);

// Line input
std::string input;

// Array of tokens
std::string tokens[3];
// Delimiter for tokens
std::string delim = " ";

typedef int ADDRESS;

ADDRESS mem_default = 0;
int memory[32] = {0};

ADDRESS stack_ptr = 16;
int stack[16] = {0};

// Jump and math flags
// bool greater; // If the last jump comparison resulted in greater than
// bool less;    // If the last jump comparison resulted in less than
bool neg;     // If the last math operation would have resulted in a negative number
// bool zero;    // If the last jump comparison resulted in zero

// Memory control flags
// bool mem_none; // If a move had no effect
bool mem_out;  // If the stack is full
// bool mem_used; // If the default memory value is being used

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
	    if (arg == "-h")
	    {
	        // Display help message
		return 0;
	    } else if (arg == "-s")
	    {
	    	silent = true;
	    } else if (arg == "-v")
	    {
	    	// Display version message
		std::cout << "HASM Version 1.0, 6/29/2020" << std::endl;
		return 0;
	    }
	}
    }
    while (input != "quit")
    {	
	if (!silent) { std::cout << "[HASM]: "; }
	std::getline(std::cin, input);
	tokenize(input, delim);
	if (tokens[0] == "mov" || tokens[0] == "m")
	{
	    // mov address_from address_to
	    mov(stoi(tokens[1]), stoi(tokens[2]));
	} else
	if (tokens[0] == "psh" || tokens[0] == "ps")
	{
	    // psh address_from
	    psh(stoi(tokens[1]));
	} else
	if (tokens[0] == "pop" || tokens[0] == "pp")
	{
	    // pop address_to
	    pop(stoi(tokens[1]));
	} else
	if (tokens[0] == "add" || tokens[0] == "a")
	{
	    // add address_from address_to
	    add(stoi(tokens[1]), stoi(tokens[2]));
	} else
	if (tokens[0] == "sub" || tokens[0] == "s")
	{
	    // sub address_from address_to
	    sub(stoi(tokens[1]), stoi(tokens[2]));
	} else
	if (tokens[0] == "inc" || tokens[0] == "i")
	{
	    // inc address_to
	    inc(stoi(tokens[1]));
	} else
	if (tokens[0] == "dec" || tokens[0] == "d")
	{
	    // dec address_to
	    dec(stoi(tokens[1]));
	} else
	if (tokens[0] == "place" || tokens[0] == "p")
	{
	    // place value address
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
	std::cout << item << "|";
    }
    std::cout << std::endl
	      << "Memory: |";
    for (auto item : memory)
    {
	std::cout << item << "|";
    }
    std::cout << std::endl;
}

// https://stackoverflow.com/a/14267455
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
	mem_out = true;
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
	neg = true;
    }
    return;
}

void sub(ADDRESS s, ADDRESS d)
{
    memory[d] -= memory[s];
    if (memory[d] < 0)
    {
	neg = true;
    }
    return;
}

void inc(ADDRESS d)
{
    memory[d] += 1;
    if (memory[d] < 0)
    {
    	neg = true;
    }
    return;
}

void dec(ADDRESS d)
{
    memory[d] -= 1;
    if (memory[d] < 0)
    {
    	neg = true;
    }
    return; 
}
