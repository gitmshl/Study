#ifndef FINITE_AUTOMATE_DESCRIPTION_HPP
#define FINITE_AUTOMATE_DESCRIPTION_HPP

#include "..\Generator\Generator.hpp"

class DFA /// Finite Automate
{
    char * S; /// массив char
    char ** Q;
    char ** F;
    int ** D;
    int q0;
    int q_current;
    int QSize, FSize, SSize;
    Generator * g;

public:
    DFA(char *S1, int S1Size, char * Q1, int Q1Size, int QLength, char * F1, int F1Size, int * D1, int q10);
    void print();
    int GetQ(char * q, int q_size);
    int GetF(char * f, int f_size);
    int GetS(char c);
    bool inF(int q);
    bool Check(char * p, int p_length); /// проверка принадлежности слова p языку КА
    bool Check(int * p, int p_length);
    void generate_lang(int Count);
};

#endif // FINITE_AUTOMATE_DESCRIPTION_HPP
