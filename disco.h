#ifndef DISCO_H
#define DISCO_H



#ifdef __cplusplus

#include <iostream>
#include <ctime>
void reporteParticiones(const char* path);
std::string parentDir(const std::string& fullPath);
inline void crearRaid(std::string path1);
inline void leerMbr(const char* path1);
inline bool exists(const std::string& name);
void crearPartPrimaria(const char* path, int size, const char* unit, const char* fit, const char* name, const char* tipo);
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
extern void crearParticion(const char* path, int size, const char* unit, const char* type, const char* fit, const char* name);
extern void eliminarParticion();
extern void addSizeparticion();
extern void mountParticion();
extern void unmountParticion();



#ifdef __cplusplus
}
#endif



#endif // DISCO_H
