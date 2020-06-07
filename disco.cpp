#include "disco.h"
#include <libgen.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iostream>
#include "disco.h"
#include <sys/stat.h>
#include <QVector>

extern "C" {
#include "gram.tab.h"
}

int signature =0;

int main () {
    //func();
    std::string s1="/home/monther/prueee.disk";
    std::string name="Part_p";
    char  f = 'b';
    char  t = 'p';
    char  u = 'k';
    char  u1 = 'b';
    //crearDisco(5,&f,&u,s1.c_str());
    crearParticion(s1.c_str(),1,&u,&t,&f,name.c_str());

    //eliminarDisco(s1.c_str());
    return 0;
}

void crearDisco(int size, const char* fit, const char* unit, const char* path){
    if(size<=0){
        std::cout<<"Error: size es menor o igual a 0"<<std::endl;
        return;
    }
    char fit_ = toupper(*fit);
    if(fit_!='B'&&fit_!='W'&&fit_!='F'){
        std::cout<<"Error: fit no identificado"<<std::endl;
        return;
    }
    char un = toupper(*unit);
    if(un!='M'&&un!='K'){
        std::cout<<"Error: unit no identificado"<<std::endl;
        return;
    }
    std::string path1 = path;
    time_t creacion = time(&creacion);
    std::string mk="mkdir -p ";
    std::string p = mk+parentDir(path);

    const int dir_err = system(p.c_str());
    if (-1 == dir_err)
    {
        printf("Error creando directorio");
    }

    int s=1024;

    if(un=='M'){
        s = s*1024;
    }
    char* buffer = (char*)malloc(s);
    memset(buffer, '0',s);

    FILE* f = fopen(path1.c_str(),"wb");
    for(int i=0;i<size;i++){
        fwrite(buffer,s,1,f);
    }

    mbr new_mbr =  {
        .mbr_tamano = size*s,
        .mbr_fecha_creacion = creacion,
        .mbr_disk_signature = signature++,
        .disk_fit = fit_,
        .mbr_partition ={
            {.empty = true},
            {.empty = true},
            {.empty = true},
            {.empty = true}
        }
    };

    fclose(f);

    FILE* f1 = fopen(path1.c_str(),"r+b");
    fseek ( f1 , 0 , SEEK_SET );
    fwrite(&new_mbr,sizeof(mbr),1,f1);
    fclose(f1);

    crearRaid(path1);
}

inline void crearRaid(std::string path1){
    std::ifstream  src(path1, std::ios::binary);
    std::string path2 = path1.substr(0, path1.size()-5);
    std::ofstream  dst(path2+"_RAID.disk",   std::ios::binary);
    dst << src.rdbuf();
}

inline std::string parentDir(const std::string& fullPath)
{
    const size_t lastSlashIndex = fullPath.find_last_of("/\\");
    std::string s = fullPath.substr(0,lastSlashIndex);
    return "\""+s+"\"";
}

inline bool exists(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

inline void leerMbr(char* path1){
    mbr pmbr;
    FILE* f2 = fopen(path1,"rb");
    fread(&pmbr,sizeof (mbr),1,f2);
    std::cout<<pmbr.mbr_tamano<<std::endl;
    std::cout<<ctime(&pmbr.mbr_fecha_creacion);
    std::cout<<pmbr.disk_fit<<std::endl;
    fclose(f2);
}

inline void eliminarDisco(const char* path){
    if( remove(path) != 0 ){
        std::cout<< "Error eliminando disco" <<std::endl;
    }else{
        std::cout<< "Disco eliminado" <<std::endl;
    }
}

//void fdisk(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name){

//}

void crearParticion(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name){
    char t = tolower(*type);
    if(exists(path)==0){
        std::cout << "Error: no existe el disco" <<std::endl;
        return;
    }
    if(t=='p'){
        crearPartPrimaria(path,size,unit,fit,name);
        crearRaid(path);
    }else if(t=='e'){

    }else if(t=='l'){

    }else{

    }
}

void crearPartPrimaria(const char* path, int size, const char* unit, const char* fit, const char* name){

    //Copia el contenido de la mbr
    mbr pmbr;
    FILE* f2 = fopen(path,"r+b");
    fseek(f2,0,SEEK_SET);
    fread(&pmbr,sizeof(mbr),1,f2);

    int s_u=1;
    char u = tolower(*unit);
    if(u=='k'){
        s_u = 1024;
    }else if(u=='m'){
        s_u = 1024*1024;
    }

    bool cabe = 0;
    int index=0;
    int t=0;

    char vacio[2];
    vacio[0] = '0';
    vacio[1] = '\0';

    //Itera todo el disco

    for(int i=sizeof (mbr);i<pmbr.mbr_tamano;i++){
        std::cout<<i<<std::endl;
        fseek(f2, i,SEEK_SET);
        char bloque[2];
        size_t readS = fread(bloque,1,1,f2);
        fflush(f2);
        bloque[1] = '\0';

        if(strcmp(bloque,vacio)==0){
            //Si se leyo el arreglo completo
            if(readS==(size_t)1){

                if(t==0){
                    index = i;
                }
                t ++;

                //Cuando el espacio libre sea el mismo que el de la particion para (frst fit)
                if(t==size*s_u){
                    cabe = 1;
                    break;
                }
            }
        } else {
            t = 0;
        }
    }
    fclose(f2);


    //Si se encontro un lugar para la particion
    if(cabe==1){

        bool espacio = false;
        //se actualiza el mbr
        for(partition part : pmbr.mbr_partition){
            if(part.empty==true){
                part.empty = false;
                strcpy(part.part_name, name);
                part.part_status = '1';
                part.part_type = 'P';
                part.part_fit = *fit;
                part.part_start = index;
                part.part_size = size;
                espacio = true;
                break;
            }
        }

        if(espacio){
            //se escribe 1 para indicar que el espacio esta ocupado por una particion
            char vacio_1[s_u];
            memset(vacio_1, '1', s_u);
            FILE* f3 = fopen(path,"r+b");
            std::cout<<size<<std::endl;
            for(int i=0; i<(size);i++){
                fseek(f3,index+(s_u*i),SEEK_SET);
                fwrite(vacio_1,s_u,1,f3);
            }
            fseek(f3,0,SEEK_SET);
            fwrite(&pmbr,sizeof (mbr),1,f3);
            fclose(f3);
        }else{
            std::cout<<"Error ya se crearon 4 particiones"<<std::endl;
        }


    }else{
        std::cout<<"Error no cabe la particion"<<std::endl;
    }



}

void eliminarParticion(){

}

void addSizeparticion(){

}

void mountParticion(){

}

void unmountParticion(){

}

