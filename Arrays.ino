


#define ELEMENTS(x)   (sizeof(x) / sizeof(x[0]))

int one[] = { 2,3,4,5,6 };     // Let the computer count how many elements you need

void setup() {
	Serial.begin(9600);
	myLED(one);              // pass along the 5 numbers to the function 'myLED()'
	yourLED(one);
}



void myLED(int myArray[]) {
	int i;

	Serial.println("In myLed:");

	for (i = 0; i < ELEMENTS(one); i++) {
		Serial.print(myArray[i]);             // Array indexing
		Serial.print("  ");
	}
	Serial.println();
}

void yourLED(int myArray[]) {
	int i;

	Serial.println("In yourLed:");

	for (i = 0; i < ELEMENTS(one); i++) {
		Serial.print(*myArray++);             // pointers
		Serial.print("  ");
	}
	Serial.println();
}
void loop() {
}
