#ifndef H_TILE_H
#define H_TILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
    char		data[64];
    unsigned int	palette_id;
} tile8;

tile8	unpack_bpp4_tile(const char* data, const unsigned int offset);
tile8	unpack_bpp3_tile(const char* data, const unsigned int offset);
tile8	unpack_bpp2_tile(const char* data, const unsigned int offset);

tile8	unpack_bpp_tile(const char* data, const unsigned int offset, const unsigned int bpp);

#ifdef __cplusplus
}
#endif

#endif
