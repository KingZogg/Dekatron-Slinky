/*
Dekatron Slinky Example - with Timer IRQ

modified April 7, 2017
by Michael Moorrees
*/
#include <TimerOne.h>

int swing = 2;        // swing range - 2 to 30
int Scnt = 0;         // step count
int state = 0;        // State - Machine State

int dk_ctA = 0;        // Dekatron Guide count - 0,1, or 2
int dk_ctB = 0;        // Dekatron Guide count - 0,1, or 2
int dk_ctC = 0;        // Dekatron Guide count - 0,1, or 2
int dk_ctD = 0;        // Dekatron Guide count - 0,1, or 2



int LED = 13;          // Test LED


const int Guide1A = 52;       // Guide 1 - G1 pin of 2-guide Dekatron
const int Guide2A = 50;       // Guide 2 - G2 pin of 2-guide Dekatron
const int IndexA = 48;       // Index   - NDX input pin. High when glow at K0

const int Guide1B = 44;       // Guide 1 - G1 pin of 2-guide Dekatron
const int Guide2B = 42;       // Guide 2 - G2 pin of 2-guide Dekatron
const int IndexB = 40;       // Index   - NDX input pin. High when glow at K0

const int Guide1C = 36;       // Guide 1 - G1 pin of 2-guide Dekatron
const int Guide2C = 34;       // Guide 2 - G2 pin of 2-guide Dekatron
const int IndexC = 32;       // Index   - NDX input pin. High when glow at K0

const int Guide1D = 28;       // Guide 1 - G1 pin of 2-guide Dekatron
const int Guide2D = 26;       // Guide 2 - G2 pin of 2-guide Dekatron
const int IndexD = 24;       // Index   - NDX input pin. High when glow at K0


int Ndx = 0;          // K0 index indicator variable
int Tick = false;     // 1mS tick

int Acceleration = 0;
int Counter = 0;





void d_step(int dk_bt,int Index, int Guide1,int Guide2) // Dekatron Step
{
	if (digitalRead(Index)) Ndx = true;   // Sample for glow at K0
	switch (dk_bt) {
	case 0:                             // glow at a main cathode
		digitalWrite(Guide1, LOW);
		digitalWrite(Guide2, LOW);
		break;
	case 1:                             // glow to a Guide 1 cathode
		digitalWrite(Guide1, HIGH);
		digitalWrite(Guide2, LOW);
		break;
	case 2:                             // glow to a Guide 2 cathode
		digitalWrite(Guide1, LOW);
		digitalWrite(Guide2, HIGH);
	}
}

void D_advA()                    // Dekatron Advance - Clockwise
{
	dk_ctA++;
	if (dk_ctA == 3) dk_ctA = 0;
	d_step(dk_ctA,IndexA,Guide1A,Guide2A);
}

void D_revA()                    // Dekatron Reverse - Counter-Clockwise
{
	dk_ctA--;
	if (dk_ctA == -1) dk_ctA = 2;
	d_step(dk_ctA, IndexA, Guide1A, Guide2A);

}

void D_advB()                    // Dekatron Advance - Clockwise
{
	dk_ctB++;
	if (dk_ctB == 3) dk_ctB = 0;
	d_step(dk_ctB, IndexB, Guide1B, Guide2B);
}

void D_revB()                    // Dekatron Reverse - Counter-Clockwise
{
	dk_ctB--;
	if (dk_ctB == -1) dk_ctB = 2;
	d_step(dk_ctB, IndexB, Guide1B, Guide2B);

}

void D_advC()                    // Dekatron Advance - Clockwise
{
	dk_ctC++;
	if (dk_ctC == 3) dk_ctC = 0;
	d_step(dk_ctC, IndexC, Guide1C, Guide2C);
}

void D_revC()                    // Dekatron Reverse - Counter-Clockwise
{
	dk_ctC--;
	if (dk_ctC == -1) dk_ctC = 2;
	d_step(dk_ctC, IndexC, Guide1C, Guide2C);

}

void D_advD()                    // Dekatron Advance - Clockwise
{
	dk_ctD++;
	if (dk_ctD == 3) dk_ctD = 0;
	d_step(dk_ctD, IndexD, Guide1D, Guide2D);
}

void D_revD()                    // Dekatron Reverse - Counter-Clockwise
{
	dk_ctD--;
	if (dk_ctD == -1) dk_ctD = 2;
	d_step(dk_ctD, IndexD, Guide1D, Guide2D);

}

void dk_action0() {             // Dekatron Action Routine 0 - Grow from Bottom, Forward (cw)
	//Serial.println("Action 0");
	//delay(100);
	
	if (Ndx) {                   //   When swing hits Ndx [K0] cathode, then max swing is achieved 
		Ndx = false;                //   Set vars for next state, and jump to state 2
		state = 2;
		swing = 30;
		Scnt = (swing / 2) - 1;
		digitalWrite(LED, HIGH);
	}
	else {                        // If not at K0 yet, but stepped "swing" number of steps
		if (Scnt == 0) {            //   increment swing length, preset count
			swing++;                  //   and jump to state 1, for reverse stepping
			Scnt = swing;
			state = 1;
		}
		else {                      // otherwise, continue stepping forward,
			Scnt--;                   //   while decrementing step counter.
		//	D_advA();
			D_advB();
			D_advC();
			D_advD();

		}
	}
}

void dk_action1() {             // Dekatron Action Routine 1 - Grow from, either bottom, or top, Reverse (ccw)
	//Serial.println("Action 1");
	//delay(100);
	if (Scnt == 0) {              //   at full steps (count = 0), increase swing, preset count
		state--;                    //   and jump to forward growth state (0 or 4)
		swing++;                    //   This routine is called while in either state 1 (bottom) or state 5 (top)
		Scnt = swing;
	}
	else {                        // If not yet at full step
		Scnt--;                     //   contiue stepping in reverse
	//	D_revA();
		D_revB();//   while drecrementing counter
		D_revC();
		D_revD();
	}
}

void dk_action2() {             // Dekatron Action Routine 2 - Shrink from, either bottom, or top, Forward (cw)
	//Serial.println("Action 2");
	//delay(100);
	if (swing < 2) {              //   until swing is at minimum (2)
		state = (state + 2) & 7;    //   then jump to state 4 (if at 2 - top) or 0 (if at 6 - bottom)
									//  if (state == 2) state = 4;
									//  if (state == 6) state = 0;
		swing = 2;
		Scnt = swing;
		Ndx = false;
	}
	else {                        // at full forward step (count = 0),
		if (Scnt == 0) {            //   decrease swing length, preset count
			swing--;                  //   and go to reverse shrink state:
			Scnt = swing;             //   state 3 (if at state 2 - top) or
			state++;                  //   state 7 (if at state 6 - bottom)
		}
		else {
			Scnt--;                   // Otherwise, contine stepping forward
		//	D_advA();
			D_advB();
			D_advC();
			D_advD();
		}

	}
}

void dk_action3() {             // Dekatron Action Routine 3 - Shrink from, either bottom, or top, Reverse (ccw)
	//Serial.println("Action 3");
	//delay(100);
	if (Scnt == 0) {              //   at full steps (count = 0),
		state--;                    //   decrease swing length, and jump to
		swing--;                    //   state 2 (if at state 3 - top) or
		Scnt = swing;               //   state 6 (if at state 7 - bottom)
	}
	else {                        // Otherwise, continue stepping in reverse
		Scnt--;                     //   while decrementing counter
	//	D_revA();
		D_revB();//   while drecrementing counter
		D_revC();
		D_revD();
	}
}

void dk_action4() {             // Dekatron Action Routine 4 - Grow from Top, Forward (cw)
	//Serial.println("Action 4");
	//delay(100);
	if (swing >= 30) {            //   When swing length reaches 30 (maximum),
		state = 6;                   //   jump to state 6, and preset swing and count
		swing = 30;
		Scnt = (swing / 2) - 1;
	}
	else {                        // If not yet at full swing, but hit maximum steps,
		if (Scnt == 0) {             //   (count = 0), then increase swing length,
			swing++;                    //   preset count and go to state 5 (reverse)
			Scnt = swing;
			state = 5;
		}
		else {                       // Otherwisw, continue stepping forward
			Scnt--;                     //   while decrementing counter.
		//	D_advA();
			D_advB();
			D_advC();
			D_advD();
		}
	}
}


void dk_action10() {             
	Serial.println("Action 10");
	//Serial.println(Acceleration);
	Serial.println(Counter);
	
	if (Counter >= 50) //if (Ndx)Acceleration = Acceleration + pow(2, Acceleration); // if you use 200  (or 50 with flip on) as the counter size it's an interesting effect.
	{
		Counter = 0;
		Acceleration = 0;

		
	}
	Counter++;
	
	
	delay(Acceleration);
	if (IndexB)Acceleration = Acceleration + pow(1, Acceleration);
	D_advB();
}


void dk_action11() {                      //Seconds
	Serial.println("Action 11");
	if (digitalRead(IndexA)) delay(1000);   // Sample for glow at K0
	delay(10);
	D_advA();
	
}

void dk_action12() {                      //Seconds
	Serial.println("Action 11");
	if (digitalRead(IndexB)) delay(1000);   // Sample for glow at K0
	delay(10);
	
	D_advB();
	
}
void dk_action13() {                      //Seconds
	Serial.println("Action 11");
	if (digitalRead(IndexC)) delay(1000);   // Sample for glow at K0
	delay(10);
	
	
	D_advC();
	
}
void dk_action14() {                      //Seconds
	Serial.println("Action 11");
	if (digitalRead(IndexD)) delay(1000);   // Sample for glow at K0
	delay(10);
	
	D_advD();
}


//void dk_action12() {                     
//	Serial.println("Action 12");
//	
//	D_advB();
//}



// setup() runs once, at reset, to initialize system
// initialize hardware and preset variables
void setup() {
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerISR);
	
	pinMode(Guide1A, OUTPUT);
	pinMode(Guide2A, OUTPUT);
	pinMode(IndexA, INPUT);

	pinMode(Guide1B, OUTPUT);
	pinMode(Guide2B, OUTPUT);
	pinMode(IndexB, INPUT);

	pinMode(Guide1C, OUTPUT);
	pinMode(Guide2C, OUTPUT);
	pinMode(IndexC, INPUT);

	pinMode(Guide1D, OUTPUT);
	pinMode(Guide2D, OUTPUT);
	pinMode(IndexD, INPUT);
	
	pinMode(LED, OUTPUT);

	digitalWrite(LED, LOW);
	swing = 2;
	Scnt = 0;
	state = 0;
	Ndx = 0;
	D_advA();
	D_advB();
	D_advC();
	D_advD();

}

// Main Loop - Slinky Pattern State Machine
//
//  States: 0 <--> 1 until max swing [Growing, at bottom]
//          2 <--> 3 until min swing [Shrinking, at top]
//          4 <--> 5 until max swing [Growing, at top]
//          6 <--> 7 until min swing [Shrinking, at bottom]
//          at min go back to state 0
//
void loop() {

	dk_action11();
	//dk_action12();
	//dk_action13();
	//dk_action14();
	
	
	
}

void timerISR() {
	Tick = true;



	if (Tick) {
		Tick = false;
		switch (state) {             // Do action per current state
		case 0:
			dk_action0();            // Grow from Bottom, Forward
			break;
		case 1:
			dk_action1();            // Grow, Reverse
			break;
		case 2:
			dk_action2();            // Shrink, Forward
			digitalWrite(LED, LOW);
			break;
		case 3:
			dk_action3();            // Shrink, Reverse
			break;
		case 4:
			dk_action4();            // Grow from Top, Forward
			break;
		case 5:
			dk_action1();            // Grow, Reverse
			break;
		case 6:
			dk_action2();            // Shrink, Forward
			break;
		case 7:
			dk_action3();            // Shrink, Reverse
		}



		// delay(1);                    // Pause 1mS
	}

}