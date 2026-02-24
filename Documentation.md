## 1. General information
#### Project Name: PotLock 
#### Author: Nestor Hilchenbach

## 2. Project Goal / Idea
The goal of this project was to build a secured chest that can only be opened by entering a correct code.
The code is entered using a potentiometer dial and a push button, the user interface is shown on an LCD screen, and a servo motor physically locks/unlocks the chest.
The project combines:
Input (potentiometer + button)
Output (LCD + LEDs + buzzer)
Physical states (servo lock)

## 3. Functionality (How it works)
User instructions:
- The LCD shows: “Enter Code!”
- The user turns the potentiometer to select a digit (0–9).
- Pressing the button confirms the digit.
- After 4 digits are entered:
- If correct: The servo unlocks, green LED turns on, success sound plays. You can now open the lock
- If wrong → error message, red LED stays on, error sound plays, input resets.
When unlocked, pressing the button again re-locks the chest.

## 4. Components Used
#### Main electronics:
- Arduino Uno
- 16×2 LCD with I²C backpack
- Potentiometer
- Push button
- Servo Motor
- Red LED
- Green LED
- 2× 220Ω resistors
- Passive buzzer

#### Wiring & building:
- Breadboard
- Perfboard
- Jumper wires,
- Chest / box for enclosure 
- Bolts and steel wire for lock mechanism
- metal door hinge

## 5. Procect Process 
This section documents the steps taken and the issues solved during the build.
### 1.step:  Choosing the project idea 
I wanted to create a little box, where I could hide something in. As I always had a little safe for my pocket money as a child. I then found a Arduino project, which did something like that.
I used it as inspiration. (https://wiki.hshl.de/wiki/index.php/Hochsicherheitssafe) <br>
At first I wanted to create a dual input system combining the keypad and the potentiometer. But as we tried out the keypad in class, I realized how deceptive it is for bugs.
Therefore I decided to just go with the potentiometer. This was also great, as I was saving a lot of wires, which later became a problem, due to limited space inside the box.

### 2. step: LCD setup
Goal: make the LCD display text correctly.<br>
At first, I was not able to communicate with the display. Fortunately, I figured out pretty quickly, that I had just used the wrong library.
After this I still was not able to see anything, but that was as I did not know, you had to set the contrast yourself.
After that, text appeared correctly.

### 3. step: Potentiometer digit mapping
Goal: show a real-time digit (0–9) on the LCD.<br>
Problem: the dial only reached 8, not 9.<br>
After looking at the output of the potentiometer never actually reaches 1023
Therefore the map()- function can fail at the upper range
I therefore edited the mapping by lowering the thresholds (while capping input possibility at 9).
This guaranteed 0–9 output.

### 4. step: Input Display improvements
Goal: better user experience.<br>
I realized that just showing the input was not user friendly at all. That is why I thought it was really important to show the live digit the user is dialing.
I was struggling to do so myself and could not get the function done. I thought about drawing indicators on the box to help, but really did not like the idea. So I asked AI for help.
It helped me write the function "ShowEnterScreen".
This really improved the user experience a lot.<br>
<br>
At the end: The input looked like this:<br>
Line 1 always shows: “Enter Code!”<br>
Line 2 shows:<br>
PIN: 0 9 _ _<br>
The current digit slot shows the live dial digit.<br>

### 5. step: Adding LED and buzzer feedback
Goal: Improving the user experience by adding physical feedback.<br>
The LED were very straightforward.<br>
RED LED for locked status.<br>
Green LED for unlocked status.<br>

For the buzzer I added short little sounds:<br>
One for input, One for success, one for wrong input.


### 6. step: Adding the servo lock
Goal: physically lock and unlock the chest.<br>
This was quite straight forward. I stored two different angles for the servo as constants. One being the locked one, and one the Open one. This made it easy to access for the needed state. 

### 7.step: Soldering
Goal: Create secure connections that last. <br>
While I was going to use the breadboard and jumper cables when possible, some parts and the wooden structure would not allow for it. Especially with how I placed the parts on the door.
Especially the LEDs and the buzzer needed to be placed securely. Therefore I took these three parts and placed them on a perfboard. I used some regular cable and cut up some jumper wires to use their ends to connect the pins directly in the Arduino.
I also was able to connect the grounds to one and safed myself some cable that way. Everything worked very well and the connections were quite secure. I added some insulations on top to be safe.<br>

The big issue was the input button. I wanted to place it on the door directly next to the potentiometer, therefore I had to place it right on top of the wood. The wood was to thick for the legs to reach through and I could not connect them to a cable. 
The solution was to cut up a small resistor and solder the wire on to those legs, elongating them. That way there were reaching through the wood and I could connect them to a wire.

### 8. step: Creating the box
Goal: Building the physical box and deploying the parts<br>
I had bought a little wooden box from Amazon. I had decided to go with wood, instead of cardboard, as stability was very important to me. Especially since I wanted to a button right on the door.<br>
First, I screwed on the hinges to connect the door. After this, I started doing the cutouts for the LCD screen, the potentiometer, the button, the LEDs and the buzzer (looking back I should have done this before connecting the door).
After this I created the lock mechanism, by drilling two bolts through the right wall and connecting them with metal wire. That way, I was able to open the box even without electricity going forward.

### 9.step: Attaching the technical parts.
Goal: Attaching all technical parts.<br>
First, I attached the breadboard to the left inside wall using power strips. After this I screwed on the lcd screen, the perfboard (including LED's and buzzer), as well as the Arduino itself. For the servo I used power strips again and the potentiometer is secured via tape.
Fortunately the button did not need anymore security, as the legs were now long enough.

### 10. step: Wiring
Goal: Finalising the build<br>
The wiring was pretty straightforward, as I just copied what I had done on the breadboard before. It took a while to do some cable management, as I was not able to close the box at first. But in the end it worked.

### 11. step: Relock function
Goal: allow the user to lock again after opening.
I figured out that one always had to restart the Arduino to lock it again. I therefore added another function and used the input button to switch from the unlocked state back to the locked state.

## 6. Possible Improvements
Locking back I have two idea for future improvements. <br>
The first one is some kind of lockout state, similar to what we have in smartphones, where you have a little cooldown, whenever you put the wrong code 3 times or so. <br>
The second is one is to further improving the cable management as there are so many and they sometimes get in the way.
