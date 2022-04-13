import time
import RPi.GPIO as GPIO
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)

control_pins = [12,16,18,22] #horizontal motor
for pin in control_pins:
      GPIO.setup(pin, GPIO.OUT)
      GPIO.output(pin, 0)

halfstep_seq = [
    [1,0,0,1],
    [1,0,0,0],
    [1,1,0,0],
    [0,1,0,0],
    [0,1,1,0],
    [0,0,1,0],
    [0,0,1,1],
    [0,0,0,1]
]

for i in range(2):
    for halfstep in range(8):
        for pin in range(4):
          GPIO.output(control_pins[pin], halfstep_seq[halfstep][pin])
        time.sleep(.005)
#         
# control_pins = [29,31,32,33] #vertical
# for pin in control_pins:
#       GPIO.setup(pin, GPIO.OUT)
#       GPIO.output(pin, 0)
# 
# halfstep_seq = [
#     [0,0,0,1],
#     [0,0,1,1],
#     [0,0,1,0],
#     [0,1,1,0],
#     [0,1,0,0],
#     [1,1,0,0],
#     [1,0,0,0],
#     [1,0,0,1], 
# ]
# for i in range(2):
#     for halfstep in range(8):
#         for pin in range(4):
#           GPIO.output(control_pins[pin], halfstep_seq[halfstep][pin])
#         time.sleep(.005)
        
        
