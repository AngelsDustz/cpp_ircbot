#include <iostream>
#include "network.h"
#include "network.cpp"

using namespace std;

int main()
{
    int iResult;
    network bot = network("JOIN #ict4-2b\r\n");
    iResult=bot.start();
    if (iResult==2)
    {
        cout << "----- MEMORY ERROR DETECTED -----\n";
        system("pause");
        return 1;
    }
    system("pause");
    return 0;
}
