#include<iostream>
#include<cstdio>
#include<cstring>
#include<stdlib.h>
#include <windows.h>

using namespace std;

const int Min_Count = 40; /// минимальный допустимый размер массива, в который могут добавляться новые задачи
const int inf = 9999; /// типа бесконечность(нельзя делать слишком большим, возникнет переполнение)
const int Max_Reserve_Size = 100; /// максимальный размер резервного массива
int Count = 0, q_add = 1; /// кол-во элементов в файле solve_problems и кол-во новых добавленных задач
int Reserve_Size = 0;
bool tip = true; /// отвечает за подсказки(если = true, то они будут выводиться)
bool tip_important = true; /// отвечает за важные подсказки(типа ввода концентрации)
bool tip_encumbering = true; /// это мешающие подсказки(типа Input numbers, при add)
int Current_position = 0; /// показывает позицию в файле Buffer в данный момент
int q_buff; /// показывает, до какого элемента нужно считывать, чтобы записать в буфер(в реальности, это слишком запутанная шткуа)
int Position; /// показывает, в каком столбце(в файле BP or BBP) находится нужный нам элемент
int Min_in_BP = -inf, Max_in_BP = inf; /// показывает на макс и мин элементы в файле
/// BP(изначально они достигают своего максимумальной амплитуды, чтобы не возникало
/// ошибок в удалении из BP

/// реальное кол-во элементов в массиве Solve_Problems = Count+q_add-1
int Reserve[Max_Reserve_Size+1]; /// резервный массив
bool flag_Recover_Back_Back_Up = false; /// если он равен true, то происходит восстановление файла Back_Back_Up
int count_Recover_Back_Back_Up; /// номер номера в файле BBUp, до которого пользователь хочет восстановить
/// если count_Recover_Back_Back_Up = 0, значит надо восстановить все

void print(int *F, int l, int r); /// выводит на экран участок массива
void generation(int *F, int start, int finish, int concentration); /// генерирует задачи из диапозона
void Add(int *F, int number); /// добавляет в набор решенных задач задачу под номером number(change q_add)
void Delete(int *F, int number); /// удаляет из набора решенных задач задачу под номером number(change q_add)
void Back_Up(int *F); /// записывает в файл бэкап изменения, которые произошли
void Back_Back_Up(int *T, bool flag); /// записывает в файл бэкапа от бэкапа основного
/// файла(сделано для ускорения программы). Если flag = true, значит мы добавляем данный объект, если false - delete
void Buffer_Save(int *Buf, int n, int flag); /// добавляет в буффер
void Add_in_BP(int *F, int n); /// добавляет в BP элементы из массива F

void return_min_max(int A[], int n, int &Min, int &Max){
    Max = Min = A[1];
    for (int i = 2; i < n+1; i++){
        if (A[i] < Min) Min = A[i];
        if (A[i] > Max) Max = A[i];
    }
}

void Counting_Sort(int A[], int n){
    int Min, Max;
    return_min_max(A, n, Min, Max);
    int B[n+1], C[Max-Min+1];

    for (int i = 0; i < Max-Min+1; i++)
        C[i] = 0;
    for (int i = 1; i < n+1; i++)
        C[A[i]-Min] += 1;

    for (int j = 1; j < Max-Min+1; j++)
        C[j] += C[j-1];

    for (int i = 1; i < n+1; i++){
       B[C[A[i]-Min]] = A[i];
     C[A[i]-Min]--;
    }

    for (int i = 1; i < n+1; i++)
        A[i] = B[i];
}

void Counting_Sort_with_Linking(int A[], int n, int Linking[]){
    int Min, Max;
    return_min_max(A, n, Min, Max);
    int B[n+1], C[Max-Min+1], B_link[n+1];

    for (int i = 0; i < Max-Min+1; i++)
        C[i] = 0;
    for (int i = 1; i < n+1; i++)
        C[A[i]-Min] += 1;

    for (int j = 1; j < Max-Min+1; j++)
        C[j] += C[j-1];

    for (int i = 1; i < n+1; i++){
       B[C[A[i]-Min]] = A[i];
    B_link[C[A[i]-Min]] = Linking[i];
     C[A[i]-Min]--;
    }

    for (int i = 1; i < n+1; i++){
        A[i] = B[i];
            Linking[i] = B_link[i];
    }
}


int bin_search(int *F, int l, int r, int key)
{
    int mid;
    while (r - l > 1)
    {
        mid = (r+l)/2;
        if (F[mid] > key) r = mid-1;
        else l = mid;
    }
    if (F[l] == key) return l;
    if (F[r] == key) return r;
    return -1;
}
/*
    функция возвращает, в случае, если key = true, то самый большой элемент среди
    меньших или равных этого числа, в противном случае, самый маленький
    среди больших или равных ему(точнее, индекс в массиве)
    них.
*/
int bin_search_in_range(int *F, int l, int r, int value, bool key)
{
    if (key)
    {
        int mid;
        while (r - l > 1)
        {
            mid = (r+l)/2;
            if (F[mid] < value) l = mid+1;
            else r = mid;
        }
       if (F[l] >= value) return l;
       if (F[r] >= value) return r;
       return -1;
    }
    else
    {
       int mid;
        while (r - l > 1)
        {
            mid = (r+l)/2;
            if (F[mid] > value) r = mid - 1;
            else l = mid;
        }
       if (F[r] <= value) return r;
       if (F[l] <= value) return l;
       return -1;

    }
}
 /*
    Выводит на экран содержимое массива Solve_Problems(в идеале произвольного, но
    будет использоваться только этот массив(скорее всего))
 */
void print(int *F, int l, int r)
{
    for (int i = l; i <= r; i++)
        cout << F[i] << " ";
    cout << "\n\n";
}
/*
    Выводит на экран содержимое файла Back_Up
*/
void Print()
{
    FILE *back_up = fopen(".tcs/Back_Up.txt", "r");
    int count; fscanf(back_up, "%d", &count);
    int data;
    if (count == 0) {cout << "No solve problems\n"; return;}
    for (int i = 0; i < count; i++)
    {
        fscanf(back_up, "%d", &data);
        cout << data << " ";
    }
    cout << "\n\n";
    fclose(back_up);
}

/*
    Обрабатывает запрос пользователя(ввод) для определения диапозона номеров,
    которые нужно вывести(т.е., те номера, которые >= start, и <= finish).
    Так же, эта функция отвечает за сам вывод этого на экран.
*/

void print_range(int *F, int n)
{
    int start, finish;
  if (tip)  cout << "If you want to break, enter -1\n";
    while (true)
    {
       if (tip_encumbering) cout << "Input start number: ";
        cin >> start; if (start == -1) break;
        if (tip_encumbering) cout << "Input finish number: ";
        cin >> finish; if (finish == -1) break;
        if (start > finish)
        {
            cout << "The start number is greater than finish number. Try again\n";
        }
        else
        {
            if (n == 0) {cout << "Range is empty\n\n"; break;}
            else
            {
            int a = bin_search_in_range(F, 1, n, start, true);
            int b = bin_search_in_range(F, 1, n, finish, false);
            if (a == -1 || b == -1) cout << "Range is empty\n\n";
            else print(F, a, b);
            break;
            }
        }
    }
}

/*
    Производит настройки подсказок.
*/

void tip_install()
{
    char s[20];
    if (tip) cout << "If you want to break, then enter 'exit'\n";

    while (true)
    {
        if (tip) cout << "\nTip install command: ";
        cin >> s;
        if (!strcmp(s, "exit")) break;
        else if (!strcmp(s, "print"))
        {
            cout << "\n";
            cout << "tip_encumbering is really bad tip!\n";
            cout << "tip is normal thing\n";
            cout << "tip_important is really important thing\n\n";
        }
        else if (!strcmp(s, "tip"))
        {
            while (true)
            {
            char command[10];
            if (tip) cout << "If you want to break, then enter 'exit'\n";
            if (tip_encumbering) cout << "Tip command(off or on): ";
            cin >> command;
            if (!strcmp(command, "exit")) break;
            else if (!strcmp(command, "off")) {tip = false; break;}
            else if (!strcmp(command, "on")) {tip = true; break;}
            }
            break;
        }
        else if (!strcmp(s, "tip_important"))
        {
             while (true)
            {
            char command[10];
            if (tip) cout << "If you want to break, then enter 'exit'\n";
            if (tip_encumbering) cout << "Tip_important command(off or on): ";
            cin >> command;
            if (!strcmp(command, "exit")) break;
            else if (!strcmp(command, "off")) {tip_important = false; break; }
            else if (!strcmp(command, "on")) {tip_important = true; break;}
            }
            break;
        }
        else if (!strcmp(s, "tip_encumbering"))
        {
             while (true)
            {
            char command[10];
            if (tip) cout << "If you want to break, then enter 'exit'\n";
            if (tip_encumbering) cout << "Tip_encumbering command(off or on): ";
            cin >> command;
            if (!strcmp(command, "exit")) break;
            else if (!strcmp(command, "off")) {tip_encumbering = false; break;}
            else if (!strcmp(command, "on")) {tip_encumbering = true; break; }
            }
            break;
        }
        else if (!strcmp(s, "all"))
        {
             while (true)
            {
            char command[10];
            if (tip) cout << "If you want to break, then enter 'exit'\n";
            if (tip_encumbering) cout << "All tip command(off or on): ";
            cin >> command;
            if (!strcmp(command, "exit")) break;
            else if (!strcmp(command, "off")) {
                tip_encumbering = false; tip_important = false; tip = false; break;}
            else if (!strcmp(command, "on")) {
                tip_encumbering = true; tip_important = true; tip = true; break;}
            }
            break;
        }
    }
}
/*
    Выводит на экран сообщение. Если key = true, то on. Else, off.
    Нужно для того, чтобы код был меньшего объема, ибо это применяется в tip_print.
*/
void logic_show(bool key)
{
    if (key) cout << "on";
    else cout << "off";
}

void tip_print()
{
    cout << "\n\n";
    cout << "tip_encumbering: "; logic_show(tip_encumbering);
    cout << "\ntip: "; logic_show(tip);
    cout << "\ntip_important: "; logic_show(tip_important);
    cout << "\n\n";
}

void Add(int *F, int number)
{
    if (Count+q_add == 1 || bin_search(F, 1, Count+q_add-1, number) == -1){
    F[Count+q_add] = number;
        q_add++;
        Counting_Sort(F, Count+q_add-1);
    }
}

/*
    Производит добавление элементов из диапозона(т.е., например, от 1 до 50)
*/

void add_range(int *F)
{
    int n = Count + q_add - 1; /// Размер массива Solve_Problems[1..n]
    if (tip) cout << "If you want to break, enter '-1'\n\n";
    while (true)
 {
   int start, finish;
        if (tip_encumbering) cout << "Input start number: ";
        cin >> start;
        if (start == -1) break;
        if (tip_encumbering) cout << "Input finish number: ";
        cin >> finish; if (finish == -1) break;
        if (start < 0 || start > finish) cout << "Input not correct. Try again.\n\n";
        else
        {
            int count = finish-start+1;
            int buff[count+1], w = 0; /// нужно для того, чтобы добавить в буффер
            for ( ; start <= finish; start++)
            {
                if (bin_search(F, 1, Count+q_add-1, start) == -1)
                {
                    buff[++w] = start;
                }
                Add(F, start);
            }
            Buffer_Save(buff, w, 1); /// добавляем в буффер добавленные элементы
            return;
        }
 }
}

void Delete(int *F, int number)
{
    if (Count + q_add > 1) /// если есть хоть что-то в массиве
    {
        int q = bin_search(F, 1, Count+q_add, number);
        if (q != -1)
        {
            F[q] = inf;
            q_add--;
            Counting_Sort(F, Count+q_add);
        }
    }
}

/*
    Производит удаление всех номеров с данного диапозона(т.е., например, с 1 по 50 номера)
*/

void delete_range(int *F)
{
    int n = Count + q_add - 1; /// Размер массива Solve_Problems[1..n]
    if (tip) cout << "If you want to break, enter '-1'\n\n";
 while (true)
 {
   int start, finish;
        if (tip_encumbering) cout << "Input start number: ";
        cin >> start;
        if (start == -1) break;
        if (tip_encumbering) cout << "Input finish number: ";
        cin >> finish; if (finish == -1) break;
        if (start < F[1] || finish > F[n] || start > finish)
        {
            while (true)
            {
                cout << "You sure, that range is correct?";
                if (tip) cout << "(yes or no)"; cout << ": ";
                char command[10];
                cin >> command;
                if (!strcmp(command, "yes"))
                {
                    int buf_count = 0, start1 = start; /// сохраняем start, ибо он
                    /// будет потерян в цикле ниже
                    for ( ; start <= finish; start++)
                        {
                            if (start <= F[n] && start >= F[1])
                            {
                            if (bin_search(F, 1, n, start) != -1)
                                buf_count++;
                            }
                            else if (start > F[n]) break;
                        }
/*
Проходим еще раз этот промежуток, по следующим причинам:
    Дело в том, что пользователь может ввести слишком большой промежуток, например
    от -2 до 100000000. Программа фиксирует момент, когда мы дошли до самого
    большого числа из массива, и выходит из цикла, чтобы бессмыссленно не
    просматривать остальные числа, которые, очевидно, т.к. больше самого большого
    числа из массива, не находятся в массиве. Так вот, мы бы проходили этот
    промежуток 1 раз, если бы заранее создали бы нужного размера массив, для записи
    его в буффер. Однако, пользователь может ввести любого размера промежуток, так
    что, для безопасности от аварийных ситуаций и использования как можно меньшего
    объема памяти, мы пройдемся еще раз по этому промежутку, но уже зная, сколько
    элементов нужно удалить(мы это подсчитывали, когда проходились 1 раз по
    промежутку).
*/
    int buff[buf_count+1], w = 0;
    for ( ; start1 <= finish; start1++)
    {
        if (start1 <= F[n] && start1 >= F[1])
        {
            if (bin_search(F, 1, n, start1) != -1)
                buff[++w] = start1;
            Delete(F, start1);
        }
        else if (start1 > F[n]) break;
    }
    Buffer_Save(buff, w, -1); /// добавляем в буффер
                        return; /// выходим нафиг из функции
                }
                else if (!strcmp(command, "no")) break;
            }
        }
        else
        {
            int buf_count = finish-start+1;
            int buff[buf_count+1], w = 0;
            for ( ; start <= finish; start++)
                        {
                            if (bin_search(F, 1, Count+q_add-1, start) != -1)
                                buff[++w] = start;
                             Delete(F, start);
                        }

            Buffer_Save(buff, w, -1); /// Добавляем в буффер
                    return;
        }

 }
}

 /* в файле Back_Back_Up на первом месте идет flag - показывает, что мы делаем с
данными номерами. Если flag = 1 - добавление, flag = 0 - удаление, flag = -1 - пусто
 */
void Back_Back_Up(int *T, int count, bool flag)
{
    FILE *back_back_up = fopen(".tcs/Back_Back_Up.txt", "w");
    if (count == -1) {fprintf(back_back_up, "%d%s", -1, "\n"); fclose(back_back_up);return;}
    fprintf(back_back_up, "%d%s", flag, "\n");
    fprintf(back_back_up, "%d%s", count, "\n");
    for (int i = 1; i <= count; i++)
        fprintf(back_back_up, "%d%s", T[i], " ");
    fclose(back_back_up);
}

void Back_Up(int *F)
{
     FILE *back_up = fopen(".tcs/Back_Up.txt", "w");
     fprintf(back_up, "%d%s", Count+q_add-1, "\n");
     for (int i = 1; i < Count + q_add; i++)
        fprintf(back_up, "%d%s", F[i], " ");
        fclose(back_up);
}

/*
    Копирует файл Buffer, в файл Buf_Buffer
*/

void Buffer_Copy()
{
    FILE *buffer = fopen(".tcs/Buffer.txt", "r");
    FILE *buf_buffer = fopen(".tcs/Buf_Buffer.txt", "w");


    int data, Count, count;
    fscanf(buffer, "%d",&Count);///считываем Count
    fprintf(buf_buffer, "%d%s%", Current_position, "\n");

    for (int i = 0; i < Current_position; i++)
    {
        fscanf(buffer, "%d", &count);
        fprintf(buf_buffer, "%d%s", count, " ");
        for (int j = 0; j < count + 1; j++) /// считываем до count + 1, ибо операция тоже учитывается
        {
            fscanf(buffer, "%d", &data);
            fprintf(buf_buffer, "%d%s", data, " ");
        }
        fprintf(buf_buffer, "%s", "\n");
    }
    fclose(buffer);
    fclose(buf_buffer);

}

/*
    Записывает в массив Buffer дополнительные данные для буфера
*/

void Buffer_Save(int *Buf, int n, int flag) /// flag = 1 - добавление, -1 - удаление
{
    Buffer_Copy(); /// делаем копию файла Buffer.txt
    /// Делаем обратную копию
    FILE *buffer = fopen(".tcs/Buffer.txt", "w");
    FILE *buf_buffer = fopen(".tcs/Buf_Buffer.txt", "r");
    int data, Count, count;
    fscanf(buf_buffer, "%d", &Count);
    fprintf(buffer, "%d%s", Count+1, "\n");
    Current_position++;
    for (int i = 0; i < Count; i++)
    {
        fscanf(buf_buffer, "%d", &count);
        fprintf(buffer, "%d%s", count, " ");
        for (int j = 0; j < count + 1; j++)
        {
            fscanf(buf_buffer, "%d", &data);
            fprintf(buffer, "%d%s", data, " ");
        }
        fprintf(buffer, "%s", "\n");
    }
    /// После почти полной копии(кроме первых чисел, увеличенных на 1)
    /// можно приступать к добавлению новой строки
    fprintf(buffer, "%d%s%d%s", n, " ", flag, " ");
    for (int i = 1; i <= n; i++)
        fprintf(buffer, "%d%s", Buf[i], " ");
    fclose(buffer);
    fclose(buf_buffer);
}

/*
    Производит АККУРАТНОЕ добавление новой информации в буффер.
    Имеется ввиду, что нельзя просто сохранять в массив список задач, которые
    пользователь хочет добавить\удалить, ибо среди них могут быть задачи, которые
    уже являются решенными, и если мы их добавим в буффер, то при вызове
    пользователем буффера для восстановления данных, некоторые данные могут
    неправильно добавиться обратно, или, даже удалиться! Из-за этого, при добавлении
    в буфер, мы должны проверить, есть ли этот элемент в массиве или его нету.
    Если пользователь хочет добавить элемент, то мы должны проверить, что его нету
    в списке решенных задач. А если хочет удалить, то проверяем, что такой элемент
    есть в списке.
    flag = 1 - значит добавление, -1 - удаление
*/

void Add_Buffer(int *Solve_Problems, int *F, int n, int flag)
{
    if (flag == 1)
    {
        int buff[n+1], q = 0;
        for (int i = 1; i <= n; i++)
            if (bin_search(Solve_Problems, 1, Count+q_add-1, F[i]) == -1)
            {
                buff[++q] = F[i];
            }
        if (q > 0) Buffer_Save(buff, q, 1);
    }
    else if (flag == -1)
    {
        int buff[n+1], q = 0;
        for (int i = 1; i <= n; i++)
            if (bin_search(Solve_Problems, 1, Count+q_add-1, F[i]) != -1)
                buff[++q] = F[i];
        if (q > 0) Buffer_Save(buff, q, -1);
    }
}

/*
    Производит запись из файла Buffer в массив Solve_Problems
    Т.е., фактически, отвечает за восстановление из данных из буффера.
    if back = 1, то считываем назад из буфера(то, что было)
        else, if back = 0, то считываем вперед
*/

void Buffer_overwrite(int *solve_problems, int back)
{
    int data, Count, count, operation;
    FILE *buffer = fopen(".tcs/Buffer.txt", "r");
    fscanf(buffer, "%d", &Count);
    back--;
    Current_position -= back;
    if (Current_position > Count) Current_position--;
    /// если можно считать буффер, то считываем
    if (Current_position >= 1 && Current_position <= Count)
    {
        for (int j = 0; j < Current_position - 1; j++) /// доходим до нужной нам
           {                                        /// считываемой строки
            fscanf(buffer, "%d%d", &count, &operation);
            for (int k = 0; k < count; k++) fscanf(buffer, "%d", &data);
           }
           fscanf(buffer, "%d%d", &count, &operation);
           if (!back) operation *= -1; /// меняем операцию на противоположную
           /// если идем назад. Если вперед, то не меняем, а оставляем как есть
        if (operation == 1) /// добавление
        {
            for (int i = 0; i < count; i++)
            {
                fscanf(buffer, "%d", &data);
                Add(solve_problems, data);
            }
        }
        else if (operation == -1) /// удаление
        {
            for (int i = 0; i < count; i++)
            {
                fscanf(buffer, "%d", &data);
                Delete(solve_problems, data);
            }
        }
        if (!back) Current_position --;
    }
    fclose(buffer);
}

/*
    Функция, выводящая на экран помощь в командах BP.
*/

void help_BP()
{
    cout << "\n";
    cout << "Command 'add' adds items to a file BP\n";
    cout << "Construction: add count number1 ... number_count[add 2 67 94]\n\n";
    cout << "Command 'delete' deletes items to a file BP\n";
    cout << "Construction: delete count number1 ... number_count [delete 2 56 87]\n\n";
    cout << "Command 'add:' adds a task list\n";
    cout << "Construction: add start finish [add: 34 57]\n\n";
    cout << "Command 'delete' deletes a task list\n";
    cout << "Construction: delete start finish [delete: 1 20]\n\n";
    cout << "Command 'copy' copies the BP file to the BBP file\n";
    cout << "Construction: copy\n\n";
    cout << "Command 'search' searches for an item in the BP file\n";
    cout << "Construction: search number [search 127]\n\n";
    cout << "Command 'print' displays the BP file\n";
    cout << "Construction: print\n\n";
    cout << "Command 'print:' displays the range of tasks from the BP file\n";
    cout << "Construction: print: start finish [print: 25 68]\n\n";
}

/*
    Ищет в одном из файлов (BP or BBP)
    Return -1, если не нашел. В противном случае, возвращает номер строки в файле
    Одновременно с этим, глобальная переменная Position
    (в случае, если нашелся элемент)
    возвращает позицию в строке(т.е. номер столбца), где лежит нужный элемент.
*/
int search_in_BP(int key, bool flag)
{
    FILE *f;
    if (flag)
    {
        f = fopen(".tcs/Base_Problems.txt", "r");
    }
    else
    {
        f = fopen(".tcs/Base_Base_Problems.txt", "r");
    }
    int Count, count;
    fscanf(f, "%d", &Count);
    for (int i = 0; i < Count; i++)
    {
        fscanf(f, "%d", &count);
        int H[count+1];
        for (int j = 0; j < count; j++)
        {
            fscanf(f, "%d", &H[j+1]);
        }
        int ind = bin_search(H, 1, count, key);
        if (ind != -1) /// нашли элемент
        {
            Position = ind;
            return (i+1);
        }
    }
    return -1;
}

/*
    if flag == true, то Производит копирование файла BP в файл BBP.
        Else, наоборот, копию BBP в BP.
*/

void Copy_BP(bool flag)
{
    FILE *bp;
     FILE *bbp;
     if (flag)
     {
         bp = fopen(".tcs/Base_Problems.txt", "r");
         bbp = fopen(".tcs/Base_Base_Problems.txt", "w");
     }
     else
     {
         bp = fopen(".tcs/Base_Base_Problems.txt", "r");
         bbp = fopen(".tcs/Base_Problems.txt", "w");
     }
    int data, Count, count;
    fscanf(bp, "%d", &Count);
    fprintf(bbp, "%d%s", Count, "\n");
    for (int i = 0; i < Count; i++)
    {
        fscanf(bp, "%d", &count);
        fprintf(bbp, "%d%s", count, " ");
        for (int j = 0; j < count; j++)
        {
            fscanf(bp, "%d", &data);
            fprintf(bbp, "%d%s", data, " ");
        }
        fprintf(bbp, "%s", "\n");
    }
    fclose(bp); fclose(bbp);
}

/*
    Производит обновленную копию файла BBP в файл BP с учетом корректировки
    кол-ва Count(увеличиваем на 1) и добавлением в конце файла нового массива
*/

void Copy_new_BP(int *H, int q)
{
    FILE *bp = fopen(".tcs/Base_Problems.txt", "w");
    FILE *bbp = fopen(".tcs/Base_Base_Problems.txt", "r");
    int data, Count, count;
    fscanf(bbp, "%d", &Count);
    fprintf(bp, "%d%s", Count+1, "\n");
    for (int i = 0; i < Count; i++)
    {
        fscanf(bbp, "%d", &count);
        fprintf(bp, "%d%s", count, " ");
        for (int j = 0; j < count; j++)
        {
            fscanf(bbp, "%d", &data);
            fprintf(bp, "%d%s", data, " ");
        }
        fprintf(bp, "%s", "\n");
    }
    fprintf(bp, "%d%s", q, " ");
    for (int i = 0; i < q; i++)
        fprintf(bp, "%d%s", H[i], " ");
    fclose(bp); fclose(bbp);
}

/*
    Функция производит добавление(сохранение) номеров, при этом предъявляется массив
    F, известен его размер(он начинается с 1), вводится старт и финиш.
    Производим добавление от старт до финиша только тех номеров, которые лежат в F.
    Номера start and finish предполагаются введенными корректно!
*/

void Save_in_BP(int *F, int n, int start, int finish)
{
    int st = bin_search(F, 1, n, start);
    int fn = bin_search(F, 1, n, finish);
    if (st <= fn)
    {
        if (st != -1 && fn != -1)
        {
            int H[fn-st+1], q = 0;
            for (int i = st; i <= fn; i++)
                H[q++] = F[i];
            Add_in_BP(H, q);
        }
    }
}

/*
    Производит добавление в файл BP списка задач, которые пользователь решил
    сохранить(чтобы решить потом). Нужно прописать систему определения алгоритма
    подсчета(если памяти много, то пусть выделяет память, а не проходит 2 раза
    цикл, ради сохранения памяти. Если же, наоборот, очень опасная ситуация с
    памятью, то пусть жертвуется скорость работы, но не выделяется много памяти,
    нежели необходимо(ибо подвисание системы на маленькое время лучше аварийного
    выхода из программы. Так же, переполнение опасно для всей системы, ибо могут
    быть стерты важные файлы для ОС).
*/

void Add_in_BP(int *F, int n) /// Массив F начинается с 0 и до n-1 !!!
{
    Copy_BP(true); /// копируем файл BP в BBP
    int q = 0; /// счетчик
    /*
        Циклом просматриваем, сколько задач из массива F нужно добавить в BP(ибо
        некоторые из задач там уже лежат)
    */
    for (int i = 0; i < n; i++)
        if (search_in_BP(F[i], false) == -1) q++;
    int H[q], w = 0;
    /*
        Проблема в том, что мы во 2 раз проходимся по массиву, при этом производя
        поиск в огромном файле BP, вместо того, чтобы чуть больше выделить памяти.
        Это невыгодно по времени, поэтому, позже, лучше проработать систему опре-
        деления нагрузки на память и в случае, если память есть, то выделить
        ее, а не проходиться по многу раз по огромным файлам!
    */
    for (int i = 0; i < n; i++)
        if (search_in_BP(F[i], false) == -1) H[w++] = F[i];///Это не выгодно по времени
    if (q) /// если есть что добавить нового в файл BP
    {
        Copy_new_BP(H, q);
    }
}

/*
    Достаточно долгое(по времени) удаление. Идея заключается в том, что
    мы, вначале, копируем файл BP в BBP, после чего начинаем идти по BBP.
    Каждую строчку BBP сраниваем на наличие в массиве F. Если есть элементы, то
    не добавляем их в новый файл BP.
*/

void Delete_in_BP(int *F, int n) /// массив F начинается с 1 и до n !!!
{
    Copy_BP(true);
    FILE *bp = fopen(".tcs/Base_Problems.txt", "w");
    FILE *bbp = fopen(".tcs/Base_Base_Problems.txt", "r");
    int data, Count, count, new_Count = 0; /// new_Count - новый счетчик для
    /// файла BP, чтобы после записи тех задач, которые не подлежат удалению, мы
    /// записали наш файл в нормальном виде(необходимом для корректной работы
    /// и корректного способа хранения информации)
    fscanf(bbp, "%d", &Count);
    for (int i = 0; i < Count; i++)
    {
        fscanf(bbp, "%d", &count);
        int H[count], q = 0; /// массив, в который будем добавлять те задачи, которых
        /// нету в массиве F(для записи в файл BP). q - счетчик
        for (int j = 0; j < count; j++)
        {
            fscanf(bbp, "%d", &data);
            if (bin_search(F, 1, n, data) == -1) H[q++] = data;
        }
        if (q) /// если есть что добавить в файл BP, то...
        {
            fprintf(bp, "%d%s", q, " ");
            for (int k = 0; k < q; k++)
                fprintf(bp, "%d%s", H[k], " ");
            fprintf(bp, "%s", "\n");
            new_Count++;
        }
    }
    fclose(bp); fclose(bbp);
    /// Теперь перезапишем наш файл в нормальном виде(вначало нужно добавить
     /// new_Count, все остальное оставить как есть(после изменений).
     bbp = fopen(".tcs/Base_Base_Problems.txt", "w");
     bp = fopen(".tcs/Base_Problems.txt", "r");
     fprintf(bbp, "%d%s", new_Count, "\n");
    for (int i = 0; i < new_Count; i++)
    {
        fscanf(bp, "%d", &count);
        fprintf(bbp, "%d%s", count, " ");
        for (int j = 0; j < count; j++)
        {
            fscanf(bp, "%d", &data);
            fprintf(bbp, "%d%s", data, " ");
        }
        fprintf(bbp, "%s", "\n");
    }
    fclose(bp); fclose(bbp);
    Copy_BP(false);

}

/*
    Проходится по файлу BP и находит максимальный и минимальный элементы, и запи-
    сывает их(т.е. эти значения максимума и минимума) в переменные Min_in_BP and
    Max_in_BP. Это нужно для Delete_range_in_BP.
*/

void Extremum_in_BP()
{
    FILE *f = fopen(".tcs/Base_Problems.txt", "r");
    int Min = inf, Max = -inf;
    int Count, count, data;
    fscanf(f, "%d", &Count);
    for (int i = 0; i < Count; i++)
    {
        fscanf(f, "%d", &count);
        for (int j = 0; j < count; j++)
        {
            fscanf(f, "%d", &data);
            if (data > Max) Max = data;
            if (data < Min) Min = data;
        }
    }
    if (Count) /// если в BP есть хоть что-то, то только тогда меняем
    {
    Min_in_BP = Min;
    Max_in_BP = Max;
    }
}

/*
    Производит удаление из файла BP последовательно, определенного пользователем,
    списка номеров. Будет реализовываться следующим образом:
    Т.к. пользователь может ввести слишком огромный диапозон, то мы будем делать
    все аккуратно: будем смотреть на те номера, которые ввел пользователь, и если
    номера выходят за пределы максимально и минимально возможного в файле BP, то
    мы игнорим остаток, это даст нам преимущество в скорости работы и оградит от
    возможного зависания системы.

*/
void Delete_range_in_BP()
{
    Extremum_in_BP(); /// обновили мин и макс в ВР
    int start, finish;
    while (true)
    {
       if (tip_encumbering) cout << "Input start number: ";
       cin >> start;
       if (tip_encumbering) cout << "Input finish number: ";
       cin >> finish;
       if (start < Min_in_BP || finish > Max_in_BP)
       {
           cout << "Are you sure, that is's correct? ";
           char s[15];
           cin >> s;
           if (!strcmp(s, "yes")) break;
       }
       else break;
    }
    if (start < Min_in_BP) start = Min_in_BP;
    if (finish > Max_in_BP) finish = Max_in_BP;
    int H[finish-start+2], q = 0;
    for ( ; start <= finish; start++)
        H[++q] = start;
    Delete_in_BP(H, q);
}

/*
    Еще одна такая же функция, но тут уже заранее известны значения start and finish
    Они не проверяются на корректность, ибо вызов производится внутри программы, а
    не пользователем, так что ошибок здесь быть не может(поэтому нет проверки
    корректности ввода start and finish).
*/

void Delete_range_in_BP(int start, int finish)
{
    if (start < Min_in_BP) start = Min_in_BP;
    if (finish > Max_in_BP) finish = Max_in_BP;
    int H[finish-start+2], q = 0;
    for ( ; start <= finish; start++)
        H[++q] = start;
    Delete_in_BP(H, q);
}

/*
    Производит удаление всех задач в файле BP.
*/

void delete_all_in_BP()
{
    Extremum_in_BP();
    Delete_range_in_BP(Min_in_BP, Max_in_BP);
}

void add_range_in_BP()
{
    Extremum_in_BP(); /// обновили мин и макс в ВР
    int start, finish;
    while (true)
    {
       if (tip_encumbering) cout << "Input start number: ";
       cin >> start;
       if (tip_encumbering) cout << "Input finish number: ";
       cin >> finish;
       if (start < 0) cout << "Not correct. Start number < 0. Try again\n";
       else if (finish > inf) cout << "Not correct. Finish number > inf. Try again\n";
       else break;
    }
  int H[finish-start+1], q = 0;
  for ( ; start <= finish; start++)
    H[q++] = start;
    Add_in_BP(H, q);
}

/*
    Производит вывод задач, которые были последними добавлены в BP.
*/

void print_last_BP()
{
    FILE *f = fopen(".tcs/Base_Problems.txt", "r");
    int Count, count, data;
    fscanf(f, "%d", &Count);
    if (!Count) cout << "In the BP file there are no tasks\n\n";
    else
    {
         for (int i = 0; i < Count-1; i++)
         {
             fscanf(f, "%d", &count);
             for (int j = 0; j < count; j++)
             {
                 fscanf(f, "%d", &data);
             }
         }
         fscanf(f, "%d", &count);
         for (int j = 0; j < count; j++)
         {
             fscanf(f, "%d", &data);
             cout << data << " ";
         }
         cout << "\n\n";
    }
}

/*
    Выводит на экран все содержимое файла BP
*/

void print_BP()
{
    FILE *f = fopen(".tcs/Base_Problems.txt", "r");
    int Count, count, data;
    fscanf(f, "%d", &Count);
    if (!Count) cout << "BP file is empty\n";
    else
    {
    for (int i = 0; i < Count; i++)
    {
        fscanf(f, "%d", &count);
        for (int j = 0; j < count; j++)
        {
            fscanf(f, "%d", &data);
            cout << data << " ";
        }
    }
    }
    cout << "\n";
}

/*
    Выводит на экран диапозон задач из BP. Преимущество отдано памяти, т.е. она
    работает так: проходимся по диапозону(при это произведя корректировку, т.е.
    если пользователь ввел слишком большой номер, то мы корректируем на основе того,
    что мы знаем мин и макс в файле  BP (start >= Min_in_BP - всегда, тоже самое и
    с максимумом). Мы проходимся по диапозону и ищем каждый элемент. Как видно, это
    долго, но зато, мы не выделяем много памяти.
*/

void print_range_BP()
{
    Extremum_in_BP();
    int start, finish;
    if (tip_encumbering) cout << "Input start number: ";
    cin >> start;
    if (tip_encumbering) cout << "Input finish number: ";
    cin >> finish;
    if (start < Min_in_BP) start = Min_in_BP;
    if (finish > Max_in_BP) finish = Max_in_BP;
    bool flag = false; /// нужен, чтобы вывести, есть ли в диапозоне хоть 1 элемент
    for ( ; start <= finish; start++)
        if (search_in_BP(start, true) != -1) {cout << start << " "; flag = true; }
    if (!flag) cout << "This range in BP file is empty";
    cout << "\n";
}

void generation(int *F, int start, int finish, int concentration)
{
    int Acces_Problems[finish+2-start]; /// массив доступных задач
    int q = 0;
    for (int i = start; i <= finish; i++)
        if (bin_search(F, 1, Count+q_add, i) == -1) Acces_Problems[q++] = i;
        int count = q*concentration/100;
    for (int i = 1; i <= count; i++)
    {
        int a = rand()%q;
        Reserve[++Reserve_Size] = Acces_Problems[a];
    }
    Counting_Sort(Reserve, count);
    print(Reserve, 1, count);
}

bool Union(int *F, int number)
{
    int q = -1;
    for (int i = 1; i <= Reserve_Size; i++)
        if (Reserve[i] == number) {q = i; break;}

    if (q == -1) return false; /// пользователь ввел неправильный номер, которого нет
    //if (Count+q_add == 0) Count = 1;
     q_buff = q; /// до сюда нужно считывать из массива Reserve, чтобы записать
     /// в буффер(сделано для того, чтобы код не был непонятным, а все структуры
    /// шли в логическом порядке
     for (int i = 1; i <= q; i++)
    {
        if (bin_search(F, 1, Count+q_add, Reserve[i]) == -1)
        {
            F[Count+q_add] = Reserve[i]; q_add++;
            Counting_Sort(F, Count+q_add-1);
        }
    }
    return true;
}
/*
    производит сохранение данных в файле solve_problems
*/
void save(int *F)
{
    FILE *solve_problems = fopen(".tcs/solve_problems.txt", "w");
    fprintf(solve_problems, "%d%s", Count+q_add-1, "\n");
    for (int i = 1; i < Count+q_add; i++)
        fprintf(solve_problems, "%d%s", F[i], " ");
    fclose(solve_problems);
}

/*
    Восстанавливает данные из файла Back_Back_Up. Причем, если count = -1, то
    восстанавливает все. Если нет, т.е. какому то положительному числу, то
    восстанавливается все, до этой задачи включительно.
    Возвращает true, если такой номер найден и все прошло успешно, и false, если такого номера нет,
    ибо пользователь мог случайно ошибиться, поэтому ему будет дана еще одна попытка
*/

void Recover_Back_Back_Up(int *solve_problems)
{
    FILE *back_back_up = fopen(".tcs/Back_Back_Up.txt", "r");
    int key; int data; int count;
    fscanf(back_back_up, "%d", &key);
    fscanf(back_back_up, "%d", &count);
    if (count_Recover_Back_Back_Up == 0) /// восстанавливаем всё
    {
        if (key == 1)
        {
            for (int i = 0; i < count; i++)
            {
                fscanf(back_back_up, "%d", &data);
                Add(solve_problems, data);
            }
        }
        else if (key == 0)
        {
            for (int i = 0; i < count; i++)
            {
                fscanf(back_back_up, "%d", &data);
                Delete(solve_problems, data);
            }
        }
    }
    else
    {
        if (key == 1)
        {
            while (true)
            {
                fscanf(back_back_up, "%d", &data);
                Add(solve_problems, data);
                if (data == count_Recover_Back_Back_Up) break;
            }
        }
        else if (key == 0)
        {
            while (true)
            {
                fscanf(back_back_up, "%d", &data);
                Delete(solve_problems, data);
                if (data == count_Recover_Back_Back_Up) break;
            }
        }
    }
    save(solve_problems);
    fclose(back_back_up);
}

bool Recover_Back_Back_Up(int count1)
{

    if (count1 == 0) {flag_Recover_Back_Back_Up = true; count_Recover_Back_Back_Up = 0; return true;}
    if (count1 == -1) {flag_Recover_Back_Back_Up = false; return true;} /// это значит, что пользователь не хочет восстанавливать
    FILE *back_back_up = fopen(".tcs/Back_Back_Up.txt", "r");
    int key; int count; fscanf(back_back_up, "%d", &key); int data;
    fscanf(back_back_up, "%d", &count);
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        fscanf(back_back_up, "%d", &data);
        if (data == count1) {found = true; break;}
    }
    fclose(back_back_up);
    if (found) {count_Recover_Back_Back_Up = count1;
        flag_Recover_Back_Back_Up = true;return true;}
    flag_Recover_Back_Back_Up = false;
    return false;
}

/*
    Восстанавливает данные из файла Back_Up
*/
void Recover_Back_Up()
{
    FILE *back_up = fopen(".tcs/Back_Up.txt", "r");
    FILE *solve_problems = fopen(".tcs/solve_problems.txt", "w");
    int count; fscanf(back_up, "%d", &count);
    fprintf(solve_problems, "%d%s", count, "\n");
    int data;

    for (int i = 0; i < count; i++)
    {
        fscanf(back_up, "%d", &data);
        fprintf(solve_problems, "%d%s", data, " ");
    }
    fclose(back_up); fclose(solve_problems);
}

void Recover()
{
      FILE *back_up = fopen(".tcs/Back_Up.txt", "r");
    int key;
     fscanf(back_up, "%d", &key);
     if (key != -1)
     {
         while (true)
        {
         cout << "Do you want to restore data since the last run?: ";
         char command[20];
         cin >> command;
         if (!strcmp(command, "yes"))
         {
            Recover_Back_Up();
            break;
         }
         else if (!strcmp(command, "no")) break;
         else if (!strcmp(command, "print")) Print();
        }
     }
     cout << "\n";
     FILE *back_back_up = fopen(".tcs/Back_Back_Up.txt", "r");
     fscanf(back_back_up, "%d", &key);
     if (key != -1)
     {
         int count; fscanf(back_back_up, "%d", &count);
         int Back_Back_Up_Turn_Array[count+1];
         cout << "At the last start, you want";
         if (key == 1) cout << " add"; else if (key == 0) cout << " delete";
        cout <<  " these problems: \n";
         for (int i = 1; i <= count; i++)
         {
            fscanf(back_back_up, "%d", &Back_Back_Up_Turn_Array[i]);
            cout << Back_Back_Up_Turn_Array[i] << " ";
         }
         cout << "\nDo you want recover?: ";
         char command[20];
         cin >> command;
         while (true)
         {
             if (!strcmp(command, "yes"))
             {
                 cout << "How much?: ";
                 char command1[20];
                 cin >> command1;
                 if (!strcmp(command1, "all")) {Recover_Back_Back_Up(0);break;}
                 else if (!strcmp(command1, "stop"))
                    {
                        while (true)
                        {
                            int a;
                            cin >> a;
                            if (Recover_Back_Back_Up(a)) break;
                            cout << "This number not found. Try again. Input number: ";
                        }
                        break;
                    }
                    else if (!strcmp(command1, "exit")) break;
             }
             else if (!strcmp(command, "no")) break;
         }
     }
    cout << "\n";
}

void buffer_print()
{
    cout << "\n";
    FILE *buf = fopen(".tcs/Buffer.txt", "r");
    int data, Count, count, operation;
    fscanf(buf, "%d", &Count);
    if (!Count) cout << "Buffer is empty\n\n";
    else
    {
        for (int i = 0; i < Count; i++)
        {
            fscanf(buf, "%d%d", &count, &operation);
            if (operation == 1) cout << "add: ";
            else if (operation == -1) cout << "delete: ";
            for (int j = 0; j < count; j++)
            {
                fscanf(buf, "%d", &data);
                cout << data << " ";
            }
            cout << "\n";
        }
        cout << "\n\n";
    }
    fclose(buf);
}

/*
    Производит первоначальную проверку(с момента запуска программы) на то, очищен
    ли буффер или нет. Если нет, то выводится на консоль сообщение пользователю,
    чтобы он выбрал, хочет ли он, чтобы буффер сохранился, или же очистить его.
*/

void Recover_Buffer(int *Solve_Problems)
{
    FILE *buf = fopen(".tcs/Buffer.txt", "r");
    int data;
    fscanf(buf, "%d", &data);
    fclose(buf);
    if (data)
    {

    cout << "After the previous start, the buffer remained unclean\n";
    cout << "Do you want to clean it?\n";
    char Command[10];
    while (true)
    {
       if (tip_important) cout << "Buffer command: ";
       cin >> Command;
       if (!strcmp(Command, "no")) {Current_position = data; break;}
       else if (!strcmp(Command, "yes"))
       {
            buf = fopen(".tcs/Buffer.txt", "w");
            fprintf(buf, "%d", 0);
            fclose(buf);
            break;
       }
       else if (!strcmp(Command, "print")) buffer_print();
       else if (!strcmp(Command, "problems") || !strcmp(Command, "print_problems"))
       {
           if (Count+q_add-1 == 0) cout << "You have not solve problems\n";
        else cout << "You have " << Count+q_add-1 << " solve problems:\n Numbers: ";
        print(Solve_Problems, 1, Count+q_add-1);
       }
    }
    cout << "\n";
    }
}

/*
    Проверяет, существует ли в данной директории файл с именем s.
    Если да, то возвращает true, else return false.
*/

bool exists(char *s)
{
    FILE *f = fopen(s, "r");
    if (f == NULL) return false;
    return true;
}

/*
    Производит инициализацию файлов. Создает необходимые файлы(Base_Problems, ...),
    чтобы не париться насчет создания файлов, а просто запустить программу, а она
    создаст все необходимые файлы в данной директории.
*/

void initialize()
{
    if (!exists(".tcs/solve_problems.txt"))
    {
        system("md .tcs");
        system("attrib +s +h .tcs");
        FILE *f = fopen(".tcs/solve_problems.txt", "w");
        fprintf(f, "%d", 0);
        fclose(f);
    }
    if (!exists(".tcs/Back_Up.txt"))
    {
        FILE *f = fopen(".tcs/Back_Up.txt", "w");
        FILE *g = fopen(".tcs/Back_Back_Up.txt", "w");
        fprintf(f, "%d", -1);
        fprintf(g, "%d", -1);
        fclose(f); fclose(g);
    }
    if (!exists(".tcs/Buffer.txt"))
    {
        FILE *f = fopen(".tcs/Buffer.txt", "w");
        FILE *g = fopen(".tcs/Buf_Buffer.txt", "w");
        fprintf(f, "%d", 0);
        fprintf(g, "%d", 0);
        fclose(f); fclose(g);
    }
    if (!exists(".tcs/Base_Problems.txt"))
    {
        FILE *f = fopen(".tcs/Base_Problems.txt", "w");
        FILE *g = fopen(".tcs/Base_Base_Problems.txt", "w");
        fprintf(f, "%d", 0);
        fprintf(g, "%d", 0);
        fclose(f); fclose(g);
    }
}

int main(){
    initialize(); ///производим инициализацию файлов(создаем необходимые для работы)
  Recover();

    FILE *solve_problems = fopen(".tcs/solve_problems.txt", "r");
    fscanf(solve_problems, "%d", &Count);
    int Size = max(Count*2, Min_Count);
    int Solve_Problems[Size+1];
    for (int i = 1; i <= Count; i++)
        fscanf(solve_problems, "%d", &Solve_Problems[i]);

    fclose(solve_problems);
    /*
         проверяем, очищен ли буффер. Это делается после загрузки основного массива
         решенных задач, ибо:
         1. Это позволит вывести пользователю нынешнюю ситуацию с задачами.
         2. Это никак не мешает производительности программы, ибо этот файл, в
         отличии от бэкапа, никак не меняет нынешний массив, а просто сохраняет
         файл и не очищает его. Поэтому, то, что это сделано после загрузки
         основного массива, никак не приносит вред оптимизации программы(ее скорости
         или объему используемой памяти), а, наоборот, увеличивает юзабилити,
         ибо пользователю так будет удобнее, а это главное!
    */
Recover_Buffer(Solve_Problems);

    if (flag_Recover_Back_Back_Up) Recover_Back_Back_Up(Solve_Problems); /// восстанавливаем из файла BBUp
    Back_Back_Up(Solve_Problems, -1, true);
char Command[15];


while (true){
       if (tip_important) cout << "Command: ";
    cin >> Command;
    if (!strcmp(Command, "go"))
    {
        int start, finish, concentration;
       if (tip_important) cout << "Input start number: "; cin >> start;
       if (tip_important) cout << "Input finish number: "; cin >> finish;
       if (tip_important) cout << "Input concentration: "; cin >> concentration;
        generation(Solve_Problems, start, finish, concentration);
        Back_Back_Up(Reserve, Reserve_Size, true);
        while (true)
        {
            cout << "What do you think?: ";
        char Command1[15];
        cin >> Command1;
        if (!strcmp(Command1, "ok"))
            {
            Union(Solve_Problems, Reserve[Reserve_Size]);
            Back_Up(Solve_Problems);
            Buffer_Save(Reserve, Reserve_Size, 1); /// добавляем в буффер
            Back_Back_Up(Reserve, -1, true);
            Delete_in_BP(Reserve, Reserve_Size); /// удаляем из BP то, что добавлено
            /// в решенные
            break;
            }
        else if (!strcmp(Command1, "stop"))
        {
            while (true)
            {
                if (tip_encumbering) cout << "Input number: ";
                int number;
                cin >> number;
          if (number == -1) break;
                if (Union(Solve_Problems, number) == true) break;
            }
            Back_Up(Solve_Problems);
            /*
             добавляем в файл Buffer до q_buff, который равен в массиве Reserve,
            номеру, на котором пользователь решил остановиться
            */
            Buffer_Save(Reserve, q_buff, 1);
            Back_Back_Up(Reserve, -1, true); /// очистка файла Back_Back_Up
             Delete_in_BP(Reserve, q_buff);/// удаляем из BP то, что добавлено
            /// в решенные
            break;
        }
         else if (!strcmp(Command1, "exit") || !strcmp(Command1, "break")) break;
         else if (!strcmp(Command1, "save"))
          {
            char Command2[30];
            cin >> Command2;
            if (!strcmp(Command2, "all"))
            {
                Save_in_BP(Reserve, Reserve_Size, Reserve[1], Reserve[Reserve_Size]);
            }
            else if (!strcmp(Command2, "stop"))
            {
                int number; cin >> number;
                int q = -1;
                for (int i = 1; i <= Reserve_Size; i++)
                    if (Reserve[i] == number) {q = i; break;}
                    q_buff = q;
                Save_in_BP(Reserve, Reserve_Size, Reserve[1], Reserve[q_buff]);
            }
            else if (!strcmp(Command2, "diap"))
            {
                int start, finish;
                while (true)
                {
                    cin >> start >> finish;
                    if (start == -1) break;
                    if (start < 0 || start > finish || finish > inf)
                        cout << "Not correct. Try again\nInput: ";
                    else break;
                }
                Save_in_BP(Reserve, Reserve_Size, start, finish);
            }
          }
        }
        Reserve_Size = 0;
    }
    else if (!strcmp(Command, "add"))
    {
        int count;
        cin >> count;
        int back_back_up[count+1];
        int s[2];
       if (tip_encumbering) cout << "Input numbers: ";
        for (int i = 1, numbers; i <= count; i++)
            {cin >> numbers;
            s[1] = numbers;
            back_back_up[i] = numbers;
            Add_Buffer(Solve_Problems, s, 1, 1); /// добавляем в буффер
            Add(Solve_Problems, numbers);
            Back_Back_Up(back_back_up, i, true);
            Delete_in_BP(s, 1); /// удаляем из BP решенную задачу(долго по времени)
            }

        Back_Up(Solve_Problems);
       /// Buffer_Save(back_back_up, count, 1);
        Back_Back_Up(back_back_up, -1, true); /// стираем файл Back_Back_Up
    }
    else if (!strcmp(Command, "delete"))
    {
        int count;
        cin >> count;
        int back_back_up[count+1];
        int s[2];
        if (tip_encumbering) cout << "Input numbers: ";
        for (int i = 1, numbers; i <= count; i++)
            {cin >> numbers;
            s[1] = numbers;
            back_back_up[i] = numbers;
            Add_Buffer(Solve_Problems, s, 1, -1); /// добавляем в буффер
            Delete(Solve_Problems, numbers);
                Back_Back_Up(back_back_up, i, false);}

            //Counting_Sort(Solve_Problems, Count+q_add-1);
        Back_Up(Solve_Problems);
        ///Buffer_Save(back_back_up, count, -1);
        Back_Back_Up(back_back_up, -1, true); /// очищаем файл Back_Back_Up
    }
    else if (!strcmp(Command, "exit"))
    {
        break;
    }
    else if (!strcmp(Command, "print"))
    {
        if (Count+q_add-1 == 0) cout << "You have not solve problems\n";
        else cout << "You have " << Count+q_add-1 << " solve problems:\n Numbers: ";
        print(Solve_Problems, 1, Count+q_add-1);
    }
    else if (!strcmp(Command, "delete_all"))
        {
            /*
                говорим, что удаляем все, и записываем это в буффер
            */
            Buffer_Save(Solve_Problems, Count+q_add-1, -1);

            Count = 0;
            q_add = 1;
            Back_Up(Solve_Problems);
        }
    else if (!strcmp(Command, "print:") || !strcmp(Command, "print_range"))
    {
        print_range(Solve_Problems, Count+q_add-1);
    }
    else if (!strcmp(Command, "tip")) tip_install();
    else if (!strcmp(Command, "delete:") || !strcmp(Command, "delete_range"))
    {
        delete_range(Solve_Problems);
        Back_Up(Solve_Problems);
    }
    else if (!strcmp(Command, "add:") || !strcmp(Command, "add_range"))
    {
        add_range(Solve_Problems);
        Back_Up(Solve_Problems);
    }
    else if (!strcmp(Command, "back")) Buffer_overwrite(Solve_Problems, 1);
    else if (!strcmp(Command, "next")) Buffer_overwrite(Solve_Problems, 0);
    else if (!strcmp(Command, "cur")) cout << Current_position << "\n";
    else if (!strcmp(Command, "clear") || !strcmp(Command, "cls"))
    {
        system("cls");
    }
    else if (!strcmp(Command, "buf_clear") || !strcmp(Command, "buf_cls"))
    {
        FILE *buf = fopen(".tcs/Buffer.txt", "w");
        fprintf(buf, "%d", 0);
        fclose(buf);
    }
    else if (!strcmp(Command, "buf_print")) buffer_print();
    else if (!strcmp(Command, "tip_print")) tip_print();
    else if (!strcmp(Command, "copy_bp")) Copy_BP(true);
    else if (!strcmp(Command, "add_bp"))
    {
        int count;
        cin >> count;
        int H[count];
       if (tip_encumbering) cout << "Input numbers: ";
        for (int i = 0; i < count; i++)
            cin >> H[i];
        Add_in_BP(H, count);
    }
    else if (!strcmp(Command, "search_bp"))
    {
        if (tip_encumbering) cout << "Input number: ";
        int key;
        cin >> key;
        cout << search_in_BP(key, true) << "\n\n";
    }
    else if (!strcmp(Command, "delete_bp"))
    {
        int count;
        cin >> count;
        int H[count+1];
        if (tip_encumbering) cout << "Input numbers: ";
        for (int i = 0; i < count; i++)
            cin >> H[i+1];
        Delete_in_BP(H, count);
    }
    else if (!strcmp(Command, "delete_bp:"))
    {
        Delete_range_in_BP();
    }
    else if (!strcmp(Command, "add_bp:"))
    {
        add_range_in_BP();
    }
    else if (!strcmp(Command, "print_bp")) print_BP();
    else if (!strcmp(Command, "print_bp:")) print_range_BP();
    else if (!strcmp(Command, "print_last_bp")) print_last_BP();
    else if (!strcmp(Command, "delete_all_bp")) delete_all_in_BP();
    else if (!strcmp(Command, "bp"))
    {
        cout << "\n";
        char Command1[40];
        while (true)
        {
            if (tip_important) cout << "BP command: ";
            cin >> Command1;
            if (!strcmp(Command1, "copy")) Copy_BP(true);
            else if (!strcmp(Command1, "add"))
            {
                int count;
                cin >> count;
                int H[count];
                if (tip_encumbering) cout << "Input numbers: ";
                for (int i = 0; i < count; i++)
                    cin >> H[i];
                Add_in_BP(H, count);
            }
            else if (!strcmp(Command1, "search"))
            {
                if (tip_encumbering) cout << "Input number: ";
                int key;
                cin >> key;
                cout << search_in_BP(key, true) << "\n\n";
            }
            else if (!strcmp(Command1, "delete"))
            {
                int count;
                cin >> count;
                int H[count+1];
                if (tip_encumbering) cout << "Input numbers: ";
                for (int i = 0; i < count; i++)
                    cin >> H[i+1];
                Delete_in_BP(H, count);
            }
            else if (!strcmp(Command1, "delete:"))  Delete_range_in_BP();
            else if (!strcmp(Command1, "delete_all")) delete_all_in_BP();
            else if (!strcmp(Command1, "add:")) add_range_in_BP();
            else if (!strcmp(Command1, "print")) print_BP();
            else if (!strcmp(Command1, "print:")) print_range_BP();
            else if (!strcmp(Command1, "print_last")) print_last_BP();
            else if (!strcmp(Command1, "help")) help_BP();
            else if (!strcmp(Command1, "exit")) break;
            else if (!strcmp(Command1, "cls") || !strcmp(Command1, "clear"))
            {
                system("cls");
            }
        }
    }
}

    solve_problems = fopen(".tcs/solve_problems.txt", "w");
    fprintf(solve_problems, "%d%s", Count+q_add-1, "\n");
    for (int i = 1; i < Count + q_add; i++)
        fprintf(solve_problems, "%d%s", Solve_Problems[i], " ");

    FILE *back_up = fopen(".tcs/Back_Up.txt", "w");
    fprintf(back_up, "%d", -1);
    back_up = fopen(".tcs/Back_Back_Up.txt", "w");
    fprintf(back_up, "%d", -1);
    back_up = fopen(".tcs/Buffer.txt", "w");
    fprintf(back_up, "%d", 0);

    return 0;
}
