#include <iostream>
#include <cmath>
#include <stdlib.h>

using namespace std;

double g(double a)
{
    return a >= 0.5 ? 1 : 0;
}

class net
{
    double gradient_step = 0.6;
    int RAND = 1000;
    double inputs[2]; /// inputs[0] = 1 (нумерация с 1)
    double o_1[2]; /// o[0] = 1 (нумерация с 1)
    double o_2[1]; /// o_2[0] = 1; o_2[1] = result (нумерация с 1)
    double weights_0_1[2][2];
    double weights_1_2[2][1];

public:
    net();
    double f(double a) {return 1.0/(1.0 + exp(-a));}
    void FeedForward(double *Inp);
    void learn(double *Inp, double Answer);
    void print();
    void print_result();
    void ex(double *Inp, double ans)
    {
        FeedForward(Inp);
        if (g(o_2[0]) != ans) { cout << "NO "; print_result();}
        else { cout << "YES "; print_result();}
    }
};

net::net()
{
    weights_0_1[0][0] = weights_0_1[1][0] = -17.1136;
    weights_0_1[0][1] = weights_0_1[1][1] = -4.98231;
    weights_1_2[0][0] = -521.501;
    weights_1_2[1][0] = 509.495;
}

void net::FeedForward(double *Inp)
{
    for (int i = 0; i < 2; i++)
        inputs[i] = Inp[i];
    for (int k = 0; k < 2; k++)
    {
        o_1[k] = 0;
        for (int i = 0; i < 2; i++)
            o_1[k] += inputs[i] * weights_0_1[i][k];
        o_1[k] = f(o_1[k]);
    }
    o_2[0] = 0;
    for (int i = 0; i < 2; i++)
        o_2[0] += o_1[i] * weights_1_2[i][0];
    o_2[0] = f(o_2[0]);
}

void net::learn(double *Inp, double Answer)
{
    FeedForward(Inp);
    double delta0[3], delta1[2], delta2[1];
    delta2[0] = (o_2[0] - Answer) * o_2[0] * (1 - o_2[0]);

    for (int i = 0; i <= 1; i++)
        delta1[i] = o_1[i]*(1-o_1[i]) * delta2[0] * weights_1_2[i][0];

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                weights_0_1[i][j] -= gradient_step * delta1[j] * inputs[i];

        for (int i = 0; i < 2; i++)
            weights_1_2[i][0] -= gradient_step * delta2[0] * o_1[i];
}

void net::print()
{
    cout << "layer 1\n";
    cout << "1 neuron\n";
    cout << "first = " << weights_0_1[0][0] << "\n";
    cout << "second = " << weights_0_1[1][0] << "\n";
    cout << "2 neuron\n";
    cout << "first = " << weights_0_1[0][1] << "\n";
    cout << "second = " << weights_0_1[1][1] << "\n";
    cout << "layer 2\n";
    cout << "first = " << weights_1_2[0][0] << "\n";
    cout << "second = " << weights_1_2[1][0] << "\n";
    cout << "\n\n";

}

void net::print_result()
{
    cout << "Result = " << o_2[0] << "\n";
}

int main()
{
    net n;
    double Inp1[] = {0, 0};
    double Inp2[] = {1, 0};
    double Inp3[] = {0, 1};
    double Inp4[] = {1, 1};
    double Ans1 = 0;
    double Ans2 = 1;
    double Ans3 = 1;
    double Ans4 = 0;

    n.print();
    n.ex(Inp1, Ans1);
    n.ex(Inp2, Ans2);
    n.ex(Inp3, Ans3);
    n.ex(Inp4, Ans4);

    for (int i = 0; i < 1000000; i++)
    {
        n.learn(Inp1, Ans1);
        n.learn(Inp2, Ans2);
        n.learn(Inp3, Ans3);
        n.learn(Inp4, Ans4);
    }

    cout << "-----------------------------------\n";
    n.ex(Inp1, Ans1);
    n.ex(Inp2, Ans2);
    n.ex(Inp3, Ans3);
    n.ex(Inp4, Ans4);

    n.print();
    //n.print_result();
    return 0;
}
