#ifndef H_DECOMPRESS_H
#define H_DECOMPRESS_H

#ifdef __cplusplus
extern "C" {
#endif
char*	decompress(const char *c_data, const unsigned int start, unsigned int* uncompressed_data_size);

#ifdef __cplusplus
}
#endif
#endif
