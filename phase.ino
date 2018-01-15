#include<math.h>
// #include<avr/io.h>
// #include<avr/interrupt.h>
// #include <util/delay.h>


cons int SW = 0;
const int PWM_pin = 5; //Digital pin 5 is PWM
const int time_L = 60; // LED blinking 
const float dt = 0.001; 
const float omega0 = 10.0;
const float domega = 1.0;

int Switch_old = 0;            
int Status = 0;            // angular frequency
int Status_light = 0;      
int Status_phipast = 1;    
int timer_LED = 0;         
float phi=0.0;             
float omega = 0.0;         

int counter_w = 0;

// phase update

void Phase_update(){
	phi += omega*dt;
	phi = Phase_reset(phi);
	Phase_LED(phi);
}


//reset phase

float Phase_reset(float p){
	float p_out=p;
	if(p_out>M_PI){
		p_out = p_out - 2*M_PI;
		Status_phipast = 0;
	}
	return p_out;
}

//LED lit by phase

void Phase_LED(float p){
	if(!Status_phipast){
		if(p>0){
			Status_phipast = 1;
//      	digitalWrite(LED, HIGH);
			OCR0B = 2;    // change duty cycle
			Status_light = 1;
		}
	}
	if(Status_light){
		timer_LED++;
			if(timer_LED>time_L){
//      		digitalWrite(LED, LOW);
				OCR0B = 4;    // Duty cycle
				Status_light = 0;
				timer_LED = 0;
			}
	}
}

// Have to change this switch with keypress
// Switch
void SWITCH_stand(){
  int Switch;
  Switch = digitalRead(SW);
  if(Switch){
    if(!Switch_old){
      Status++;
      if(Status==4){
        Status=0;
      }
      omega=OMEGA_change(Status);
    }
  }
  Switch_old = Switch;
}

// omega change
float OMEGA_change(int S){
  float rom=0.0;
  switch(S){
    case 0: rom = 0.0;
    break;
    case 1: rom = omega0;
    break;
    case 2: rom = omega0+domega;
    break;
    case 3: rom = omega0-domega;
    break;
  }
  return rom;
}

// Setup
void setup() {
	pinMode(PWM_pin, OUTPUT);  
	pinMode(SW, INPUT);
	cli();
	
//  TCCR1 |= (0<<CS13) | (1<<CS12) | (0<<CS11) | (0<<CS10);// 1[μs]
	
	TCCR0A=0xF3;           // OCR0A and OCR0B is fast pwm
	TCCR0B=0x0A;           // Prescaler equal to 1MHz
	OCR0A = 4;             //Set top counter frequency 
	OCR0B = 4;             //duty(OCR0B+1)/(OCR0A+1), this duty cycle is 100%
	TIMSK0 |= (1 << OCIE0A);    //Set the ISR COMPA vect
	
//	TIMSK0 = (1 << OCIE0A);  // set the ISR COMPA vect
// 	TIMSK1 |= (1<< OCIE1A);  //Set interrupt on compare match  
//	TCCR1=0x97;            // 125[kHz] using timer 1
//	OCR1C = 124;            // 1[ms]ごとにISR(TIM1_COMPA_vect) を実行
//  TIMSK = (1 << OCIE1A);  // (timer1)

	TCCR1A = 0x50; //OC1A/OC1B in toggle state
	TCCR1B = 0x0B; //set to CTC and 125kHz frequency.

	sei(); //enable the interrupt
}

// Loop
void loop() {
}

// Timer0 
//ISR(TIMER1_COMPA_vect)
//{
//	SWITCH_stand();
//	Phase_update();
//}




// Timer1 割り込み関数
ISR(TIMER0_COMPA_vect)  
{

	SWITCH_stand();
	Phase_update();
	
//周期短いとムリゲー
//  int l,s;
//l=digitalRead(LED);
//digitalWrite(LED,!l);
//  s=digitalRead(SW);
//  if(s){
//    OCR0B = 2;    // デューティ比変更
//  }else{
//    OCR0B = 4;    // デューティ比変更
//  }  
}
