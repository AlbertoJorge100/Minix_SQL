#ifndef USUARIOS_H_INCLUDED
#define USUARIOS_H_INCLUDED
#include <string.h>
#include <iostream>
#include "Archivos.h"
#include <string.h>
#include <fstream>
#include <stdio.h>
#include "Cifrado.h"
#include <sstream>
#include <sys/stat.h>
#include "Menu.h"
#include <string>
///Salir del sistema: exitosamente...
#define EXIT_COMMAND 1

///Directorios
#define DB_USR_TXT "USUARIOS.txt"
#define DB_PATH_USR "DB/USUARIOS/"
#define DB_PATH_USR_TXT "DB/USUARIOS/USUARIOS.txt"

///Limpiar pantalla
#define CLEAR_SCREEN "clear"
#define SPACES "\t"

///Tamaño de menu
#define MENU_SIZE 14
#define FILE_EXTENSION ".txt"

///Nomenclatura de permisos
#define GRANTED_ACCESS "granted"
#define DENIED_ACCESS "denied"


///Menu de opciones
#define OPCION1  "Ajustes"
#define OPCION2  "Crear base de datos"
#define OPCION3  "Listar base de datos"
#define OPCION4  "Modificar base de datos"
#define OPCION5  "Eliminar base de datos"
#define OPCION6  "Crear tabla"
#define OPCION7  "Listar tablas"
#define OPCION8  "Ingresar registros de una tabla"
#define OPCION9  "Listar registros de una tabla"
#define OPCION10 "Actualizar registros"
#define OPCION11 "Eliminar registros"
#define OPCION12 "Modificar tabla"
#define OPCION13 "Eliminar tabla"
#define OPCION14 "Shell"


///ids menus
#define ID1  1
#define ID2  2
#define ID3  3
#define ID4  4
#define ID5  5
#define ID6  6
#define ID7  7
#define ID8  8
#define ID9  9
#define ID10 10
#define ID11 11
#define ID12 12
#define ID13 13
#define ID14 14

#define ID_SALIR 15
using namespace std;

class Usuarios{
private:
    Cifrado c;
    string usuario;

    struct _Usuario{
        string usuario, password;
    }Usuario;
    Menu *menu_opciones[MENU_SIZE];
    /*
     _Menu menu_opciones[MENU_SIZE]={
        {Menu.id=ID1,
        Menu.menu=OPCION1},
        {Menu.id=ID2,
        Menu.menu=OPCION2},
        {Menu.id=ID3,
        Menu.menu=OPCION3},
        {Menu.id=ID4,
        Menu.menu=OPCION4},
        {Menu.id=ID5,
        Menu.menu=OPCION5},
        {Menu.id=ID6,
        Menu.menu=OPCION6},
        {Menu.id=ID7,
        Menu.menu=OPCION7},
        {Menu.id=ID8,
        Menu.menu=OPCION8},
        {Menu.id=ID9,
        Menu.menu=OPCION9},
        {Menu.id=ID10,
        Menu.menu=OPCION10},
        {Menu.id=ID11,
        Menu.menu=OPCION11},
        {Menu.id=ID12,
        Menu.menu=OPCION12},
        {Menu.id=ID13,
        Menu.menu=OPCION13},
    };
*/
public:
    Usuarios(){
        setMenus();
    }
    void setMenus(){
        this->menu_opciones[0]=new Menu(ID1, OPCION1);
        this->menu_opciones[1]=new Menu(ID2, OPCION2);
        this->menu_opciones[2]=new Menu(ID3, OPCION3);
        this->menu_opciones[3]=new Menu(ID4, OPCION4);
        this->menu_opciones[4]=new Menu(ID5, OPCION5);
        this->menu_opciones[5]=new Menu(ID6, OPCION6);
        this->menu_opciones[6]=new Menu(ID7, OPCION7);
        this->menu_opciones[7]=new Menu(ID8, OPCION8);
        this->menu_opciones[8]=new Menu(ID9, OPCION9);
        this->menu_opciones[9]=new Menu(ID10, OPCION10);
        this->menu_opciones[10]=new Menu(ID11, OPCION11);
        this->menu_opciones[11]=new Menu(ID12, OPCION12);
        this->menu_opciones[12]=new Menu(ID13, OPCION13);
        this->menu_opciones[13]=new Menu(ID14, OPCION14);
    }
    ~Usuarios(){}

    #pragma region Usuarios
    void setUsuario(string usuario){
        this->usuario=usuario;
    }
    string getUsuario(){
        return this->usuario;
    }
    void mostrar_menu(){
        for(int i=0;i<MENU_SIZE;i++)
            cout<<SPACES<<menu_opciones[i]->getIdMenu() <<" "<< menu_opciones[i]->getOpcion()<<endl;
    }
    #pragma endregion Usuarios

    #pragma region Usuarios

    void create_usr(string user, string password){
        //ofstream es para agregar o concatenar
        ofstream archivo;
        char res;
        bool continuar=false;
        string cadena="", permiso;
        if(!find_usr(user)){
            //No existe la base de datos, procedemos a crearla

           //abriendo el archivo, si no existe lo crea si existe lo sobre escribe...
            archivo.open(DB_PATH_USR_TXT,ios::app);//DB_TITULO donde se almacenan todas las cabeceras de las bases de datos ...
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }

            archivo<<c.cifrar(user)<<endl;
            archivo<<c.cifrar(password)<<endl;
            archivo<<endl;
            archivo.close();
            cout<<"\n";
            cout<<SPACES<<"---------------Lista de privilegios a elegir: \n\n";
            mostrar_menu();
            cout<<"\n\n";
            cout<<SPACES<<"Seleccione los privilegios del usuario: s:si, n:no "<<endl;
            for(int i=0;i<MENU_SIZE;i++){
                do{
                    continuar=false;
                    res='n';
                    fflush(stdin);
                    cout<<SPACES<<"Permiso de: "<<menu_opciones[i]->getOpcion()<<endl;
                    cout<<SPACES;
                    cin>>res;
                    if(res=='s' || res=='n')
                       continuar=true;
                    else
                        cout<<SPACES<<"... Error: respuesta incorrecta "<<endl;
                }while(!continuar);
                if(continuar){
                    if(res=='s'){
                            stringstream ss;
                            ss<<menu_opciones[i]->getIdMenu();
                        //cadena+=std::to_string(ss.str())+"\n";
                        cadena+=c.cifrar(ss.str())+"\n";
                    }
                }
            }

            string path=DB_PATH_USR;
            path+=user;
            path+=FILE_EXTENSION;
            archivo.open(path.c_str(),ios::out);//DB_TITULO donde se almacenan todas las cabeceras de las bases de datos ...
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            archivo<<cadena;
            archivo.close();
            cout<<SPACES<<"... Usuario creado con exito "<<endl;
        }else
            cout<<SPACES<<"... Error el usuario ya existe !"<<endl;

    }

    bool find_usr(string usuario, string password=""){
        bool encontrado=false;
        ifstream archivo;

        archivo.open(DB_PATH_USR_TXT,ios::in);
        if(archivo.fail())
            return false;

        string cadena;
        //archivo.seekg(0);
        string salto;
        while(!archivo.eof()){
            getline(archivo, Usuario.usuario);
            getline(archivo, Usuario.password);
            getline(archivo, salto);
            //cout<<Usuario.usuario<<endl;
            //cout<<Usuario.password<<endl;
            //cout<<usuario<<" "<<password<<endl;
            //cout<<"usuario: "<<Usuario.usuario<<endl;
            //cout<<"password: "<<Usuario.password<<endl;
            if(c.cifrar(usuario)==Usuario.usuario){
                if(password!=""){
                    if(c.cifrar(password)==Usuario.password){
                        encontrado=true;
                        break;
                    }
                }else{
                    encontrado=true;
                    break;
                }
            }
            Usuario.usuario="";
            Usuario.password="";
        }
        archivo.close();
        return encontrado;
    }

    void show_usr(){
        ifstream archivo;
        archivo.open(DB_PATH_USR_TXT,ios::in);
        if(archivo.fail()){
            cout<<"error al abrir el archivo"<<endl;
            exit(EXIT_COMMAND);
        }
        string cadena;
        string salto;
        cout<<"\n\n"<<SPACES<<"---------------- Usuarios ----------------\n"<<endl;
        while(!archivo.eof()){
            getline(archivo, Usuario.usuario);
            getline(archivo, Usuario.password);
            getline(archivo, salto);
            if(Usuario.usuario!=""){
                cout<<SPACES<<c.descifrar(Usuario.usuario)<<endl;
                //cout<<SPACES<<"password: "<<Usuario.password<<"\n"<<endl;
                Usuario.usuario="";
                Usuario.password="";
            }
        }
        archivo.close();
    }

    ///Valida si el usuario tiene permiso de una accion
    bool perm_usr(int id_permiso, string usuario=""){
        bool permiso=false;
        ifstream archivo;
        string path=DB_PATH_USR;
        if(usuario!="")
            path+=usuario;
        else
            path+=this->usuario;

        path+=FILE_EXTENSION;

        archivo.open(path.c_str(),ios::in);
        if(archivo.fail()){
            cout<<"error al abrir el archivo"<<endl;
            exit(EXIT_COMMAND);
        }
        string id;
        string salto;

        int a = 10;
        stringstream s_id_permiso;
        s_id_permiso << id_permiso;

        while(!archivo.eof()){
            getline(archivo,id);
            //cout<<c.descifrar(id)<<" "<<s_id_permiso.str()<<endl;
            if(s_id_permiso.str()==c.descifrar(id)){
                permiso=true;
                break;
            }
            id_permiso=0;
        }
        archivo.close();
        return permiso;
    }

    ///Muestra los permisos que un usuario tiene
    void show_perm_usr(string usuario){
        //bool permiso=false;
        ifstream archivo;
        string path=DB_PATH_USR;
        string cadena="", id;
        //string usuario;
        path+=usuario;//Modificar x this->
        path+=FILE_EXTENSION;
        //cout<<SPACES<<"Ingese el nombre del usuario: "<<endl;
        archivo.open(path.c_str(),ios::in);
        if(archivo.fail()){
            cout<<"error al abrir el archivo"<<endl;
            exit(EXIT_COMMAND);
        }

        stringstream s_id_permiso;
        //s_id_permiso << id_permiso;
        int id_menu, i=0, ix;

        ///Recorrer el archivo de permisos
        while(!archivo.eof()){
            for(int i=0;i<MENU_SIZE;i++){
                getline(archivo,id);

                ///Buscar el permiso en la funcion que nos retornara el indice de "menu_opciones"
                ix=find_perm_it(atoi(c.descifrar(id).c_str()));
                if(ix!=-1)
                    ///El id del permiso existe en la base de datos
                    cadena+=SPACES+c.descifrar(id)+" "+menu_opciones[ix]->getOpcion()+"\n";
                //if(find_perm(atoi(id.c_str())))
                    ///El orden de almacenamiento en el archivo, es aleatorio, x ende
                    ///Hay que validar que el permiso sea valido...
                  //  cadena+=SPACES+id+" "+menu_opciones[i].menu+"\n";
                id="";
            }
        }
        system(CLEAR_SCREEN);
        cout<<endl;
        cout<<SPACES<<"Permisos del usuario: \n"<<endl;
        cout<<cadena<<endl;
        archivo.close();
        //return permiso;
    }

    ///Muestra los permisos que un usuario no tiene
    void show_rev_usr(string usuario){
        //bool permiso=false;
        string cadena="", id;
        for(int i=0;i<MENU_SIZE;i++){
            if(!perm_usr(menu_opciones[i]->getIdMenu(), usuario)){
                stringstream s_id_permiso;
                s_id_permiso << menu_opciones[i]->getIdMenu();
                cadena+=SPACES+s_id_permiso.str()+" "+menu_opciones[i]->getOpcion()+"\n";
            }
        }
        cout<<cadena;
    }


    ///Buscar un permiso dentro de la lista de permisos generica
    bool find_perm(int id_permiso){
        for(int i=0;i<MENU_SIZE;i++){
            if(menu_opciones[i]->getIdMenu()==id_permiso)
                return true;
        }
        return false;
    }
    int find_perm_it(int id_permiso){
        for(int i=0;i<MENU_SIZE;i++){
            if(menu_opciones[i]->getIdMenu()==id_permiso)
                return i;
        }
        return -1;
    }

     void revoke_perm_usr(){
        bool permiso=false;
        string cadena="", usuario, id_perm;
        ifstream archivo;
        ofstream arch_aux;
        string path=DB_PATH_USR;

        show_usr();
        fflush(stdin);
        cout<<"\n";
        cout<<SPACES<<"Ingrese el nombre del usuario "<<endl;
        cout<<SPACES;
        //getline(cin,usuario);
        //fflush(stdin);
        cin>>usuario;
        if(find_usr(usuario)){
            path+=usuario;
            path+=FILE_EXTENSION;
            cout<<"\n\n";
            cout<<SPACES<<"---- Permisos que el usuario tiene ----"<<endl;
            show_perm_usr(usuario);
            string id;
            string salto;
            fflush(stdin);
            cout<<SPACES<<"Ingrese el id del permiso a eliminar: "<<endl;
            cout<<SPACES;
            //getline(cin,id);
            //fflush(stdin);
            cin>>id;
            ///id de permiso encontrado..
            if(perm_usr(atoi(id.c_str()), usuario)){
                ///El usuario puede eliminar ese permiso...
                archivo.open(path.c_str(),ios::in);
                if(archivo.fail()){
                    cout<<SPACES<<"Error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                while(!archivo.eof()){
                    getline(archivo, id_perm);
                    if(strcmp(c.descifrar(id_perm).c_str(),id.c_str())!=0 && id_perm!="")
                    	cadena+=id_perm+"\n";

                    id_perm="";
                }
                archivo.close();
                arch_aux.open(path.c_str(),ios::out);
                if(arch_aux.fail()){
                	cout<<SPACES<<"Error al abrir el archivo"<<endl;
                	exit(EXIT_COMMAND);
                }
                arch_aux<<cadena;
                arch_aux.close();
                cout<<SPACES<<"... El permiso fue revocado"<<endl;
            }else
                cout<<SPACES<<"... Error: el permiso no fue encontrado"<<endl;
        }else
            cout<<SPACES<<"... Error: el usuario no existe"<<endl;

    }

    void upd_perm_usr(){
        bool permiso=false;
        string cadena="", usuario;
        ofstream archivo;
        string path=DB_PATH_USR;

        show_usr();

        fflush(stdin);
        cout<<SPACES<<"Ingrese el nombre del usuario "<<endl;
        cout<<SPACES;
        //getline(cin,usuario);
        //fflush(stdin);
        cin>>usuario;

        if(find_usr(usuario)){
            system(CLEAR_SCREEN);
            path+=usuario;
            path+=FILE_EXTENSION;
            ///Mostrando los permisos que el usuario tiene
            cout<<SPACES<<"Permisos que el usuario tiene: \n"<<endl;
            show_perm_usr(usuario);
            cout<<"\n";
            cout<<SPACES<<"Permisos que el usuario no tiene: \n"<<endl;
            show_rev_usr(usuario);
            string id, id_aux;
            string salto;

            do{
                fflush(stdin);
                cout<<SPACES<<"Ingrese el id del permiso a agregar: "<<endl;
                cout<<SPACES;
                //getline(cin,id);
                //fflush(stdin);
                cin>>id;
                if(find_perm(atoi(id.c_str()))){
                    permiso=true;
                    ///id de permiso valido...
                    if(!perm_usr(atoi(id.c_str()), usuario)){
                        ///El usuario puede agregar ese permiso...
                        archivo.open(path.c_str(),ios::app);
                        if(archivo.fail()){
                            cout<<"Error al abrir el archivo"<<endl;
                            exit(EXIT_COMMAND);
                        }
                        archivo<<c.cifrar(id);
                        archivo<<endl;
                        archivo.close();
                        cout<<SPACES<<"... Permiso añadido !"<<endl;
                    }else
                        cout<<SPACES<<"... Error: el permiso ya existe"<<endl;

                }else
                    cout<<SPACES<<"... Error: el id no fue encontrado "<<endl;
            }while(!permiso);
        }else
            cout<<SPACES<<"... Error: el usuario no existe "<<endl;
        //return permiso;
    }

    void del_usr(){
        string usr;
        fflush(stdin);
        cout<<SPACES<<"Ingrese el nombre del usuario"<<endl;
        cout<<SPACES;
        //getline(cin,usr);
        //fflush(stdin);
        cin>>usr;
        ifstream archivo;
        ofstream arch_auxiliar;
        if(find_usr(usr)){
            archivo.open(DB_PATH_USR_TXT,ios::in);
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            string cadena="";
            string salto;
            while(!archivo.eof()){
                getline(archivo, Usuario.usuario);
                getline(archivo, Usuario.password);
                getline(archivo, salto);
                if(Usuario.usuario!=c.cifrar(usr) && Usuario.usuario!="")
                    cadena+=Usuario.usuario+"\n"+Usuario.password+"\n\n";
                Usuario.usuario="";
                Usuario.password="";
            }
            archivo.close();

            arch_auxiliar.open(DB_PATH_USR_TXT,ios::out);
            if(arch_auxiliar.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            arch_auxiliar<<cadena;
            arch_auxiliar.close();
            cout<<SPACES<<"... Usuario eliminado"<<endl;
        }else
            cout<<SPACES<<"... No se encuentra el usuario"<<endl;

    }

    ///Queda pendiente .......
    ///Actualizar un usuario, nombre y contraseña
    void upd_usr(){
        string usr, user, pass, pass_aux;
        string path_modificar=DB_PATH_USR;
        bool acceso=false;
        fflush(stdin);
        cout<<SPACES<<"Ingrese el usuario a modificar"<<endl;
        cout<<SPACES;
        //getline(cin,usr);
        cin>>usr;
        ifstream archivo;
        ofstream arch_auxiliar;

        if(find_usr(usr)){
            ///El usuario existe...
            cout<<SPACES<<"Ingrese el nuevo usuario"<<endl;
            cout<<SPACES;
            //getline(cin,user);
            //fflush(stdin);
            cin>>user;

            cout<<SPACES<<"Ingrese la nueva contraseña"<<endl;
            cout<<SPACES;
            //getline(cin,pass);
            //fflush(stdin);
            cin>>pass;

            ///Mientras la contraseña sea incorrecta
            do{
                cout<<SPACES<<"Repita la nueva contraseña"<<endl;
                cout<<SPACES;
                //getline(cin,pass_aux);
                //fflush(stdin);
                cin>>pass_aux;
                if(pass==pass_aux)
                    acceso=true;
                else
                    cout<<SPACES<<"... las contraseñas no coinciden"<<endl;
            }while(!acceso);

            ///Archivo en modo lectura, donde se almacenan todos los usuarios...
            archivo.open(DB_PATH_USR_TXT,ios::in);

            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            string cadena="";
            string salto;

            ///Recorrido del archivo de los usuarios...
            while(!archivo.eof()){
                getline(archivo, Usuario.usuario);
                getline(archivo, Usuario.password);
                getline(archivo, salto);
                if(Usuario.usuario!=c.cifrar(usr) && Usuario.usuario!="")
                    ///Si el usuario es diferente del que buscamos, almacenamos en cadena...

                    cadena+=Usuario.usuario+"\n"+Usuario.password+"\n\n";
                else{
                    if(Usuario.usuario!="")
                        ///Si es diferente de fin de archivo...

                        cadena+=c.cifrar(user)+"\n"+c.cifrar(pass)+"\n\n";
                }

                Usuario.usuario="";
                Usuario.password="";
            }
            archivo.close();
            //cout<<cadena;
            arch_auxiliar.open(DB_PATH_USR_TXT,ios::out);
            if(arch_auxiliar.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            arch_auxiliar<<cadena;
            arch_auxiliar.close();

            ///Construccion de las rutas: donde esta el archivo a modificar

            string old_path=DB_PATH_USR;
            old_path+="/"+usr+FILE_EXTENSION;
            string new_path=DB_PATH_USR;
            new_path+="/"+user+FILE_EXTENSION;

            this->usuario=user;
            ///Renombrando el archivo anterior por el nuevo usuario...
            rename(old_path.c_str(), new_path.c_str());

            cout<<SPACES<<"... Usuario modificado"<<endl;
        }else
            cout<<SPACES<<"... No se encuentra el usuario"<<endl;

    }

    void ingresar_usuario(){
        string pass_aux, usuario, password;
        bool acceso=false, salir=false;
        fflush(stdin);
        cout<<SPACES<<"Ingrese el usuario a registrar: "<<endl;
        cout<<SPACES;
        //getline(cin, usuario);
        //fflush(stdin);
        cin>>usuario;
        cout<<SPACES<<"Ingrese la contraseña: "<<endl;
        cout<<SPACES;
        //getline(cin, password);
        //fflush(stdin);
        cin>>password;
        do{
            cout<<SPACES<<"Repita la contraseña: "<<endl;
            cout<<SPACES;
            //getline(cin,pass_aux);
            //fflush(stdin);
            cin>>pass_aux;
            pass_aux;
            if(pass_aux==""){
                salir=true;
                break;
            }

            if(password==pass_aux)
                acceso=true;
            else
                cout<<SPACES<<"... Contraseña incorrecta "<<endl;
        }while(!acceso);
        if(!salir)
            create_usr(usuario, password);
    }
    #pragma endregion Usuarios

};


#endif // USUARIOS_H_INCLUDED
