#ifndef FILTER_H
#define FILTER_H

#include <fstream>
#include <iostream>
#include "Consts.hpp"
/*
    ���� ���� �������� �������, ������� ��������� ��������� ������, ����� ������-
    ��� �� ������������, ����� ����� ����� ���������, ��� ����� �� ���������
    ���������� �������������, ������ ����, ����� ��������������� ��
*/


/*
    �������������� ������ ����������� ���������, ������� �����
    ������������
*/
void Initialize_NotAcces_Array()
{

    FILE *f = fopen("Moduls_For_Project/Filter_Process.txt", "r");
    char Name[20];

if (f == NULL) std::cout << "Fall  file\n";

else {
    while (!feof(f))
    {
      fscanf(f, "%s", &Name);
        HWND s = FindWindow(NULL, TEXT(Name));
        if (s != NULL && s)
            NotAcces[NotAcces_Size++] = s;
    }

    fclose(f);
}
}

bool acces(HWND h)
{
    for (int i = 0; i < NotAcces_Size; i++)
    {
        if (h == NotAcces[i]) return false;
    }
    return true;
}

#endif // FILTER_H
