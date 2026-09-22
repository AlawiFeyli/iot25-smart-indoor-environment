# Säkerhetsanalys – Smart Indoor Environment

## Översikt  
Detta dokument beskriver säkerhetsrisker, genomförda säkerhetsåtgärder och kvarvarande begränsningar i IoT‑lösningen. Fokus ligger på transportskydd, autentisering, behörighetskontroll och hantering av känslig konfiguration.

---

# 1. Identifierade säkerhetsrisker

## 1.1 Avlyssning av nätverkstrafik  
Temperaturdata skickas över nätverket och kan potentiellt avlyssnas om kommunikationen inte är krypterad.

## 1.2 Falsk server eller MITM‑attack  
En angripare kan presentera en falsk MQTT‑broker eller API‑server för att fånga upp eller manipulera data.

## 1.3 Obehörig åtkomst till API  
Utan behörighetskontroll kan externa klienter läsa sensordata eller överbelasta tjänsten.

## 1.4 Läckta autentiseringsuppgifter  
API‑nyckel, Wi‑Fi‑lösenord eller CA‑certifikat kan läcka om de lagras felaktigt.

## 1.5 Felaktig eller manipulerad JSON  
Ogiltig eller skadlig data kan orsaka fel i bearbetningstjänsten.

---

# 2. Genomförda säkerhetsåtgärder

## 2.1 TLS för transportskydd  
All kommunikation mellan ESP32 och MQTT‑broker sker över MQTTS (TLS/8883).  
API‑kommunikation sker över HTTPS (TLS/8443).  
Klienten verifierar serverns certifikat med CA‑fil.

**Effekt:**  
Skyddar mot avlyssning och MITM‑attacker.

---

## 2.2 Certifikatverifiering  
ESP32 använder ett inbyggt CA‑certifikat för att verifiera brokerns identitet.  
Bearbetningstjänsten verifierar API‑serverns certifikat.

**Effekt:**  
Förhindrar anslutning till falska servrar.

---

## 2.3 API‑nyckelbaserad behörighetskontroll  
Alla API‑anrop kräver en giltig API‑nyckel i headern:

```
X-API-Key: <nyckel>
```

Felaktig nyckel ger HTTP 401.

**Effekt:**  
Skyddar API:t mot obehörig åtkomst.

---

## 2.4 Säker hantering av hemligheter  
Känsliga uppgifter lagras inte i Git.  
En `.env`‑fil används för API‑nyckel, certifikatvägar och MQTT‑konfiguration.  
ESP32 använder inbyggd konfiguration som inte checkas in.

**Effekt:**  
Minskar risken för läckage av lösenord och nycklar.

---

## 2.5 Validering av inkommande JSON  
Bearbetningstjänsten kontrollerar att inkommande data följer datakontraktet.  
Ogiltig JSON loggas och ignoreras.

**Effekt:**  
Skyddar mot felaktig eller manipulerad data.

---

# 3. Kvarvarande begränsningar

## 3.1 Självsignerade certifikat i utvecklingsmiljö  
Systemet använder demo‑certifikat.  
I produktion krävs certifikat från betrodd CA.

## 3.2 Ingen databaslagring  
Senaste mätvärde lagras i minne.  
Vid omstart förloras historik.

## 3.3 Endast en fysisk sensor  
Systemet är utbyggbart men har begränsad datamängd i nuvarande version.

## 3.4 Ingen rate‑limiting i API  
API:t saknar skydd mot överbelastning.

---

# 4. Hantering av känslig konfiguration

## 4.1 Wi‑Fi SSID och lösenord  
Lagring sker i ESP32‑firmware och inkluderas inte i versionshantering.  
Lösenord kan bytas via OTA‑uppdatering.

## 4.2 API‑nyckel  
Lagring sker i `.env` som inte checkas in.  
Nyckeln kan roteras utan att ändra kod.

## 4.3 Certifikat  
CA‑certifikat lagras i firmware.  
Servercertifikat lagras på MQTT‑broker och API‑server.  
Privata nycklar lagras aldrig i Git.

---

# 5. Sammanfattning  
Systemet använder TLS, certifikatverifiering, API‑nyckel och datavalidering för att uppnå säker kommunikation och åtkomstkontroll. Kvarvarande begränsningar är främst kopplade till utvecklingsmiljö och kan åtgärdas vid en produktionssättning. Lösningen uppfyller kursens krav på säkerhet och är robust mot vanliga nätverks‑ och kommunikationsfel.