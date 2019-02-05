#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

const int MAX_WORD_LENGTH = 30;

ofstream fout;
ifstream fin;

bool GameOver = false; /// ;) если true, то выходим из программы
bool OverlookMistakes = true; /// true - пропускать ошибки, false - выход из программы при
///   обнаружении ошибки (настраивается пользователем)
bool CheckMistakes = false; /// проверка наличия ошибок
bool ShowMistakes = true;
int Err = 0; /// глобальная переменная, хранящая код ошибки (0 - отсутствие ошибок)

int line, column;

char Symbols[] = {':', '(', ')', '.', '*', ';', ',', '#', '[', ']'};
char NTerminals[50][MAX_WORD_LENGTH]; int NTerminalsCounter = 0;
char Terminals[50][MAX_WORD_LENGTH]; int TerminalsCounter = 0;
char Semantics[50][MAX_WORD_LENGTH]; int SemanticsCounter = 0;

int SymbolResolver(char c);
int SemanticResolver(char *word);
int TerminalResolver(char *word);
int NTerminalResolver(char *word);
int EofgramResolver(char *word);
int Resolver(char *word);

int SymbolCode(char *word);
int SemanticCode(char *word);
int TerminalCode(char *word);
int NTerminalCode(char *word);

void Output(int code);
void CodeComparer(int key, char *word);
void ExceptionHandler(char *word);

void SymbolCheck(char * word); /// Код ошибки (Err): 1
void SemanticCheck(char * word); /// 2
void TerminalCheck(char * word);   /// 3
void NTerminalCheck(char * word);   /// 4

int main()
{
    setlocale(LC_CTYPE, "Russian");
    cout << "Настройка транслятора:\nВвыберите один из режимов работы транслятора\n";
    cout << "0 - Транслятор выводит предупреждения, игнорируя их при трансляции\n";
    cout << "1 - Транслятор выводит предупреждения, завершая работу программы при их обнаружении\n";
    cout << "2 - Транслятор не выводит предупреждений. Все гипотетические ошибки считаются нетерминалами\n";
    cout << "3 - Ничего не понимаю\n";
    int choose;
    cout << "Выберите номер: ";
    cin >> choose;

    switch (choose)
    {
        case 0: CheckMistakes = true; break;
        case 1: CheckMistakes = true; OverlookMistakes = false; break;
        case 2: ShowMistakes = false; break;
        case 3: ShowMistakes = false; break;
    }
    char FileName[50];
    cout << "Введите название файла, откуда нужно считать текст: ";
    cin >> FileName;
    fin.open(FileName, fin.binary);

    cout << "Введите название файла, куда нужно записать результат: ";
    cin >> FileName;
    fout.open(FileName);
    column = 0;
    line = 1;
    bool next_line = false; /// идентификатор перехода на следующую строку

    char word[MAX_WORD_LENGTH];
    int key;
    while (fin >> word && !GameOver)
    {
        if (next_line)
        {
            fout << "\n";
            line++;
            column = 0;
            next_line = false;
        }

         column++;

         /* Для проверки перехода на новую строку */
        char a[2];
        fin.read(a, 2);
        if (a[1] == '\n')
        {
            next_line = true;
        }
        fin.seekg(-2, fin.cur);

        //////////////////////// INIT ///////////////////////
        key = -1;
        Err = 0;
        ////////////////////////////////////////////////////
        key = Resolver(word);
        if (key == -1 || Err)
        {
            ExceptionHandler(word);
            CodeComparer(key, word);
        }
        else CodeComparer(key, word);
    /// Вызов Output производится внутри CodeComparer и, мб, ExceptionHandler(в случае, если нужно)
    }
    cout << "\n\n";
    if (OverlookMistakes) cout << "Работа транслятора успешно окончена\n";
    else if (Err) cout << "Транслятор принудительно остановлен из-за возможной ошибки\n";
    else cout << "Работа транслятора успешно окончена\n";
    system("pause");
    return 0;
}

void SymbolCheck(char * word)
{
    for (int i = 0; i < 10; i++)
        if (Symbols[i] == word[0])
        {
            Err = 1; /// код ошибки, означающий, что это гипотетически символ
            break;
        }
}

void SemanticCheck(char * word)
{
    if (word[0] == '$')
    {
        for (int i = 1; word[i]; i++)
            if (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z')))
            {
                Err = 2;
                return;
            }
    }
}

void TerminalCheck(char * word)
{
    int Count = 0;
    for (int i = 0; word[i]; i++)
    {
        if (word[i] >= 'a' && word[i] <= 'z') Count++;
    }
    if (Count >= strlen(word)/2) Err = 3;
}


void NTerminalCheck(char * word)
{
    for (int i = 0; word[i]; i++)
    {
        if (!((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z')) || (word[i] >= '0' && word[i] <= '9'))
        {
            Err = 4;
            return;
        }
    }
}

int EofgramResolver(char *word)
{
    if (!strcmp(word, "Eofgram")) return 4;
    return -1;
}

int NTerminalResolver(char *word)
{
    if (!CheckMistakes || Err) return 3;
    NTerminalCheck(word);
    return 3;
}

int TerminalResolver(char *word)
{

    char a[30];
    fin >> a;
     int len1 = strlen(a);
    if (!strcmp(a, ":")) /// проверка того, что это не терминал
    {
        fin.seekg(-len1, fin.cur);
        return -1;
    }
    fin.seekg(-len1, fin.cur);
    bool err = false;
    int len = strlen(word);
    if ((word[0] == '\'' && word[len-1] == '\'') || (word[0] == '"' && word[len-1]== '"')) return 2;

    for (int i = 0; i < len; i++)
    {
        if (!(word[i] >= 'a' && word[i] <= 'z')) {err = true; break;}
    }
    if (!err) return 2;

    if (CheckMistakes && !Err) TerminalCheck(word);
    return -1;
}

int SemanticResolver(char *word)
{
    if (word[0] == '$')
    {
        if (CheckMistakes && !Err) SemanticCheck(word);
        return 1;
    }
    return -1;
}

int SymbolResolver(char *word)
{
    if (strlen(word) == 1)
    {
        for (int i = 0; i < 10; i++)
            if (Symbols[i] == word[0]) return 0;
    }
    else if (CheckMistakes) SymbolCheck(word);
    return -1;
}

int Resolver(char *word)
{
    int key = -1;
    key = EofgramResolver(word);
    if (key == 4) return key;
    key = SymbolResolver(word);
    if (!key) return key;
    key = SemanticResolver(word);
    if (key == 1) return key;
    key = TerminalResolver(word);
    if (key == 2) return key;
    key = NTerminalResolver(word);
    if (key == 3) return key;
    return key;
}

void Output(int code)
{
    if (code == 1998) {fout << "? "; return;} /// отмечаем ошибку
    fout << code << " ";
    if (code == 1000) GameOver = true;
}

int SymbolCode(char *word)
{
    for (int i = 0; i < 10; i++)
        if (word[0] == Symbols[i]) return (i+1);
    return -1;
}

int SemanticCode(char *word)
{
    for (int i = 0; i < SemanticsCounter; i++)
    {
        if (!strcmp(word, Semantics[i])) return (101 + i);
    }
    strcat(Semantics[SemanticsCounter], word);
    SemanticsCounter++;
    if (SemanticsCounter > 49) cout << "ERR l pc 267\n";
    return 101 + (SemanticsCounter - 1);
}

int TerminalCode(char *word)
{
    for (int i = 0; i < TerminalsCounter; i++)
    {
        if (!strcmp(word, Terminals[i])) return (51 + i);
    }
    strcat(Terminals[TerminalsCounter], word);
    TerminalsCounter++;
    if (TerminalsCounter > 49) cout << "ERR l pc 279\n";
    return 51 + (TerminalsCounter - 1);
}

int NTerminalCode(char *word)
{
    for (int i = 0; i < NTerminalsCounter; i++)
    {
        if (!strcmp(word, NTerminals[i])) return (11 + i);
    }
    strcat(NTerminals[NTerminalsCounter], word);
    NTerminalsCounter++;
    if (NTerminalsCounter > 49) cout << "ERR l pc 291\n";
    return 11 + (NTerminalsCounter - 1);
}

void CodeComparer(int key, char *word)
{
    switch (key)
    {
        case 0: Output(SymbolCode(word)); break;
        case 1: Output(SemanticCode(word));  break;
        case 2: Output(TerminalCode(word)); break;
        case 3: Output(NTerminalCode(word)); break;
        case 4: Output(1000); break;
    }

}
void ExceptionHandler(char *word = 0)
{
    cout << "Предупреждение в тексте: строка " << line << ", " << column << " слово: " << word << " \n";

    switch (Err)
    {
        case 1: cout << "Возможно, это должен быть символ\n"; break;
        case 2: cout << "Возможно, это должна быть семантика. После символа '$' встречены символы не из латинского алфавита\n"; break;
        case 3: cout << "Возможно, это должен быть терминал\n"; break;
        case 4: cout << "Возможно, это должен быть нетерминал. Транслятор считал как нетерминал, однако, встречаются символы не из латинского алфавита\n"; break;
    }

    if (!OverlookMistakes)
    {
        GameOver = true;
        return;
    }
}
