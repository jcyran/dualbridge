import datetime

import serial
import vgamepad as vg
import db
import time
import sqlite3

class ControllerData:
    def __init__(self):
        self.right = False
        self.down = False
        self.up = False
        self.left = False

        self.square = False
        self.cross = False
        self.circle = False
        self.triangle = False

        self.L1 = False
        self.R1 = False
        self.L3 = False
        self.R3 = False

        self.share = False
        self.options = False
        self.touchpad = False
        self.ps_button = False

        self.L2 = 0
        self.R2 = 0
        self.l_stick_x = 0
        self.l_stick_y = 0
        self.r_stick_x = 0
        self.r_stick_y = 0

        self.gamepad = vg.VX360Gamepad()

    def get_data(self, data: bytes):
        self.right = (data[0] & 0b10000000) > 0
        self.down = (data[0] & 0b01000000) > 0
        self.up = (data[0] & 0b00100000) > 0
        self.left = (data[0] & 0b00010000) > 0

        self.square = (data[0] & 0b00001000) > 0
        self.cross = (data[0] & 0b00000100) > 0
        self.circle = (data[0] & 0b00000010) > 0
        self.triangle = (data[0] & 0b00000001) > 0

        self.L1 = (data[1] & 0b10000000) > 0
        self.R1 = (data[1] & 0b01000000) > 0
        self.L3 = (data[1] & 0b00100000) > 0
        self.R3 = (data[1] & 0b00010000) > 0

        self.share = (data[1] & 0b00001000) > 0
        self.options = (data[1] & 0b00000100) > 0
        self.touchpad = (data[1] & 0b00000010) > 0
        self.ps_button = (data[1] & 0b00000001) > 0

        self.L2 = int(data[2])+2**32
        self.R2 = int(data[3])+2**32

        self.l_stick_x = int((int(data[4]) * 32767) / 127)
        self.l_stick_y = int((int(data[5]) * 32767) / 127)
        self.r_stick_x = int((int(data[6]) * 32767) / 127)
        self.r_stick_y = int((int(data[7]) * 32767) / 127)

    def emulate(self):
        if self.right:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_RIGHT)

        if self.down:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_DOWN)

        if self.up:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_UP)

        if self.left:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)

        if self.square:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_X)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_X)

        if self.cross:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_A)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_A)

        if self.circle:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_B)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_B)

        if self.triangle:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_Y)

        if self.L1:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER)

        if self.R1:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER)

        if self.L3:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_THUMB)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_THUMB)

        if self.R3:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB)

        if self.share:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_BACK)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_BACK)

        if self.options:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_START)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_START)

        if self.ps_button or self.touchpad:
            self.gamepad.press_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_GUIDE)
        else:
            self.gamepad.release_button(vg.XUSB_BUTTON.XUSB_GAMEPAD_GUIDE)

        self.gamepad.left_trigger(self.L2)
        self.gamepad.right_trigger(self.R2)
        self.gamepad.left_joystick(self.l_stick_x, self.l_stick_y)
        self.gamepad.right_joystick(self.r_stick_x, self.r_stick_y)

        self.gamepad.update()


# Initialization

db.connect("button_stats.db")
db.create_database()
db.update_button("Cross")



ser = serial.Serial('COM4', 115200)
controller = ControllerData()

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

        try:
            # get the data and emulate on gamepad
            controller.get_data(x)
            controller.emulate()
        except ValueError:
            print("Invalid data received")

        # print("Buttons:", x[0])
        # print("Functional Buttons:", x[1])
        # print("L2:", x[2])
        # print("R2:", x[3])
        # print("L Stick X axis:", x[4])
        # print("L Stick Y axis:", x[5])
        # print("R Stick X axis:", x[6])
        # print("R Stick Y axis:", x[7])
        # print()
except KeyboardInterrupt:
    print("\nStopping...")
except serial.SerialException as e:
    print(f"Serial error: {e}")
finally:
    ser.close()
    controller.gamepad.reset()
    controller.gamepad.update()
    print("Serial closed")
