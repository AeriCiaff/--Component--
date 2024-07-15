from pynput import keyboard
import serial
import threading
import sys

ser = serial.Serial("COM3",baudrate=115200)


def on_press(key):
    try:
        if key.char == "q":
            ser.write("q".encode())
        elif key.char == "1":
            ser.write("1".encode())
        elif key.char == "2":
            ser.write("2".encode())
        elif key.char == "a":
            ser.write("a".encode())
        elif key.char == "d":
            ser.write("d".encode())
        elif key.char == "z":
            ser.write("z".encode())
        elif key.char == "x":
            ser.write("x".encode())
        elif key.char == "w":
            ser.write("w".encode())
        elif key.char == "s":
            ser.write("s".encode())
        else:
            pass
    except AttributeError:
        pass

def read_data():
    while True:
        data = ser.readline()
        print(data)

def listen_keyboard():
    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()

serial_thread = threading.Thread(target=read_data)
keyboard_thread = threading.Thread(target=listen_keyboard)

serial_thread.start()
keyboard_thread.start()

serial_thread.join()
keyboard_thread.join()