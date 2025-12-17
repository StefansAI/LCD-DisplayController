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

char* AddrEn[] = {
//   12345678
	"/PC_OUT_EN",
	"/PC_OUT_EN++",
	"/AR_OUT_EN",
	"/AR_OUT_EN++",
	"/AR_OUT_EN_WR",
	"/AR_OUT_EN_WR++",
	"/SP_OUT_EN",
	"/SP_OUT_EN_WR"
};

char* OutEn[] = {
//   12345678
	"",
	"none_1",
	"none_2",
	"none_3",
	"none_4",
	"none_5",
	"none_6",
	"none_7",
	"/OE_iDB",
	"/OE_iDB_LD_SP",
	"/OE_A_BCD",
	"/OE_A",
	"/OE_X",
	"/OE_Y",
	"/OE_SP",
	"/OE_SR",
	"/OE_PC_H",
	"/OE_PC_L",
	"/OE_PC_L_BR",
	"/OE_BRKL",
	"/OE_SIX_L",
	"/OE_SIX_H",
	"/CH_SR",
	"/ACK_EXC",
	"/OE_ALU",
	"/OE_ALU_NZ",
	"/OE_ALU_NZC",
	"/OE_ALU_NZCV",
	"/OE_ALU_SH",
	"/OE_ALU_HDCY",
	"/OE_ALU_FDCY",
	"/OE_ALU_CY"
};

char* LoadCode[] = {
//   12345678
	"",
	"/LD_IR",
	"/LD_A",
	"/LD_X",
	"/LD_Y",
	"/LD_AL",
	"/LD_PC_H",
	"/LD_AR_ZP",
	"/LD_AR_H",
	"/LD_ALU_A",
	"/LD_ALU_B",
	"/LD_ALU_AB",
	"/LD_ALU_AH",
	"/LD_SR",
	"/LD_SP*",
	"/LD_SP"
};

char* AluCode[] = {
//   12345678
	"A+Cy",
	"(A|B)+Cy",
	"(A|!B)+Cy",
	"-1+Cy",
	"A+(A&/B)+Cy",
	"(A|B)+(A&!B)+Cy",
	"A-B-!Cy",
	"A&!B",
	"A+(A&B)+Cy",
	"A+B+Cy",
	"(A|!B)+(A&B)+Cy",
	"(A&B)-!Cy",
	"A+A+Cy",
	"(A|B)+A+Cy",
	"(A|!B)+A+Cy",
	"A-!Cy",
	"!A",
	"A|!B",
	"A&!B",
	"0",
	"A&!B",
	"!B",
	"A^B",
	"A&!B",
	"!A|B",
	"A^!B",
	"B",
	"A&B",
	"1",
	"A|!B",
	"A|B",
	"A"
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

// Variable to capture the input data from the 74HCT574 or 74HCT573
int addr_en = 0;
int out_en = 0;
int load_code = 0;
int alu_code = 0;
int cy_in = 0;


// Initialization function called before the loop
void setup() {
	// put your setup code here, to run once:
		for (int i = 0; i<4; i++)
 		pinMode(IN[i], INPUT);

	for (int i = 0; i<8; i++)
  	pinMode(DIN[i], INPUT_PULLUP);

  for (int i=0; i<2; i++)
	{
  	pinMode(OE[i], OUTPUT);
  	digitalWrite(OE[i], HIGH);
	}

  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
}

char buffer[64];
                 //12345678901234567890
char* sformat = "%s                     ";

// Main loop function
void loop() {
  // put your main code here, to run repeatedly:
  int a = get_data(0);
  int b = get_data(1);

  addr_en = a & 0x07;
  out_en = (a >> 3) & 0x1F;
  load_code = b & 0x0F;
  alu_code = (b >> 4) & 0x0F;
  
  if (digitalRead(IN[1]) == HIGH)
		alu_code |= 1<<4;

  if (digitalRead(IN[2]) == HIGH)
		cy_in = 0;
  else
		cy_in = 1;
								  
  lcd.setCursor(1, 0);
	lcd.print(addr_en);
  lcd.setCursor(3, 0);
	sprintf(buffer,sformat,AddrEn[addr_en]);
	buffer[16]= 0;
  lcd.print(buffer);

  lcd.setCursor(0, 1);
  lcd.print(out_en >> 4);
 	lcd.print(out_en & 0xF, HEX);
  lcd.setCursor(3, 1);
	sprintf(buffer,sformat,OutEn[out_en]);
	buffer[16]= 0;
  lcd.print(buffer);

  lcd.setCursor(1, 2);
 	lcd.print(load_code, HEX);
  lcd.setCursor(3, 2);
 	sprintf(buffer,sformat,LoadCode[load_code]);
	buffer[16]= 0;
  lcd.print(buffer);

  lcd.setCursor(0, 3);
 	lcd.print(alu_code >> 4);
	lcd.print(alu_code & 0xF, HEX);
  lcd.setCursor(3, 3);
	sprintf(buffer,sformat,AluCode[alu_code]);
	buffer[16]= 0;
  lcd.print(buffer);

  if ((alu_code >> 4) == 0)
	{
		 lcd.setCursor(strlen(AluCode[alu_code])+3, 3);
		 lcd.print("=");
		 lcd.print(cy_in);
	}

  delay(250);
}

