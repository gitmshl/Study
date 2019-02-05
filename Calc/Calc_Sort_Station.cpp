#include <iostream>
#include <stdlib.h>
#include <stack>
#include <cmath>

using namespace std;

const int sum = 1;
const int Minus = -1;
const int mult = 2;
const int divide = -2;
const int Cos = 3;
const int Sin = -3;
const int bracket_open = 4;
const int bracket_close = -4;

class Ex{};

stack<double> s;
stack<int> operation;

int abs(int a)
{
    return a >= 0 ? a : -a;
}

double ans_reverse_polish_entry(char *F, int n) ///
{
    int i = 0;
    while (i < n)
    {
        if (F[i] >= '0' && F[i] <= '9')
        {
            char N[10];
            int q = 0;
            while (F[i] != ',' && i < n)
            {
                N[q] = F[i];
                q++;
                i++;
            }
            char T[q];
            for (int k = 0; k < q; k++)
                T[k] = N[k];
            s.push(atof(T));
        }
        else if (F[i] == '+')
        {
            if (s.size() == 1)
            {
                /// ничего не делаем
            }
            if (s.size() < 2)
            {
                cout << "ERROR. SIZE < 2!\n";
                throw new Ex();
            }
            double b = s.top();
            s.pop();
            double a = s.top();
            s.pop();
            double c = a + b;
            s.push(c);
        }
        else if (F[i] == '-')
        {
            if (s.size() == 1)
            {
                double b = s.top();
                s.pop();
                s.push(-b);
            }
             if (s.size() < 2)
            {
                cout << "ERROR. SIZE < 2!\n";
                throw new Ex();
            }
            double b = s.top();
            s.pop();
            double a = s.top();
            s.pop();
            double c = a - b;
            s.push(c);
        }
        else if (F[i] == '*')
        {
             if (s.size() < 2)
            {
                cout << "ERROR. SIZE < 2!\n";
                throw new Ex();
            }
            double b = s.top();
            s.pop();
            double a = s.top();
            s.pop();
            double c = a * b;
            s.push(c);
        }
        else if (F[i] == '/')
        {
             if (s.size() < 2)
            {
                cout << "ERROR. SIZE < 2!\n";
                throw new Ex();
            }
            double b = s.top();
            s.pop();
            double a = s.top();
            s.pop();
            double c = a / b;
            s.push(c);
        }
        else if (F[i] == 's')
        {
             if (s.size() < 1)
            {
                cout << "ERROR. SIZE < 1!\n";
                throw new Ex();
            }
            double b = s.top();
            s.pop();
            double c = sin(b);
            s.push(c);
        }
        else if (F[i] == 'c')
        {
            if (s.size() < 1)
            {
                cout << "ERROR. SIZE < 1!\n";
                throw new Ex();
            }
            double b = s.top();
            s.pop();
            double c = cos(b);
            s.push(c);
        }

        i++;
    }


    return s.top();

}

int Size(char *F){
    int j = 0;
    while (F[j])
        j++;
    return j;
}

double ans(char *F, int n)
{
    char T[400];
    int q = 0;
    int i = 0;
    while (i < n)
    {
        if (F[i] >= '0' && F[i] <= '9')
        {
            char N[10];
            int w = 0;
            while (i < n && ((F[i] >= '0' && F[i] <= '9')||(F[i] == '.') || (F[i]=='e')))
            {
                N[w] = F[i];
                w++;
                i++;
            }
            for (int k = 0; k < w; k++)
                T[q++] = N[k];
            T[q++] = ',';
        }
        else if (F[i] == '+')
        {
            while (operation.size() && operation.top() < bracket_open && abs(operation.top()) >= sum)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.push(sum);
            i++;
        }
        else if (F[i] == '-')
        {
            while (operation.size() && operation.top() < bracket_open && abs(operation.top()) >= sum)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.push(Minus);
            i++;
        }
        else if (F[i] == '*')
        {
            while (operation.size() && operation.top() < bracket_open && abs(operation.top()) >= mult)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.push(mult);
            i++;
        }
        else if (F[i] == '/')
        {
            while (operation.size() && operation.top() < bracket_open && abs(operation.top()) >= mult)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.push(divide);
            i++;
        }
        else if (F[i] == 's')
        {
            while (operation.size() && operation.top() < bracket_open && abs(operation.top()) >= Cos)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.push(Sin);
            i+=3;
        }
        else if (F[i] == 'c')
        {
            while (operation.size() && operation.top() < bracket_open && abs(operation.top()) >= Cos)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.push(Cos);
            i+=3;
        }
        else if (F[i] == '(')
        {
            operation.push(bracket_open);
            i++;
        }
        else if (F[i] == ')')
        {
            while (operation.size() && operation.top() != bracket_open)
            {
                switch (operation.top())
                {
                    case sum: T[q++] = '+'; break;
                    case Minus: T[q++] = '-'; break;
                    case mult: T[q++] = '*'; break;
                    case divide: T[q++] = '/'; break;
                    case Cos: T[q++] = 'c'; break;
                    case Sin: T[q++] = 's'; break;
                }
                T[q++] = ',';
                operation.pop();
            }
            operation.pop();
            i++;
        }
        else i++;
    }

    while (operation.size())
    {
        switch (operation.top())
        {
            case sum: T[q++] = '+'; break;
            case Minus: T[q++] = '-'; break;
            case mult: T[q++] = '*'; break;
            case divide: T[q++] = '/'; break;
            case Cos: T[q++] = 'c'; break;
            case Sin: T[q++] = 's'; break;
        }
        T[q++] = ',';
        operation.pop();
    }

    char * res = new char[q-1];
    for (int k = 0; k < q-1; k++)
    {
         res[k] = T[k];

    }
    return ans_reverse_polish_entry(res, q-1);
}

int main()
{

    char F[50];
    cin >> F;

    cout << ans(F, Size(F));

    ///cout << res;
    return 0;
}
