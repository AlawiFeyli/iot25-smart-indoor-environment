# Arkitektur – Smart Indoor Environment

## Översikt  
Systemet består av en fysisk IoT‑enhet (ESP32‑C6) med temperatursensor, en MQTT‑broker, en bearbetningstjänst som konsumerar och validerar data, samt ett REST‑API som tillgängliggör senaste mätvärdet. Kommunikationsvägarna är säkrade med TLS och API‑nyckel.

## Komponenter

### 1. ESP32‑C6  
- Läser temperatur från fysisk sensor  
- Skapar JSON‑payload  
- Publicerar data periodiskt via MQTT  
- Hanterar återanslutning vid nätverksfel  
- Använder CA‑certifikat för att verifiera broker

### 2. MQTT‑broker (Mosquitto)  
- Tar emot telemetri från ESP32  
- Körs lokalt eller på server  
- TLS‑aktiverad  
- Exponerar topic för temperaturdata

### 3. Bearbetningstjänst (Python)  
- Prenumererar på MQTT‑topic  
- Validerar JSON‑data  
- Loggar inkommande värden  
- Uppdaterar övervakningsmått  
- Tillgängliggör data via REST‑API

### 4. REST‑API  
- Exponerar senaste mätvärde  
- Kräver API‑nyckel  
- Körs över HTTPS  
- Används av klienter eller andra system

---

## Kommunikationsflöde

### 1. ESP32 → MQTT  
- Protokoll: MQTTS  
- Port: 8883  
- Topic: `iot25/indoor/temperature`  
- Modell: Publish  
- JSON‑format enligt datakontrakt  
- Återanslutning: exponential backoff + automatisk reconnect

### 2. MQTT → Bearbetningstjänst  
- Modell: Subscribe  
- Tjänsten validerar JSON  
- Felaktig JSON loggas och ignoreras  
- Godkänd data lagras i minne och uppdaterar övervakning

### 3. Bearbetningstjänst → API‑klient  
- Protokoll: HTTPS  
- Port: 443 eller 8443 (beroende på konfiguration)  
- Endpoint: `/api/v1/temperature`  
- Behörighet: API‑nyckel i header  
- Felhantering: 401 vid fel nyckel, 500 vid interna fel

---

## Dataformat (JSON‑kontrakt)

```
{
  "sensorId": "indoor-temp-01",
  "timestamp": "2026-09-17T10:30:00+02:00",
  "value": 21.7,
  "unit": "C"
}
```

### Regler  
- `sensorId`: sträng  
- `timestamp`: ISO‑8601  
- `value`: numeriskt värde  
- `unit`: C eller F  
- Alla fält måste finnas

---

## Återanslutning och felhantering

### ESP32  
- Automatisk reconnect till Wi‑Fi  
- Automatisk reconnect till MQTT  
- Loggar anslutningsstatus  
- Publicerar “offline”‑status vid längre avbrott

### Bearbetningstjänst  
- Hanterar broker‑timeout  
- Loggar felaktig JSON  
- Räknar antal fel  
- Fortsätter prenumerera vid återanslutning

---

## Arkitekturdiagram (textbaserat)

```
ESP32-C6 (temperatursensor)
        |
        |  MQTTS (TLS/8883)
        v
MQTT-broker (Mosquitto)
        |
        |  Subscribe
        v
Bearbetningstjänst (Python)
        |
        |  HTTPS + API-nyckel
        v
REST-API / Klient
```
