# SL2022

## Sådan bruges apparatet
- Apparatet skal forsynes med 12V DC på den brune og blå ledning. Polariteten er ligegyldig.
- Når begge telefonrør er løftet, kan der tales igennem telefonerne.
- Når begge telefonrør er løftet, vises der på displayet, hvilke telefonnumre, der drejes på telefonerne. 
- Ved hvert registrerede ciffer, blinker både MCUens indbygggede LED og LCD'ens backlight, en enkelt gang.

## Hardware
De to tilsluttede telefoner sidder i serie med spændingsforsyningen og med en lille modstand. Spændingen over denne modstand kører ind i en optokobler. Udgangen fra optokobleren kører ind i en digital indgang på en ESP8266. Desuden findes et display, som er tisluttet samme MCU, og en 12V-USB konverter som forsyner MCU'en.
Når begge telefonrør er løftet, løber en lille strøm igennem dem. Denne strøm giver en lille spænding over seriemodstanden. Spændingen tænder LEDen i optokobleren, 

## Software
Når spændingen på indgangen falder, opfattes det som en puls. Så længe spædningen er lav mellem 5 og 150ms og høj mellem 12 og 30ms, tælles pulserne. Hvis der går længere tid, opfattes det som et nyt ciffer. Hvis der går kortere tid, opfattes det som støj på linjen. Denne støj ignoreres. 
