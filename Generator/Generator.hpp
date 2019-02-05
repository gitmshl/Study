#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>
#include "Generator_Description.hpp"

Generator::Generator(int N, char * A = 0, int MAX_LENGTH = 1000)
{
    S = new int[MAX_LENGTH];
    Max_Length = MAX_LENGTH;
    n = N;
    if (A) Alphabet = A;
    length = 1;
    S[0] = 0;
}

void Generator::print_current()
{
    for (int i = 0; i < length; i++)
        std::cout << Alphabet[S[i]];

    std::cout << "\n";
}

void Generator::Next()
{
    S[length - 1]++;
    if (S[length-1] < n) return;
    S[length - 1] = 0;
    int q = 1, i = length - 2;

    while (q && i > -1)
    {
        S[i]++; q = 0;
        if (S[i] == n) { q = 1; S[i] = 0;}
        i--;
    }

    if (q) {length++; Zeroing();}
}

void Generator::print(int Count)
{
    Origin();

    for (int i = 0; i < Count; i++)
    {
        print_current();
        Next();
    }
}

void Generator::print()
{
    print(Max_Length);
}

void Generator::Origin()
{
    length = 1;
    S[0] = 0;
}

void Generator::Zeroing()
{
    for (int i = 0; i < length; i++)
        S[i] = 0;
}

#endif // GENERATOR_HPP
