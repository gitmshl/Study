#ifndef NETWORK_DESCRIPTION
#define NETWORK_DESCRIPTION

class Network
{
    int RAND = 10000; /// показывает степень размаха случаной инициализации весов
    double learning_rate = 0.6;
    int layers;
    int * sizes;
    double *** weights;
    double ** o;
    double *** derWeights; /// dL/dw

public:
    Network(int *Sizes, int Layers, double LEARNING_RATE, int RAND1);
    Network(char *FileName);
    void FeedForward(double *Inp, int n);
    double f(double a);
    void BackPropagation(double *Answers);
    void learn(double *Inp, int InpSize, double *Answers, int AnswersSize);
    void learn(char *FileName, int Epochs, int Count);
    int IndMax_in_Array(double *F, int n);
    bool ex(double *Inp, int InpSize, int Answer);
    void examen(char *FileName, int Epochs, int Count, bool print1);
    void print();
    void print_result();
    void save_to_file(char * FileName);
};

#endif // NETWORK_DESCRIPTION
