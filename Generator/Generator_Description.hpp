#ifndef GENERATOR_DESCRIPTION_HPP
#define GENERATOR_DESCRIPTION_HPP

class Generator
{
public:
    int n; /// ���-�� �������� � ��������
    int Max_Length; /// ������������ ���-�� �����������, ������� ������� ���������
    char * Alphabet;
    int length;
    int * S;
    Generator(int N, char * A, int MAX_LENGTH);
    void print_current();
    void Next();
    void Origin(); /// ���������� ��������� � ��������� ���������
    void Zeroing(); /// ��������� ���� ��������� ������� S, � ����������� length
    void print(int Count);
    void print();
};

#endif // GENERATOR_DESCRIPTION_HPP
