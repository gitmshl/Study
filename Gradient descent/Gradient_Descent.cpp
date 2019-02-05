#include <iostream>
#include <cmath>

const double err = 0.00001;
const double precision_part_der = 0.00001;
const double gdstep = 0.05;
const int MAX_STEPS = 1000;

using namespace std;

double F(double *Arguments, int ArgumentsCount); /// ok
double partder(double *Arguments, int ArgumentsCount, int k); /// ok
void GradientDescent(double *Arguments, int ArgumentsCount);
double EuclideanNorm(double *X1, double *X2, double n); /// ok
void print(double *Arguments, int ArgumentsCount);

int main()
{
    double Arguments[2] = {1, 54};
    GradientDescent(Arguments, 2);
    //print(Arguments, 2);
    //cout << "F(x, y) = " << F(Arguments, 2);
    //cout << F(Arguments, 2);
    //cout << "\n" << partder(Arguments, 2, 1);
    return 0;
}

double F(double *Arguments, int ArgumentsCount)
{
    double x = Arguments[0];
    double y = Arguments[1];
    return 3*x*x + 2*x + y*y;
}

double partder(double *Arguments, int ArgumentsCount, int k)
{
    double F0 = F(Arguments, ArgumentsCount);
    Arguments[k] += precision_part_der;
    double F1 = F(Arguments, ArgumentsCount);
    Arguments[k] -= precision_part_der;
    return (F1-F0)/precision_part_der;
}

double EuclideanNorm(double *X1, double *X2, double n)
{
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += (X1[i] - X2[i]) * (X1[i] - X2[i]);
    return sqrt(sum);
}

void GradientDescent(double *V, int VCount)
{
    double V0[VCount];
    int q = 0;

    do
    {
        for (int i = 0; i < VCount; i++)
            V0[i] = V[i];

        for (int k = 0; k < VCount; k++)
            V[k] = V0[k] - gdstep * partder(V0, VCount, k);
        q++;
        print(V, VCount);

    } while (EuclideanNorm(V, V0, VCount) > err && q < MAX_STEPS);

    if (q == MAX_STEPS) cout << "q == MAX_STEPS!!!\n";
}

void print(double *Arguments, int ArgumentsCount)
{
    for (int i = 0; i < ArgumentsCount; i++)
        cout << Arguments[i] << " ";
    cout << "\n";
}

