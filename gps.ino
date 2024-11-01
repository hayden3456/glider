// Define the RX and TX pins for Serial1
#define RX_PIN 44 // GPIO2 (Pin D2)
#define TX_PIN 43  // GPIO1 (Pin D1)

void setup() {
  // Initialize Serial Monitor over USB
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for the Serial Monitor to connect
  }
  Serial.println("Serial Monitor initialized.");

  // Initialize Serial1 for GPS device communication
  // Adjust the baud rate if your GPS device uses a different one
  Serial1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Serial1 initialized for GPS communication.");
}

void loop() {
  // Check if data is available from the GPS device
    // Read a byte from the GPS device
    int incomingByte = Serial1.read();
    // Send the received byte to the Serial Monitor
    Serial.write(incomingByte);
    delay(100);
}
