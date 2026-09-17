# Smart Indoor Environment – IoT25  
En säker och integrerad IoT‑lösning för insamling, bearbetning och tillgängliggörande av sensordata.

## Syfte  
Systemet samlar in temperaturdata från en ESP32‑C6 med fysisk sensor, överför informationen via MQTT, bearbetar den i en serverapplikation och tillgängliggör den genom ett REST‑API. Lösningen är designad för att vara säker, övervakningsbar och robust mot vanliga kommunikationsfel.

## Funktioner  
- Insamling av fysiskt mätvärde från ESP32‑C6  
- Periodisk publicering av JSON‑data via MQTT  
- Bearbetning och validering av inkommande data  
- REST‑API för åtkomst till senaste mätvärde  
- TLS‑skyddad kommunikation  
- API‑nyckelbaserad åtkomstkontroll  
- Loggning av anslutningar, dataflöde och fel  
- Övervakning av antal mottagna meddelanden och senaste värde  
- Dokumenterad felsökning av två avsiktliga fel

## Installation  
### 1. Klona projektet  
```
git clone https://github.com/<user>/iot25-smart-indoor-environment.git
cd iot25-smart-indoor-environment
```

### 2. Installera beroenden  
Bearbetningstjänsten använder Python 3:

```
pip install -r requirements.txt
```

ESP32‑koden byggs med ESP‑IDF.

### 3. Konfiguration  
Skapa en `.env`‑fil baserat på `src/config/example.env`:

- MQTT_HOST  
- MQTT_PORT  
- API_KEY  
- TLS‑certifikatvägar  


## Starta systemet  
### 1. Starta MQTT‑broker  
Mosquitto används som broker.

### 2. Starta bearbetningstjänsten  
```
python src/processing-service/app.py
```

### 3. Starta ESP32‑enheten  
Flash:a firmware och anslut enheten till nätverket.

## Verifiera dataflödet  
1. ESP32 publicerar JSON‑data till MQTT‑topic  
2. Bearbetningstjänsten tar emot och loggar värdet  
3. API‑klienten kan hämta senaste värdet via:  
```
GET /api/v1/temperature
```

## Kända begränsningar  
- Demo‑certifikat används i utvecklingsmiljö  
- Ingen databaslagring i denna version  
- Endast en fysisk sensor används  
- Felhantering  

# Projektstruktur

```
iot25-smart-indoor-environment/
│
├── README.md
│
├── src/
│   ├── esp32/
│   │   └── main.cpp
│   ├── processing-service/
│   │   ├── app.py
│   │   ├── mqtt_handler.py
│   │   ├── api.py
│   │   └── monitor.py
│   └── config/
│       └── example.env
│
├── docs/
│   ├── arkitektur.md
│   ├── api.md
│   ├── sakerhet.md
│   └── felsokning.md
│
└── tests/
    └── testprotokoll.md
```
