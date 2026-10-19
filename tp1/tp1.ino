byte D1 {5};
byte D2 {4};
byte D3 {0};
byte D4 {2};
byte D5 {14};
byte D6 {12};
byte D7 {13};
byte D8 {15};
int count = 0;
byte target = 0;
byte score = 0;
byte power = 0;
bool readyToFire = false;
byte miss = 0;
int timer = 0;

void shift1(byte v) {
	digitalWrite(D1,v);
	digitalWrite(D2, HIGH);
	digitalWrite(D2,LOW);
}

void latch1() {
	digitalWrite(D4,HIGH);
	digitalWrite(D4,LOW);
}

void clear1(){
	shift1(LOW);
	shift1(LOW);
	shift1(LOW);
	shift1(LOW);
	shift1(LOW);
	shift1(LOW);
	shift1(LOW);
	shift1(LOW);
}

void shift2(byte v){
	digitalWrite(D5,v);
	digitalWrite(D6, HIGH);
	digitalWrite(D6,LOW);
}
void latch2() {
	digitalWrite(D7,HIGH);
	digitalWrite(D7,LOW);
}

void clear2(){
	shift2(LOW);
	shift2(LOW);
	shift2(LOW);
	shift2(LOW);
	shift2(LOW);
	shift2(LOW);
	shift2(LOW);
	shift2(LOW);
}

void startGame(){
	target = 0;
	score = 0;
	readyToFire = false;
	miss = 0;

	startingAnimation();

	setRound();
}

void startingAnimation(){
	bool toggleLights = true;

	for (byte i = 0; i < 10; i++){
		clear1();
		clear2();
		for(int j = 0; j < 8; j++){
			if (toggleLights){
				shift1(HIGH);
				shift2(HIGH);
				tone(D3,(1000*i));
			} else {
				shift1(LOW);
				shift2(LOW);
				tone(D3,(8000/i));
			}
		}
		
		latch1();
		latch2();
		toggleLights = !toggleLights;
		
		delay(350);
	}
	noTone(D3);
	delay(3000);
}


void tirer(bool hit){
	readyToFire = false;

	fireSound();
	delay(500);

	if(hit){
		// ascendint tone for hit
		tone(D3,4000);
		delay(200);
		tone(D3,5000);
		delay(200);
		noTone(D3);
		score++;
		pointAnimation();
	} else {
		// descending tone for miss
		tone(D3,4000);
		delay(200);
		tone(D3,3000);
		delay(200);
		noTone(D3);
		miss++;
		if(miss == 1){
			missAnimation();
		}
	}

	delay(200);
	power = 0;
	setRound();
}

void pointAnimation(){
	bool toggleLights = true;
	for (int i = 0; i < 11; i++){
		clear2();
		if (toggleLights){
			if (score == 1){
				shift2(HIGH);
				shift2(LOW);
				shift2(LOW);
				shift2(miss);
			} else if (score == 2){
				shift2(HIGH);
				shift2(HIGH);
				shift2(LOW);
				shift2(miss);
			} else if (score == 3){
				shift2(HIGH);
				shift2(HIGH);
				shift2(HIGH);
				shift2(miss);
			}
		} else {
			if (score == 1){
				shift2(LOW);
				shift2(LOW);
				shift2(LOW);
				shift2(miss);
			} else if (score == 2){
				shift2(HIGH);
				shift2(LOW);
				shift2(LOW);
				shift2(miss);
			} else if (score == 3){
				shift2(HIGH);
				shift2(HIGH);
				shift2(LOW);
				shift2(miss);
			}
		}
		toggleLights = !toggleLights;
		latch2();
		delay(100);
	}
	delay(500);
}

void missAnimation(){
	bool toggleLights = true;
	
	for (int i = 0; i < 11; i++){
		clear2();
		setPoints();
		
		if(toggleLights) {
			shift2(HIGH);
		} else {
			shift2(LOW);
		}

		toggleLights = !toggleLights;
		latch2();
		delay(100);
	}
	
	

}

void fireSound(){
	tone(D3,4000);
	delay(150);
	tone(D3,3900);
	delay(150);
	tone(D3,3800);
	delay(150);
	tone(D3,3700);
	delay(150);
	tone(D3,3600);
	delay(150);
	tone(D3,3500);
	delay(150);
	noTone(D3);
}

void setRound(){
	Serial.println("setRound");
	if (score < 3){
		if (miss < 2){
			selectTarget();
			setPoints();
			setMiss();

			latch2();
			delay(200);
			readyToFire = true;
		} else {
			clear2();
			latch2();
			delay(250);
			lossAnimation();
		}

	} else {
		winAnimation();
	}
}

void setPoints(){
	if(score > 0){  	//Q3
		shift2(HIGH); 
	} else {
		shift2(LOW);
	}
	if(score > 1){ 		//Q2
		shift2(HIGH); 
	} else {
		shift2(LOW);
	}
	if(score > 2){ 		//Q1
		shift2(HIGH);
	} else {
		shift2(LOW);
	}
}

void setMiss(){
	if (miss == 0){ 	//Q0
		shift2(LOW);
	} else {
		shift2(HIGH);
	}
}

void winAnimation(){
	bool toggleLights = true;
	byte sound = 1;
	for (byte i = 0; i < 10; i++){
		clear2();
		if (toggleLights){
			shift2(HIGH);
			shift2(HIGH);
			shift2(HIGH);
		} else {
			shift2(LOW);
			shift2(LOW);
			shift2(LOW);
		}
		shift2(LOW);
		latch2();
		if(sound == 4){
			sound = 1;
		} else {
			sound++;
		}
		tone(D3,(1500 * sound));

		toggleLights = !toggleLights;
		delay(200);
	}
	noTone(D3);
	delay(2000);
	startGame();
}

void lossAnimation(){
	bool toggleLights = true;
	byte sound = 1;
	for (byte i = 0; i < 11; i++){
		clear2();
		if (toggleLights){
			shift2(HIGH);
			shift2(HIGH);
			shift2(HIGH);

		} else {
			shift2(LOW);
			shift2(LOW);
			shift2(LOW);
		}

		shift2(LOW); //points & miss LED
		shift2(LOW);
		shift2(LOW);
		shift2(LOW);
		latch2();
		if(sound == 3){
			tone(D3, 4000);
		} else if (sound == 4) {
			sound = 1;
			tone(D3,3000);
		} else {
			tone(D3,3000);
		}

		sound++;
		toggleLights = !toggleLights;
		delay(200);
		noTone(D3);
		delay(50);
	}
	noTone(D3);
	delay(2000);
	startGame();
}

void selectTarget(){
	target = random(1,4);
	clear2();
	targetAnimation();
	delay(200);
	clear2();
	if(target == 1){
		shift2(LOW);
		shift2(LOW);
		shift2(HIGH);
	} else if (target == 2){
		shift2(LOW);
		shift2(HIGH);
		shift2(LOW);
	} else if (target == 3){
		shift2(HIGH);
		shift2(LOW);
		shift2(LOW);
	} 
	setPoints();
	setMiss();
	latch2();
	delay(1000);

	clear2();
	latch2();
}

void targetAnimation(){
	bool toggleLights = true;
	for(int i = 0; i < 10;i++){
		clear2();
		if(toggleLights){
			shift2(HIGH);
			shift2(HIGH);
			shift2(HIGH);
		} else {
			shift2(LOW);
			shift2(LOW);
			shift2(LOW);
		}
		toggleLights = !toggleLights;
		setPoints(); 
		setMiss();
		latch2();
		delay(150);
	}

}

void setup(){
	pinMode(D1, OUTPUT); //Serial 1
	pinMode(D2, OUTPUT); //Shift 1
	pinMode(D3, OUTPUT); //Buzzer
	pinMode(D4, OUTPUT); //Latch 1
	pinMode(D5, OUTPUT); //Serial 2
	pinMode(D6, OUTPUT); //Shift 2
	pinMode(D7, OUTPUT); //Latch 2
	pinMode(D8, INPUT);
	Serial.begin(115200);
	startGame();
}

void charge(){
	if (count == 30000){
		power++;
		shift1(HIGH);
		tone(D3,600);
	}
	if (count == 60000){
		shift1(HIGH);
		tone(D3,800);
	}
	if (count == 90000){
		power++;
		shift1(HIGH);
		tone(D3,1000);
	}
	if (count == 120000){
		shift1(HIGH);
		tone(D3,1200);
	}
	if (count == 150000){
		power++;
		shift1(HIGH);
		tone(D3,1400);
	}
	if(count == 210000){
		tirer(false);
		clear1();
	}
}

void loop() {
	byte buttonState = digitalRead(D8);

	//if (readyToFire){
		//
	//}

	if(buttonState == HIGH && readyToFire){
		count++;
	}  else {
		clear1();
		latch1();
		if (count >= 30000 && count < 210000){
			if (power == target){
				tirer(true);
			} else {
				tirer(false);
			}
		}

		count = 0;
		
	}

	charge();
	latch1();
}
