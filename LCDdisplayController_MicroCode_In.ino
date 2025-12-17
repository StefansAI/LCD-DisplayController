// include the library code:
#include <LiquidCrystal.h>

// Input data pin definition array
const int DIN[8] = { 0, 1, 2, 3, 4, 5, 6, 7 }; 

// Pin definition for the additional input 0
const int IN[4] = {A0, A1, A2, A3 };

// LCD data output pins
const int LCD_D[4] = { 8, 9, 10, 11 };

// LCD enable pin
const int LCD_E = 12;
// LCD rs pin
const int LCD_RS = 13;

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D[0], LCD_D[1], LCD_D[2], LCD_D[3]);

// Input buffer enable
const int OE[2] = { 18, 19 };


//#include "Mnemonics.c"
char* Mnemonic[] = {
	"BRK",
	"ORA (ind,X)",
	"",
	"",
	"",
	"ORA zpg",
	"ASL zpg",
	"",
	"PHP",
	"ORA #",
	"ASL A",
	"",
	"",
	"ORA abs",
	"ASL abs",
	"",
	"BPL rel",
	"ORA (ind),Y",
	"",
	"",
	"",
	"ORA zpg,X",
	"ASL zpg,X",
	"",
	"CLC",
	"ORA abs,Y",
	"",
	"",
	"",
	"ORA abs,X",
	"ASL abs,X",
	"",
	"JSR abs",
	"AND (ind,X)",
	"",
	"",
	"BIT zpg",
	"AND zpg",
	"ROL zpg",
	"",
	"PLP",
	"AND #",
	"ROL A",
	"",
	"BIT abs",
	"AND abs",
	"ROL abs",
	"",
	"BMI rel",
	"AND (ind),Y",
	"",
	"",
	"",
	"AND zpg,X",
	"ROL zpg,X",
	"",
	"SEC",
	"AND abs,Y",
	"",
	"",
	"",
	"AND abs,X",
	"ROL abs,X",
	"",
	"RTI",
	"EOR (ind,X)",
	"",
	"",
	"",
	"EOR zpg",
	"LSR zpg",
	"",
	"PHA",
	"EOR #",
	"LSR A",
	"",
	"JMP abs",
	"EOR abs",
	"LSR abs",
	"",
	"BVC rel",
	"EOR (ind),Y",
	"",
	"",
	"",
	"EOR zpg,X",
	"LSR zpg,X",
	"",
	"CLI",
	"EOR abs,Y",
	"",
	"",
	"",
	"EOR abs,X",
	"LSR abs,X",
	"",
	"RTS",
	"ADC (ind,X)",
	"",
	"",
	"",
	"ADC zpg",
	"ROR zpg",
	"",
	"PLA",
	"ADC #",
	"ROR A",
	"",
	"JMP (ind)",
	"ADC abs",
	"ROR abs",
	"",
	"BVS rel",
	"ADC (ind),Y",
	"",
	"",
	"",
	"ADC zpg,X",
	"ROR zpg,X",
	"",
	"SEI",
	"ADC abs,Y",
	"",
	"",
	"",
	"ADC abs,X",
	"ROR abs,X",
	"",
	"",
	"STA (ind,X)",
	"",
	"",
	"STY zpg",
	"STA zpg",
	"STX zpg",
	"",
	"DEY",
	"",
	"TXA",
	"",
	"STY abs",
	"STA abs",
	"STX abs",
	"",
	"BCC rel",
	"STA (ind),Y",
	"",
	"",
	"STY zpg,X",
	"STA zpg,X",
	"STX zpg,Y",
	"",
	"TYA",
	"STA abs,Y",
	"TXS",
	"",
	"",
	"STA abs,X",
	"",
	"",
	"LDY #",
	"LDA (ind,X)",
	"LDX #",
	"",
	"LDY zpg",
	"LDA zpg",
	"LDX zpg",
	"",
	"TAY",
	"LDA #",
	"TAX",
	"",
	"LDY abs",
	"LDA abs",
	"LDX abs",
	"",
	"BCS rel",
	"LDA (ind),Y",
	"",
	"",
	"LDY zpg,X",
	"LDA zpg,X",
	"LDX zpg,Y",
	"",
	"CLV",
	"LDA abs,Y",
	"TSX",
	"",
	"LDY abs,X",
	"LDA abs,X",
	"LDX abs,Y",
	"",
	"CPY #",
	"CMP (ind,X)",
	"",
	"",
	"CPY zpg",
	"CMP zpg",
	"DEC zpg",
	"",
	"INY",
	"CMP #",
	"DEX",
	"",
	"CPY abs",
	"CMP abs",
	"DEC abs",
	"",
	"BNE rel",
	"CMP (ind),Y",
	"",
	"",
	"",
	"CMP zpg,X",
	"DEC zpg,X",
	"",
	"CLD",
	"CMP abs,Y",
	"",
	"",
	"",
	"CMP abs,X",
	"DEC abs,X",
	"",
	"CPX #",
	"SBC (ind,X)",
	"",
	"",
	"CPX zpg",
	"SBC zpg",
	"INC zpg",
	"",
	"INX",
	"SBC #",
	"NOP",
	"",
	"CPX abs",
	"SBC abs",
	"INC abs",
	"",
	"BEQ rel",
	"SBC (ind),Y",
	"",
	"",
	"",
	"SBC zpg,X",
	"INC zpg,X",
	"",
	"SED",
	"SBC abs,Y",
	"",
	"",
	"",
	"SBC abs,X",
	"INC abs,X",
	"",
	""
};


// Query all data input bits and return the 8bit data value read from the input chip.
int get_data(int idx)
{
	digitalWrite(OE[idx], LOW);

  int result=0;
  for (int i=0; i<8; i++)
  {
    int value = digitalRead(DIN[i]);
    if (value == HIGH)
      result += 1<<i;    
  }

	digitalWrite(OE[idx], HIGH);
  return result;
}

void write_hex(int x, int y, int code)
{
	  // set up the LCD's number of columns and rows:
  lcd.setCursor(x, y);
  if ((code & 0xF0) == 0)
  {
    lcd.print(0);
    lcd.setCursor(x+1, y);
  }

  // Print a message to the LCD.
  lcd.print(code,HEX);

  // set up the LCD's number of columns and rows:
  lcd.setCursor(x+3, y);
}


// Variable to capture the input data from the 74HCT574 or 74HCT573
int opcode = 0;
int microstep = 0;
int flags = 0;


// Initialization function called before the loop
void setup() {
	// put your setup code here, to run once:
	for (int i = 0; i<4; i++)
 		pinMode(IN[i], INPUT);
 
	for (int i = 0; i<8; i++)
  	pinMode(DIN[i], INPUT);

  for (int i=0; i<2; i++)
	{
  	pinMode(OE[i], OUTPUT);
  	digitalWrite(OE[i], HIGH);
	}

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

char buffer[32];

// Main loop function
void loop() {
  // put your main code here, to run repeatedly:
  opcode = get_data(1);
  microstep = get_data(0);
  
	sprintf(buffer,"%s                ",Mnemonic[opcode]);
	buffer[13]= 0;

	write_hex(0, 0, microstep);

	lcd.print("T");
	lcd.print(microstep >> 1);
	lcd.print("_");
	lcd.print(microstep & 1);
  lcd.print("  ");

	lcd.setCursor(12, 0);

	if (digitalRead(IN[3]) == 0)
		lcd.print("e");
	else
		lcd.print("E");

	if (digitalRead(IN[2]) == 0)
		lcd.print("s");
	else
		lcd.print("S");

	if (digitalRead(IN[1]) == 0)
		lcd.print("h");
	else
		lcd.print("H");

	if (digitalRead(IN[0]) == 0)
		lcd.print("f");
	else
		lcd.print("F");

 	write_hex(0, 1, opcode);

   // Print a message to the LCD.
  lcd.print(buffer);

  delay(250);
}
