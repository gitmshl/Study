#ifndef FINITE_AUTOMATE_HPP
#define FINITE_AUTOMATE_HPP

#include "Finite_Automate_Description.hpp"
#include <cstring>
#include <ctime>

DFA::DFA(char *S1, int S1Size, char * Q1, int Q1Size, int QLength, char * F1, int F1Size, int * D1, int q10)
{
    int start = clock();
    S = S1; SSize = S1Size; QSize = Q1Size; FSize = F1Size; q0 = q10;
    q_current = q0;

    Q = new char*[QSize];
    F = new char*[FSize];
    D = new int*[QSize];

    for (int i = 0; i < QSize; i++)
    {
        Q[i] = &Q1[i * QLength];
    }

    for (int i = 0; i < FSize; i++)
    {
        F[i] = &F1[i * QLength];
    }

    for (int i = 0; i < QSize; i++)
    {
        D[i] = &D1[i * SSize];
    }

    g = new Generator(SSize, S);
}

void DFA::print()
{
    std::cout << "S: ";
    for (int i = 0; i < SSize; i++)
        std::cout << S[i] << " ";
   std::cout << "\nQ: ";
    for (int i = 0; i < QSize; i++)
        std::cout << Q[i] << " ";
    std::cout << "\nF: ";
    for (int i = 0; i < FSize; i++)
        std::cout << F[i] << " ";
    std::cout << "\n";
}


bool DFA::Check(char * p, int p_length = 0) /// p - одномерный, каждый символ из S -алфавит из одиночных символов!
{
    q_current = q0;
    for (int i = 0; i < p_length; i++)
    {
        int s = GetS(p[i]);
        q_current = D[q_current][s];
    }
    return inF(q_current);
}

bool DFA::Check(int *p, int p_length)
{
    q_current = 0;
    for (int i = 0; i < p_length; i++)
    {
        q_current = D[q_current][p[i]];
    }
    return inF(q_current);
}

int DFA::GetS(char c)
{
    for (int i = 0; i < SSize; i++)
        if (S[i] == c) return i;
    return -1;
}

int DFA::GetQ(char * q, int q_size = 0)
{
    if (q_size) q[q_size] = '\0';
    for (int i = 0; i < QSize; i++)
        if (!strcmp(Q[i], q)) return i;
    return -1;
}

int DFA::GetF(char *f, int f_size = 0)
{
    if (f_size) f[f_size] = '\0';
    for (int i = 0; i < FSize; i++)
        if (!strcmp(F[i], f)) return i;
    return -1;
}

bool DFA::inF(int q)
{
    char *p = Q[q];
    for (int i = 0; i < FSize; i++)
        if (!strcmp(F[i], p)) return true;
    return false;
}

void DFA::generate_lang(int Count)
{
    std::cout << "Generation language\n";
    g->Origin();

    for (int i = 0; i < Count; i++)
    {
        if (Check(g->S, g->length)) g->print_current();
        g->Next();
    }
}

#endif // FINITE_AUTOMATE_HPP
