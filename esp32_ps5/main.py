import serial
import vgamepad as vg
import time

# ser = serial.Serial('COM3', 115200)

gamepad = vg.VX360Gamepad()

while True:
    gamepad.press_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
    gamepad.update()
    time.sleep(1)
    gamepad.release_button(button=vg.XUSB_BUTTON.XUSB_GAMEPAD_DPAD_LEFT)
    gamepad.update()
    time.sleep(1)
