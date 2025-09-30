#include <iostream>
using namespace std;
 
int main()
{
    try  {
       throw 10;
    }
    catch (char *excp)  {
        cout << "Excp " << excp;
    }
    catch (...)  {
        cout << "Excp por defecto\n";
    }
    return 0;
}

