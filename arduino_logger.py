import serial
import csv
import time

# === CONFIGURATION ===
PORT = "COM4"          # Change this to your Arduino port (e.g., COM3 on Windows, /dev/ttyUSB0 on Linux)
BAUDRATE = 9600        # Must match Serial.begin(9600) in your Arduino code
OUTPUT_FILE = "temperatures.csv"

# === SETUP ===
ser = serial.Serial(PORT, BAUDRATE, timeout=1)
time.sleep(2)  # wait for Arduino to reset

print(f"Connected to {PORT} at {BAUDRATE} baud.")
print(f"Logging data to {OUTPUT_FILE}...\nPress Ctrl+C to stop.\n")

# Open CSV file for writing
with open(OUTPUT_FILE, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    header_written = False

    try:
        while True:
            line = ser.readline().decode("utf-8").strip()
            if line:
                print(line)  # Optional: show data in console

                # Write to CSV
                # Automatically split by commas if CSV formatted
                if not header_written and "Sample" in line:
                    writer.writerow(line.split(","))
                    header_written = True
                elif header_written:
                    writer.writerow(line.split(","))
                    csvfile.flush()  # write immediately to disk
    except KeyboardInterrupt:
        print("\nLogging stopped by user.")
    finally:
        ser.close()
