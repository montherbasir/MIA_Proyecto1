#ifndef DISCO_H
#define DISCO_H



#ifdef __cplusplus

#include <iostream>
#include <ctime>

struct block{
    int inicio;
    int size;
    char t;
    char name[20];
};
struct mPart {
    char disk_p[250];
    char id[10];
    char nombre[16];
};

struct mDisk {
    char disk_p[250];
    char id;
    int count;
};
void reporteParticiones(const char* path, const char* id);
std::string parentDir(const std::string& fullPath);
inline void crearRaid(std::string path1);
inline void leerMbr(const char* path1);
inline bool exists(const std::string& name);
void crearPartPrimaria(const char* path, int size, const char* unit, const char* fit, const char* name, const char* tipo);
void crearPartLogica(const char* path, int size, const char* unit, const char* fit, const char* name, int inicioEbr, int finEbr);
int estaMontadoDisk(const char* path);
bool existeParticion(const char* path, const char *name);
std::string diskPathParticion(const char* id);
void reporteDisco(const char* pRep, const char* id);
inline bool ordenInicio(const block b1, const block b2);
void crearParticion(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name);
void eliminarParticion(const char* del_, const char* name, const char* path);
block getParticion(const char* path, const char *name);
    struct partition{
        char part_status;
        char part_type;
        char part_fit;
        int part_start;
        int part_size;
        char part_name[16];
        bool empty;
    };

    struct ebr{
        char part_status;
        char part_fit;
        int part_start;
        int part_size;
        int part_next;
        char part_name[16];
    };

    struct mbr {
      int mbr_tamano;
      time_t mbr_fecha_creacion;
      int mbr_disk_signature;
      char disk_fit;
      partition mbr_partition[4];
    };





extern "C"
{
#endif



extern void testf(int n);
extern void crearDisco(int size, const char* fit, const char* unit, const char* path);
extern void eliminarDisco(const char* path);
extern void addSizeparticion();
extern void mountParticion(const char* path, const char* name);
extern void unmountParticion(const char* id);
extern void fdisk_(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name, const char* del, int add);
extern void reportes(const char* name, const char* path, const char* id);

#ifdef __cplusplus
}
#endif




#endif // DISCO_H
