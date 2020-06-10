#include "union.h"
#include <stdio.h>
#include "disco.h"
#include <stdlib.h>
void prueba(){

}

void crDk(char* size, char* fit, char* unit, char* path){
    crearDisco(atoi(size),fit,unit,path);
}

void rmDk(char* path){
    eliminarDisco(path);
};

void fDk(char* path, char* size, char* unit, char* type, char* fit, char* name, char* del, char* add){
    fdisk_(path,atoi(size),unit,type,fit,name,del,atoi(add));
};


void mtPt(char* path, char* name){
    mountParticion(path,name);
};

void umtPt(char* id){
    unmountParticion(id);
};

void rpt(char* name, char* path, char* id){
    reportes(name,path,id);
}


