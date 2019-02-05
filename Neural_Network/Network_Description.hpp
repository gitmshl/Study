#ifndef NETWORK_DESCRIPTION
#define NETWORK_DESCRIPTION

class Network
{
    int RAND = 10000; /// показывает степень размаха случаной инициализации весов
    double gradient_step = 0.6;
    int layers;
    int * sizes;
    double *** weights;
    public:
    double ** o;
    double *** derWeights; /// dL/dw

public:
    Network(int *Sizes, int Layers, double GRADIENT_STEP);
    Network(char *FileName);
    double f_ellum(double a);
    double f(double a);
    double derf(double a);
    void FeedForward(double *Inp, int n);
    void BackPropagation(double * Answers);
    void learn(double *Inp, int InpSize, double *Answers, int AnswerSize);
    void learn_with_teacher(char * FileName);
    bool ex(double *Inp, int InpSize, double *Answers, int AnswerSize, bool ellum);
    void examen(char *FileName, bool ellum);
    void derweightsprint();
    void print();
    void print_result();
    void Init_from_file(char *FileName);
    void save_to_file(char *FileName);
};

#endif // NETWORK_DESCRIPTION
