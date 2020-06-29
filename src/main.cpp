#include <iostream>
#include <string>

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
bool greater; // If the last jump comparison resulted in greater than
bool less;    // If the last jump comparison resulted in less than
bool neg;     // If the last math operation would have resulted in a negative number
bool zero;    // If the last jump comparison resulted in zero

// Memory control flags
bool mem_none; // If a move had no effect
bool mem_out;  // If the stack is full
bool mem_used; // If the default memory value is being used

void mov(ADDRESS s, ADDRESS d);
void psh(ADDRESS s);
void pop(ADDRESS d);
void add(ADDRESS s, ADDRESS d);
void sub(ADDRESS s, ADDRESS d);
void jmp(ADDRESS s);
void jmpz(ADDRESS s, ADDRESS d);
void jmpl(ADDRESS s, ADDRESS d);
void jmpg(ADDRESS s, ADDRESS d);

#ifdef DEBUG
void mov();
void psh();
void pop();
void add();
void sub();
void jmp();
void jpmz();
void jpml();
void jmpg();
#endif

int main()
{
    while (input != "quit")
    {
        std::getline(std::cin, input);
        tokenize(input, delim);
        if (tokens[0] == "place" || tokens[0] == "p")
        {
            // place value address
            memory[stoi(tokens[2])] = stoi(tokens[1]);
        }
        if (tokens[0] == "mov" || tokens[0] == "m")
        {
// mov address_from address_to
#ifdef DEBUG
            if (tokens[1] == "" && tokens[2] == "")
            {
                mov();
            }
            else
            {
                mov(stoi(tokens[1]), stoi(tokens[2]));
            }
            continue;
#endif
            mov(stoi(tokens[1]), stoi(tokens[2]));
        }
        if (tokens[0] == "psh" || tokens[0] == "ps")
        {
// psh address_from
#ifdef DEBUG
            if (tokens[1] == "")
            {
                psh();
            }
            else
            {
                psh(stoi(tokens[1]));
            }
            continue;
#endif
            psh(stoi(tokens[1]));
        }
        if (tokens[0] == "pop" || tokens[0] == "pp")
        {
// pop address_to
#ifdef DEBUG
            if (tokens[1] == "")
            {
                pop();
            }
            else
            {
                pop(stoi(tokens[1]));
            }
            continue;
#endif
            pop(stoi(tokens[1]));
        }
        if (tokens[0] == "add" || tokens[0] == "a")
        {
// add address_from address_to
#ifdef DEBUG
            if (tokens[1] == "" && tokens[2] == "")
            {
                add();
            }
            else
            {
                add(stoi(tokens[1]), stoi(tokens[2]));
            }
            continue;
#endif
            add(stoi(tokens[1]), stoi(tokens[2]));
        }
        if (tokens[0] == "sub" || tokens[0] == "s")
        {
// sub address_from address_to
#ifdef DEBUG
            if (tokens[1] == "" && tokens[2] == "")
            {
                sub();
            }
            else
            {
                sub(stoi(tokens[1]), stoi(tokens[2]));
            }
            continue;
#endif
            sub(stoi(tokens[1]), stoi(tokens[2]));
        }
        print();
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
}

void sub(ADDRESS s, ADDRESS d)
{
    memory[d] -= memory[s];
    if (memory[d] < 0)
    {
        neg = true;
    }
}

#ifdef DEBUG
void mov()
{
    mem_used = true;
    pop(mem_default);     // Des
    pop(mem_default + 1); // Src
    memory[memory[mem_default]] = memory[memory[mem_default + 1]];
    memory[memory[mem_default] - 1] = 0;
    mem_used = false;
    return;
}

void psh()
{
    mem_used = true;
    pop(mem_default);
    stack_ptr--;
    if (stack_ptr > 0)
    {
        stack[stack_ptr] = memory[memory[mem_default]];
        mem_used = false;
        return;
    }
    else
    {
        mem_out = true;
        mem_used = false;
        stack_ptr++;
        return;
    }
}

void pop()
{
    mem_default = true;
    pop(mem_default);
    memory[memory[mem_default]] = stack[stack_ptr];
    mem_default = false;
    stack_ptr++;
    if (stack_ptr >= 15) // Maybe change this to 16?
    {
        stack_ptr--;
        mem_out = true;
        return;
    }
    else
    {
        return;
    }
}

void add()
{
    mem_used = true;
    pop(mem_default);     // Des
    pop(mem_default + 1); // Src
    memory[memory[mem_default]] += memory[memory[mem_default + 1]];
    if (memory[memory[mem_default + 1]] < 0)
    {
        neg = true;
    }
    mem_used = false;
    return;
}

void sub()
{
    mem_used = true;
    pop(mem_default);     // Des
    pop(mem_default + 1); // Src
    memory[memory[mem_default]] -= memory[memory[mem_default + 1]];
    if (memory[memory[mem_default + 1]] < 0)
    {
        neg = true;
    }
    mem_used = false;
    return;
}
#endif