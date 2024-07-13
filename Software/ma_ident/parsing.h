#ifndef _PARSING_H_INCLUDED
#define _PARSING_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

// split a comma-separated string to up to siz tokens
// return token count                                                                            
int split( char *str, char **toks, int siz);

// convert NEMA lat/long to decimal degrees
// format is:  dddmm.ffff
//    ddd is integer degrees (1-3 digits)
//     mm is integer minutes
//   ffff is fractional minutes
float conv_ll( char *str);

char * _float_to_char(float x, char *p, int siz);


#endif
