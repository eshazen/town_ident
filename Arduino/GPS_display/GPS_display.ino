 
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("GPS Test");
  Serial.begin(9600);
}

static char buff[80];
static char *tokn[4];
#define NTOK (sizeof(tokn)/sizeof(tokn[0]))

static float lat_deg;
static float lon_deg;

//                                                                                                
// split a comma-separated string to up to siz tokens                                             
// return token count                                                                             
//                                                                                                
int split( char *str, char **toks, int siz) {
  char *p = strtok( str, ",");
  int nt = 0;

  if( !p) return 0;

  toks[nt++] = p;
  while( nt < siz) {
    p = strtok( NULL, ",");
    if( !p)
      return( nt);
    else
      toks[nt++] = p;
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

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  // lcd.print(millis() / 1000);
  Serial.readBytesUntil( '\n', buff, sizeof(buff));
  if( !strncmp( buff, "$GPGLL", 6)) {
    // see if we have lat/long
    int n = split( buff+6, tokn, NTOK);
    if( n == 4 && *tokn[1] == 'N' && *tokn[3] == 'W') {
      // convert lat/lon
      lat_deg = conv_ll( tokn[0]);
      lon_deg = -conv_ll( tokn[2]);
      lcd.clear();
      lcd.setCursor( 0, 0);
      lcd.print(lat_deg, 6);
      lcd.setCursor( 0, 1);
      lcd.print(lon_deg, 6);
    }
  }
}

