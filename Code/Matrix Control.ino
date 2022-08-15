#define A {B01111110,B10000001,B10000001,B10000001,B10000001,B11111111,B10000001,B10000001,B10000001,B10000001}
#define B {B11111110,B10000001,B10000001,B10000001,B11111110,B11111110,B10000001,B10000001,B10000001,B11111110}
#define C {B01111111,B10000000,B10000000,B10000000,B10000000,B10000000,B10000000,B10000000,B10000000,B01111111}
#define D {B11111110,B10000001,B10000001,B10000001,B10000001,B10000001,B10000001,B10000001,B10000001,B11111110}
#define E {B11111111,B10000000,B10000000,B10000000,B11111000,B11111000,B10000000,B01000000,B10000000,B11111111}
#define Fx {B11111111,B10000000,B10000000,B11111000,B11111000,B10000000,B10000000,B10000000,B10000000,B10000000}
#define G {B00011111,B00100000,B01000000,B10000000,B10000000,B10011111,B10011111,B01000001,B00100001,B00011111}
#define H {B10000001,B10000001,B10000001,B10000001,B11111111,B11111111,B10000001,B10000001,B10000001,B10000001}
#define I {B11111111,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000,B11111111}
#define J {B11111111,B11111111,B00011000,B00011000,B00011000,B00011000,B00011000,B10011000,B1111100,B01110000}
#define N {B10000001,B11000001,B11100001,B11110001,B10111001,B10011101,B10001111,B10000111,B10000011,B10000001}
#define M {B10000001,B11000011,B11100111,B10111101,B10011001,B10000001,B10000001,B10000001,B10000001,B10000001}
#define T {B11111111,B11111111,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000}
#define S {B11111111,B10000000,B10000000,B10000000,B10000000,B11111111,B00000001,B00000001,B00000001,B11111111}
#define P {B11111110,B10000001,B10000001,B10000001,B10000001,B11111110,B10000000,B10000000,B10000000,B10000000}
#define Y {B00000000,B10000001,B01000010,B00100100,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000}
#define Block {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111}                    

int latchPin = 10;
int clockPin = 13;
int dataPin = 11;
int clock = 9;
int Reset = 8;
int latchPinPORTB = latchPin - 8;
int clockPinPORTB = clockPin - 8;
int dataPinPORTB = dataPin - 8;
int i = 0;
long scrolling_word[10];
int array_turn=0;
byte your_text[56][10]={A,B,C,D,E,Fx,G,H,I,J,N,M,T,S,P,Y}; // Text goes here


void setup(){
  Serial.begin(9600);
  pinMode(dataPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(latchPin,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(Reset,OUTPUT);
  digitalWrite(Reset,HIGH);
  digitalWrite(Reset,LOW);
  setupSPI();
}

void display_word(int loops,byte word_print[][10],int num_patterns,int delay_langth){// this function displays your symbols
  i = 0;// resets the counter fot the 4017
  for(int g=0;g<10;g++)//resets the the long int where your word goes
    scrolling_word[g] = 0;
  for(int x=0;x<num_patterns;x++){//main loop, goes over your symbols
   // you will need to find a better way to make the symbols scroll my way is limited for 24 columns

   for(int r=0;r<10;r++)//puts the buildes the first symbol
      scrolling_word[r] |= word_print[x][r]; 
    for (int z=0;z<10;z++){//the sctolling action
        for(int p=0;p<10;p++)
          scrolling_word[p] = scrolling_word[p] << 1;
// end of the scrolling funcion
      for(int t=0;t<delay_langth;t++){// delay function, it just loops over the same display
        for(int y=0;y<10;y++){// scaning the display
          if(i == 10){// counting up to 6 with the 4017
            digitalWrite(Reset,HIGH);
            digitalWrite(Reset,LOW);
            i = 0;
          }
          latchOff();
          spi_transfer(make_word(0x01000000,y));// sending the data
          spi_transfer(make_word(0x00010000,y));
          spi_transfer(make_word(0x00000100,y));
          latchOn();
          delayMicroseconds(800);//waiting a bit
          latchOff();
          spi_transfer(0);// clearing the data
          spi_transfer(0);
          spi_transfer(0);
          latchOn();
          digitalWrite(clock,HIGH);//counting up with the 4017
          digitalWrite(clock,LOW);
          i++;
        }
      }
    }
  }
  finish_scroll(delay_langth);
}

void finish_scroll(int delay_scroll){// this function is the same as the funcion above, it just finishing scrolling
  for (int n=0;n<56;n++){
        for(int h=0;h<10;h++)
          scrolling_word[h] = scrolling_word[h] << 1;
      for(int w=0;w<delay_scroll;w++){
        for(int k=0;k<10;k++){
          if(i == 10){
            digitalWrite(Reset,HIGH);
            digitalWrite(Reset,LOW);
            i = 0;
          }
          latchOff();
          spi_transfer(make_word(0x01000000,k));
          spi_transfer(make_word(0x00010000,k));
          spi_transfer(make_word(0x00000100,k));
          latchOn();
          delayMicroseconds(800);
          latchOff();
          spi_transfer(0);
          spi_transfer(0);
          spi_transfer(0);
          latchOn();
          digitalWrite(clock,HIGH);
          digitalWrite(clock,LOW);
          i++;
        }
      }
    }
}

byte make_word (long posistion,byte turn){
  byte dummy_word = 0;
  for(int q=0;q<56;q++){
    if(scrolling_word[turn] & (posistion<<q))
      dummy_word |= 0x01<<q;
  }
  return dummy_word;
}   


  void loop() {

        display_word(1,your_text,56,5);// calls for the display_pattern function and says that int loop = 15(if you do more loop the pattern whould scrole slower).
   
       }
  
  

void latchOn(){
  bitSet(PORTB,latchPinPORTB);
}

void latchOff(){
  bitClear(PORTB,latchPinPORTB);
}


void setupSPI(){
  byte clr;
  SPCR |= ( (1<<SPE) | (1<<MSTR) ); // enable SPI as master
  //SPCR |= ( (1<<SPR1) | (1<<SPR0) ); // set prescaler bits
  SPCR &= ~( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
  clr=SPSR; // clear SPI status reg
  clr=SPDR; // clear SPI data reg
  SPSR |= (1<<SPI2X); // set prescaler bits
  //SPSR &= ~(1<<SPI2X); // clear prescaler bits

  delay(10);
}
byte spi_transfer(byte data)
{
  SPDR = data;        // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;        // return the received byte, we don't need that
}
