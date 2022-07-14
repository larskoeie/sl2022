# SL2022

## Sådan bruges apparatetet
- Apparatet skal forsynes med 12V DC på den brune og blå ledning. Polariteten er ligegyldig.
- Når begge telefonrør er løftet, kan der tales igennem telefonerne.
- Når begge telefonrør er løftet, vises der på displayet, hvilke telefonnumre, der drejes på telefonerne. 

## Hardware
De to tilsluttede telefoner sidder i serie med spændingsforsyningen og med en lille modstand. Spændingen over denne modstand kører ind i en optokobler. Udgangen fra optokobleren kører ind i en digital indgang på en ESP8266. Desuden findes et display, som er tisluttet samme MCU, og en 12V-USB konverter som forsyner MCU'en.
Når begge telefonrør er løftet, løber en lille strøm igennem dem. Denne strøm giver en lille spænding over seriemodstanden. Spændingen tænder LEDen i optokobleren, 

## Software
- Når spænindge
