#include <iostream>
#include "Archivos.h"
#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include "Cifrado.h"
#include <regex.h>

// C++ program to find all the matches
//#include <bits/stdc++.h>


int menu();
int menu_ajustes();
void login();
void sistema();
void salir();
void expresion(string, string&);
bool db_selected=false;
Archivos* archivo=new Archivos();
Usuarios* usuario=new Usuarios();



int main()
{
	usuario->ingresar_usuario();
	//sistema();

    return 0;
}

void expresion(string cadena,string &database){
	 enum expresion {USE, UPDATE, SELECT, INSERT, DELETE, DROP, EXIT, SHOW};
	    expresion opcion;
	  regex_t rgx_select, rgx_insert, rgx_use, rgx_update, rgx_delete, rgx_drop, rgx_show;

	  regcomp(&rgx_select,"select",0);
	  regcomp(&rgx_update,"update",0);
	  regcomp(&rgx_use,"use",0);
	  regcomp(&rgx_insert,"insert",0);
	  regcomp(&rgx_delete,"delete",0);
	  regcomp(&rgx_drop,"drop",0);
	  regcomp(&rgx_show,"show",0);

	  //return_value = regexec(&rgx_select, cadena.c_str(), 0, NULL, 0);
	  if(regexec(&rgx_select, cadena.c_str(), 0, NULL, 0)==0)
		  opcion=SELECT;
	  else if(regexec(&rgx_insert, cadena.c_str(), 0, NULL, 0)==0)
		  opcion=INSERT;
	  else if(regexec(&rgx_update, cadena.c_str(), 0, NULL, 0)==0)
		  opcion=UPDATE;
	  else if(regexec(&rgx_use, cadena.c_str(), 0, NULL, 0)==0)
	  	  opcion=USE;
	  else if(regexec(&rgx_delete, cadena.c_str(), 0, NULL, 0)==0)
	  	  opcion=DELETE;
	  else if(regexec(&rgx_drop, cadena.c_str(), 0, NULL, 0)==0)
	  	  	  opcion=DROP;
	  else if(regexec(&rgx_drop, cadena.c_str(), 0, NULL, 0)==0)
	  	  	  	  opcion=DROP;
	  else if(regexec(&rgx_show, cadena.c_str(), 0, NULL, 0)==0)
	  	  	  	  	  opcion=SHOW;


	  if(opcion!=USE && database.length()==0){
		  if(!opcion==SHOW){
			  cout<<SPACES<<"Error: no ha seleccionado la base de datos"<<endl;
			  return;
		  }
	  }

	    switch(opcion){
	        case USE:
	            database=archivo->use_database(cadena);
	            break;
	        case SELECT:
	        	archivo->select(database, cadena);
	            break;
	        case INSERT:
	            archivo->insert_into(database, cadena);
	            break;
	        case UPDATE:
	            archivo->update_row(database, cadena);
	            break;
	        case DELETE:
	            archivo->delete_row(database, cadena);
	            break;
	        case DROP:
				archivo->drop_table(database, cadena);
				break;
	        case SHOW:
				archivo->show(database, cadena);
				break;
	        default:
	            cout<<"comando desconocido"<<endl;
	            break;
	    }
}

///Menus
int menu(){
    int opcion=0;
    cout<<"\n\n"<<SPACES<<"********************** Menu de opciones **********************"<<endl;

    ///El menu esta almacenado en la clase usuarios...
    usuario->mostrar_menu();

    cout<<SPACES<<"15- Salir"<<endl;
    cout<<"\n";
    cout<<SPACES<<"Ingrese una opcion: \n"<<endl;
    cout<<SPACES;
    cin>>opcion;
    return opcion;
}


int menu_ajustes(){
    int opcion=0;
    cout<<"\n\n"<<SPACES<<"********************** Menu de ajustes **********************"<<endl;
    cout<<SPACES<<"1- Crear usuario"<<endl;
    cout<<SPACES<<"2- Listar usuarios"<<endl;
    cout<<SPACES<<"3- Modificar usuario"<<endl;
    cout<<SPACES<<"4- Eliminar usuario"<<endl;
    cout<<SPACES<<"5- Modificar permisos"<<endl;
    cout<<SPACES<<"6- Eliminar permisos"<<endl;
    cout<<SPACES<<"7- Mostrar permisos"<<endl;
    cout<<SPACES<<"8- Salir"<<endl;
    cout<<"\n"<<SPACES<<"Ingrese una opcion: "<<endl;
    cout<<SPACES;
    cin>>opcion;
    return opcion;
}

void sistema(){
   login();
   int n=0;
   string database, table;
   do{
        n=menu();
        system(CLEAR_SCREEN);
        cout<<"\n\n";
        if(n==ID_SALIR)
            break;
        if(usuario->perm_usr(n)){
            switch(n){
                case ID1:{
                    ///Submenu de ajustes....
                    int i=0;
                    do{
                        system(CLEAR_SCREEN);
                        i=menu_ajustes();
                        switch(i){
                            case 1:
                                usuario->ingresar_usuario();
                                break;
                            case 2:
                                usuario->show_usr();
                                cin.get();
                                break;
                            case 3:
                                usuario->show_usr();
                                usuario->upd_usr();
                                cin.get();
                                break;
                            case 4:
                                usuario->show_usr();
                                usuario->del_usr();
                                cin.get();
                                break;
                            case 5:
                                ///Modificar permisos
                                usuario->upd_perm_usr();
                                cin.get();
                                break;
                            case 6:
                                ///Eliminar permisos
                                usuario->revoke_perm_usr();
                                cin.get();
                                break;
                            case 7:
                                ///Mostrar los permisos que un usuario tiene
                                ///Ya no quiero modificar el codigo.... :( entonces quedara asi...
                                usuario->show_perm_usr(usuario->getUsuario());
                                cin.get();
                                break;
                        }
                        cin.get();
                    }while(i!=8);
                    break;
                }
                case ID2:
                    ///Crear base de datos
                    archivo->create_db();
                    cin.get();
                    break;
                case ID3:
                    ///Listar base de datos..
                    archivo->show_db();
                    cin.get();
                    break;
                case ID4:
                    ///Modificar base de datos
                    archivo->upd_db();
                    cin.get();
                    break;
                case ID5:
                    ///Eliminar base de datos
                    archivo->del_db();
                    cin.get();
                    break;
                case ID6:
                    ///Crear tabla
                    archivo->create_tb();
                    cin.get();
                    break;
                case ID7:
                    ///Listar tablas
                    archivo->show_tb_details();
                    cin.get();
                    break;
                case ID8:
                    ///Agregar registros
                    archivo->insert_tb();
                    cin.get();
                    break;
                case ID9:
                    ///Listar registros de una tabla
                    archivo->show_rw();
                    cin.get();
                    break;
                case ID10:
                    ///Actualizar registros de una tabla
                    archivo->update_rw();
                    cin.get();
                    break;
                case ID11:
                    ///Eliminar registros de una tabla
                    archivo->delete_rw();
                    cin.get();
                    break;
                case ID12:
                    ///Modificar tabla
                    archivo->upd_tb();
                    cin.get();
                    break;
                case ID13:
                    ///Eliminar tabla
                    archivo->del_tb();
                    cin.get();
                    break;
                case ID14:{
                	///Eliminar tabla
					string comando="";
					string database;
					do{
						comando="";
						cout<<"myShell# ";
						fflush(stdin);
						getline(cin,comando);//include library <string>
						fflush(stdin);
						if(strcmp(comando.c_str(),"exit")!=0 && strcmp(comando.c_str(),"clear")!=0 && strcmp(comando.c_str(),"")!=0)
							expresion(comando,database);
						if(strcmp(comando.c_str(),"clear")==0)
							system(CLEAR_SCREEN);
					}while(comando!="exit");
					//cin.get();
					break;
                }
                case ID_SALIR:
                    ///Salir
                    //salir();
                    break;
            }
        }else{
        	cout<<"\n\n"<<SPACES<<"...Error:  \""<<usuario->getUsuario()<<"\" no tienes permiso para esta accion !"<<endl;
        	cin.get();
        }


        cout<<"\n\n";
        cin.get();
        system(CLEAR_SCREEN);
   }while(n!=ID_SALIR);
    salir();
}
void login(){
    bool acceso=false;
    string user, password;
    do{
        cout<<"\n"<<SPACES<<"***************************************************************"<<endl;
        cout<<SPACES<<"***************************************************************\n\n\n"<<endl;
        cout<<SPACES<<"------------------------- ProyectoSQL --------------------------\n"<<endl;
        cout<<SPACES<<"------------------------- Version 1.0.0 -----------------------\n"<<endl;
        cout<<SPACES<<"------------------------- Powered by USO ------------------\n\n\n\n"<<endl;
        cout<<SPACES<<"***************************************************************"<<endl;
        cout<<SPACES<<"***************************************************************\n"<<endl;
        cout<<SPACES<<"Usuario: "<<endl;
        cout<<SPACES;
        getline(cin, user);
        cout<<"\n"<<SPACES<<"Contraseña: "<<endl;
        cout<<SPACES;
        getline(cin, password);
        if(user=="" || password=="")
            exit(EXIT_COMMAND);
        if(usuario->find_usr(user, password)){
            ///Credenciales correctas

            acceso=true;
            ///Importantisimo almacenar el usuario en una variable de sesion... para poder hacer validaciones posteriores....
            usuario->setUsuario(user);
        }
        else{
            cout<<"\n\n"<<SPACES<<"*********** Error: usuario o contraseña incorrecta! ***********\n\n"<<endl;
            cin.get();
        }
        cout<<SPACES;
        system(CLEAR_SCREEN);
    }while(!acceso);
}

void salir(){
        cout<<"\n"<<SPACES<<"***************************************************************"<<endl;
        cout<<SPACES<<"***************************************************************\n\n\n"<<endl;
        cout<<SPACES<<"------------------------- ProyectoSQL --------------------------\n"<<endl;
        cout<<SPACES<<"------------------------- Version 1.0.0 -----------------------\n"<<endl;
        cout<<SPACES<<"------------------------- Powered by USO ------------------\n\n\n\n"<<endl;
        cout<<SPACES<<"***************************************************************"<<endl;
        cout<<SPACES<<"***************************************************************\n\n"<<endl;
        cout<<SPACES<<"... Saliendo"<<endl;
}
