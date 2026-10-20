#pragma once
// Copy to secrets.h (git-ignored) and fill in. NEVER commit real values.
#define WIFI_SSID    "Wokwi-GUEST"
#define WIFI_PASS    ""
#define AWS_ENDPOINT "YOUR-ENDPOINT-ats.iot.REGION.amazonaws.com"
#define THING_NAME   "p1-esp32-sim-01"

static const char AWS_CA[] = R"EOF(
-----BEGIN CERTIFICATE-----
(Amazon Root CA 1)
-----END CERTIFICATE-----
)EOF";

static const char AWS_CERT[] = R"EOF(
-----BEGIN CERTIFICATE-----
(device certificate)
-----END CERTIFICATE-----
)EOF";

static const char AWS_KEY[] = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
(device private key)
-----END RSA PRIVATE KEY-----
)EOF";
