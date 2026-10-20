#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <time.h>
#include "secrets.h"

#define DHT_PIN 15
#define OW_PIN   4

// ---- Simulated sensor error (documented in README) ----
const float SIM_T_GAIN = 1.04f, SIM_T_OFFSET = -2.0f;   // temperature
const float SIM_H_OFFSET = 3.0f;                         // humidity (%RH)

// ---- Calibration constants from least-squares fit (analysis/calibrate.py) ----
const float CAL_T_GAIN = 0.96154f, CAL_T_OFFSET = 1.9231f;
const float CAL_H_OFFSET = -3.0f;

const uint32_t INTERVAL_MS = 5000;                       // 5 s
const char* TOPIC = "dt/lab1/" THING_NAME "/telemetry";

WiFiClientSecure net;
PubSubClient mqtt(net);
DHT dht(DHT_PIN, DHT22);
OneWire ow(OW_PIN);
DallasTemperature ds(&ow);

void connectAws() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to AWS IoT... ");
    if (mqtt.connect(THING_NAME)) Serial.println("OK");
    else { Serial.printf("failed rc=%d\n", mqtt.state()); delay(2000); }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);
  Serial.println("WiFi connected");
  configTime(0, 0, "pool.ntp.org");              // TLS needs correct time
  while (time(nullptr) < 1700000000) delay(300);
  Serial.println("Time synced");

  net.setCACert(AWS_CA);
  net.setCertificate(AWS_CERT);
  net.setPrivateKey(AWS_KEY);
  mqtt.setServer(AWS_ENDPOINT, 8883);
  mqtt.setBufferSize(512);
  dht.begin();
  ds.begin();
  connectAws();
}

void loop() {
  if (!mqtt.connected()) connectAws();
  mqtt.loop();

  static uint32_t last = 0;
  if (millis() - last < INTERVAL_MS) return;
  last = millis();

  float tTrue = dht.readTemperature();
  float hTrue = dht.readHumidity();
  ds.requestTemperatures();
  float tRef = ds.getTempCByIndex(0);
  if (isnan(tTrue) || isnan(hTrue) || tRef == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor read failed"); return;
  }

  float tRaw = SIM_T_GAIN * tTrue + SIM_T_OFFSET;
  float hRaw = hTrue + SIM_H_OFFSET;
  float tCal = CAL_T_GAIN * tRaw + CAL_T_OFFSET;
  float hCal = hRaw + CAL_H_OFFSET;

  JsonDocument d;
  d["device_id"]   = THING_NAME;
  d["ts"]          = (uint32_t)time(nullptr);
  d["temp_raw_c"]  = serialized(String(tRaw, 2));
  d["temp_cal_c"]  = serialized(String(tCal, 2));
  d["temp_ref_c"]  = serialized(String(tRef, 2));
  d["hum_raw_pct"] = serialized(String(hRaw, 1));
  d["hum_cal_pct"] = serialized(String(hCal, 1));
  d["hum_ref_pct"] = serialized(String(hTrue, 1));
  d["unit_temp"]   = "degC";
  d["unit_hum"]    = "%RH";

  char buf[400];
  size_t n = serializeJson(d, buf);
  bool ok = mqtt.publish(TOPIC, (const uint8_t*)buf, n);
  Serial.println(ok ? buf : "publish failed");
}
