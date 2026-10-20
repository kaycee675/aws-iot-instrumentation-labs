# AWS IoT Core Instrumentation Labs

Vacation project: 10 instrumentation labs built on ESP32 and AWS IoT Core
(TLS + X.509 certificates, MQTT, JSON telemetry, local validation against a reference).

| # | Project | Status |
|---|---|---|
| 1 | [Temperature & Humidity Telemetry Station](01-temp-humidity-station/) | Done (simulated in Wokwi) |
| 2 | DC Voltage Measurement & Cloud Logging | Planned |
| 3 | Current Measurement (Hall-effect) | Planned |
| 4 | Multi-Channel Environmental Monitoring + Device Shadow | Planned |
| 5 | Resistance / Bridge Circuit Monitoring | Planned |
| 6 | Power Monitoring (V x I) | Planned |
| 7 | Frequency / Period Measurement | Planned |
| 8 | Remote Calibration & Offset Correction | Planned |
| 9 | Anomaly Detection & Alerting | Planned |
| 10 | End-to-End Instrumentation Dashboard | Planned |

**Security:** no certificates, keys or endpoints with secrets are stored in this repo.
Each project has a `secrets.example.h` with placeholders only.
