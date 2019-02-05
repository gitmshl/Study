#include <iostream>
#include "NN.hpp"

using namespace std;

int main()
{

    int Sizes[] = {783, 100, 10};

    //Network n(Sizes, 2, 0.3, 1000);
    Network n("neuro.txt");
   // n.print();

    n.examen("mnist_train.txt", 10, 783, true);

    for (int i = 0; i < 30; i++)
        n.learn("mnist_train.txt", 100, 783);

    n.examen("mnist_train.txt", 10, 783, true);

    n.print();
    n.save_to_file("neuro.txt");

    return 0;
}
