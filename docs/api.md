# API‑dokumentation – Smart Indoor Environment

## Översikt  
API:t tillgängliggör senaste mottagna temperaturvärde från bearbetningstjänsten.  
Kommunikationen sker över HTTPS och kräver API‑nyckel för åtkomst.

---

# 1. Bas‑URL  
API:t körs lokalt i utvecklingsmiljö:

```
https://localhost:8443/api/v1
```

Porten kan ändras i konfigurationen.

---

# 2. Autentisering  
API‑nyckel krävs för alla anrop.

### Header
```
X-API-Key: <din-nyckel>
```

Felaktig eller saknad nyckel ger HTTP 401 Unauthorized.

---

# 3. Endpoint: Hämta senaste temperaturvärde

### **GET /temperature**

Returnerar senaste giltiga temperaturmätning som bearbetningstjänsten tagit emot via MQTT.

### Exempelanrop
```
GET https://localhost:8443/api/v1/temperature
X-API-Key: my-secret-key
```

### Exempelsvar (HTTP 200)
```
{
  "sensorId": "indoor-temp-01",
  "timestamp": "2026-09-17T10:30:00+02:00",
  "value": 21.7,
  "unit": "C"
}
```

---

# 4. Felkoder

| Kod | Betydelse | Beskrivning |
|-----|-----------|-------------|
| 200 | OK | Mätvärde returneras |
| 401 | Unauthorized | API‑nyckel saknas eller är felaktig |
| 404 | Not Found | Inget mätvärde har mottagits ännu |
| 500 | Internal Server Error | Bearbetningstjänsten kunde inte hämta data |

---

# 5. Felhantering  
API:t hanterar fel enligt följande:

- Felaktig API‑nyckel → 401  
- Inget mätvärde mottaget → 404  
- Fel i bearbetningstjänsten → 500  
- Ogiltig JSON från MQTT loggas och ignoreras (påverkar inte API:t)

---

# 6. Exempel på klientanrop (curl)

```
curl -X GET "https://localhost:8443/api/v1/temperature" \
     -H "X-API-Key: my-secret-key" \
     -k
```

Flaggan `-k` används endast i utvecklingsmiljö för att acceptera självsignerade certifikat.