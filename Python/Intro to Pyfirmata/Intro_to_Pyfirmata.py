from pyfirmata import Arduino, util
import time

board = Arduino("COM6") # Set this to the corresponding COM port from the Arduino IDE

print("Running pyFirmata version " + str(board.get_firmata_version())) #Show firmata version

loopTimes = input("How many blinks would you like")
print("Blinking" + loopTimes + " Times")
for x in range(int(loopTimes)):
    board.digital[13].write(1)  # write HIGH to pin 13 (internal LED)
    time.sleep(0.5)             # wait 0.5 seconds
    board.digital[13].write(0)  # write LOW to pin 13 (internal LED)
    time.sleep(0.5)             # wait 0.5 seconds
