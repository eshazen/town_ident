# ma_ident

Initial software to identify nearest town in MA

GPS powers up and starts sending a blast of stuff at 1Hz.
All we care about is the `$GPGLL` record, which has 10 fields
separated by commas.

| No | Name    | Format      | Example     | Description            |
|----|---------|-------------|-------------|------------------------|
| 0  | xxGLL   | string      | $GPGLL      | GLL message ID         |
| 1  | lat     | ddmm.mmmmm  | 4717.11364  | Latitude, deg and min  |
| 2  | NS      | char        | N           | N/S indicator          |
| 3  | lon     | dddmm.mmmmm | 04333.91565 | Longitude, deg and min |
| 4  | EW      | char        | W           | E/W indicator          |
| 5  | time    | hhmmss.ss   | 092321.00   | UTC time               |
| 6  | status  | char        | A           | Data valid status      |
| 7  | posMode | char        | A           | Positioning mode       |
| 8  | cs      | hex         | *60         | checksum               |
| 9  | eol     | char        | CR,LF       | CR and LF terminator   |

### Latitude and Longitude format

Received format is `ddmm.mmmmm` for latitude and `dddmm.mmmmm` for
longitude.  `dd` or `ddd` is integer degrees.  `mm.mmmmm` is
floating point minutes.

The code checks the fields 2 and 4 are 'N' and 'W' respectively.
(this has to be fixed for the southern or eastern hemispheres!)

### Database

Data is obtained from the US Census
[link](https://www.census.gov/cgi-bin/geo/shapefiles/index.php?year=2023&layergroup=Places).

Select a state, get a zip file.  In the zip is a dBase III file, e.g.
`tl_2023_25_place.dbf`.  Create a text dump using unix `dbview`.
Convert to C source using `place_to_c.pl` in the `Tools` folder.
This creates a database which will be stored in flash memory on the
AVR.




