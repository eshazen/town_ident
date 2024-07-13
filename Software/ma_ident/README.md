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

The best way to determine that there is valid position data is just to
observe that fields 1-4 have non-zero length contents.



