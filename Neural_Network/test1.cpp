#include <iostream>
#include <fstream>

using namespace std;

char FileName[] = "learn.txt";
ofstream fout(FileName);

bool End = false;
int length = 1;

void Next(int *S, int n)
{
    S[0]++;
    if (S[0] < 2) return;
    S[0] = 0;
    int q = 1, i = 1;
    while (q && i < n * n)
    {
        S[i]++; q = 0;
        if (S[i] == 2) {q = 1; S[i] = 0;}
        i++;
    }
    if (q && i == n*n) End = true;

}

bool allowed(int *S, int n)
{
    ifstream fin("test.txt");
    int Count;
    fin >> Count;
    for (int i = 0; i < Count; i++)
    {
        int a;
        bool eq = true;
        for (int j = 0; j < n * n; j++)
        {
            fin >> a;
            if (S[j] != a) eq = false;
        }
        if (eq) {fin.close(); return true; }
    }
    fin.close();
    return false;
}

void load(int *S, int n)
{
    for (int i = 0; i < n*n; i++)
        fout << S[i] << " ";

    int answer = 0;
    if (allowed(S, n)) answer = 1;

    fout << answer << "\n";
}

int main()
{
    int n = 3;

    int S[n*n];

    for (int i = 0; i < n*n; i++)
        S[i] = 0;

   while (!End)
   {
       load(S, n);
       Next(S, n);
   }


    return 0;
}
