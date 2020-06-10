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
#include <unistd.h>

extern "C" {
#include "gram.tab.h"
}

int signature = 0;
QVector<mPart>* montadas = new QVector<mPart>();
QVector<mDisk>* discosMont = new QVector<mDisk>();
int letraDisk = 97;
int contRep =0;
int main () {
    func();
    std::string p = "/home/Disco4.disk";
//    eliminarDisco(p.c_str());
//    std::string s1="/home/monther/prueee.disk";
//    std::string s2="/home/monther/nuevo.disk";
//    std::string s3="/home/monther/reportes/reporte2/disco.jpg";
//    std::string name="Part_";
//    std::string name1="Part_1";
//    std::string name2="Part_2";
//    std::string name3="Part_3";
//    std::string name4="Part_4";
//    std::string name5="Part_5";
//    char  f = 'b';
//    char  t = 'p';
//    char  u = 'b';
//    char  u1 = 'k';
//    char t1 = 'e';
//    char t2 = 'l';
//    std::string id = "vda1";
//    std::string id2 = "vdb4";

//    crearDisco(5,&f,&u1,s1.c_str());
//    crearParticion(s1.c_str(),1,&u1,&t,&f,name.c_str());
//    crearParticion(s1.c_str(),2,&u1,&t1,&f,name1.c_str());
//    crearParticion(s1.c_str(),350,&u,&t2,&f,name2.c_str());
//    crearParticion(s1.c_str(),1,&u,&t2,&f,name1.c_str());
//        crearDisco(5,&f,&u1,s2.c_str());
//        crearParticion(s2.c_str(),1,&u1,&t,&f,name.c_str());
    //    crearParticion(s2.c_str(),3,&u1,&t1,&f,name1.c_str());
    //crearParticion(s2.c_str(),350,&u,&t,&f,name2.c_str());


//    mountParticion(s1.c_str(),name.c_str());
//        mountParticion(s2.c_str(),name.c_str());
    //    mountParticion(s1.c_str(),name2.c_str());
//    mountParticion(s1.c_str(),name1.c_str());
    //    mountParticion(s2.c_str(),name1.c_str());
    //    mountParticion(s2.c_str(),name2.c_str());
    //    mountParticion(s2.c_str(),name4.c_str());

    //reporteParticiones(s2.c_str(),id.c_str());
//    reporteDisco(s3.c_str(),id.c_str());
    //reporteDisco(s2.c_str(),id2.c_str());

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
    int s=1024;
    if(un=='M'){
        s = s*1024;
    }else if(un=='K'){

    }else{
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
    std::string chmod = "sudo chmod 777 -R ";
    chmod += parentDir(path);
    system(chmod.c_str());

    char* buffer = (char*)malloc(s);
    memset(buffer, '0',s);

    FILE* f = fopen(path1.c_str(),"wb");
    for(int i=0;i<size;i++){
        fwrite(buffer,s,1,f);
    }

    char name[16] = " ";
    mbr new_mbr;

    new_mbr.mbr_tamano = size*s;
    new_mbr.mbr_fecha_creacion = creacion;
    new_mbr.mbr_disk_signature = signature++;
    new_mbr.disk_fit = fit_;

    for(int x=0; x<4; x++){
        new_mbr.mbr_partition[x].empty = true;
        new_mbr.mbr_partition[x].part_fit = ' ';
        strcpy(new_mbr.mbr_partition[x].part_name, name);
        new_mbr.mbr_partition[x].part_size = 0;
        new_mbr.mbr_partition[x].part_type = ' ';
        new_mbr.mbr_partition[x].part_start = 0;
        new_mbr.mbr_partition[x].part_status = 1;
    }


    fclose(f);

    FILE* f1 = fopen(path1.c_str(),"r+b");
    fseek ( f1 , 0 , SEEK_SET );
    fwrite(&new_mbr,sizeof(struct mbr),1,f1);
    fclose(f1);

    std::cout<<"Disco creado: "<<size<<" "<<fit<<" "<<unit<<" "<<path<<std::endl;
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

inline void leerMbr(const char* path1){
    mbr pmbr;
    FILE* f2 = fopen(path1,"r+b");
    fseek(f2,0,SEEK_SET);
    fread(&pmbr,sizeof (struct mbr),1,f2);

    std::cout<<"tamano "<<pmbr.mbr_tamano<<std::endl;
    std::cout<<"creacion "<<ctime(&pmbr.mbr_fecha_creacion);
    std::cout<<"fit "<<pmbr.disk_fit<<std::endl;
    for(partition part : pmbr.mbr_partition){
        std::cout<<"name "<<part.part_name <<std::endl;
        std::cout<<"empty "<<part.empty <<std::endl;
        std::cout<<"type "<<part.part_type <<std::endl;
        std::cout<<"fit "<<part.part_fit <<std::endl;
        std::cout<<"start "<<part.part_start <<std::endl;
        std::cout<<"size "<<part.part_size <<std::endl;
    }
    fclose(f2);
}

void eliminarDisco(const char* path){
    std::cout<<"Desea eliminar el disco: "<<path<<"? (y/n)"<<std::endl;
    char c = ' ';
    std::cin>>c;
    if(c=='y'||c=='Y'){
        if( remove(path) != 0 ){
            std::cout<< "Error eliminando disco" <<std::endl;
        }else{
            std::cout<< "Disco eliminado" <<std::endl;
        }
}

}

void fdisk_(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name, const char* del, int add){
    //std::cout<<type<<" "<<size<<" "<<fit<<" "<<unit<<" "<<path<<" "<<name<<" "<<del<<" "<<add<<std::endl;
    if(strcmp(del,"-1")!=0){
        //Eliminar particion
        eliminarParticion(del,name,path);
    }else if(add!=0){
        //Agregar espacio
    }else{
        crearParticion(path,size,unit,type,fit,name);
    }
}

void crearParticion(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name){
    char t = tolower(*type);
    if(exists(path)==0){
        std::cout << "Error: no existe el disco" <<std::endl;
        return;
    }
    if(existeParticion(path,name)){
        std::cout << "Error: ya existe la particion" <<std::endl;
        return;
    }
    if(t=='p'){
        crearPartPrimaria(path,size,unit,fit,name,&t);
        std::cout<<"Particion creada: "<<size<<" "<<fit<<" "<<unit<<" "<<path<<" "<<name<<" "<<type<<std::endl;
        crearRaid(path);
    }else if(t=='e'){
        mbr pmbr;
        FILE* f2 = fopen(path,"r+b");
        fseek(f2,0,SEEK_SET);
        fread(&pmbr,sizeof (struct mbr),1,f2);
        fclose(f2);

        for(partition p : pmbr.mbr_partition){
            if(p.part_type=='E'){
                std::cout << "Error: ya existe una particion extendida" <<std::endl;
                return;
            }
        }



        crearPartPrimaria(path,size,unit,fit,name,&t);
                std::cout<<"Particion creada: "<<size<<" "<<fit<<" "<<unit<<" "<<path<<" "<<name<<" "<<type<<std::endl;
        crearRaid(path);

    }else if(t=='l'){
        int inicio = 0;
        int fin = 0;
        mbr pmbr;
        FILE* f2 = fopen(path,"r+b");
        fseek(f2,0,SEEK_SET);
        fread(&pmbr,sizeof (struct mbr),1,f2);
        fclose(f2);
        bool ext = false;
        for(partition p : pmbr.mbr_partition){
            if(p.part_type=='E'){
                ext = true;
                inicio = p.part_start;
                fin = inicio+p.part_size;
                break;
            }
        }

        if(ext){
            crearPartLogica(path,size,unit,fit,name,inicio,fin);
                    std::cout<<"Particion creada: "<<size<<" "<<fit<<" "<<unit<<" "<<path<<" "<<name<<" "<<type<<std::endl;
            crearRaid(path);
        }else{
            std::cout << "Error: no existe una particion extnedida" <<std::endl;
        }
    }else{
        std::cout << "Error: tipo de particion descconocido" <<std::endl;
    }
}

void crearPartPrimaria(const char* path, int size, const char* unit, const char* fit, const char* name, const char* tipo){

    //Copia el contenido de la mbr
    mbr pmbr;
    FILE* f2 = fopen(path,"r+b");
    fseek(f2,0,SEEK_SET);
    fread(&pmbr,sizeof(struct mbr),1,f2);


    char un = toupper(*unit);
    int s_u=1;
    if(un=='M'){
        s_u = 1024*1024;
    }else if(un=='K'){
        s_u = 1024;
    }else if(un=='B'){
        s_u=1;
    }else{
        std::cout<<"Error: unit no identificado"<<std::endl;
        return;
    }

    bool cabe = 0;
    int index=0;
    int t=0;

    char vacio[1025];
    memset(vacio,'0',1024);
    vacio[1024] = '\0';

    //Itera todo el disco

    for(int i=sizeof (mbr);i<pmbr.mbr_tamano;i+=1024){
        fseek(f2, i,SEEK_SET);
        char bloque[1025];
        size_t readS = fread(bloque,1,1024,f2);
        fflush(f2);
        bloque[1024] = '\0';

        if(strcmp(bloque,vacio)==0){
            //Si se leyo el arreglo completo
            if(readS==(size_t)1024){
                if(t==0){
                    index = i;
                }
                t += 1024;

            }else{
                if(t==0){
                    index = i;
                }
                t += (int)readS;
            }
            //Cuando el espacio libre sea el mismo que el de la particion para (frst fit)
            if(t>=size*s_u){
                cabe = 1;
                break;
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
        for(int x=0; x<4; x++){
            partition *part = &pmbr.mbr_partition[x];
            if(part->empty==true){
                part->empty = false;
                strcpy(part->part_name, name);
                part->part_status = '1';
                part->part_type = toupper(*tipo);
                part->part_fit = toupper(*fit);
                part->part_start = index;
                part->part_size = size*s_u;
                espacio = true;
                break;
            }
        }

        if(espacio){

            //se escribe 1 para indicar que el espacio esta ocupado por una particion
            char vacio_1 = '1';
            FILE* f3 = fopen(path,"r+b");
            fseek(f3,0,SEEK_SET);
            fwrite(&pmbr,sizeof(struct mbr),1,f3);
            if(size*s_u<1024)
                size=1024;


            for(int i=0; i<(size*s_u/1024);i++){
                fseek(f3,index+(i*1024),SEEK_SET);
                fwrite(&vacio_1,1,1,f3);
            }

            if(*tipo=='e'){
                ebr pebr;
                pebr.part_fit = ' ';
                pebr.part_name[0] = ' ';
                pebr.part_next = -1;
                pebr.part_size = 0;
                pebr.part_start = index;
                pebr.part_status = '0';
                fseek(f2,index,SEEK_SET);
                fwrite(&pebr,sizeof (struct ebr),1,f3);
            }

            fclose(f3);
        }else{
            std::cout<<"Error ya se crearon 4 particiones"<<std::endl;
        }


    }else{
        std::cout<<"Error no cabe la particion"<<std::endl;
    }

    //leerMbr(path);

}

void crearPartLogica(const char* path, int size, const char* unit, const char* fit, const char* name, int inicioEbr, int finEbr){

    //Copia el contenido de la mbr
    ebr pebr;
    FILE* f2 = fopen(path,"r+b");
    fseek(f2,inicioEbr,SEEK_SET);
    fread(&pebr,sizeof(ebr),1,f2);


    char un = toupper(*unit);
    int s_u=1;
    if(un=='M'){
        s_u = 1024*1024;
    }else if(un=='K'){
        s_u = 1024;
    }else if(un=='B'){
        s_u=1;
    }else{
        std::cout<<"Error: unit no identificado"<<std::endl;
        return;
    }

    bool cabe = 0;
    int index=inicioEbr;
    int t=0;

    char vacio[1025];
    memset(vacio,'0',1024);
    vacio[1024] = '\0';
    vacio[0] = '1';
    //Itera todo el disco

    //    std::cout<<pebr.part_status<<std::endl;
    //    if(pebr.part_status=='0'){
    //            t = 1024;
    //    }
    for(int i=inicioEbr;i<finEbr;i+=1024){
        fseek(f2, i,SEEK_SET);
        char bloque[1025];
        size_t readS = fread(bloque,1,1024,f2);
        fflush(f2);
        bloque[1024] = '\0';

        if(strcmp(bloque,vacio)==0){
            //Si se leyo el arreglo completo
            if(readS==(size_t)1024){
                if(t==0){
                    index = i;
                }
                t += 1024;
                //Cuando el espacio libre sea el mismo que el de la particion para (frst fit)
                if(t>=size*s_u){
                    cabe = 1;
                    break;
                }
            }
        } else {
            t = 0;
            if(pebr.part_status=='0'&&i==inicioEbr){
                t += 1024;
            }
        }
    }
    fclose(f2);

    //Si se encontro un lugar para la particion
    if(cabe==1){

        int size_new = size;
        char vacio_1 = '2';
        FILE* f3 = fopen(path,"r+b");
        if(size*s_u<1024)
            size=1024;


        for(int i=0; i<(size*s_u/1024);i++){
            fseek(f3,index+(i*1024),SEEK_SET);
            fwrite(&vacio_1,1,1,f3);
        }
        fclose(f3);

        //se actualizan los ebr
        ebr *pebr2;
        ebr pebr3 = pebr;
        FILE* f4 = fopen(path,"r+b");
        while(pebr3.part_next!=-1){
            pebr2 = &pebr3;
            fseek(f4,pebr2->part_next,SEEK_SET);
            fread(&pebr3,sizeof(struct ebr),1,f4);
        }

        ebr *pebr4 = &pebr3;
        //si es la primera particion
        if(pebr4->part_status=='0'){
            pebr4->part_fit = toupper(*fit);
            strcpy(pebr4->part_name,name);
            pebr4->part_next = -1;
            pebr4->part_size = size_new*s_u;
            pebr4->part_status = '1';
            fseek(f4,inicioEbr,SEEK_SET);
            fwrite(&pebr3,sizeof (ebr),1,f4);
        }else{
            pebr4->part_next = index;
            fseek(f4,pebr4->part_start,SEEK_SET);
            fwrite(&pebr3,sizeof (ebr),1,f4);
            ebr pebr5;
            pebr5.part_fit = toupper(*fit);
            strcpy(pebr5.part_name,name);
            pebr5.part_next = -1;
            pebr5.part_size = size_new*s_u;
            pebr5.part_start = index;
            pebr5.part_status = '1';
            fseek(f4,index,SEEK_SET);
            fwrite(&pebr5,sizeof (ebr),1,f4);
        }


        fclose(f4);

    }else{
        std::cout<<"Error no cabe la particion"<<std::endl;
    }


}

void reportes(const char* name, const char* path, const char* id){
    std::cout<<name<<" "<<path<<" "<<id<<std::endl;
    std::string sl = name;
    transform(sl.begin(), sl.end(), sl.begin(), ::tolower);
    if(strcmp(sl.c_str(),"mbr")==0){
        reporteParticiones(path,id);
    }else if(strcmp(sl.c_str(),"disk")==0){
        reporteDisco(path,id);
    }else{
        std::cout<<"Error reporte no identificado"<<std::endl;
    }
}

void reporteParticiones(const char* pRep, const char* id){

    std::string pa = diskPathParticion(id);

    std::string mk="mkdir -p ";
    std::string p = mk+parentDir(pRep);

    const int dir_err = system(p.c_str());
    if (-1 == dir_err)
    {
        printf("Error creando directorio");
    }
    std::string chmod = "sudo chmod 777 -R ";
    chmod += parentDir(pRep);
    system(chmod.c_str());

    std::string ext = pRep;
    const size_t lastIndex = ext.find_last_of(".");
    ext = ext.substr(lastIndex+1,ext.size());

    const char *path = pa.c_str();
    std::string graph = "digraph {\n splines=\"line\";\n rankdir = TB;\n node [shape=plain, fontsize=25];\n graph[dpi=90];\n\n N [label=<\n";
    int primer_ebr = -1;
    std::string fpath = path;
    const size_t lastSlashIndex = fpath.find_last_of("/\\");
    std::string s = fpath.substr(lastSlashIndex+1);

    mbr pmbr;
    FILE* f2 = fopen(path,"r+b");
    fseek(f2,0,SEEK_SET);
    fread(&pmbr,sizeof (struct mbr),1,f2);
    fclose(f2);

    std::string g = " <table border=\"0\" cellborder=\"1\" cellpadding=\"8\">\n";

    g += " \t<tr><td colspan=\"2\"><b>MBR ";
    g += s;
    g += "</b></td></tr>\n";

    g += " \t<tr><td><b>Nombre</b></td><td><b>Valor</b></td></tr>\n";

    g += " \t<tr><td align=\"left\">";
    g += "<b>mbr_tamano</b>";
    g += "</td><td>";
    g += std::to_string(pmbr.mbr_tamano);
    g += "</td></tr>\n";

    g += " \t<tr><td align=\"left\">";
    g += "<b>mbr_fecha_creacion</b>";
    g += "</td><td>";
    std::string mystr = ctime(&pmbr.mbr_fecha_creacion);
    g += mystr.substr(0, mystr.size()-1);
    g += "</td></tr>\n";

    g += " \t<tr><td align=\"left\">";
    g += "<b>mbr_disk_signature</b>";
    g += "</td><td>";
    g += std::to_string(pmbr.mbr_disk_signature);
    g += "</td></tr>\n";

    g += " \t<tr><td align=\"left\">";
    g += "<b>disk_fit</b>";
    g += "</td><td>";
    g += pmbr.disk_fit;
    g += "</td></tr>\n";

    int i = 1;
    for(partition p : pmbr.mbr_partition){
        if(p.part_type=='E'){
            primer_ebr = p.part_start;
        }
        if(!p.empty){
            g += " \t<tr><td align=\"left\">";
            g += "<b>part_status_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(!p.empty);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_type_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_type;
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_fit_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_fit;
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_start_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_start);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_size_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_size);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_name_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_name;
            g += "</td></tr>\n";
        }
        i++;
    }

    i=1;
    if(primer_ebr!=-1){
        g += " \t<tr><td colspan=\"2\"><b>EBR</b></td></tr>\n";
        ebr pebr;
        FILE* f4 = fopen(path,"r+b");
        fseek(f4,primer_ebr,SEEK_SET);
        fread(&pebr,sizeof(ebr),1,f4);

        ebr *pebr2;
        ebr p = pebr;

        while(p.part_next!=-1){

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_status_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_status;
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_fit_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_fit;
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_start_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_start);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_size_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_size);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_next_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_next);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_name_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_name;
            g += "</td></tr>\n";

            pebr2 = &p;
            fseek(f4,pebr2->part_next,SEEK_SET);
            fread(&p,sizeof(struct ebr),1,f4);
            i++;
        }
        fclose(f4);

        if(p.part_status=='1'){
            g += " \t<tr><td align=\"left\">";
            g += "<b>part_status_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_status;
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_fit_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_fit;
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_start_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_start);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_size_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_size);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_next_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += std::to_string(p.part_next);
            g += "</td></tr>\n";

            g += " \t<tr><td align=\"left\">";
            g += "<b>part_name_";
            g += std::to_string(i);
            g += "</b></td><td>";
            g += p.part_name;
            g += "</td></tr>\n";
        }
    }



    graph += g;
    graph += " </table>\n >];\n}";

    std::ofstream myfile;
    std::string myf = "mbr";
    myf += std::to_string(contRep++);
    myf += ".dot";
    myfile.open(myf);
    myfile << graph;
    myfile.close();
    std::string sys = "dot -T";
    sys += ext;
    sys += " ";
    sys += myf;
    sys += " -o ";
    sys += pRep;
    system(sys.c_str());
    sleep(1);
}

void reporteDisco(const char* pRep, const char* id){

    std::string path = diskPathParticion(id);
    std::string mk="mkdir -p ";
    std::string p = mk+parentDir(pRep);

    const int dir_err = system(p.c_str());
    if (-1 == dir_err)
    {
        printf("Error creando directorio");
    }
    std::string chmod = "sudo chmod 777 -R ";
    chmod += parentDir(pRep);
    system(chmod.c_str());

    std::string ext = pRep;
    const size_t lastIndex = ext.find_last_of(".");
    ext = ext.substr(lastIndex+1,ext.size());
    if(path!=""){
        std::string graph = "digraph {\n splines=\"line\";\n rankdir = TB;\n node [shape=plain, fontsize=25];\n graph[dpi=90];\n\n N [label=<\n";
        int primer_ebr = -1;
        std::string fpath = path;
        const size_t lastSlashIndex = fpath.find_last_of("/\\");
        std::string s = fpath.substr(lastSlashIndex+1);

        mbr pmbr;
        FILE* f2 = fopen(path.c_str(),"r+b");
        fseek(f2,0,SEEK_SET);
        fread(&pmbr,sizeof (struct mbr),1,f2);


        std::string g = " <table border=\"0\" cellborder=\"1\" cellpadding=\"8\">\n";

        int index=0;
        int t=0;



        QVector<block>* espacios = new QVector<block>();
        char vacio[1025];
        memset(vacio,'0',1024);
        vacio[1024] = '\0';


        //Algoritmo para buscar espacios libres

        //Itera todo el disco
        for(int i=sizeof (mbr);i<pmbr.mbr_tamano;i+=1024){
            //std::cout<<i<<std::endl;
            fseek(f2, i,SEEK_SET);
            char bloque[1025];
            size_t readS = fread(bloque,1,1024,f2);
            fflush(f2);
            bloque[1024] = '\0';
            //std::cout<<bloque<<std::endl;
            if(strcmp(bloque,vacio)==0){
                //Si se leyo el arreglo completo
                if(readS==(size_t)1024){

                    if(t==0){
                        index = i;
                    }
                    t += 1024;
                }else{
                    if(t==0){
                        index = i;
                    }
                    t += (int)readS;
                }
            } else {
                if(t==0){

                }else{
                    block nuevo;
                    nuevo.size = t;
                    nuevo.inicio = index;
                    nuevo.t = 'e';
                    espacios->append(nuevo);
                    t = 0;
                }

            }
        }

        fclose(f2);

        //agrega el ultimo espacio vacio a la lista
        if(t>0){
            block nuevo;
            nuevo.size = t;
            nuevo.inicio = index;
            nuevo.t = 'e';
            espacios->append(nuevo);
            t = 0;
        }


        //agrega las particiones a la lista de bloques
        for(partition p : pmbr.mbr_partition){
            if(!p.empty){
                block b;
                b.t = 'p';
                if(p.part_type=='E'){
                    b.t = 'x';
                    primer_ebr = p.part_start;
                }
                b.size = p.part_size;
                b.inicio = p.part_start;
                strcpy(b.name,p.part_name);
                espacios->append(b);
            }
        }


        g += " \t<tr><td colspan=\"";
        g += std::to_string(espacios->size()+1);
        g += "\"><b>";
        g += s;
        g += "</b></td></tr>\n";

        g += " \t<tr>";
        g += "<td><b>MBR</b></td>";

        //ordena la lista de bloques por su punto de inicio ascendentemente
        std::sort(espacios->begin(), espacios->end(), ordenInicio);

        for(block e: *espacios){

            std::string n = e.name;
            g += "<td>";
            if(e.t=='e'){
                g += "<b>Libre</b><br></br>";
                float percent = ((float)e.size/(float)pmbr.mbr_tamano)*100;

                g += std::to_string(percent);
                g += "%";
            }else if(e.t=='p'){
                g += "<b>";
                g += e.name;
                g += "</b><br></br>";
                float percent = ((float)e.size/(float)pmbr.mbr_tamano)*100;

                g += std::to_string(percent);
                g += "%";
            }else if(e.t=='x'){
                int ocupado = 0;
                std::string g1;
                if(primer_ebr!=-1){
                    g1 += " \t\t<tr>";
                    ebr pebr;
                    FILE* f4 = fopen(path.c_str(),"r+b");
                    fseek(f4,primer_ebr,SEEK_SET);
                    fread(&pebr,sizeof(ebr),1,f4);

                    ebr *pebr2;
                    ebr p = pebr;

                    int col = 1;

                    while(p.part_next!=-1){
                        g1 += "<td><b>EBR</b></td>";
                        g1 += "<td><b>";
                        g1 += p.part_name;
                        g1 += "</b><br></br>";
                        float percent = ((float)p.part_size/(float)pmbr.mbr_tamano)*100;
                        g1 += std::to_string(percent);
                        g1 += "%</td>";
                        col++;
                        ocupado += p.part_size;
                        pebr2 = &p;
                        fseek(f4,pebr2->part_next,SEEK_SET);
                        fread(&p,sizeof(struct ebr),1,f4);
                    }

                    if(p.part_status=='1'){
                        g1 += "<td><b>EBR</b></td>";
                        g1 += "<td><b>";
                        g1 += p.part_name;
                        g1 += "</b><br></br>";
                        float percent = ((float)p.part_size/(float)pmbr.mbr_tamano)*100;

                        g1 += std::to_string(percent);
                        g1 += "%</td>";
                        ocupado += p.part_size;

                    }else{

                        g1 += "<td><b>EBR</b></td>";
                    }

                    if(ocupado<e.size){
                        g1 += "<td><b>Libre</b><br></br>";
                        g1 += std::to_string(((float)(e.size-ocupado)/(float)pmbr.mbr_tamano)*100);
                        g1 += "%</td>";
                        ocupado = 1;
                    }else{
                        ocupado = 0;
                    }

                    g1 += "</tr>\n";


                    g += " \n\t<table border=\"0\" cellborder=\"1\" cellpadding=\"8\">\n";
                    g += " \t\t<tr><td colspan=\"";
                    g += std::to_string(col*2+ocupado);
                    g += "\"><b>Extendida";
                    g += "</b></td></tr>\n";
                    g += g1;
                    g += " \t</table>\n";
                }
            }
            g += "</td>";
        }
            g += " </tr>\n";

            graph += g;
            graph += " </table>\n >];\n}";


            std::ofstream myfile;
            std::string myf = "disk";
            myf += std::to_string(contRep++);
            myf += ".dot";
            myfile.open(myf);
            myfile << graph;
            myfile.close();
            std::string sys = "dot -T";
            sys += ext;
            sys += " ";
            sys += myf;
            sys += " -o ";
            sys += pRep;
            system(sys.c_str());
            sleep(1);

    }
}

inline bool ordenInicio(const block b1, const block b2)
{
    return b1.inicio < b2.inicio;
}

void eliminarParticion(const char* del_, const char* name, const char* path){
    std::string sl = del_;
    transform(sl.begin(), sl.end(), sl.begin(), ::tolower);
    if(strcmp(sl.c_str(),"full")!=0&&strcmp(sl.c_str(),"fast")!=0){
        std::cout<<"Error metodo de eliminacion incorrecto"<<std::endl;
        return;
    }
    block part = getParticion(path,name);

    if(part.t=='0'){
        std::cout<<"Error no existe la particion"<<std::endl;
    }else if(part.t=='p'){
        FILE* f3 = fopen(path,"r+b");
        int size = part.size;
        if(size<1024){
            size = 1024;
        }
        char vacio[1024];
        memset(vacio,'0',1024);
        for(int i=0; i<(size/1024);i++){

            fseek(f3,part.inicio+(i*1024),SEEK_SET);
            fwrite(&vacio,1024,1,f3);

        }
        fclose(f3);
        std::cout<<"Particion eliminada con el metodo "<<del_<<std::endl;
        crearRaid(path);
    }else if(part.t=='l'){
        std::cout<<"Esta version no soporta la eliminacion de particiones logicas :("<<std::endl;
    }
}

void addSizeparticion(){

}

void mountParticion(const char* path, const char* name){
    std::cout<<path<<" "<<name<<std::endl;

    if(exists(path)==0){
        std::cout << "Error: no existe el disco" <<std::endl;
        return;
    }
    if(!existeParticion(path,name)){
        std::cout << "Error: no existe la particion" <<std::endl;
        return;
    }
    std::string ruta = path;
    std::string id = "vd";
    int newd = estaMontadoDisk(path);
    if(newd==-1){
        if(discosMont->size()==0)
            letraDisk = 97;
        mDisk md;
        strcpy(md.disk_p,path);
        md.id = (char)letraDisk++;
        id += md.id;
        id += "1";
        md.count = 2;
        discosMont->append(md);
    }else{
        mDisk *mdk = &(*(discosMont))[newd];
        id += mdk->id;
        id += std::to_string(mdk->count);
        mdk->count = mdk->count+1;
    }

    for(mPart p : *montadas){
        std::string r = p.disk_p;
        if(ruta==r){
            if(strcmp(p.nombre,name)==0){
                std::cout<<"Error ya esta montada la particion"<<std::endl;
                return;
            }
        }
    }

    mPart mp;
    strcpy(mp.id,id.c_str());
    strcpy(mp.disk_p,path);
    strcpy(mp.nombre,name);
    montadas->append(mp);
    std::cout<<"Particion montada con id: "<<mp.id<<std::endl;
}

int estaMontadoDisk(const char* path){

    std::string ruta = path;
    int i=0;
    for(mDisk d : *discosMont){
        std::string discP = d.disk_p;
        if(discP==ruta){
            return i;
        }
        i++;
    }
    return -1;
}

void unmountParticion(const char* id){
    std::string strid = id;
    for(int i=0;i<montadas->size();i++){
        mPart mp = montadas->at(i);
        std::string mpid = mp.id;
        if(mpid==strid){
            montadas->remove(i);
            std::cout<<"Particion desmontada con id: "<<mpid<<std::endl;
            if(montadas->size()==0){
                discosMont->clear();
            }
            return;
        }
    }

    std::cout<<"Error la particion no esta montada"<<std::endl;
}

std::string diskPathParticion(const char* id){
    std::string strid = id;
    for(int i=0;i<montadas->size();i++){
        mPart mp = montadas->at(i);
        std::string mpid = mp.id;
        if(mpid==strid){
            return mp.disk_p;
        }
    }

    std::cout<<"Error la particion no esta montada"<<std::endl;
    return "";
}

bool existeParticion(const char* path, const char *name){
    if(exists(path)==0){
        return false;
    }
    int primer_ebr = -1;
    mbr pmbr;
    FILE* f2 = fopen(path,"r+b");
    fseek(f2,0,SEEK_SET);
    fread(&pmbr,sizeof (struct mbr),1,f2);
    fclose(f2);
    for(partition p : pmbr.mbr_partition){
        if(p.part_type=='E'){
            primer_ebr = p.part_start;
        }
        if(strcmp(p.part_name,name)==0){
            return true;
        }
    }

    if(primer_ebr!=-1){
        ebr pebr;
        FILE* f4 = fopen(path,"r+b");
        fseek(f4,primer_ebr,SEEK_SET);
        fread(&pebr,sizeof(ebr),1,f4);

        ebr *pebr2;
        ebr p = pebr;

        while(p.part_next!=-1){
            if(strcmp(p.part_name,name)==0){
                fclose(f4);
                return true;
            }
            pebr2 = &p;
            fseek(f4,pebr2->part_next,SEEK_SET);
            fread(&p,sizeof(struct ebr),1,f4);
        }
        if(strcmp(p.part_name,name)==0){
            fclose(f4);
            return true;
        }

        fclose(f4);
    }
    return false;
}

block getParticion(const char* path, const char *name){
    block bl;
    bl.t = '0';
    if(exists(path)==0){

        return bl;
    }
    int primer_ebr = -1;
    mbr pmbr;
    FILE* f2 = fopen(path,"r+b");
    fseek(f2,0,SEEK_SET);
    fread(&pmbr,sizeof (struct mbr),1,f2);
    fclose(f2);
    partition *delP;
    int x =0;
    for(partition p : pmbr.mbr_partition){
        if(p.part_type=='E'){
            primer_ebr = p.part_start;
        }
        if(strcmp(p.part_name,name)==0){
            bl.t = 'p';
            bl.size = p.part_size;
            bl.inicio = p.part_start;
            FILE* f3 = fopen(path,"r+b");
            fseek(f3,0,SEEK_SET);
            delP = &pmbr.mbr_partition[x];
            delP->empty = true;
            delP->part_fit = ' ';
            char nnnn[20] = " ";
            strcpy(delP->part_name, nnnn);
            delP->part_size = 0;
            delP->part_type = ' ';
            delP->part_start = 0;
            delP->part_status = 1;
            fwrite(&pmbr,sizeof (struct mbr),1,f3);
            fclose(f3);
            return bl;
        }
        x++;
    }

    if(primer_ebr!=-1){
        ebr pebr;
        FILE* f4 = fopen(path,"r+b");
        fseek(f4,primer_ebr,SEEK_SET);
        fread(&pebr,sizeof(ebr),1,f4);

        ebr *pebr2;
        ebr p = pebr;

        while(p.part_next!=-1){
            if(strcmp(p.part_name,name)==0){
                fclose(f4);
                bl.t = 'l';
                bl.inicio = p.part_start;
                bl.size = p.part_size;
                return bl;
            }
            pebr2 = &p;
            fseek(f4,pebr2->part_next,SEEK_SET);
            fread(&p,sizeof(struct ebr),1,f4);
        }
        if(strcmp(p.part_name,name)==0){
            fclose(f4);
            bl.t = 'l';
            bl.inicio = p.part_start;
            bl.size = p.part_size;
            return bl;
        }

        fclose(f4);
    }
    return bl;
}
