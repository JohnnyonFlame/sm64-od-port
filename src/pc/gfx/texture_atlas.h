#ifndef __TEXTURE_ATLAS_H__
#define __TEXTURE_ATLAS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct Atlas Atlas;

    extern int atlas_create(Atlas **atlas_dptr, uint16_t dimensions, uint16_t padding);
    extern void atlas_destroy(Atlas *atlas);
    extern int atlas_gen_texture(Atlas *atlas, uint32_t *id_ptr);
    extern int atlas_destroy_vtex(Atlas *atlas, uint32_t id);
    extern int atlas_allocate_vtex_space(Atlas *atlas, uint32_t id, uint16_t w, uint16_t h);
    extern int atlas_get_vtex_uvst_coords(Atlas *atlas, uint32_t id, int padding, float *uvst);
    extern int atlas_get_vtex_xywh_coords(Atlas *atlas, uint32_t id, int padding, uint16_t *xywh);
    extern uint16_t atlas_get_dimensions(Atlas *atlas);
    extern uint16_t atlas_get_padding(Atlas *atlas);
#ifdef __cplusplus
}
#endif

#endif /* __TEXTURE_ATLAS_H__ */