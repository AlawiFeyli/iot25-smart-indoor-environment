# Testprotokoll – Smart Indoor Environment

Detta dokument beskriver genomförda end‑to‑end‑tester för IoT‑lösningen.  
Testerna verifierar dataflödet från ESP32‑enheten till API‑tjänsten, inklusive säkerhet, felhantering och övervakning.

---

# 1. Testmiljö

| Komponent | Version / Info |
|----------|----------------|
| ESP32‑C6 | ESP‑IDF 5.x |
| MQTT‑broker | Mosquitto med TLS (port 8883) |
| Bearbetningstjänst | Python 3.x |
| API‑server | HTTPS (port 8443) |
| Verktyg | mosquitto_sub, curl, seriell monitor |

---

# 2. Testfall

## **Testfall 1 – ESP32 publicerar giltig JSON till MQTT**

### Syfte  
Verifiera att ESP32 skickar korrekt JSON‑data och att bearbetningstjänsten tar emot den.

### Steg  
1. Starta MQTT‑broker  
2. Starta bearbetningstjänsten  
3. Starta ESP32‑enheten  
4. Kontrollera loggar

### Förväntat resultat  
- JSON‑payload tas emot  
- Övervakningsräknaren ökar  
- API:t returnerar senaste värdet

### Utfört resultat  
```
Received MQTT message: {"sensorId":"indoor-temp-01", "value":21.7, "unit":"C"}
```

**Status:** Godkänt

---

## **Testfall 2 – API‑anrop med korrekt API‑nyckel**

### Syfte  
Verifiera att API:t returnerar senaste mätvärde vid korrekt autentisering.

### Steg  
1. Kör curl‑anrop med giltig nyckel  
2. Kontrollera svar

### Förväntat resultat  
HTTP 200 + JSON

### Utfört resultat  
```
HTTP 200 OK
{
  "sensorId": "indoor-temp-01",
  "value": 21.7,
  "unit": "C"
}
```

**Status:** Godkänt

---

## **Testfall 3 – API‑anrop med felaktig API‑nyckel**

### Syfte  
Verifiera att obehöriga klienter nekas åtkomst.

### Steg  
1. Kör curl‑anrop med fel nyckel  
2. Kontrollera svar

### Förväntat resultat  
HTTP 401 Unauthorized

### Utfört resultat  
```
HTTP 401 {"error":"unauthorized"}
```

**Status:** Godkänt

---

## **Testfall 4 – Felaktigt MQTT‑topic**

### Syfte  
Verifiera att systemet hanterar fel topic korrekt.

### Steg  
1. Ändra ESP32‑topic till fel värde  
2. Starta systemet  
3. Kontrollera loggar

### Förväntat resultat  
- Inga meddelanden tas emot  
- Övervakningsräknaren står kvar på 0  
- API returnerar 404

### Utfört resultat  
Bearbetningstjänsten tog inte emot några meddelanden.

**Status:** Godkänt

---

## **Testfall 5 – TLS‑verifiering**

### Syfte  
Verifiera att ESP32 endast ansluter till broker med korrekt CA.

### Steg  
1. Konfigurera ESP32 med fel CA  
2. Starta enheten  
3. Kontrollera loggar

### Förväntat resultat  
TLS‑handshake misslyckas

### Utfört resultat  
```
TLS_VERIFY_FAILED certificate authority is not trusted
```

**Status:** Godkänt

---

# 3. Sammanfattning

Samtliga testfall har genomförts och uppfyller förväntade resultat.  
Systemet hanterar:

- Giltig kommunikation  
- Felaktig autentisering  
- Fel topic  
- TLS‑verifiering  
- API‑åtkomstkontroll  