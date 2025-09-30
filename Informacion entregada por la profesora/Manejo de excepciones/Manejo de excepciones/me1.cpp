#include <iostream>
using namespace std;
 
int main()
{
   int x = -1;
 
   // Some code
   cout << "Antes del try \n";
   try {
      cout << "Dentro del try \n";
      if (x < 0)
      {
         throw x;
         cout << "Desp del throw (Nunca ejecutado) \n";
      }
   }
   catch (int x ) {
      cout << "Captura de exc. \n";
   }
 
   cout << "Desp. del catch \n";
   return 0;
}
