#include "parsing.h"


//
// split a comma-separated string to up to siz tokens
// return token count
//
int split( char *str, char **toks, int siz) {
  char *p = str;
  int nt = 0;
  
  while( *p) {
    toks[nt++] = p;
    while( *p && *p != ',')		/* skip over non-delims */
      ++p;
    // check if delim or end of string
    if( *p) {  // delim
      // check for trailing delim
      if( !p[1])
	toks[nt++] = p+1;
      *p = '\0';
      ++p;
    }
  }
  return nt;
}

// convert NEMA lat/long to decimal degrees
// format is:  dddmm.ffff
//    ddd is integer degrees (1-3 digits)
//     mm is integer minutes
//   ffff is fractional minutes
float conv_ll( char *str) {
  float minu;
  float degr;
  char *dp = strchr( str, '.');  // point to decimal
  if( dp == NULL)
    return( 0.0);
  dp -= 2;              // point to minutes
  minu = atof(dp);      // convert floating minutes
  *dp = '\0';           // terminate degrees
  degr = atof( str);    // convert float degrees
  degr += minu / 60.0;  // add minutes
  return degr;          // return result
}


