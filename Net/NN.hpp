#ifndef NETWORK
#define NETWORK

#include <stdlib.h>
#include "NN_Description.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

class Except{};

Network::Network(int *Sizes, int Layers, double LEARNING_RATE = 0, int RAND1 = 0)
{
    if (LEARNING_RATE) learning_rate = LEARNING_RATE;
    if (RAND1) RAND = RAND1;

    layers = Layers;
    sizes = new int[layers + 1];

    for (int i = 0; i <= layers; i++)
        sizes[i] = Sizes[i];

/// Создаем необходимые массивы
    weights = new double ** [layers];
    for (int i = 0; i < layers; i++)
    {
        weights[i] = new double * [sizes[i]];
        for (int j = 0; j < sizes[i]; j++)
            weights[i][j] = new double [sizes[i + 1]];
    }

    o = new double * [layers + 1];
    for (int i = 0; i <= layers; i++)
        o[i] = new double [sizes[i]];


    derWeights = new double ** [layers];
    for (int i = 0; i < layers; i++)
    {
        derWeights[i] = new double * [sizes[i]];
        for (int j = 0; j < sizes[i]; j++)
            derWeights[i][j] = new double [sizes[i + 1]];
    }

    /// Инициализация
    for (int layer = 0; layer < layers; layer++)
        for (int i = 0; i < sizes[layer]; i++)
            for (int j = 0; j < sizes[layer + 1]; j++)
                weights[layer][i][j] = ((rand() % RAND)-RAND/2)/(1.0 * RAND);
}

Network::Network(char * FileName)
{
    std::ifstream fin(FileName);

    fin >> layers;
    for (int i = 0; i <= layers; i++)
        fin >> sizes[i];
    for (int layer = 0; layer < layers; layer++)
    {
        for (int i = 0; i < sizes[layer]; i++)
            for (int j = 0; j < sizes[layer + 1]; j++)
                fin >> weights[layer][i][j];
    }
}

double Network::f(double a)
{
    return 1.0/(1.0 + exp(-a));
}


void Network::FeedForward(double *Inp, int n = -1)
{
    if (n == -1) n = sizes[0];
    if (n != sizes[0])
    {
        std::cout << "Error! FeedForward. len(Inp) != len(RealInput)!\n";
        throw new Except();
    }
    for (int k = 0; k < n; k++)
        o[0][k] = Inp[k];

    for (int layer = 1; layer <= layers; layer++)
    {
        for (int k = 0; k < sizes[layer]; k++)
        {
            o[layer][k] = 0;
            for (int t = 0; t < sizes[layer - 1]; t++)
                o[layer][k] += o[layer - 1][t] * weights[layer - 1][t][k];
            o[layer][k] = f(o[layer][k]);
        }
    }
}

void Network::BackPropagation(double *Answers)
{

    double errors[783];
    double errors1[783];

    for (int i = 0; i < sizes[layers]; i++)
    {
        errors[i] = o[layers][i] - Answers[i];
        errors1[i] = errors[i];
    }

    for (int i = 0; i < sizes[layers-1]; i++)
        for (int j = 0; j < sizes[layers]; j++)
            derWeights[layers-1][i][j] = errors[j]*o[layers][j]*(1.0-o[layers][j])*o[layers-1][i];

    for (int layer = layers - 1; layer > 0; layer--)
    {
        for (int i = 0; i < sizes[layer]; i++)
        {
            errors[i] = 0.0;
            for (int j = 0; j < sizes[layer+1]; j++)
                errors[i] += weights[layer][i][j] * errors1[j];
        }


        for (int i = 0; i < sizes[layer]; i++)
            errors1[i] = errors[i];

        for (int i = 0; i < sizes[layer-1]; i++)
            for (int j = 0; j < sizes[layer]; j++)
                derWeights[layer-1][i][j] = errors[j]*o[layer][j]*(1.0-o[layer][j])*o[layer-1][i];
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
        for (int i = 0; i < sizes[layer]; i++)
            for (int j = 0; j < sizes[layer + 1]; j++)
                weights[layer][i][j] -=  learning_rate * derWeights[layer][i][j];


}

void Network::learn(char *FileName, int Epochs, int Count)
{
    std::ifstream fin(FileName);
    double Answers[sizes[layers]];
    for (int i = 0; i < sizes[layers]; i++)
        Answers[i] = 0.01;
    double Inp[Count];
    double a;  int answer;
    for (int epoch = 0; epoch < Epochs; epoch++)
    {
        fin >> answer;
        Answers[answer] = 0.99;
        for (int i = 0; i < Count; i++)
        {
            fin >> a;
            Inp[i] = a/255.0 * 0.99 + 0.01;
        }
        learn(Inp, Count, Answers, sizes[layers]);
        //std::cout << epoch << "\n";
    }
    fin.close();
}

int Network::IndMax_in_Array(double *F, int n)
{
    if (n <= 0)
    {
        std::cout << "Error. IndMax_in_Array. n <= 0!!!\n";
        throw new Except();
    }
    int ind = 0;
    for (int i = 1; i < n; i++)
        if (F[i] > F[ind]) ind = i;
    return ind;
}

bool Network::ex(double *Inp, int InpSize, int Answer)
{
    if (InpSize != sizes[0])
    {
        std::cout << "Error! ex. InpSize != RealInputSize\n";
        throw new Except();
    }

    FeedForward(Inp, InpSize);
    int result = IndMax_in_Array(o[layers], sizes[layers]);

    if (result == Answer) return true;

    return false;
}

void Network::examen(char *FileName, int Epochs, int Count, bool print1 = false)
{
    std::ifstream fin(FileName);
    int Answer;
    double Inp[Count];
    int a;
    int q = 0;
    for (int epoch = 0; epoch < Epochs; epoch++)
    {
        fin >> Answer;
        for (int i = 0; i < Count; i++)
        {
            fin >> a;
            Inp[i] = a/255.0 * 0.99 + 0.01;
        }
        if (ex(Inp, Count, Answer))
        {
            std::cout << "YES\n";
            q++;
        }
        else
        {
            std::cout << "NO\n";
            if (print1)
                std::cout << "Answer: " << Answer << " Our Result: " << IndMax_in_Array(o[layers], sizes[layers]) << "\n";
        }
    }

    std::cout << "Result: " << q << "/" << Epochs <<"\n\n";
    fin.close();
}


void Network::print()
{
    for (int layer = 0; layer < layers; layer++)
    {
        std::cout << "layer " << layer << "\n";
        for (int i = 0; i < sizes[layer]; i++)
        {
            for (int j = 0; j < sizes[layer + 1]; j++)
                std::cout << weights[layer][i][j] << " ";
            std::cout << "\n";
        }
    }
}

void Network::print_result()
{
    std::cout << "Result: ";
    for (int i = 0; i < sizes[layers]; i++)
        std::cout << o[layers][i] << " ";
    std::cout << "\n";
}

void Network::save_to_file(char * FileName)
{
    std::ofstream fout(FileName);

    fout << layers << " ";

    for (int i = 0; i <= layers; i++)
        fout << sizes[i] << " ";
    fout << "\n";
    for (int layer = 0; layer < layers; layer++)
    {
        for (int i = 0; i < sizes[layer]; i++)
            for (int j = 0; j < sizes[layer + 1]; j++)
                fout << weights[layer][i][j] << " ";
        fout << "\n";
    }
}

#endif // NETWORK
