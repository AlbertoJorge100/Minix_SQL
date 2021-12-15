#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <string.h>
using namespace std;
class Menu{
private:
    int IdMenu;
    string Opcion;
public:
    Menu(int IdMenu, string Opcion){
        this->IdMenu=IdMenu;
        this->Opcion=Opcion;
    }
    int getIdMenu(){
        return this->IdMenu;
    }
    string getOpcion(){
        return this->Opcion;
    }
};


#endif // MENU_H_INCLUDED
