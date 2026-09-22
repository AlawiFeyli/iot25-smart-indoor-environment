# Felsökning – Smart Indoor Environment

Detta dokument beskriver två avsiktligt införda fel i systemet, hur de upptäcktes, analyserades och åtgärdades.  
Felsökningen omfattar både MQTT‑kommunikation och API‑åtkomst.

---

# 1. Fel 1 – Fel MQTT‑topic

## 1.1 Observerat symptom  
Bearbetningstjänsten tog inte emot några mätvärden trots att ESP32 publicerade data.  
Övervakningsmåttet “antal mottagna meddelanden” stod kvar på 0.

## 1.2 Identifiering  
Loggar från bearbetningstjänsten visade inga inkommande MQTT‑meddelanden.  
ESP32‑loggar visade att publicering skedde som vanligt.

## 1.3 Verktyg och loggar  
- `mosquitto_sub` användes för att lyssna på rätt topic  
- ESP32‑seriell monitor  
- Bearbetningstjänstens loggar

## 1.4 Orsak  
ESP32 publicerade till topic:

```
iot25/indoor/temp
```

Bearbetningstjänsten prenumererade på:

```
iot25/indoor/temperature
```

Skillnaden i topic‑namn gjorde att inga meddelanden nådde konsumenten.

## 1.5 Åtgärd  
Topic ändrades i ESP32‑koden till:

```
iot25/indoor/temperature
```

Bearbetningstjänsten startades om.

## 1.6 Verifiering  
Efter ändringen visade loggar:

```
Received MQTT message: {"sensorId":"indoor-temp-01", ...}
```

Övervakningsmåttet ökade från 0 till 1.  
API:t returnerade ett giltigt mätvärde.

---

# 2. Fel 2 – Felaktig API‑nyckel

## 2.1 Observerat symptom  
Klientanrop mot API:t returnerade:

```
HTTP 401 Unauthorized
```

Trots att bearbetningstjänsten hade ett giltigt mätvärde.

## 2.2 Identifiering  
Loggar från API‑tjänsten visade:

```
Invalid API key provided
```

## 2.3 Verktyg och loggar  
- curl‑anrop mot API  
- Loggar från `api.py`  
- Kontroll av `.env`‑fil

## 2.4 Orsak  
Klienten använde en gammal API‑nyckel som inte matchade den som låg i `.env`.

## 2.5 Åtgärd  
Klienten uppdaterades till korrekt nyckel:

```
X-API-Key: my-secret-key
```

## 2.6 Verifiering  
Nytt anrop gav:

```
HTTP 200 OK
{
  "sensorId": "indoor-temp-01",
  "value": 21.7,
  "unit": "C"
}
```

API‑tjänsten loggade:

```
Authorized request
```

---

# 3. Sammanfattning  
Felsökningen visar två vanliga fel i IoT‑system:

1. Fel topic → ingen data når konsumenten  
2. Fel API‑nyckel → klienten nekas åtkomst  

Båda felen identifierades genom loggar och verktyg som `mosquitto_sub` och curl.  
Åtgärderna verifierades genom att dataflödet åter fungerade och API:t svarade korrekt.