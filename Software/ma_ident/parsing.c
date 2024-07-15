#include "parsing.h"


//
// split a comma-separated (or other delim) string to up to siz tokens
// return token count
//
// NOTE:  overwrites string by terminating tokens with '\0'
#
#define SPLIT_DELIM ','

int split( char *str, char **toks, int siz) {
  char *p = str;		/* point to start of string */
  int nt = 0;			/* token counter */
  
  while( *p && nt < siz) {  /* loop to string end or over siz */
    toks[nt++] = p;	    /* always point to token here */
    while( *p && *p != SPLIT_DELIM)	/* skip over non-delims */
      ++p;
    // check if delim or end of string
    if( *p) {  // non-null char must be delim
      // check for empty token
      if( !p[1])	    /* is this the end of the string */
	toks[nt++] = p+1;   /* yes, add pointer to enpty token */
      *p = '\0';	    /* and terminate it */
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


