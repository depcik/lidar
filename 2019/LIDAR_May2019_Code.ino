/*
  |-----------------------------------------|
  | Fall 2018 - Spring 2019                 |
  |-----------------------------------------|
  | University of Kansas                    |
  | Department of Mechanical Engineering    |
  |-----------------------------------------|
  | ECOHAWKS LIDAR Capstone / Senior Design |
  |-----------------------------------------|
  | Members (alphabetical):                 |
  | Michael Duncan                          |
  | Jaret Halberstadt                       |
  | Mark Heim                               |
  | Theo Wiklund                            |
  |-----------------------------------------|
  | Advisers (alphabetical):                |
  | Professor DeAgostino                    |
  | Dr. Depcik                              |
  |-----------------------------------------|
 */
 //latest substantial revision: April 14, 2019
 //latest commenting/readability revision: May 4, 2019
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
//Libraries: 
//Wire.h includes i2c communication which Lidar Lite V3 requires
#include <Wire.h>
//LIDARLite.h is library supplied by Garmin, manufacturer of Lidar Lite V3
#include <LIDARLite.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Creating object for the LIDARLite.h library
LIDARLite myLidarLite;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
// Reference diagrams for magnet locations of the stepper motors used
// There are four magnets inside each motor 1, 2, 3, and 4. The motor can go in between magnets.
//                     (1)                         
//              4 & 1 ----- 1 & 2                
//                |           |                  
//            (4) |           | (2)          
//                |           |                  
//              3 & 4 ----- 2 & 3               
//                     (3)                               
// Two stepper motors used. Model: 28BYJ-48. Gear ratio of 1/64. Degrees per step = 5.625. With gear ratio = (1/64)*5.625 = 0.087890625 degrees per step. 
// 360 degrees per full rotation. Total steps for full rotation = 360/0.087890625 = 4096 steps per full rotation.
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Declaring motor pins:
//Horizontal motor pins
int pin4 = 4;
int pin5 = 5;
int pin6 = 6;
int pin7 = 7;

//Creating array for vertical motor pin location 
 int vert_pins[] = {8,9,10,11};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Creating variables for the vertical motor code:
//iteration variables for vertical motor code 
int i = 1; //FOR ODD SWEEPS
int j = 0; //FOR EVEN SWEEPS
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/ 
//Horizontal step count variable
int hsteps = 0;

//Vertical step count variable
int vsteps = 0;

//Number of horizontal sweeps, both clockwise and counter-clockwise
int sweeps = 0;

//Distance variable
int dis; //Using dis variable to temporarily store myLidarLite.distance() value for each point

//Delay for the loops controlling motors
int deltime = 5;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//Main code for moving motors and reading distances from rangefinder
void setup() 
{ 
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  //Stepper motor configurations:
  
  //Horizontal motor. Each pin controlls a single electromagnet inside motor. There are four magnets in each motor.
  pinMode(pin4,OUTPUT);pinMode(pin5,OUTPUT);pinMode(pin6,OUTPUT);pinMode(pin7,OUTPUT);
  
  //assigning all vertical motor pins on arduino as outputs
  for(i=0; i<=3; i++)
  {
    pinMode(vert_pins[i], OUTPUT);
  }

  //Setting initial horizontal motor pin voltages, intitial position is magnet 1
  digitalWrite(pin4,HIGH);digitalWrite(pin5,LOW);digitalWrite(pin6,LOW);digitalWrite(pin7,LOW);
  
  //Setting initial vertical motor pin voltages, intitial position is magnet 1
  digitalWrite(vert_pins[0],HIGH);digitalWrite(vert_pins[1],LOW);digitalWrite(vert_pins[2],LOW);digitalWrite(vert_pins[3],LOW);


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  //Starting serial, 115200 baud is specified in Lidar Lite V3 manual
  Serial.begin(115200);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  //Clears out previous serial data
  Serial.println("CLEARDATA");
  Serial.println("RESETTIMER");
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  //Lidar Lite V3 configuration settings:
  myLidarLite.begin(0, true);
  myLidarLite.configure(0);
      //  configuration descriptions:
      //    0: Default mode, balanced performance.
      //    1: Short range, high speed. Uses 0x1d maximum acquisition count.
      //    2: Default range, higher speed short range. Turns on quick termination
      //        detection for faster measurements at short range (with decreased
      //        accuracy)
      //    3: Maximum range. Uses 0xff maximum acquisition count.
      //    4: High sensitivity detection. Overrides default valid measurement detection
      //        algorithm, and uses a threshold value for high sensitivity and noise.
      //    5: Low sensitivity detection. Overrides default valid measurement detection
      //        algorithm, and uses a threshold value for low sensitivity and noise.
      //    lidarliteAddress: Default 0x62. Fill in new address here if changed. See
      //        operating manual for instructions.
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  //Motor control code:
  //For loop increments vertical motor after completing each horizontal sweep. FOR VSTEPS <= #, USE STEP NUMBER - NOT DEGREES
  for (int vsteps = 0; vsteps <= 512;)//vsteps <= rotation: full,half,quarter,eigth = steps: 4096,2048,1024,512 [which are equivalent to degrees: 360,180,90,45]
  {
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    //START of vertical motor control code
    //i starts at i = 1
    //j starts at j = 0
    if(((sweeps % 2) == 0) && sweeps>0) //activates when horizontal motor sweeps clockwise since sweeps must be even to spin cw. when horiz motor starts its first sweep, sweep = 0.
    {                 
                       //first run: sweeps = 0, so if = true, so BEFORE horizontal motor starts moving the vertical motor will move up one magnet position. how do you fix this? not critical.
                       //second run: sweeps = 1, if statement = false, moves to else statement below.
                       //third run: sweeps = 2, if statement = true
                       //fourth run: sweeps = 3, if statement = false, moves to else
                       //fifth run: sweeps = 4, if = true
                       //sixth run: sweeps = 5, if = false, move to else
                       //seventh run: sweeps = 6, if = true
                       //eighth run: sweeps = 7, if = false, move to else
      if(i == 4) //responsible for making i return to 0 which will write pina to high voltage - aka: motor position 8. Starts by turning pinb high alongside pina, but must end on turning pina high by itself.
      {
        i = i - 4; //first run: false, i = 1
                   //third run: false, i = 2
                   //fifth run: false, i = 3
                   //seventh run: true, i was = 4, now i = 0
      }//end of if(i ==4)
      digitalWrite(vert_pins[i],HIGH); //first run:   write to vert_pins[1],HIGH; move motor between magnet 1 and 2
                                      //third run:   write to vert_pins[2],HIGH; move motor between magnet 2 and 3
                                      //fifth run:   write to vert_pins[3],HIGH; move motor between magnet 3 and 4
                                      //seventh run: write to vert_pins[0],HIGH; move motor between magnet 4 and 1
      delay(deltime);
      dis = myLidarLite.distance();
      Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
      vsteps = vsteps + 1;
      i = i + 1; //first run: i was 1, now = 2
                 //third run: i was = 2, now i = 3
                 //fifth run: i was = 3, now i = 4
    }//end of if(sweeps %2 == 0)
    else //activates when horizontal motor spins counter-clockwise since sweeps must be odd to spin ccw
    {
      //j begins at j = 0;
      if(j == 4) //responsible for making i return to 0 which will write pina to high voltage - aka: motor position 8. Starts by turning pinb high alongside pina, but must end on turning pina high by itself.
      {
        j = j - 4; //first run: false, i = 0
                   //third run: false, i = 1
                   //fifth run: false, i = 2
                   //seventh run: false, i = 3, 
                   //ninth run: true, j = 4, now j = 0
      }//end of if(j ==4)
      digitalWrite(vert_pins[j],LOW); //second run: sweeps = 1,  write to vert_pins[0],LOW; move motor to magnet 2
                                     //fourth run: sweeps = 3,  write to vert_pins[1],LOW; move motor to magnet 3
                                     //sixth run: sweeps = 5,   write to vert_pins[2],LOW; move motor to magnet 4
                                     //eigth run: sweeps = 7,   write to vert_pins[3],LOW; move motor to magnet 1
      delay(deltime);
      dis = myLidarLite.distance();
      Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
      vsteps = vsteps + 1;
      j = j + 1; //second run: j was 0, j now = 1
                 //fourth run: j was 1, j now = 2
                 //sixth run: j was 2, j now = 3
    }//end of else
    //END of vertical motor control code
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    //START of Clockwise code for horizontal motor
    if ((vsteps % 2)==0) //if vertical step count is even, horizontal motor rotates clockwise. If false, makes the horizontal motor go to ccw loop. 
    {
      while (hsteps <= 1024) //hsteps <= rotation: full,half,quarter,eigth = steps: 4096,2048,1024,512 [which are equivalent to degrees: 360,180,90,45] 1366 steps = 120 degrees
      {  //One thing to figure out is if 1024 steps actually is 90 degrees. Maybe the eight motor positions require it to be 1024*8 hsteps to make 90 degrees.
         //Starting motor position is at magnet 1 - see top of void setup under section "Stepper Motor Configurations"
         //Counter-clockwise loop had motor end on magnet 1
         //[1] Moving motor clockwise between magnets 1 and 2
         digitalWrite(pin5,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[2] Moving motor to magnet 2
         digitalWrite(pin4,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[3] Moving motor between magnets 2 and 3
         digitalWrite(pin6,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[4] Moving motor to magnet 3
         digitalWrite(pin5,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[5] Moving motor between magnets 3 and 4
         digitalWrite(pin7,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[6] Moving motor to magnet 4
         digitalWrite(pin6,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[7] Moving motor between magnets 4 and 1
         digitalWrite(pin4,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;

         //[8] Moving motor to magnet 1
         digitalWrite(pin7,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps + 1;
      } //end of while loop
      sweeps = sweeps +1;//completed one clockwise sweep
    } // end of if loop
    //END of clockwise code motor code
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    //START of Counterclockwise code for horizontal motor
    if (vsteps & 0x01) //If vsteps is odd then sweep the motor counterclockwise. If false, for loop will enter previous if statement for cw loop.
    {
      while (hsteps > 0) //hsteps > rotation: full,half,quarter,eigth = steps: 4096,2048,1024,512 [which are equivalent to degrees: 360,180,90,45]
      {  
         //Clockwise loop had motor end at magnet 1
         //[1] Moving motor counter-clockwise between magnets 4 and 1
         digitalWrite(pin7,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1; 

         //[2] Moving motor to magnet 4
         digitalWrite(pin4,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1;  

         //[3] Moving motor between magnets 3 and 4
         digitalWrite(pin6,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1;  

         //[4] Moving motor to magnet 3
         digitalWrite(pin7,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1;  

         //[5] Moving motor between magnets 2 and 3
         digitalWrite(pin5,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1;  

         //[6] Moving motor to magnet 2
         digitalWrite(pin6,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1;  

         //[7] Moving motor between magnets 2 and 1
         digitalWrite(pin4,HIGH);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1; 

         //[8] Moving motor to magnet 1
         digitalWrite(pin5,LOW);delay(deltime);
         dis = myLidarLite.distance();
         Serial.print(dis);Serial.print("\t");Serial.print(hsteps);Serial.print("\t");Serial.println(vsteps);
         hsteps = hsteps - 1;  
      } //end of while loop
      sweeps = sweeps + 1;//completed one counter-clockwise sweep
    } //end of if loop
    //END of counter clockwise horizontal motor code
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  } //end of for loop
  Serial.end();
} //end of void setup loop

void loop() {
  //not used
}
