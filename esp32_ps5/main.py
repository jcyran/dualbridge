import serial

ser = serial.Serial('COM4', 115200)

#ser.open()
print("start")
while(True):
    x = ser.read()
    print("message: ", x)