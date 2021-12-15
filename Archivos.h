#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED
#include <iostream>
#include <cstdlib>
#include "Usuarios.h"
#include <fstream>
#include <stdio.h>
#include "Cifrado.h"
#include <regex.h>
///tamaño de arreglo directorio
#define SIZE_DIRECTORY 250

///tamaño del buffer de columnas
#define SIZE_COLUMNS 45

///Directorios
#define TB_ARCHIVO "tables"
#define DB_PATH "DB/DATABASES/"
#define DB_TITULO "DB/DATABASES/DB.txt"
#define DB_PATH_TABLE "/tables.txt"
#define DB_PATH_ROWS "/rows.txt"
#define DB_PATH_STRUCTURE "/structure.txt"
#define DB_PATH_LENGTH "/length.txt"
#define DB_PATH_DATATYPE "/datatype.txt"
#define dcc "DB/sistema_db/tables.txt"
using namespace std;

class Archivos{
    private:
    Cifrado c;
    struct _Usuario{
        string usuario, password;
    }Usuario;
    char* convert(char pal[]){
        for (int i = 0; i < strlen(pal); i++)
            pal[i] = toupper(pal[i]);
        return pal;
    }
    public:
    Archivos(){

    }
    ~Archivos(){
    }

    /**
        ______________
        Bases de datos
        ______________
    */
    #pragma region Base de datos
    //buscar base de datos
    bool find_db(string db){
        bool encontrado=false;
        ifstream archivo;
        archivo.open(DB_TITULO,ios::in);
        if(archivo.fail())
            return false;

        string cadena;
        /*Cifrado s;
        string dd=s.cifrar(db);
        cout<<dd<<endl;*/

        while(!archivo.eof()){
            getline(archivo, cadena);
            if(cadena==c.cifrar(db)){
                encontrado=true;
                //cout<<"cadena: "<<s.descifrar(cadena)<<endl;
                break;
            }
        }
        archivo.close();
        return encontrado;
    }


    void create_db(){
        //ofstream es para agregar o concatenar
        string db;
        Cifrado s;
        fflush(stdin);
        cout<<SPACES<<"Ingrese el nombre de la base de datos: "<<endl;
        cout<<SPACES;
        //getline(cin,db);
        //fflush(stdin);
        cin>>db;
        ofstream archivo;
        if(!find_db(db)){
            //No existe la base de datos, procedemos a crearla

           //abriendo el archivo, si no existe lo crea si existe lo sobre escribe...
            archivo.open(DB_TITULO,ios::app);//DB_TITULO donde se almacenan todas las cabeceras de las bases de datos ...
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }

            archivo<<s.cifrar(db)<<endl;
            archivo.close();

            //archivo<<db<<endl;
            //archivo.close();

            //char* s=strcat(CREATE_DB,db);
            string directorio=DB_PATH;
            directorio+=db;

            /*char path[SIZE_DIRECTORY];
            for(int i=0;i<=directorio.length();i++)
                path[i]=directorio[i];*/
            //Creando la ruta de la base de datos
            const int crear_directorio=mkdir(directorio.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            //cout<<directorio<<endl;
            if(crear_directorio==-1){
            	cout<<"error al crear el directorio";
            	exit(EXIT_COMMAND);
            }

            //mkdir(directorio.c_str());
            cout<<SPACES<<"... Base de datos creada "<<endl;
        }else
            cout<<SPACES<<"... Error la base de datos ya existe !"<<endl;

    }

    void show_db(){
        ifstream archivo;
        archivo.open(DB_TITULO,ios::in);
        if(archivo.fail()){
            cout<<"error al abrir el archivo"<<endl;
            exit(EXIT_COMMAND);
        }
        string cadena;
        cout<<"\n\n"<<SPACES<<"---------------- Base de datos disponibles ----------------\n"<<endl;
        while(!archivo.eof()){
            getline(archivo, cadena);
            cout<<SPACES<<c.descifrar(cadena)<<endl;
            cadena="";
        }
        archivo.close();
    }


    void del_db(){
        string database;
        fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese el nombre de la base de datos"<<endl;
        cout<<SPACES;
        //getline(cin,database);
        //fflush(stdin);
        cin>>database;
        ifstream archivo;
        ofstream arch_auxiliar;
        if(find_db(database)){
            archivo.open(DB_TITULO,ios::in);
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            string cadena="", db, db_cifrado;
            while(!archivo.eof()){
                getline(archivo, db);
                //getline(archivo, salto);
                if(db!=c.cifrar(database) && db!="")
                    cadena+=db+"\n";
                db="";
            }
            archivo.close();
            arch_auxiliar.open(DB_TITULO,ios::out);
            if(arch_auxiliar.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(1);
            }
            arch_auxiliar<<cadena;
            arch_auxiliar.close();
            cout<<SPACES<<"... Base de datos eliminada"<<endl;
        }else
            cout<<SPACES<<"... Error: no se puede encontrar la base de datos"<<endl;

    }
    void upd_db(){
        string database, db_aux;
        bool encontrado=true;
        ifstream archivo;
        ofstream arch_auxiliar;

        fflush(stdin);
        ///Mostrando las bases de datos...
        show_db();

        cout<<SPACES<<"Ingrese el nombre de la base de datos: "<<endl;
        cout<<SPACES;
        //getline(cin,database);
        //fflush(stdin);
        cin>>database;

        if(find_db(database)){
            ///Se encontro la base de datos...

            ///Mientras la base de datos exista...
            do{
                cout<<SPACES<<"Ingrese el nuevo nombre de la base de datos: "<<endl;
                cout<<SPACES;
                //getline(cin,db_aux);
                //fflush(stdin);
                cin>>db_aux;

                if(!find_db(db_aux))
                    encontrado=false;
                else
                    cout<<SPACES<<"... Error: la base de datos ya existe"<<endl;
            }while(encontrado);

            ///Apertura en modo lectura...
            archivo.open(DB_TITULO,ios::in);

            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }

            string cadena="", db;
            string salto;

            ///Recorrido del archivo de las bases de datos...
            while(!archivo.eof()){
                getline(archivo, db);

                if(db!=c.cifrar(database)&& db!="")
                    cadena+=db+"\n";
                else{
                    if(db!="")
                        cadena+=c.cifrar(db_aux)+"\n";
                }
                db="";
            }
            archivo.close();

            ///Re-escritura del archivo...
            arch_auxiliar.open(DB_TITULO,ios::out);

            if(arch_auxiliar.fail()){
                cout<<SPACES<<"... Error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }

            arch_auxiliar<<cadena;
            arch_auxiliar.close();

            ///Construccion de las rutas..
            string old_path=DB_PATH;
            old_path+="/"+database;

            string new_path=DB_PATH;
            new_path+="/"+db_aux;

            ///Renombrar los ficheros...
            rename(old_path.c_str(), new_path.c_str());

            cout<<SPACES<<"... Base de datos actualizada"<<endl;
        }else
            cout<<SPACES<<"... Error: no se puede encontrar la base de datos"<<endl;

    }


    #pragma endregion Base de datos

    #pragma region Tablas
    void del_tb(){
        string database, tb;
        fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese el nombre de la base de datos"<<endl;
        cout<<SPACES;
        //getline(cin,database);
        //fflush(stdin);
        cin>>database;

        ifstream archivo;
        ofstream arch_auxiliar;
        if(find_db(database)){
            ///Se encontro la base de datos...

            ///Mostrar las tablas que tiene la base de datos...
            show_tb(database);

            cout<<SPACES<<"Ingrese el nombre de la tabla"<<endl;
            cout<<SPACES;
            //getline(cin,tb);
            //fflush(stdin);
            cin>>tb;

            if(find_tb(database, tb)){
                ///Se encontro la tabla...

            	if(show_rw_detail(database, tb).length()<=1){
            		string path=DB_PATH;
					path+=database;
					path+=DB_PATH_TABLE;

					///Apertura del archivo en modo lectura
					archivo.open(path.c_str(),ios::in);
					if(archivo.fail()){
						cout<<"error al abrir el archivo"<<endl;
						exit(EXIT_COMMAND);
					}
					string cadena="", table;
					string salto;

					///Recorrer el archivo...
					while(!archivo.eof()){
						getline(archivo, table);
						//getline(archivo, salto);
						if(table!=c.cifrar(tb) && table!="")
							cadena+=table+"\n";
						table="";
					}
					archivo.close();
					arch_auxiliar.open(path.c_str(),ios::out);
					if(arch_auxiliar.fail()){
						cout<<"error al abrir el archivo"<<endl;
						exit(EXIT_COMMAND);
					}
					arch_auxiliar<<cadena;
					arch_auxiliar.close();
					cout<<SPACES<<"... Tabla eliminada"<<endl;
            	}else
            		cout<<SPACES<<"... Error: no se puede eliminar la tabla porque tiene registros"<<endl;

            }
            else
                cout<<SPACES<<"... Error: no se puede encontrar la tabla"<<endl;
        }else
            cout<<SPACES<<"... Error: no se puede encontrar la base de datos"<<endl;

    }

    void upd_tb(){
        string database, tb, tb_aux;
        bool encontrado=true;
        ifstream archivo;
        ofstream arch_auxiliar;

        fflush(stdin);

        show_db();

        cout<<SPACES<<"Ingrese el nombre de la base de datos"<<endl;
        cout<<SPACES;
        //getline(cin,database);
        //fflush(stdin);
        cin>>database;


        if(find_db(database)){
            show_tb(database);
            cout<<SPACES<<"Ingrese el nombre de la tabla"<<endl;
            cout<<SPACES;
            //getline(cin,tb);
            //fflush(stdin);
            cin>>tb;

            if(find_tb(database, tb)){
                ///La base de datos existe..

                string path=DB_PATH;
                path+=database;
                path+=DB_PATH_TABLE;
                archivo.open(path.c_str(),ios::in);

                if(archivo.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                ///Mientras la tabla exista...
                do{
                    cout<<SPACES<<"Ingrese el nuevo nombre de la tabla"<<endl;
                    cout<<SPACES;
                    //getline(cin,tb_aux);
                    //fflush(stdin);
                    cin>>tb_aux;

                    if(!find_tb(database, tb_aux))
                        encontrado=false;
                    else
                        cout<<SPACES<<"Error: la tabla ya existe "<<endl;
                }while(encontrado);

                string cadena="", table;
                string salto;

                ///Recorrido del archivo de tablas...
                while(!archivo.eof()){
                    getline(archivo, table);
                    //getline(archivo, salto);
                    if(table!=c.cifrar(tb) && table!="")
                        cadena+=table+"\n";
                    else{
                        if(table!="")
                            cadena+=c.cifrar(tb_aux)+"\n";
                    }
                    table="";
                }

                archivo.close();
                arch_auxiliar.open(path.c_str(),ios::out);

                if(arch_auxiliar.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                arch_auxiliar<<cadena;
                arch_auxiliar.close();

                ///Construccion de las rutas...
                string old_path=DB_PATH;
                old_path+=database+"/"+tb;

                string new_path=DB_PATH;
                new_path+=database+"/"+tb_aux;

                ///Renombrando los archivos....
                rename(old_path.c_str(), new_path.c_str());
                cout<<SPACES<<"... Tabla actualizada"<<endl;
            }
            else
                cout<<SPACES<<"... Error: no se puede encontrar la tabla"<<endl;
        }else
            cout<<SPACES<<"... Error: no se puede encontrar la base de datos"<<endl;
    }

    void create_tb(){
        //ofstream es para agregar o concatenar
        ofstream archivo, arch_aux, arch_long;
        string database,tb,items;
        show_db();
        fflush(stdin);
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)){
            cout<<SPACES<<"Ingrese el nombre de la tabla "<<endl;
            cout<<SPACES;
            //getline(cin, tb);
            //fflush(stdin);
            cin>>tb;
            if(!find_tb(database,tb)){
                string path=DB_PATH;
                path+=database;
                path+=DB_PATH_TABLE;
                archivo.open(path.c_str(),ios::app);//DB_TITULO donde se almacenan todas las cabeceras de las bases de datos ...

                if(archivo.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                archivo<<c.cifrar(tb)<<endl;
                archivo.close();

                //char* s=strcat(CREATE_DB,db);
                string directorio=DB_PATH, directorio2, directorio3;
                directorio+=database;
                directorio+="/";
                directorio+=tb;
                /*char dir[SIZE_DIRECTORY];
                for(int i=0;i<=directorio.length();i++)
                    dir[i]=directorio[i];*/

                const int crear_directorio=mkdir(directorio.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                if(crear_directorio==-1){
                	cout<<"error al crear el directorio";
                	exit(EXIT_COMMAND);
                }

                //mkdir(directorio.c_str());
                directorio2=directorio;
                directorio3=directorio;

                //Creando la estructura de la tabla
                directorio+=DB_PATH_STRUCTURE;
                directorio2+=DB_PATH_DATATYPE;
                directorio3+=DB_PATH_LENGTH;

                archivo.open(directorio.c_str(),ios::out);
                arch_aux.open(directorio2.c_str(),ios::out);
                arch_long.open(directorio3.c_str(),ios::out);

                if(archivo.fail()||arch_aux.fail()|| arch_long.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                //Ingresar los campos de la tabla
                string columnas[SIZE_COLUMNS], datos[SIZE_COLUMNS], longitud[SIZE_COLUMNS], add_variable;
                int i=0;
                bool acceso_variable=false, acceso_long=false, add_columna=false;
                do{
                    acceso_variable=false;
                    add_columna=false;
                    acceso_long=false;

                    cout<<SPACES<<"Ingrese el nombre de la columna: "<<i+1<<endl;
                    cout<<SPACES;
                    //getline(cin,columnas[i]);
                    //fflush(stdin);
                    cin>>columnas[i];
                    do{
                        cout<<SPACES<<"Ingrese el tipo de dato: int: i, varchar: v, double: d "<<endl;
                        cout<<SPACES;
                        //getline(cin,datos[i]);
                        //fflush(stdin);
                        cin>>datos[i];
                        if(datos[i]=="i"||datos[i]=="v"||datos[i]=="d"){
                            acceso_variable=true;
                        }else
                            cout<<SPACES<<"... Error: debe seleccionar un dato valido"<<endl;
                    }while(!acceso_variable);
                    do{
                    	cout<<SPACES<<"Ingrese la longitud del campo: "<<endl;
						cout<<SPACES;
						//getline(cin,datos[i]);
						//fflush(stdin);
						cin>>longitud[i];
						if(validate_datatype("i",longitud[i]))
							acceso_long=true;
						else
							cout<<SPACES<<"... Error: debe seleccionar una longitud valida"<<endl;
                    }while(!acceso_long);

                    cout<<SPACES<<"Desea seguir añadiendo mas columnas? s:si, n:no"<<endl;
                    cout<<SPACES;
                    //getline(cin,add_variable);
                    //fflush(stdin);
                    cin>>add_variable;
                    if(add_variable=="s")
                        add_columna=true;
                    i++;
                }while(add_columna);


                //string keys[40];
                //Leer la cadena sql y almacenarla en keys
                //int n=leerSQL(items, keys);

                for(int j=0;j<i;j++){
                    archivo<<c.cifrar(columnas[j])<<endl;
                    arch_aux<<c.cifrar(datos[j])<<endl;
                    arch_long<<c.cifrar(longitud[j])<<endl;
                }


                archivo.close();
                arch_aux.close();
                arch_long.close();
                cout<<SPACES<<"... Tabla creada con exito !"<<endl;
            }else
                cout<<SPACES<<"... Error la tabla ya existe"<<endl;
            //cout<<directorio<<endl;
        }else
                cout<<SPACES<<"... Error la base de datos no existe"<<endl;

    }

    bool validate_datatype(string datatype, string cadena){
    	if(strcmp(datatype.c_str(),"i")==0){
    		for(int i=0;i<cadena.length();i++){
    			if(!(cadena[i]>=48 && cadena[i]<=57))
    				return false;
    		}
    	}else if(strcmp(datatype.c_str(),"v")==0){
    		for(int i=0;i<cadena.length();i++){
    			if(!(cadena[i]>=32 && cadena[i]<=126 && cadena[i]!=34 && cadena[i]!=39))
    				return false;
    		}
    	}else{
    		int cont_puntos=0;
    		for(int i=0;i<cadena.length();i++){
    			cont_puntos=(cadena[i]==46)?cont_puntos+=1:cont_puntos;
    			if(!(cadena[i]>=48 && cadena[i]<=57 || cadena[i]==46 && cont_puntos<=1))
    				return false;
    				//cadena[i]==46 && cont_puntos<=1
    		}
    	}


    	return true;
    }

    string data_type(string dt){
      	if(strcmp(dt.c_str(),"i")==0)
      		return "entero";
      	else if(strcmp(dt.c_str(),"v")==0)
      		return "varchar";
      	else
      		return "double";
      }

    void show_rw(){
        //ofstream es para agregar o concatenar
        ifstream archivo, arch_aux;
        ofstream archivo_escritura;
        string database,tb,items;
        //fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)){
            show_tb(database);
            cout<<SPACES<<"Ingrese el nombre de la tabla "<<endl;
            cout<<SPACES;
            //getline(cin, tb);
            //fflush(stdin);
            cin>>tb;
            if(find_tb(database,tb)){
                system(CLEAR_SCREEN);
                string path=DB_PATH, path2;
                path+=database;
                path+="/";
                path+=tb;
                path2=path;
                path+=DB_PATH_STRUCTURE;
                path2+=DB_PATH_DATATYPE;

                archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
                arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...

                string columnas[SIZE_COLUMNS];
                string registros[SIZE_COLUMNS];
                string datatypes[SIZE_COLUMNS];
                int i=0, j=0;
                string cl="";
                if(archivo.fail() || arch_aux.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                while(!archivo.eof()){
                    getline(archivo,cl);
                    if(cl!=""){
                        columnas[i]=c.descifrar(cl);
                        i++;
                    }
                    cl="";
                }

                while(!arch_aux.eof()){
                    getline(arch_aux,cl);
                    if(cl!=""){
                        datatypes[j]=c.descifrar(cl);
                        j++;
                    }
                    cl="";
                }
                archivo.close();
                arch_aux.close();

                path=DB_PATH;
                path+=database;
                path+="/";
                path+=tb;
                path+=DB_PATH_ROWS;

                archivo.open(path.c_str(),ios::in);

                if(archivo.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                string cadena="";
                while(!archivo.eof()){
                    for(int k=0;k<i;k++){
                        getline(archivo,cl);
                        if(cl!="")
                            cadena+=SPACES+columnas[k]+": "+c.descifrar(cl)+"\n";

                    }
                    cl="";
                    getline(archivo,cl);
                    cadena+="\n";
                }
                archivo_escritura.close();
                cout<<"\n\n"<<SPACES<<"Registros encontrados: \n\n";
                cout<<cadena;
            }else
                cout<<SPACES<<"... Error la tabla no existe"<<endl;
            //cout<<directorio<<endl;
        }else
                cout<<SPACES<<"... Error la base de datos no existe"<<endl;
    }

    string show_rw_detail(string database, string tb){
    	string cadena="";
        //ofstream es para agregar o concatenar
        ifstream archivo, arch_aux;
        ofstream archivo_escritura;

        //system(CLEAR_SCREEN);
        string path=DB_PATH, path2;
        path+=database;
        path+="/";
        path+=tb;
        path2=path;
        path+=DB_PATH_STRUCTURE;
        path2+=DB_PATH_DATATYPE;

        archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
        arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
        string columnas[SIZE_COLUMNS];
        string registros[SIZE_COLUMNS];
        string datatypes[SIZE_COLUMNS];
        int i=0, j=0;
        string cl="";
        if(archivo.fail() || arch_aux.fail()){
            cout<<"error al abrir el archivo"<<endl;
            exit(1);
        }
        while(!archivo.eof()){
            getline(archivo,cl);
            if(cl!=""){
                columnas[i]=cl;
                i++;
            }
            cl="";
        }

        while(!arch_aux.eof()){
            getline(arch_aux,cl);
            if(cl!=""){
                datatypes[j]=cl;
                j++;
            }
            cl="";
        }
        archivo.close();
        arch_aux.close();

        path=DB_PATH;
        path+=database;
        path+="/";
        path+=tb;
        path+=DB_PATH_ROWS;

        archivo.open(path.c_str(),ios::in);

        if(archivo.fail()){
            cout<<"error al abrir el archivo"<<endl;
            return "";
        }

        while(!archivo.eof()){
            for(int k=0;k<i;k++){
                getline(archivo,cl);
                if(cl!="")
                    cadena+=SPACES+c.descifrar(columnas[k])+": "+c.descifrar(cl)+"\n";
            }
            cl="";
            getline(archivo,cl);
            cadena+="\n";
        }
        archivo_escritura.close();
        //cout<<cadena;
            //cout<<directorio<<endl;
        return cadena;
    }

    void delete_rw(){
        //ofstream es para agregar o concatenar
        ifstream archivo, arch_aux;
        ofstream archivo_escritura;
        string database,tb,items;
        fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)){
            show_tb(database);
            cout<<SPACES<<"Ingrese el nombre de la tabla "<<endl;
            cout<<SPACES;
            //getline(cin, tb);
            //fflush(stdin);
            cin>>tb;
            if(find_tb(database,tb)){
                system(CLEAR_SCREEN);
                string path=DB_PATH, path2;
                path+=database;
                path+="/";
                path+=tb;
                path2=path;
                path+=DB_PATH_STRUCTURE;
                path2+=DB_PATH_DATATYPE;

                archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
                arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
                string columnas[SIZE_COLUMNS];
                string registros[SIZE_COLUMNS];
                string datatypes[SIZE_COLUMNS];
                int i=0, j=0;
                string cl="";

                if(archivo.fail() || arch_aux.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                while(!archivo.eof()){
                    getline(archivo,cl);
                    if(cl!=""){
                        columnas[i]=cl;
                        i++;
                    }
                    cl="";
                }
                while(!arch_aux.eof()){
                    getline(arch_aux,cl);
                    if(cl!=""){
                        datatypes[j]=cl;
                        j++;
                    }
                    cl="";
                }
                archivo.close();
                arch_aux.close();

                ///Mostrando los registros...
                cout<<show_rw_detail(database,tb);

                path=DB_PATH;
                path+=database;
                path+="/";
                path+=tb;
                path+=DB_PATH_ROWS;

                archivo.open(path.c_str(),ios::in);

                if(archivo.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                int indice;
                string columna;
                cout<<SPACES<<"Ingrese el indice de la tabla con el que operara el registro: "<<endl;
                cout<<SPACES;
                cin>>indice;
                fflush(stdin);

                cout<<SPACES<<"Ingrese el: "<<c.descifrar(columnas[indice])<<endl;
                cout<<SPACES;
                cin>>columna;
                fflush(stdin);

                string cadena="", cadena_aux="";
                bool del=false, encontrado=false, continuar=true;
                int espacios=0;
                while(!archivo.eof() && continuar){
                    cadena_aux="";
                    cl="";
                    del=false;
                    espacios=0;
                    for(int k=0;k<i;k++){
                        getline(archivo,cl);
                        //cadena+=columnas[k]"\n";
                        if(cl=="")
                            espacios++;
                        if(espacios>1){
                            continuar=false;
                            break;
                        }

                        if(k==indice){
                            if(cl!=c.cifrar(columna))
                                cadena_aux+=cl+"\n";
                            else{
                                del=true;
                                encontrado=true;
                            }
                        }
                        else
                             cadena_aux+=cl+"\n";

                        //if(cl!="")
                            //cadena+=SPACES+columnas[k]+": "+cl+"\n";
                    }
                    if(!continuar)
                        break;
                    getline(archivo,cl);
                    cadena_aux+="\n";
                    //cadena auxiliar...
                    if(!del)
                        cadena+=cadena_aux;
                }
                archivo.close();

                if(!encontrado)
                    cout<<SPACES<<"...Error: el registro no fue encontrado"<<endl;
                else{
                    archivo_escritura.open(path.c_str(),ios::out);
                    if(archivo_escritura.fail()){
                        cout<<"error al abrir el archivo"<<endl;
                        exit(EXIT_COMMAND);
                    }
                    archivo_escritura<<cadena;
                    archivo_escritura.close();
                    cout<<SPACES<<"... Registro eliminado..."<<endl;
                }

            }else
                cout<<SPACES<<"... Error la tabla no existe"<<endl;
            //cout<<directorio<<endl;
        }else
                cout<<SPACES<<"... Error la base de datos no existe"<<endl;
    }

    void update_rw(){
        //ofstream es para agregar o concatenar
        ifstream archivo, arch_aux;
        ofstream archivo_escritura;
        string database,tb,items;
        fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)){
            show_tb(database);
            cout<<SPACES<<"Ingrese el nombre de la tabla "<<endl;
            cout<<SPACES;
            //getline(cin, tb);
            //fflush(stdin);
            cin>>tb;
            if(find_tb(database,tb)){
                system(CLEAR_SCREEN);
                string path=DB_PATH, path2;
                path+=database;
                path+="/";
                path+=tb;
                path2=path;
                path+=DB_PATH_STRUCTURE;
                path2+=DB_PATH_DATATYPE;

                archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
                arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
                string columnas[SIZE_COLUMNS];
                string registros[SIZE_COLUMNS];
                string datatypes[SIZE_COLUMNS];
                int i=0, j=0;
                string cl="";

                if(archivo.fail() || arch_aux.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                while(!archivo.eof()){
                    getline(archivo,cl);
                    if(cl!=""){
                        columnas[i]=c.descifrar(cl);
                        i++;
                    }
                    cl="";
                }
                while(!arch_aux.eof()){
                    getline(arch_aux,cl);
                    if(cl!=""){
                        datatypes[j]=c.descifrar(cl);
                        j++;
                    }
                    cl="";
                }
                archivo.close();
                arch_aux.close();

                ///Mostrando los registros...
                cout<<show_rw_detail(database,tb);

                path=DB_PATH;
                path+=database;
                path+="/";
                path+=tb;
                path+=DB_PATH_ROWS;

                archivo.open(path.c_str(),ios::in);

                if(archivo.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                int indice;
                string columna;
                cout<<SPACES<<"Ingrese el indice de la tabla con el que operara el registro: "<<endl;
                cout<<SPACES;
                cin>>indice;
                fflush(stdin);

                cout<<SPACES<<"Ingrese el: "<<columnas[indice]<<endl;
                cout<<SPACES;
                cin>>columna;
                fflush(stdin);

                string cadena="", cadena_aux="";
                bool del=false, encontrado=false, continuar=true;
                int espacios=0;
                while(!archivo.eof() && continuar){
                    cadena_aux="";
                    cl="";
                    del=false;
                    espacios=0;
                    for(int k=0;k<i;k++){
                        getline(archivo,cl);
                        //cadena+=columnas[k]"\n";
                        if(cl=="")
                            espacios++;
                        if(espacios>1){
                            continuar=false;
                            break;
                        }
                        if(k==indice){
                            if(cl!=c.cifrar(columna))
                                cadena_aux+=cl+"\n";
                            else{
                                del=true;
                                encontrado=true;
                            }
                        }
                        else
                             cadena_aux+=cl+"\n";
                        //if(cl!="")
                            //cadena+=SPACES+columnas[k]+": "+cl+"\n";
                    }

                    if(!continuar)
                        break;
                    getline(archivo,cl);
                    cadena_aux+="\n";
                    //cadena auxiliar...
                    if(!del)
                        cadena+=cadena_aux;
                    else{
                        string data_aux="", cad_aux="";
                        bool acceso=false;
                        for(int y=0;y<i;y++){
                            fflush(stdin);
                            acceso=false;
                            data_aux="";
                            do{
                                cout<<SPACES<<"Ingrese: "<<columnas[y]<<endl;
                                cout<<SPACES;
                                //getline(cin, data_aux);
                                cin>>data_aux;
                                if(validate_datatype(datatypes[y], data_aux))
                                    acceso=true;
                                else
                                    cout<<SPACES<<"...Error: tipo de dato incorrecto"<<endl;
                            }while(!acceso);
                            if(acceso)
                                cad_aux+=c.cifrar(data_aux)+"\n";
                        }
                        cad_aux+="\n";
                        cadena+=cad_aux;
                    }
                }
                archivo.close();

                if(!encontrado)
                    cout<<SPACES<<"...Error: el registro no fue encontrado"<<endl;
                else{
                    //cout<<cadena<<endl;
                    archivo_escritura.open(path.c_str(),ios::out);
                    if(archivo_escritura.fail()){
                        cout<<"error al abrir el archivo"<<endl;
                        exit(EXIT_COMMAND);
                    }
                    archivo_escritura<<cadena;
                    archivo_escritura.close();
                    cout<<SPACES<<"... Registro actualizado..."<<endl;
                }

            }else
                cout<<SPACES<<"... Error la tabla no existe"<<endl;
            //cout<<directorio<<endl;
        }else
                cout<<SPACES<<"... Error la base de datos no existe"<<endl;
    }


    void insert_tb(){
        //ofstream es para agregar o concatenar
        ifstream archivo, arch_aux, arch_long;
        ofstream archivo_escritura;
        string database,tb,items;
        fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)){
            show_tb(database);
            cout<<SPACES<<"Ingrese el nombre de la tabla "<<endl;
            cout<<SPACES;
            //getline(cin, tb);
            //fflush(stdin);
            cin>>tb;
            if(find_tb(database,tb)){
                string path=DB_PATH, path2, path3;
                path+=database;
                path+="/";
                path+=tb;
                path2=path;
                path3=path;

                path+=DB_PATH_STRUCTURE;
                path2+=DB_PATH_DATATYPE;
                path3+=DB_PATH_LENGTH;

                archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
                arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
                arch_long.open(path3.c_str(),ios::in);

                string columnas[SIZE_COLUMNS];
                string registros[SIZE_COLUMNS];
                string datatypes[SIZE_COLUMNS];
                string longitudes[SIZE_COLUMNS];

                int i=0, j=0, lj=0;
                string cl="";
                if(archivo.fail() || arch_aux.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                while(!archivo.eof()){
                    getline(archivo,cl);
                    if(cl!=""){
                        columnas[i]=c.descifrar(cl);
                        i++;
                    }
                    cl="";
                }

                while(!arch_aux.eof()){
                    getline(arch_aux,cl);
                    if(cl!=""){
                        datatypes[j]=c.descifrar(cl);
                        j++;
                    }
                    cl="";
                }

                while(!arch_long.eof()){
                	getline(arch_long, cl);
                	if(cl!=""){
                		longitudes[lj]=c.descifrar(cl);
                		lj++;
                	}
                	cl="";
                }

                archivo.close();
                arch_aux.close();
                arch_long.close();

                //Ruta de los registros...
                path=DB_PATH;
                path+=database;
                path+="/";
                path+=tb;
                path+=DB_PATH_ROWS;


                archivo_escritura.open(path.c_str(),ios::app);
                if(archivo_escritura.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                int k=0;
                bool dato=false, long_dato=false;
                char s;
                do{
                    fflush(stdin);
                    s='x';
                    k=0;
                    system(CLEAR_SCREEN);
                    do{
                        do{
                        	dato=false;
                        	long_dato=false;

                            cout<<"\n"<<endl;
                            cout<<SPACES<<"Ingrese el valor de: "<<columnas[k]<<endl;
                            cout<<SPACES;
                            //getline(cin, registros[k]);
                            //fflush(stdin);
                            cin>>registros[k];
                            //cout<<datatypes[k]<<endl;
                            if(validate_datatype(datatypes[k],registros[k]))
                                dato=true;
                            else
                                cout<<SPACES<<"... Error: el tipo de dato es incorrecto se espera un dato: \""<<data_type(datatypes[k])<<"\""<<endl;

                            if(registros[k].length()<=atoi(longitudes[k].c_str()))
                            	long_dato=true;
                            else
                            	cout<<SPACES<<"... Error: la longitud del dato es mayor que:  \""<<longitudes[k]<<"\""<<endl;
                        }while(!dato || !long_dato);
                        k++;
                    }while(k<i);
                    for(int m=0;m<i;m++)
                        archivo_escritura<<c.cifrar(registros[m])<<endl;
                    archivo_escritura<<endl;
                    cout<<SPACES<<"... Registro ingresado exitosamente !"<<endl;
                    //insert_tb();
                    cout<<SPACES<<"Desea seguir agregando registros? s:si, n:no"<<endl;
                    cout<<SPACES;
                    cin>>s;
                }while(s=='s');
                archivo_escritura.close();

            }else
                cout<<SPACES<<"... Error la tabla no existe"<<endl;
            //cout<<directorio<<endl;
        }else
                cout<<SPACES<<"... Error la base de datos no existe"<<endl;
    }



    void insert_tb5(){
        //ofstream es para agregar o concatenar
        ifstream archivo, arch_aux;
        ofstream archivo_escritura;
        string database,tb,items;
        fflush(stdin);
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)){
            cout<<SPACES<<"Ingrese el nombre de la tabla "<<endl;
            cout<<SPACES;
            //getline(cin, tb);
            //fflush(stdin);
            cin>>tb;
            if(find_tb(database,tb)){
                string path=DB_PATH, path2;
                path+=database;
                path+="/";
                path+=tb;
                path2=path;
                path+=DB_PATH_STRUCTURE;
                path2+=DB_PATH_DATATYPE;

                archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
                arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
                string columnas[SIZE_COLUMNS];
                string registros[SIZE_COLUMNS];
                string datatypes[SIZE_COLUMNS];
                int i=0, j=0;
                string cl="";
                if(archivo.fail() || arch_aux.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }
                while(!archivo.eof()){
                    getline(archivo,cl);
                    if(cl!=""){
                        columnas[i]=cl;
                        i++;
                    }
                    cl="";
                }

                while(!arch_aux.eof()){
                    getline(arch_aux,cl);
                    if(cl!=""){
                        datatypes[j]=cl;
                        j++;
                    }
                    cl="";
                }
                archivo.close();
                arch_aux.close();

                int k=0;
                bool correcto=false;
                do{
                    correcto=false;

                    do{
                        cout<<SPACES<<"Ingrese el valor de: "<<columnas[k]<<endl;
                        cout<<SPACES;
                        //getline(cin, registros[k]);
                        //fflush(stdin);
                        cin>>registros[k];
                        if(validate_datatype(datatypes[k],registros[k]))
                            correcto=true;
                        else
                            cout<<SPACES<<"... Error: el tipo de dato es incorrecto "<<endl;
                    }while(!correcto);
                    k++;
                }while(k<i);


                path=DB_PATH;
                path+=database;
                path+="/";
                path+=tb;
                path+=DB_PATH_ROWS;

                archivo_escritura.open(path.c_str(),ios::app);
                if(archivo_escritura.fail()){
                    cout<<"error al abrir el archivo"<<endl;
                    exit(EXIT_COMMAND);
                }

                for(int m=0;m<i;m++)
                    archivo_escritura<<registros[m]<<endl;
                archivo_escritura<<endl;

                archivo_escritura.close();

                cout<<SPACES<<"... Registro ingresado exitosamente !"<<endl;
            }else
                cout<<SPACES<<"... Error la tabla no existe"<<endl;
            //cout<<directorio<<endl;
        }else
                cout<<SPACES<<"... Error la base de datos no existe"<<endl;
    }


    bool find_tb(string db, string tb){
        string path=DB_PATH;
        path+=db;
        path+=DB_PATH_TABLE;
        bool encontrado=false;
        ifstream archivo;

        archivo.open(path.c_str(),ios::in);
        if(archivo.fail())
            return false;

        string cadena;
        while(!archivo.eof()){
            getline(archivo, cadena);
            if(cadena==c.cifrar(tb)){
                encontrado=true;
                break;
            }
        }
        archivo.close();
        return encontrado;
    }

    int leerSQL(string cadena, string keys[]){
        int c=0;
        string pal;
        for(int i=0;i<cadena.length();i++){
            if(cadena[i]!=','){
                pal+=cadena[i];
                if(i==(cadena.length()-1))
                    keys[c++]=pal;
            }
            else {
                keys[c++]=pal;
                pal="";
            }
        }
        //keys[c]=pal;
        //for(int i=0;i<c;i++)
            //cout<<keys[i]<<" "<<c<<endl;
        return c;
    }

    ///Mostrar tablas
    void show_tb(string database){
        ifstream archivo;
        if(find_db(database)) {
            string path=DB_PATH;
            path+=database;
            path+=DB_PATH_TABLE;
            archivo.open(path.c_str(),ios::in);
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            string cadena;
            cout<<"\n\n"<<SPACES<<"---------------- Tablas disponibles ----------------\n"<<endl;
            while(!archivo.eof()){
                getline(archivo, cadena);
                cout<<SPACES<<c.descifrar(cadena)<<endl;
            }
            archivo.close();
        }else
            cout<<SPACES<<"... La base de datos no existe"<<endl;
    }

    ///Mostrar tablas
    void show_tb_details(){
        ifstream archivo;
        string database;
        fflush(stdin);
        show_db();
        cout<<SPACES<<"Ingrese la base de datos "<<endl;
        cout<<SPACES;
        //getline(cin, database);
        //fflush(stdin);
        cin>>database;
        if(find_db(database)) {
            string path=DB_PATH;
            path+=database;
            path+=DB_PATH_TABLE;
            archivo.open(path.c_str(),ios::in);
            if(archivo.fail()){
                cout<<"error al abrir el archivo"<<endl;
                exit(EXIT_COMMAND);
            }
            string cadena;
            cout<<"\n\n"<<SPACES<<"---------------- Tablas disponibles ----------------\n"<<endl;
            while(!archivo.eof()){
                getline(archivo, cadena);
                cout<<SPACES<<c.descifrar(cadena)<<endl;
            }
            archivo.close();
        }else
            cout<<SPACES<<"... La base de datos no existe"<<endl;
    }

    #pragma endregion Tablas

	#pragma region Shell
	string use_database(string cadena){
		string database="";
		for(int i=0;i<cadena.length();i++){
			if(i>=4 && i<=cadena.length()-2)
				database+=cadena[i];
		}
		//cout<<"esta es la base de datos: "<<database<<endl;
		if(this->find_db(database))
			return database;
		cout<<SPACES<<"Error: la base de datos no fue encontrada"<<endl;
		return "";
	}

	void select(string db, string cadena){
		string table="";
		for(int i=0;i<cadena.length();i++){
			if(i>=14 && i<=cadena.length()-2)
				table+=cadena[i];
		}

		if(find_tb(db, table)){
			system(CLEAR_SCREEN);
			cout<<SPACES<<"Registros encontrados: \n"<<endl;
			cout<<show_rw_detail(db,table);
		}
		else
			cout<<SPACES<<"Error: la tabla no existe"<<endl;
		//cout<<"tabla: "<<table<<endl;
		//show_rw_detail(db,table);
	}

	//update productos set id_producto=hola, nombre=hola, total=333;
	void update_row(string db, string cadena){
			string tabla="";
			int i=0;

			for(;i<cadena.length();i++){
				if(i>=7){
					if(cadena[i]==' ')
						break;
					tabla+=cadena[i];
				}
			}
			cout<<tabla<<endl;
			int set=i+5, key_size=0, value_size=0;
			bool continuar=true;
			string value="", id_key="", id_value="";
			string keys[15], values[15], aux_key="", aux_value="";

			i+=5;
			cout<<cadena[i]<<endl;
			for(;i<cadena.length();i++){
				if(!continuar)
					break;
				if(cadena[i]!='='){
					 if(cadena[i]!=' ')
						 aux_key+=cadena[i];
				}else { //if(cadena[i]=='='){
					keys[key_size++]=aux_key;
					aux_key="";
					for(; i<cadena.length();i++){
						if(cadena[i]!=',' && cadena[i]!='=' && cadena[i]!=' ')
							aux_value+=cadena[i];
						else if(cadena[i]==',' || cadena[i]==' '){
							values[value_size++]=aux_value;
							if(cadena[i]==' ')
								continuar=false;
							aux_value="";
							break;
						}
					}
				}
			}
			cout<<tabla<<endl;
			for(int k=0; k<key_size;k++)
				cout<<"key:"<<keys[k]<<"   , value:"<<values[k]<<endl;

			i+=6;
			//cout<<"hola "<<i<<" "<<cadena.length()<<endl;
			for(;i<cadena.length();i++){
				if(cadena[i]!='=')
					id_key+=cadena[i];
				else
					break;
			}
			i++;
			for(;i<cadena.length();i++){
				if(cadena[i]!=';')
					id_value+=cadena[i];
				else
					break;
			}

			cout<<id_key<<" "<<id_value<<endl;

			update_row_aux(db, tabla, keys, values, id_key, id_value, key_size);

			/*string value="";
			string keys[15], values[15], aux_key="", aux_value="";
			cout<<"tabla: "<<tabla<<endl;;*/



			/*int size_keys=0, size_values=0;
			for(;i<cadena.length();i++){
				if(i>=set+1){
					//if(cadena[i]!=',')
					if(cadena[i]!=',' && cadena[i]!='='){
						if(cadena[i]!='='){
							aux_key+=cadena[i];
						}

					}
					else if(cadena[i]==',' || cadena[i]==')'){
						keys[size_keys++]=aux;
						aux="";
						if(cadena[i]==')')
							break;
					}
				}
			}

			int parentesis_cierre=i+8, w=0;
			for(;i<cadena.length();i++){
				if(i>=parentesis_cierre+1){
					if(cadena[i]!=',' && cadena[i]!=')'){
						if(cadena[i]!=' ')
							aux+=cadena[i];
					}
					else if(cadena[i]==',' || cadena[i]==')'){
						values[size_values++]=aux;
						aux="";
					}
				}
			}


			if(size_keys==size_values){
				insert_tb(db, tabla, keys, values, size_keys);
			}else
				cout<<SPACES<<"Error: cantidad de columnas diferente de cantidad de valores"<<endl;

*/
			/*if(find_tb(db, tabla)){
			}else
				cout<<SPACES<<"Error: la tabla no existe"<<endl;*/

		}

	void update_row_aux(string db, string tb, string keys[], string values[], string id_key, string id_value, int size){
	        //ofstream es para agregar o concatenar
	        ifstream archivo, arch_aux;
	        ofstream archivo_escritura;
	        string items;

			if(find_tb(db,tb)){
				system(CLEAR_SCREEN);
				string path=DB_PATH, path2;
				path+=db;
				path+="/";
				path+=tb;
				path2=path;
				path+=DB_PATH_STRUCTURE;
				path2+=DB_PATH_DATATYPE;

				archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
				arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
				string columnas[SIZE_COLUMNS];
				string registros[SIZE_COLUMNS];
				string datatypes[SIZE_COLUMNS];
				int i=0, j=0;
				string cl="";

				if(archivo.fail() || arch_aux.fail()){
					cout<<"error al abrir el archivo"<<endl;
					exit(EXIT_COMMAND);
				}

				while(!archivo.eof()){
					getline(archivo,cl);
					if(cl!=""){
						columnas[i]=c.descifrar(cl);
						i++;
					}
					cl="";
				}
				while(!arch_aux.eof()){
					getline(arch_aux,cl);
					if(cl!=""){
						datatypes[j]=c.descifrar(cl);
						j++;
					}
					cl="";
				}
				archivo.close();
				arch_aux.close();


				path=DB_PATH;
				path+=db;
				path+="/";
				path+=tb;
				path+=DB_PATH_ROWS;

				archivo.open(path.c_str(),ios::in);

				if(archivo.fail()){
					cout<<"error al abrir el archivo"<<endl;
					exit(EXIT_COMMAND);
				}

				for(int m=0;m<i;m++){
					if(keys[m]!=columnas[m]){
						cout<<SPACES<<"Error: la columna \""+keys[m]+"\" no existe "<<endl;
						return;
					}
				}
				if(strcmp(id_key.c_str(), columnas[0].c_str())!=0){
					cout<<SPACES<<"Error: la columna \""+id_key+"\" no existe "<<endl;
					return;
				}

				string cadena="", cadena_aux="";
				bool del=false, encontrado=false, continuar=true;
				int espacios=0;
				while(!archivo.eof() && continuar){
					cadena_aux="";
					cl="";
					del=false;
					espacios=0;
					for(int k=0;k<i;k++){
						getline(archivo,cl);
						//cadena+=columnas[k]"\n";
						if(cl=="")
							espacios++;
						if(espacios>1){
							continuar=false;
							break;
						}
						if(k==0){
							if(cl!=c.cifrar(id_value))
								cadena_aux+=cl+"\n";
							else{
								del=true;
								encontrado=true;
							}
						}
						else
							 cadena_aux+=cl+"\n";
						//if(cl!="")
							//cadena+=SPACES+columnas[k]+": "+cl+"\n";
					}

					if(!continuar)
						break;
					getline(archivo,cl);
					cadena_aux+="\n";
					//cadena auxiliar...
					if(!del)
						cadena+=cadena_aux;
					else{
						string data_aux="", cad_aux="";
						bool acceso=false;
						for(int y=0;y<i;y++){
							fflush(stdin);
							acceso=false;
							data_aux="";
							if(validate_datatype(datatypes[y], values[y]))
								cad_aux+=c.cifrar(values[y])+"\n";
							else{
								cout<<SPACES<<"...Error: tipo de dato incorrecto en \""<<columnas[y]<<"\" se esperaba: "<<data_type(datatypes[y])<<endl;
								return;
							}
						}
						cad_aux+="\n";
						cadena+=cad_aux;
					}
				}
				archivo.close();

				if(!encontrado)
					cout<<SPACES<<"...Error: el registro no fue encontrado"<<endl;
				else{
					//cout<<cadena<<endl;
					archivo_escritura.open(path.c_str(),ios::out);
					if(archivo_escritura.fail()){
						cout<<"error al abrir el archivo"<<endl;
						exit(EXIT_COMMAND);
					}
					archivo_escritura<<cadena;
					archivo_escritura.close();
					cout<<SPACES<<"... Registro actualizado..."<<endl;
				}

			}else
	                cout<<SPACES<<"... Error la tabla no existe"<<endl;

	    }

	//drop table productos;
	void drop_table(string db, string cadena){
		int i=11;
		string tabla="";
		for(; i<cadena.length(); i++){
			if(cadena[i]!=';')
				tabla+=cadena[i];
			else break;
		}
		//cout<<db<<" "<<tabla<<endl;
		drop_table_aux(db, tabla);

	}
	void drop_table_aux(string db, string tb){
		ifstream archivo;
		ofstream arch_auxiliar;
			if(find_tb(db, tb)){
				///Se encontro la tabla...
				if(show_rw_detail(db, tb).length()<=1){
					string path=DB_PATH;
					path+=db;
					path+=DB_PATH_TABLE;

					///Apertura del archivo en modo lectura
					archivo.open(path.c_str(),ios::in);
					if(archivo.fail()){
						cout<<"error al abrir el archivo"<<endl;
						exit(EXIT_COMMAND);
					}
					string cadena="", table;
					string salto;

					///Recorrer el archivo...
					while(!archivo.eof()){
						getline(archivo, table);
						//getline(archivo, salto);
						if(table!=c.cifrar(tb) && table!="")
							cadena+=table+"\n";
						table="";
					}
					archivo.close();
					arch_auxiliar.open(path.c_str(),ios::out);
					if(arch_auxiliar.fail()){
						cout<<"error al abrir el archivo"<<endl;
						exit(EXIT_COMMAND);
					}
					arch_auxiliar<<cadena;
					arch_auxiliar.close();
					cout<<SPACES<<"... Tabla eliminada"<<endl;
				}else
					cout<<SPACES<<"... Error: no se puede eliminar la tabla porque tiene registros"<<endl;
			}
			else
				cout<<SPACES<<"... Error: no se puede encontrar la tabla"<<endl;

	}


	///delete from pasfasdffsfdfsaffsf where id=10
	void delete_row(string db, string cadena){
		int i=12;
		string tabla="", id_key="", id_value="";
		for(;i<cadena.length();i++){
			if(cadena[i]!=' ')
				tabla+=cadena[i];
			else break;
		}
		i+=7;
		for(;i<cadena.length();i++){
			if(cadena[i]!='=')
				id_key+=cadena[i];
			else break;
		}
		i++;
		for(;i<cadena.length();i++){
			if(cadena[i]!=';')
				id_value+=cadena[i];
			else break;
		}
		//cout<<tabla<<" "<<id_key<<" "<<id_value<<endl;
		delete_row_aux(db, tabla, id_key, id_value);
	}

	void delete_row_aux(string db, string tb, string id_key, string id_value){
		//ofstream es para agregar o concatenar
		ifstream archivo, arch_aux;
		ofstream archivo_escritura;
		string items;

		if(find_tb(db,tb)){
			//system(CLEAR_SCREEN);
			string path=DB_PATH, path2;
			path+=db;
			path+="/";
			path+=tb;
			path2=path;
			path+=DB_PATH_STRUCTURE;
			path2+=DB_PATH_DATATYPE;

			archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
			arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
			string columnas[SIZE_COLUMNS];
			string registros[SIZE_COLUMNS];
			string datatypes[SIZE_COLUMNS];
			int i=0, j=0;
			string cl="";

			if(archivo.fail() || arch_aux.fail()){
				cout<<"error al abrir el archivo"<<endl;
				exit(EXIT_COMMAND);
			}

			while(!archivo.eof()){
				getline(archivo,cl);
				if(cl!=""){
					columnas[i]=cl;
					i++;
				}
				cl="";
			}
			while(!arch_aux.eof()){
				getline(arch_aux,cl);
				if(cl!=""){
					datatypes[j]=cl;
					j++;
				}
				cl="";
			}
			archivo.close();
			arch_aux.close();


			path=DB_PATH;
			path+=db;
			path+="/";
			path+=tb;
			path+=DB_PATH_ROWS;

			archivo.open(path.c_str(),ios::in);

			if(archivo.fail()){
				cout<<"error al abrir el archivo"<<endl;
				exit(EXIT_COMMAND);
			}

			if(strcmp(columnas[0].c_str(), c.cifrar(id_key).c_str())!=0){
				cout<<SPACES<<"Error: la columna \""<<id_key<<"\" no existe"<<endl;
				return;
			}


			string cadena="", cadena_aux="";
			bool del=false, encontrado=false, continuar=true;
			int espacios=0;
			while(!archivo.eof() && continuar){
				cadena_aux="";
				cl="";
				del=false;
				espacios=0;
				for(int k=0;k<i;k++){
					getline(archivo,cl);
					//cadena+=columnas[k]"\n";
					if(cl=="")
						espacios++;
					if(espacios>1){
						continuar=false;
						break;
					}

					if(k==0){
						if(cl!=c.cifrar(id_value))
							cadena_aux+=cl+"\n";
						else{
							del=true;
							encontrado=true;
						}
					}
					else
						 cadena_aux+=cl+"\n";

					//if(cl!="")
						//cadena+=SPACES+columnas[k]+": "+cl+"\n";
				}
				if(!continuar)
					break;
				getline(archivo,cl);
				cadena_aux+="\n";
				//cadena auxiliar...
				if(!del)
					cadena+=cadena_aux;
			}
			archivo.close();

			if(!encontrado)
				cout<<SPACES<<"...Error: el registro no fue encontrado"<<endl;
			else{
				archivo_escritura.open(path.c_str(),ios::out);
				if(archivo_escritura.fail()){
					cout<<"error al abrir el archivo"<<endl;
					exit(EXIT_COMMAND);
				}
				archivo_escritura<<cadena;
				archivo_escritura.close();
				cout<<SPACES<<"... Registro eliminado..."<<endl;
			}

		}else
			cout<<SPACES<<"... Error la tabla no existe"<<endl;
		//cout<<directorio<<endl;
	}



	//insert into productos (id, nombre, precio) values(1, teclado, 100);
	void insert_into(string db, string cadena){
		string tabla="";
		int i=0;
		for(;i<cadena.length();i++){
			if(i>=12){
				if(cadena[i]==' ')
					break;
				tabla+=cadena[i];
			}
		}
		int parentesis=i+1;
		string value="";
		string keys[15], values[15], aux="";

		int size_keys=0, size_values=0;
		for(;i<cadena.length();i++){
			if(i>=parentesis+1){
				//if(cadena[i]!=',')
				if(cadena[i]!=',' && cadena[i]!=')'){
					if(cadena[i]!=' ')
						aux+=cadena[i];
				}
				else if(cadena[i]==',' || cadena[i]==')'){
					keys[size_keys++]=aux;
					aux="";
					if(cadena[i]==')')
						break;
				}
			}
		}

		int parentesis_cierre=i+8, w=0;
		for(;i<cadena.length();i++){
			if(i>=parentesis_cierre+1){
				if(cadena[i]!=',' && cadena[i]!=')'){
					if(cadena[i]!=' ')
						aux+=cadena[i];
				}
				else if(cadena[i]==',' || cadena[i]==')'){
					values[size_values++]=aux;
					aux="";
				}
			}
		}


		if(size_keys==size_values){
			insert_tb(db, tabla, keys, values, size_keys);
		}else
			cout<<SPACES<<"Error: cantidad de columnas diferente de cantidad de valores"<<endl;


		/*if(find_tb(db, tabla)){
		}else
			cout<<SPACES<<"Error: la tabla no existe"<<endl;*/

	}


	void insert_tb(string db, string tb, string keys[], string values[], int size){
	        //ofstream es para agregar o concatenar
	        ifstream archivo, arch_aux, arch_long;
	        ofstream archivo_escritura;
	        string items;

			if(find_tb(db, tb)){
				string path=DB_PATH, path2, path3;
				path+=db;
				path+="/";
				path+=tb;
				path2=path;
				path3=path;

				path+=DB_PATH_STRUCTURE;
				path2+=DB_PATH_DATATYPE;
				path3+=DB_PATH_LENGTH;

				archivo.open(path.c_str(),ios::in);//Declaracion de lectura...
				arch_aux.open(path2.c_str(),ios::in);//Declaracion de lectura...
				arch_long.open(path3.c_str(),ios::in);

				string columnas[SIZE_COLUMNS];
				string registros[SIZE_COLUMNS];
				string datatypes[SIZE_COLUMNS];
				string longitudes[SIZE_COLUMNS];

				int i=0, j=0, lj=0;
				string cl="";
				if(archivo.fail() || arch_aux.fail()){
					cout<<"error al abrir el archivo"<<endl;
					exit(EXIT_COMMAND);
				}
				while(!archivo.eof()){
					getline(archivo,cl);
					if(cl!=""){
						columnas[i]=c.descifrar(cl);
						i++;
					}
					cl="";
				}

				while(!arch_aux.eof()){
					getline(arch_aux,cl);
					if(cl!=""){
						datatypes[j]=c.descifrar(cl);
						j++;
					}
					cl="";
				}

				while(!arch_long.eof()){
					getline(arch_long, cl);
					if(cl!=""){
						longitudes[lj]=c.descifrar(cl);
						lj++;
					}
					cl="";
				}

				archivo.close();
				arch_aux.close();
				arch_long.close();

				//Ruta de los registros...
				path=DB_PATH;
				path+=db;
				path+="/";
				path+=tb;
				path+=DB_PATH_ROWS;


				archivo_escritura.open(path.c_str(),ios::app);
				if(archivo_escritura.fail()){
					cout<<"error al abrir el archivo"<<endl;
					exit(EXIT_COMMAND);
				}

				int k=0, cs=0;
				string datos="";
				bool dato=false, long_dato=false;
				char s;

				for(int it=0;it<i;it++){
					if(strcmp(columnas[it].c_str(),keys[it].c_str())!=0){
						cout<<SPACES<<"Error: la columna: \""<<keys[it]<<"\" no existe "<<endl;
						//Quiebra la funcion, simpere y cuando no retorne ningun valor...
						return;
					}else{
						if(validate_datatype(datatypes[it],values[it])){
							if(values[it].length()<=atoi(longitudes[it].c_str()))
								datos+=c.cifrar(values[it])+"\n";
							else{
								cout<<SPACES<<"... Error: la longitud de \""+columnas[it]+"\" mayor que \""<<longitudes[it]<<"\""<<endl;
								return;
							}
						}
						else{
							cout<<SPACES<<"... Error: el tipo de dato ingresado para: \""+columnas[it]+"\" es incorrecto, se espera un dato: \""<<data_type(datatypes[it])<<"\""<<endl;
							return;
						}
					}
				}
				archivo_escritura<<datos<<endl;
				cout<<SPACES<<"... Registro ingresado exitosamente !"<<endl;
				archivo_escritura.close();
			}else
				cout<<SPACES<<"... Error la tabla no existe"<<endl;
			//cout<<directorio<<endl;

	    }

	//insert into 'nombre_tabla' (idtabla, nombre, detalle) values(1, 'tablilla', 'perfecto..');
	//show databases;
		void show(string db, string cadena){
			int i=5;
			string aux="";
			for(;i<cadena.length();i++){
				if(cadena[i]!=';')
					aux+=cadena[i];
			}
			if(strcmp(aux.c_str(), "databases")==0)
				show_db();
			else if(strcmp(aux.c_str(), "tables")==0){
				if(db.length()!=0)
					show_tb(db);
				else
					cout<<SPACES<<"Error: no ha seleccionado la base de datos !"<<endl;
			}

			else
				cout<<SPACES<<"Error: comando erroneo !"<<endl;
		}



	#pragma endregion Shell
};




#endif // ARCHIVOS_H_INCLUDED
