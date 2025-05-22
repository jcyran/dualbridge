import serial
import time

ser = serial.Serial('COM3', 115200)

time.sleep(2)

print("start")

buffer = b""
ready_signal = b"Ready.\r\n"

try:
    while True:
        byte = ser.read(1)

        if not byte:
            continue

        buffer += byte

        if ready_signal in buffer:
            print("Received ready signal.")

            buffer = buffer[buffer.find(ready_signal):]
            break

    while True:
        x = ser.read(8)

        print("Buttons:", x[0])
        print("Functional Buttons:", x[1])
        print("L2:", x[2])
        print("R2:", x[3])
        print("L Stick X axis:", x[4])
        print("L Stick Y axis:", x[5])
        print("R Stick X axis:", x[6])
        print("R Stick Y axis:", x[7])
        print()
except KeyboardInterrupt:
    print("\nStopping...")
except serial.SerialException as e:
    print(f"Serial error: {e}")
finally:
    ser.close()
    print("Serial closed")
