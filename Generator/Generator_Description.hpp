#ifndef GENERATOR_DESCRIPTION_HPP
#define GENERATOR_DESCRIPTION_HPP

class Generator
{
public:
    int n; /// кол-во символов в алфавите
    int Max_Length; /// максимальное кол-во предложений, которые выведет генератор
    char * Alphabet;
    int length;
    int * S;
    Generator(int N, char * A, int MAX_LENGTH);
    void print_current();
    void Next();
    void Origin(); /// ¬озвращает генератор в начальное состо€ние
    void Zeroing(); /// обнуление всех элементов массива S, с сохранением length
    void print(int Count);
    void print();
};

#endif // GENERATOR_DESCRIPTION_HPP
