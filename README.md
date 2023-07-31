# LPS25HB I2C

## importer librery

```C
/* USER CODE BEGIN Includes */
#include  "LPS25HB.h"
#include  <math.h>
```
## init capteur en i2c 
crée variable 
```C
/* USER CODE BEGIN 1 */
LPS25HB baro;
float alt=0.0f; // pour res altitude
```

verifer si capteur bien sur i2c
```C
while (testWhoAmI(&hi2c1))
{
    //while car peut ne par marcher la premier fois
}
```

init le barometre avec une addresse i2c (ex : hi2c1)
```C
if(LPS25HB_Initialise(&baro, &hi2c1 ) == 255)
{
    Error_Handler();
}
```

## recuperation donner 

```C
LPS25HB_ReadTemperature(&baro);
LPS25HB_ReadPressure(&baro);
alt=LPS25HB_FindAltitude(&baro);
```