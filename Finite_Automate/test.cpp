#include <iostream>

#include "Finite_Automate.hpp"

using namespace std;


int main()
{
    int Q1Size = 4;
    int S1Size = 2;
    int QLength = 3;
    int FSize = 1;

    char S1[] = {'0', '1'};
    char Q1[Q1Size][QLength] = {
        "q0", "q1", "q2", "q3"
        };
    char F1[FSize][QLength] = {
        "q0"
        };

    int D1[Q1Size][S1Size] = {
        {2, 1},
        {3, 0},
        {0, 3},
        {1, 2}
    };


 DFA dfa(S1, S1Size, Q1[0], Q1Size, QLength, F1[0], FSize, D1[0], 0);

dfa.print();

 dfa.generate_lang(1000);

    return 0;
}
