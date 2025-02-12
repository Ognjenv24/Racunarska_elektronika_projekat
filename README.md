# Projekat: Kontrola Klime korišćenjem DS18B20 senzora i Flask servera

## Predmet: Računarska elektronika

**Mentor:** dr Mezei Ivan  
**Autor:** Ognjen Višnjić, EE 217/2020  
**Datum:** Septembar 2024.  

---

## Sadržaj

1. [Uvod](#uvod)
2. [Cilj projekta](#cilj-projekta)
3. [Povezivanje DS18B20 senzora sa Raspberry Pi](#povezivanje-ds18b20-senzora-sa-raspberry-pi)
4. [Softversko rešenje](#softversko-rešenje)
5. [Zaključak](#zaključak)

---

## Uvod

U savremenom svetu tehnologije, temperaturni senzori igraju ključnu ulogu u razvoju automatizovanih sistema za kontrolu klime i praćenje uslova okoline. **DS18B20** je digitalni temperaturni senzor sa visokom preciznošću i jednostavnom integracijom, pogodan za razne aplikacije od pametnih kućnih sistema do industrijskih kontrola temperature.

U ovom projektu implementiran je **sistem za kontrolu klime** koji koristi **DS18B20 senzor** za merenje temperature i **Flask server** za upravljanje podacima i interakciju sa korisnikom. Sistem omogućava prikaz trenutne temperature na **veb interfejsu** i LCD ekranu, uz mogućnost automatskog upravljanja klimom na osnovu izmerenih vrednosti.

---

## Cilj projekta

Cilj projekta je:
- **Analiza rada DS18B20 senzora** i njegove tehničke karakteristike.
- **Povezivanje senzora sa Raspberry Pi** i konfiguracija OneWire komunikacije.
- **Implementacija sistema za kontrolu klime** pomoću Flask servera.
- **Prikaz temperaturnih podataka** na veb interfejsu i LCD ekranu.
- **Automatska regulacija klime** na osnovu izmerenih vrednosti.

---

## Povezivanje DS18B20 senzora sa Raspberry Pi

**DS18B20 senzor** ima tri pina koji se povezuju na Raspberry Pi na sledeći način:

| Senzor Pin | Raspberry Pi Pin |
|------------|-----------------|
| **GND**    | GND (GPIO 5)    |
| **VCC**    | 3.3V (GPIO 1)   |
| **DATA**   | GPIO 4 (pin 7)  |

> *Potrebno je koristiti otpornik od **4.7kΩ** između VCC i DATA pina za ispravan rad OneWire protokola.*

Za prikaz rezultata merenja temperature koristi se **LCD displej** na kojem se očitane vrednosti prikazuju u realnom vremenu.

---

## Softversko rešenje

Sistem je razvijen pomoću **C programskog jezika** i **Flask servera**.

### Implementacija
- Očitavanje temperature iz **DS18B20 senzora** koristeći OneWire protokol.
- Slanje podataka ka **Flask serveru**.
- Prikaz trenutne temperature na **veb stranici**.
- Pokretanje web servera na Raspberry Pi-u.

#### Pokretanje sistema
1. Kompajliranje i pokretanje **C programa** za očitavanje temperature:
   ```sh
   gcc -o temperature_reader temperature_reader.c -lwiringPi
   ./temperature_reader
   ```
2. Pokretanje Flask servera:
   ```sh
   python3 app.py
   ```
3. Pristup web interfejsu putem browsera:
   ```
   http://<IP_RASPBERRY_PI>:5000
   ```

---

## Zaključak

Rezultati eksperimenta pokazuju da je **DS18B20 senzor pouzdan alat** za precizno merenje temperature, dok je njegova integracija sa Raspberry Pi omogućila jednostavno očitavanje podataka i njihovu dalju obradu putem Flask servera.

### Ključni nalazi:
- **Efikasnost senzora**: Stabilni i precizni temperaturni podaci.
- **Jednostavna integracija sa Raspberry Pi** putem OneWire protokola.
- **Praktične primene**: Pogodno za pametne kućne sisteme, industrijske aplikacije i IoT primene.
- **Potencijal za dalji razvoj**: Mogućnost dodavanja dodatnih senzora, poboljšanja korisničkog interfejsa i IoT integracije.

Projekat demonstrira funkcionalan sistem koji omogućava **praćenje i automatsku kontrolu temperature**, pružajući osnovu za dalja poboljšanja i primene u raznim okruženjima.

