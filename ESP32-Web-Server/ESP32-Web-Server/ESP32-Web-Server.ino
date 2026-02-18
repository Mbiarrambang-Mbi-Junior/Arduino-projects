// Load Wi-Fi library
#include <WiFi.h>

// --- Network Configuration ---
// NOTE: This code is configured for standard Wi-Fi station mode (connecting to a router).
const char* ssid = "Landmark Wounder Estate";
const char* password = "iamagoodtenant";

// Set web server port number to 80
WiFiServer server(80);

// --- GPIO Pin Configuration ---
const int LED_PIN_16 = 16;
const int LED_PIN_17 = 17;
const int LED_PIN_4 = 4;
const int LED_PIN_18 = 18;

// Variables to store the current LED states
bool isLed16On = false;
bool isLed17On = false;
bool isLed4On = false;
bool isLed18On = false;

// --- HTTP Request & Timeout Variables ---
String header = "";
const long REQUEST_TIMEOUT_MS = 2000; 
unsigned long previousDataTime = 0;


// --- HTML Web Page Template (Simplified for JS interaction) ---
const char* MAIN_WEBPAGE_HTML = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <title>Four-Device Dashboard</title>
  <style>
    html { font-family: monospace; text-align: center; }
    h1 { color: #333; }
    .device-card {
        border: 1px solid #ccc;
        padding: 20px;
        margin: 10px auto;
        width: 80%;
        max-width: 400px;
        box-shadow: 2px 2px 5px rgba(0,0,0,0.1);
    }
    .status { font-weight: bold; font-size: 1.5em; }
    .on { color: #4CAF50; }
    .off { color: #888888; }
  </style>
</head>
<body>
  <h1>Four-Device Monitoring Dashboard</h1>

  <div class="device-card">
    <h2>LED 16 (GPIO 16)</h2>
    <p>Status: <span id="led16Status" class="status">Waiting...</span></p>
    <p><a href="/16/on">ON</a> | <a href="/16/off">OFF</a></p>
  </div>
  
  <div class="device-card">
    <h2>LED 17 (GPIO 17)</h2>
    <p>Status: <span id="led17Status" class="status">Waiting...</span></p>
    <p><a href="/17/on">ON</a> | <a href="/17/off">OFF</a></p>
  </div>

  <div class="device-card">
    <h2>LED 4 (GPIO 4)</h2>
    <p>Status: <span id="led4Status" class="status">Waiting...</span></p>
    <p><a href="/4/on">ON</a> | <a href="/4/off">OFF</a></p>
  </div>
  
  <div class="device-card">
    <h2>LED 18 (GPIO 18)</h2>
    <p>Status: <span id="led18Status" class="status">Waiting...</span></p>
    <p><a href="/18/on">ON</a> | <a href="/18/off">OFF</a></p>
  </div>

  <script>
    // --- JavaScript for Periodic Status Update (AJAX/Fetch) ---
    function updateDashboard() {
        // Use Fetch API to request data from the new /status endpoint
        fetch('/status')
        .then(response => response.text())
        .then(data => {
            // Data format expected: "1,0,1,0" (16, 17, 4, 18)
            const [status16, status17, status4, status18] = data.split(',');
            
            // Reusable function to update status
            function setStatus(id, statusVal) {
                const element = document.getElementById(id);
                if (statusVal === '1') {
                    element.textContent = 'ON';
                    element.className = 'status on';
                } else {
                    element.textContent = 'OFF';
                    element.className = 'status off';
                }
            }
            
            // Update all four devices
            setStatus('led16Status', status16);
            setStatus('led17Status', status17);
            setStatus('led4Status', status4); 
            setStatus('led18Status', status18); 

        })
        .catch(error => console.error('Error fetching status:', error));
    }
    
    // Call the function immediately and then every 2 seconds
    updateDashboard();
    setInterval(updateDashboard, 2000); 
  </script>
</body>
</html>
)rawliteral";


// Function to generate the HTML (now static, no processing needed)
String processHtml(bool led16State, bool led17State) {
  // Parameters are unused since the HTML is static and updates via JS.
  return MAIN_WEBPAGE_HTML;
}

// NEW FUNCTION: Handle the /status request (returns raw data)
void handleStatusRequest(WiFiClient client) {
  // Format: "status16,status17,status4,status18" (e.g., "1,0,1,0")
  String statusData = 
      String(isLed16On ? 1 : 0) + "," + 
      String(isLed17On ? 1 : 0) + "," +
      String(isLed4On ? 1 : 0) + "," +
      String(isLed18On ? 1 : 0);

  // Send HTTP Header for plain text
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/plain"); // Crucial: Tell the browser it's just text
  client.println("Connection: close");
  client.println();

  // Send only the status data
  client.print(statusData);
  client.println();
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);

  // Set LED pins as OUTPUT and initialize them to LOW (off)
  pinMode(LED_PIN_16, OUTPUT);
  digitalWrite(LED_PIN_16, LOW);
  pinMode(LED_PIN_17, OUTPUT);
  digitalWrite(LED_PIN_17, LOW);
  pinMode(LED_PIN_4, OUTPUT); 
  digitalWrite(LED_PIN_4, LOW);
  pinMode(LED_PIN_18, OUTPUT);  
  digitalWrite(LED_PIN_18, LOW);

  // Connect to Wi-Fi
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;

    // After 20 attempts (10 seconds), print the reason for failure
    if (attempts >= 20) {
      Serial.println("\nFailed to connect. Retrying...");
      Serial.print("Status: ");
      Serial.println(WiFi.status()); // Prints a status code (e.g., 6 for wrong password)
      attempts = 0; // Reset counter
    }
  }

  // Print IP address and start web server
  Serial.println("\n--- WiFi Connected ---");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// --- LOOP ---
void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("\nNew Client Connected.");
    header = "";
    String currentLine = "";
    previousDataTime = millis();

    while (client.connected() && (millis() - previousDataTime <= REQUEST_TIMEOUT_MS)) {
      if (client.available()) {
        char c = client.read();
        header += c;
        previousDataTime = millis();

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Found the end of the request header (blank line)
            
            // --- ROUTING LOGIC ---

            // 1. Handle Status Request (AJAX/Fetch)
            if (header.indexOf("GET /status") >= 0) {
              handleStatusRequest(client);
              break; // EXIT after sending status data

            // 2. Handle Control Requests (All 4 Devices)
            } else if (header.indexOf("GET /16/on") >= 0) {
              isLed16On = true;
              digitalWrite(LED_PIN_16, HIGH);
            } else if (header.indexOf("GET /16/off") >= 0) {
              isLed16On = false;
              digitalWrite(LED_PIN_16, LOW);
            } else if (header.indexOf("GET /17/on") >= 0) {
              isLed17On = true;
              digitalWrite(LED_PIN_17, HIGH);
            } else if (header.indexOf("GET /17/off") >= 0) {
              isLed17On = false;
              digitalWrite(LED_PIN_17, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              isLed4On = true;
              digitalWrite(LED_PIN_4, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              isLed4On = false;
              digitalWrite(LED_PIN_4, LOW);
            } else if (header.indexOf("GET /18/on") >= 0) {
              isLed18On = true;
              digitalWrite(LED_PIN_18, HIGH);
            } else if (header.indexOf("GET /18/off") >= 0) {
              isLed18On = false;
              digitalWrite(LED_PIN_18, LOW);
            }
            
            // 3. Handle Root Page Request (Initial load, control redirects, and default)
            
            // Check for the root request or any request not handled above
            if (header.indexOf("GET / ") >= 0 || header.indexOf("GET /favicon.ico") >= 0 
                || header.indexOf("GET /16") >= 0 || header.indexOf("GET /17") >= 0
                || header.indexOf("GET /4") >= 0 || header.indexOf("GET /18") >= 0 ) 
            {
                // Send HTTP Header for HTML
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println("Connection: close");
                client.println();

                // Send the static HTML page with embedded JS
                String finalHtml = processHtml(isLed16On, isLed17On);
                client.print(finalHtml);
                client.println();
                
                // *** CRITICAL FIX: BREAK HERE TO AVOID STALLING ***
                break; 
            }

          } else {
            // Not a blank line yet, clear the current line
            currentLine = "";
          }
        } else if (c != '\r') {
          // Add the character to currentLine to build the header line
          currentLine += c; 
        }
      }
    }
    // This ensures the connection is immediately terminated once the loop is broken
    client.stop();
    Serial.println("Client Disconnected.");
  }
}