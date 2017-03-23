#ifndef H_PALETTE_H
#define H_PALETTE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uchar;

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} m_color;

typedef struct {
  unsigned int	id;
  unsigned int	size;
  m_color*	colors;
} s_palette;


s_palette*	palette_create(const unsigned int size, const unsigned int id);

s_palette*	extract_palette(const char* data, const unsigned int offset, const unsigned int palette_size);

m_color		convertcolor_snes_to_rgb(const unsigned short snes_color);
unsigned short 	convertcolor_rgp_to_snes(const m_color color);
unsigned short 	convertcolor_rgp_to_snes2(const uchar red, const uchar green, const uchar blue);

#ifdef __cplusplus
}
#endif

#endif
