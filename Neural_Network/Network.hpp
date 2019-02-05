#ifndef NETWORK
#define NETWORK

#include <stdlib.h>
#include "Network_Description.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

class Except{};

Network::Network(int *Sizes, int Layers, double GRADIENT_STEP = 0)
{
    if (GRADIENT_STEP) gradient_step = GRADIENT_STEP;
    layers = Layers;
    sizes = new int[layers + 1];
    for (int i = 0; i <= layers; i++)
        sizes[i] = Sizes[i];

/// Создаем необходимые массивы
    weights = new double ** [layers];
    for (int i = 0; i < layers; i++)
    {
        weights[i] = new double * [sizes[i] + 1];
        for (int j = 0; j <= sizes[i]; j++)
            weights[i][j] = new double [sizes[i + 1] + 1];
    }

    o = new double * [layers + 1];
    for (int i = 0; i <= layers; i++)
    {
        o[i] = new double [sizes[i] + 1];
        o[i][0] = 1;
    }

    derWeights = new double ** [layers];
    for (int i = 0; i < layers; i++)
    {
        derWeights[i] = new double * [sizes[i] + 1];
        for (int j = 0; j <= sizes[i]; j++)
            derWeights[i][j] = new double [sizes[i + 1] + 1];
    }

/// Инициализация
    for (int layer = 0; layer < layers; layer++)
        for (int i = 0; i <= sizes[layer]; i++)
            for (int j = 0; j <= sizes[layer + 1]; j++)
                weights[layer][i][j] = (rand() % RAND)/(1.0 * RAND*RAND);
}

double Network::f_ellum(double a)
{
    return a > 0.5 ? 1 : 0.01;
}

double Network::f(double a)
{
   // return a > 0 ? 1 : 0;
    return 1.0/(1.0 + exp(-a));
}

double Network::derf(double a)
{
    double b = f(a);
    return b * (1 - b);
    //return 1;
}

void Network::FeedForward(double *Inp, int n = -1) /// Inp нумерация с 0!
{
    if (n == -1) n = sizes[0];
    if (n != sizes[0])
    {
        std::cout << "Error! FeedForward. len(Inp) != len(RealInput)!\n";
        throw new Except();
    }
    for (int k = 0; k < n; k++)
        o[0][k + 1] = Inp[k];

    for (int layer = 1; layer <= layers; layer++)
    {
        for (int k = 1; k <= sizes[layer]; k++)
        {
            o[layer][k] = 0;
            for (int t = 0; t <= sizes[layer - 1]; t++)
                o[layer][k] += o[layer - 1][t] * weights[layer - 1][t][k];
            o[layer][k] = f(o[layer][k]);
        }
    }
}

int Max_in_Array(int *Sizes, int n)
{
    int Max = Sizes[0];
    for (int i = 1; i < n; i++)
        if (Sizes[i] > Max) Max = Sizes[i];
    return Max;
}

void Network::BackPropagation(double *Answers)
{
    int leng = Max_in_Array(sizes, layers + 1) + 1;
    double delta[leng], delta1[leng];

    ///Считаем на последнем слое
    for (int i = 1; i <= sizes[layers]; i++)
        delta[i] = 2 * (o[layers][i] - Answers[i-1])*o[layers][i]*(1-o[layers][i]);

    for (int i = 1; i <= sizes[layers]; i++)
        delta1[i] = delta[i];

    for (int i = 0; i <= sizes[layers-1]; i++)
        for (int j = 1; j <= sizes[layers]; j++)
            derWeights[layers-1][i][j] = delta[j] * o[layers-1][i];

    for (int layer = layers - 1; layer > 0; layer--)
    {
        for (int i = 1; i <= sizes[layer]; i++)
        {
            delta[i] = 0;
            for (int k = 1; k <= sizes[layer+1]; k++)
                delta[i] += delta1[k] * weights[layer][i][k];
            delta[i] *= o[layer][i] * (1-o[layer][i]);
        }
        /// копия
        for (int i = 1; i <= sizes[layer]; i++)
            delta1[i] = delta[i];

        for (int i = 0; i <= sizes[layer-1]; i++)
            for (int j = 1; j <= sizes[layer]; j++)
                derWeights[layer-1][i][j] = delta[j] * o[layer-1][i];
    }
}


void Network::learn(double *Inp, int InpSize, double *Answers, int AnswerSize)
{
    if (InpSize != sizes[0] || AnswerSize != sizes[layers])
    {
        std::cout << "Error! learn. InpSize != RealInputSize or AnswerSize != RealAnswerSize!\n";
        throw new Except();
    }

    FeedForward(Inp, InpSize);
    BackPropagation(Answers);
    for (int layer = 0; layer < layers; layer++)
        for (int i = 0; i <= sizes[layer]; i++)
            for (int j = 1; j <= sizes[layer + 1]; j++)
                weights[layer][i][j] -=  gradient_step * derWeights[layer][i][j];
}

void Network::learn_with_teacher(char * FileName)
{
    std::ifstream fin(FileName);
    int Count, InpSize, AnsSize;
    fin >> Count >> InpSize >> AnsSize;
    double Inp[InpSize], Answers[AnsSize];
    int q = 0;

    for (int i = 0; i < Count; i++)
    {
        for (int j = 0; j < InpSize; j++)
            fin >> Inp[j];
        for (int j = 0; j < AnsSize; j++)
            fin >> Answers[j];
        learn(Inp, InpSize, Answers, AnsSize);
    }
}

bool Network::ex(double *Inp, int InpSize, double *Answers, int AnswerSize, bool ellum = false)
{
    if (InpSize != sizes[0] || AnswerSize != sizes[layers])
    {
        std::cout << sizes[0] << " " << InpSize << " " << AnswerSize << " "
         << sizes[layers] << "\n";
        std::cout << "Error! ex. InpSize != RealInputSize or AnswerSize != RealAnswerSize!\n";
        throw new Except();
    }

    FeedForward(Inp, InpSize);

    if (!ellum)
    {
        for (int i = 1; i <= sizes[layers]; i++)
            if (o[layers][i] != Answers[i-1]) return false;
    }
    else
    {
        for (int i = 1; i <= sizes[layers]; i++)
            if (f_ellum(o[layers][i]) != Answers[i-1])
            {
                //std::cout << i-1 << "\n";
                //std::cout << f_ellum(o[layers][i]) << " " << Answers[i-1]<<"\n";
                return false;
            }
    }


    return true;
}

void Network::examen(char *FileName, bool ellum = false)
{
    std::ifstream fin(FileName);
    int Count, InpSize, AnsSize;
    fin >> Count >> InpSize >> AnsSize;
    double Inp[InpSize], Answers[AnsSize];
    int q = 0;

    for (int i = 0; i < Count; i++)
    {
        for (int j = 0; j < InpSize; j++)
            fin >> Inp[j];
        for (int j = 0; j < AnsSize; j++)
            fin >> Answers[j];
        if (ex(Inp, InpSize, Answers, AnsSize, ellum)) {std::cout << "YES "; print_result(); q++;}
        else {std::cout << "NO "; print_result();}
    }
    std::cout << "Right: " << q << " / " << Count << "\n";
}


void Network::derweightsprint()
{
    for (int layer = 0; layer < layers; layer++)
    {
        std::cout << "layer " << layer << "\n";
        for (int i = 0; i <= sizes[layer]; i++)
        {
            for (int j = 1; j <= sizes[layer + 1]; j++)
                std::cout << derWeights[layer][i][j] << " ";
            std::cout << "\n";
        }
    }
}

void Network::print()
{
    for (int layer = 0; layer < layers; layer++)
    {
        std::cout << "layer " << layer << "\n";
        for (int i = 0; i <= sizes[layer]; i++)
        {
            for (int j = 1; j <= sizes[layer + 1]; j++)
                std::cout << weights[layer][i][j] << " ";
            std::cout << "\n";
        }
    }
}

void Network::print_result()
{
    std::cout << "Result: ";
    for (int i = 1; i <= sizes[layers]; i++)
        std::cout << o[layers][i] << " ";
    std::cout << "\n";
}

Network::Network(char *FileName)
{
    std::ifstream fin(FileName);
    int SizesSize;
    fin >> SizesSize;

    layers = SizesSize;

    sizes = new int[layers + 1];

    for (int i = 0; i <= layers; i++)
        fin >> sizes[i];

    weights = new double ** [layers];
    for (int i = 0; i < layers; i++)
    {
        weights[i] = new double * [sizes[i] + 1];
        for (int j = 0; j <= sizes[i]; j++)
            weights[i][j] = new double [sizes[i + 1] + 1];
    }

    o = new double * [layers + 1];
    for (int i = 0; i <= layers; i++)
    {
        o[i] = new double [sizes[i] + 1];
        o[i][0] = 1;
    }

    derWeights = new double ** [layers];
    for (int i = 0; i < layers; i++)
    {
        derWeights[i] = new double * [sizes[i] + 1];
        for (int j = 0; j <= sizes[i]; j++)
            derWeights[i][j] = new double [sizes[i + 1] + 1];
    }

    for (int layer = 0; layer < layers; layer++)
    {
        for (int i = 0; i <= sizes[layer]; i++)
        {
            for (int j = 1; j <= sizes[layer + 1]; j++)
                fin >> weights[layer][i][j];
        }
    }

    fin.close();
}

void Network::save_to_file(char *FileName)
{
    std::ofstream fout(FileName);
    fout << layers << " ";
    for (int i = 0; i <= layers; i++)
        fout << sizes[i] << " ";
    fout << "\n";
    for (int layer = 0; layer < layers; layer++)
    {
        for (int i = 0; i <= sizes[layer]; i++)
        {
            for (int j = 1; j <= sizes[layer + 1]; j++)
                fout << weights[layer][i][j] << " ";
            fout << "\n";
        }
    }
    fout.close();
}

#endif // NETWORK
