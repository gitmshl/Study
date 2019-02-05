#include <iostream>
#include <fstream>
#include "Network.hpp"

using namespace std;

int main()
{
   int Sizes[] = {784, 16, 16, 10};

    Network p(Sizes, 3, 0.5);

    int epochs = 1;

    int MAX_ITERATION = 100;

    double Answers[10];

    double Inputs[784];

    for (int epoch = 0; epoch < epochs; epoch++)
    {

        ifstream fin("mnist_train.txt");

        for (int iteration = 0; iteration < MAX_ITERATION; iteration++)
        {
            for (int i = 0; i < 10; i++) Answers[i] = 0;

            int answer;
            fin >> answer; Answers[answer] = 1;
            for (int j = 0; j < 784; j++)
                fin >> Inputs[j];

            p.learn(Inputs, 784, Answers, 10);

        }

    }

    cout << "ok\n";

    p.save_to_file("neuro.txt");


    //Network p("neuro.txt");

 /*

    p.print();



   p.examen("test.txt", true);
    cout << "---------------------------\n";
     for (int i = 0; i < 1000; i++)
            p.learn_with_teacher("learn.txt");

    p.examen("test.txt", true);

    p.print();

    p.save_to_file("neuro.txt");

*/


    return 0;
}
