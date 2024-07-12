from pynput import keyboard
import serial

ser = serial.Serial("COM3",baudrate=115200)
speed = 1100

def on_press(key):
    try:
        global speed
        if key.char == "q":
            ser.write("q".encode())
        elif key.char == "w":
            ser.write("w".encode())
            speed = speed + 5
            print(speed)
        elif key.char == "s":
            ser.write("s".encode())
            speed = speed - 5
            print(speed)
        elif key.char == "a":
            ser.write("a".encode())
        elif key.char == "d":
            ser.write("d".encode())
        elif key.char == "z":
            ser.write("z".encode())
        elif key.char == "x":
            ser.write("x".encode())
        elif key.char == "0":
            ser.write("0".encode())
        elif key.char == "1":
            ser.write("1".encode())
        else:
            pass
    except AttributeError:
        pass

with keyboard.Listener(on_press=on_press) as listener:
    listener.join()