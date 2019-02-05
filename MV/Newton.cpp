#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

class Err{};

const double Separation_eps = 0.0005;
const double Newton_eps = 0.0000001;
const double Iteration_eps = 0.000001;

double Left[20]; /// предполагаемые промежутки, где могут находиться корни
double Right[20];
int counter = 0; /// счетчик

double NewtonRoots[20];
int Ncounter = 0;

double IterationRoots[20];
int Icounter = 0;

int Nq = 0, Ni = 0;

double f(double x)
{
    /*
    double sq = 1 - 0.2 * x * x;
    if (sq < 0) throw new Err;

    double p2 = pow(x, 2), p3 = p2 * x, p5 = p3 * p2, p7 = p5 * p2;
    return 64 * p7 - 112 * p5 + 56 * p3 - 7 * x + sqrt(sq);
    */
    double p2 = pow(x, 2);
    double p3 = p2 * x;
    double p5 = p2 * p3;
    double p7 = p5 * p2;
    return x + p2 - p3/6.0 + p5/120.0 - p7/(720*7.0) - 0.85;
}

double fder(double x)
{
    /*
    double sq = 1 - 0.2 * x * x;
    if (sq <= 0) throw new Err;

    double p2 = pow(x, 2), p4 = p2 * p2, p6 = p4 * p2;
    return 448 * p6 - 560 * p4 + 168 * p2 - 0.2 * x/sqrt(sq) - 7;
    */
    double p2 = x * x;
    double p4 = p2 * p2;
    double p6 = p2 * p4;
    return 1 + 2*x-p2/2.0 + p4/24.0 - p6/720.0;
}

double find_max_fder(double a, double b)
{
    double eps = 0.005;
    double M = fder(a);
    double f;
    a += eps;
    while (a <= b)
    {
        try
        {
           f = fder(a);
        }
        catch(Err)
        {
            cout << "Error. Division by zero. find_max_fder function\n";
            return 1;
        }
        if (f > M) M = f;
        a += eps;
    }
    return M;
}

void Separation(double a0, double b0)
{
    double a, b = b0;

    double fleft, fright;

    while (b >= a0 + Separation_eps)
    {
        a = b - Separation_eps;
        try
        {
            fleft = f(a);
            fright = f(b);
        }
        catch(Err)
        {
            cout << "Error: Out of range. Separation\n";
            return;
        }
        if (fleft * fright < 0)
        {
            Left[counter] = a;
            Right[counter] = b;
            counter++;
        }
        b -= Separation_eps;
    }
}

double NewtonMethod(double a, double b)
{
    double x1 = (a + b) / 2, x0 = b;
    double delta;

    Nq = 2;

    while (abs(x1-x0) > Newton_eps)
    {
        x0 = x1;
        try
        {
            delta = -f(x0)/fder(x0);
        }
        catch(Err)
        {
            cout << "Error. Division by zero. Solve function\n";
            return 0;
        }
        x1 = x0 + delta;

        Nq += 34;
    }
    return x1;
}

void NewtonMethod()
{
    for (int i = 0; i < counter; i++)
        NewtonRoots[Ncounter++] = NewtonMethod(Left[i], Right[i]);
}

void printNewton()
{
    cout << "Корни по Ньютону:\n";
    for (int i = 0; i < Ncounter; i++)
        cout << setprecision(12) << NewtonRoots[i] << "\n";
}

double iteration_method(double a, double b)
{
    double lambda = 1.0/find_max_fder(a, b);

    double x1 = (a + b)/2, x0 = b;

    Ni = 2;

    while (abs(x1-x0) > Iteration_eps)
    {
        x0 = x1;
        x1 = x0 - lambda * f(x0);
        Ni += 19;
    }
    return x1;
}

void IterationMethod()
{
    for (int i = 0; i < counter; i++)
        IterationRoots[Icounter++] = iteration_method(Left[i], Right[i]);
}

void printIteration()
{
    cout << "Корни по методу итераций:\n";
    for (int i = 0; i < Icounter; i++)
        cout << setprecision(12) << IterationRoots[i] << "\n";
}

void print_intervals()
{
    cout << "Интервалы, где содержится по 1 корню:\n";
    for (int i = 0; i < counter; i++)
        cout << setprecision(12) << Left[i] << " " << Right[i] << "\n";
}

int main()
{

    setlocale(LC_CTYPE, "Russian");

    Separation(-2, 2);
    print_intervals();

    NewtonMethod();
    printNewton();

    IterationMethod();
    printIteration();


    cout << "Количество итерация в методе Ньютона: " << Nq << "\n";
    cout << "Количество итерация в методе итераций: " << Ni << "\n";

    return 0;
}
