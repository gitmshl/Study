#include "..\Converter\csvTotxt.hpp"

int main()
{
    /// 5,0,1,4,2,1,226,1884,6,0,2,6

    convert_from_csv_to_txt("mnist_train.csv", "mnist_train.txt");

    return 0;
}
