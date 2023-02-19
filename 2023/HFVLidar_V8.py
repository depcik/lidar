#!/usr/bin/python3
#Nate Ahlgren Lidar Gen4 dev code V8
from lidar_lite import Lidar_Lite
lidar = Lidar_Lite()
import math
connected = lidar.connect(1)
import time
from datetime import datetime
import os
import RPi.GPIO as GPIO
import lidar_lite
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)


while(True):
    
    
    hsweep = int(45)      #Degrees Change this for horizontal sweep angle
    vsweep = int(45)      #Degrees Change this for vertical sweep angle
    
    #Initial location values
    hstep = 0    
    vstep = 0
    hangle = 0
    vangle = 0
    hanglepos = 0

#Pin locations/definitions
    
    Vertax = [6,13,19,26]
    Horzax = [20,21,4,5]
    for pin in Horzax:
        GPIO.setup(pin,GPIO.OUT)
        GPIO.output(pin,0)
    for pin in Vertax:
        GPIO.setup(pin,GPIO.OUT)
        GPIO.output(pin,0)
            
    halfstepf = [[1,0,0,0],   #Clockwise, halfstep logic
                 [1,1,0,0],
                 [0,1,0,0],
                 [0,1,1,0],
                 [0,0,1,0],
                 [0,0,1,1],
                 [0,0,0,1],
                 [1,0,0,1]]

    fullstepf = [[1,0,0,0],   #Full step logic
                 [0,1,0,0],
                 [0,0,1,0],
                 [0,0,0,1]]

    threestepf = [[1,0,0,0],  #1/3 step logic
                  [0,1,1,0],
                  [0,0,0,1]]

    halfstepr = [[0,0,0,1],   #same but for counter clockwise
                 [0,0,1,1],
                 [0,0,1,0],
                 [0,1,1,0],
                 [0,1,0,0],
                 [1,1,0,0],
                 [1,0,0,0],
                 [1,0,0,1]]

    fullstepr = [[0,0,0,1],
                 [0,0,1,0],
                 [0,1,0,0],
                 [1,0,0,0]]

    threestepr = [[0,0,0,1], 
                  [0,1,1,0],
                  [1,0,0,0]]

    forward = [halfstepf,   #Fun indexing#
               fullstepf,
               threestepf]

    rev = [halfstepr,       #gnixedni nuF#
           fullstepr,
           threestepr]
    GPIO.setup(25, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #button the first
    GPIO.setup(7, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  #button the second
    GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) #button the third
    GPIO.setup(24, GPIO.OUT) #LED
    GPIO.output(24, GPIO.HIGH) #ready light on
    easybutton = 0
    try:
        while easybutton<1:
             
            if GPIO.input(25) == GPIO.HIGH:
                ssl = 0  #ssl: step size logic, fun indexing and all that
                easybutton = easybutton+1
            if GPIO.input(7) == GPIO.HIGH:     
                ssl = 1
                easybutton = easybutton+1
            if GPIO.input(23) == GPIO.HIGH:
                ssl = 2        
                easybutton = easybutton+1
    except:
        pass
    
    GPIO.output(24,GPIO.LOW)
    print(ssl)
    stepsizelogic = [8,4,3] #how many steps in sequence
    thresholdlogic = [7,3,2] #how many steps if step 1 is step 0 
    anglesizelogic = [.0879, .176, .234] #step sizes

    forward = forward[ssl]
    rev = rev[ssl]
    hsweep1 = math.ceil(hsweep/anglesizelogic[ssl]/stepsizelogic[ssl]) #Convert angle to loop count
    vsweep1 = math.ceil(vsweep/anglesizelogic[ssl]/stepsizelogic[ssl])
    hsweep2 = math.ceil(hsweep1/2) #So lidar starts pointed at target, moves to start of sweep
    vsweep2 = math.ceil(vsweep1/2)
    hsweep3 = math.ceil(hsweep/2) #this logic should take the angles defined earlier, divide them by 2 and round up
    

    #print(anglesizelogic[ssl]) debugging


    ti = datetime.now()
    filename1 = "HVF7Lidar_1a.csv"
    dir_contents = os.listdir("/home/pi/LidarRuns7") #this can be changed to modify where the data is saved


    if filename1 in dir_contents: #run number = filename + # of files in folder +1
        dir_contents.sort()    
        max_index = 1
        split = dir_contents[-1].split("_")
        if len(split)>1:
            max_index = len(dir_contents) +1
        filename1 = "HFV7Lidar_" +str(max_index) + ".csv"
        #filename2 = "HFV7Lidar_" +str(max_index) + ".csv"

    with open("/home/pi/LidarRuns7/"+str(filename1),"w") as file1:#, open("/home/pi/LidarRuns7/"+str(filename2),"w") as file2:
      
        for i in range(int(hsweep2)):                    #half sweep to center about zero, no DAQ
            for halfstep in range(stepsizelogic[ssl]):   #This allows you to point the lidar device at the target
                print(halfstep,stepsizelogic[ssl],rev[halfstep],Horzax)
                GPIO.output(Horzax, rev[halfstep])
                hstep = hstep-1
                time.sleep(.05)
        for i in range(int(vsweep2)):
            for halfstep in range(stepsizelogic[ssl]):
                GPIO.output(Vertax, rev[halfstep])
                vstep = vstep+1
                time.sleep(.05)
        while hstep*anglesizelogic[ssl] <= hsweep3:              #vertax takes a half step each time horzax ends a CW/CCW sweep
            for i in range(int(vsweep1)):                  #horzax loops
                for halfstep in range(stepsizelogic[ssl]):             #         8 halfstep sequence                                   #           4 coils on the motor                
                    distance = lidar.getDistance()                        
                    file1.write(str(hstep*anglesizelogic[ssl])+"," +str(vstep*anglesizelogic[ssl])+  "," + str(distance) +"\n")
                    GPIO.output(Vertax, forward[halfstep])
                    vstep = vstep-1
                    #time.sleep(.05)
                        
            if hanglepos < (thresholdlogic[ssl]):     # if statement loop to allow vangle to step continuously
                distance = lidar.getDistance()        # without looping through 8 step sequence
                file1.write(str(hstep*anglesizelogic[ssl])+"," +str(vstep*anglesizelogic[ssl]-.7605)+  "," + str(distance) +"\n")#f2  #dcvstep
                GPIO.output(Horzax,forward[int(hanglepos)]) #"hanglepos" is how I made this work with individaul steps in the  horizontal direction
                hstep = hstep+1
                hanglepos= hanglepos+1
                #time.sleep(.05)
                print("Azimuth =" + str(hstep*anglesizelogic[ssl])+ "of" + str(hsweep))
                
            else:
                distance = lidar.getDistance()
                file1.write(str(hstep*anglesizelogic[ssl])+"," +str(vstep*anglesizelogic[ssl]-.7605)+  "," + str(distance) +"\n")#f2
                GPIO.output(Horzax,forward[int(hanglepos)])
                hstep = hstep+1
                hanglepos= hanglepos-(thresholdlogic[ssl])
                time.sleep(.05)
               

            for j in range(int(vsweep1)):
                for halfstep in range(stepsizelogic[ssl]):              # reverse side of sweep                        
                    distance = lidar.getDistance()                       
                    file1.write(str(hstep*anglesizelogic[ssl])+","+str(vstep*anglesizelogic[ssl]-.7605)+"," +str(distance)+  "," +"\n")   #f2        #Single direction data acquisition     
                    GPIO.output(Vertax, rev[halfstep])
                    vstep = vstep+1
                    time.sleep(.05)
                    #print("vstep = %s" % (vstep))
                    
            if hanglepos < (thresholdlogic[ssl]):                          # if statement loop to allow vangle to step continuously
                distance = lidar.getDistance()        # without looping through 8 step sequence
                file1.write(str(hstep*anglesizelogic[ssl])+"," +str(vstep*anglesizelogic[ssl])+  "," + str(distance) +"\n")
                GPIO.output(Horzax,forward[int(hanglepos)])
                hstep = hstep+1
                hanglepos= hanglepos+1
                #time.sleep(.05)
                print("Azimuth =" + str(hstep*anglesizelogic[ssl])+ "of" + str(hsweep))
                
            else:
                distance = lidar.getDistance()
                file1.write(str(hstep*anglesizelogic[ssl])+"," +str(vstep*anglesizelogic[ssl])+  "," + str(distance) +"\n")
                GPIO.output(Horzax,forward[int(hanglepos)])
                hstep = hstep+1
                hanglepos= hanglepos-(thresholdlogic[ssl])
                time.sleep(.05)

           

    tf = datetime.now() #this was to determine the data acquisition rate
    tdelta = tf-ti
    filename1 = "HFV7Lidartimelog.csv"
    with open("/home/pi/LidarRuns7/HFV7Lidartimelog.csv","a") as file: #really dont need this
        file.write(filename1 +","+ str(tdelta)+"\n")
            
    if hanglepos!=0:           #return to origin section
        hanglepos2 = (stepsizelogic[ssl])-hanglepos
    else:
        hanglepos2 = 0
    hhse = hstep/(stepsizelogic[ssl])             #hor half step loop equivalent
    hcf = math.ceil(hhse)      #Horizontal correction factor
    if hstep>0:
        for k in range(int(hanglepos2)):
            GPIO.output(Horzax,forward[int(hanglepos)])
            hstep = hstep+1
            hanglepos= hanglepos+1
            time.sleep(.05)
            print("hstep #2 = %s" % (hstep))
        for f in range(int(hcf)):
            for halfstep in range(stepsizelogic[ssl]):
                GPIO.output(Horzax,rev[halfstep])
                hstep = hstep-1
                time.sleep(.05)
                print("hstep #1 = %s" % (hstep))

    vhse = vstep/(stepsizelogic[ssl])
    if vstep >0:
        for l in range(int(vhse)):
            for halfstep in range(stepsizelogic[ssl]):
                GPIO.output(Horzax,rev[halfstep])
                vstep = vstep - 1
                print("vstep #1 = %s" % (vstep))
    GPIO.cleanup() #good luck
#except:
#GPIO.cleanup()