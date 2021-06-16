/**
 * This sketch programs the microcode EEPROMs for the 8-bit breadboard computer
 * It includes support for a flags register with carry and zero flags
 * See this video for more: https://youtu.be/Zg1NdPKoosU
 */

//#define CHIP1
//#define CHIP2
#define CHIP3

#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

#ifdef CHIP1
  #define HLT 0b10000000 // Halt clock
  #define CI  0b01000000 // Jump (program counter in)
  #define CO  0b00100000 // Program counter out
  #define CE  0b00010000 // Program counter enable
  #define MI  0b00001000 // Memory address in
  #define RI  0b00000100 // RAM data in
  #define RO  0b00000010 // RAM data out
  #define OI  0b00000001 // IO in
  #define OO  0
  #define AI  0
  #define AO  0
  #define FI  0
  #define FR  0
  #define II  0
  #define IR  0
  #define FS  0
  #define SI  0
  #define SO  0
  #define S0  0
  #define S1  0
  #define S2  0
  #define S3  0
  #define SC  0
  #define SM  0
#endif

#ifdef CHIP2
  #define HLT 0
  #define CI  0
  #define CO  0
  #define CE  0
  #define MI  0
  #define RI  0
  #define RO  0
  #define OI  0
  #define OO  0b10000000 // IO out
  #define AI  0b01000000 // A register in
  #define AO  0b00100000 // A register out
  #define FI  0b00010000 // Flag register in
  #define FR  0b00001000 // Flag register reset
  #define II  0b00000100 // Instruction register in
  #define IR  0b00000010 // Microcode counter reset
  #define FS  0b00010001 // Flag register set.  This also sets Flags In.
  #define SI  0
  #define SO  0
  #define S0  0
  #define S1  0
  #define S2  0
  #define S3  0
  #define SC  0
  #define SM  0
#endif

#ifdef CHIP3
  #define HLT 0
  #define CI  0
  #define CO  0
  #define CE  0
  #define MI  0
  #define RI  0
  #define RO  0
  #define OI  0
  #define OO  0
  #define AI  0
  #define AO  0
  #define FI  0
  #define FR  0
  #define II  0
  #define IR  0
  #define FS  0
  #define SI  0b10000000 // ALU register in
  #define SO  0b01000000 // ALU out
  #define S0  0b00100000 // ALU S0
  #define S1  0b00010000 // ALU S1
  #define S2  0b00001000 // ALU S2
  #define S3  0b00000100 // ALU S3
  #define SC  0b00000010 // ALU Carry in
  #define SM  0b00000001 // ALU Math / Logic flag
#endif

#define FLAGS_Z0C0 0
#define FLAGS_Z0C1 2
#define FLAGS_Z1C0 1
#define FLAGS_Z1C1 3

byte UCODE_TEMPLATE[32][8] = {
  { CO|MI,  RO|II|CE,                                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  
  { CO|MI,  RO|II|CE,  CO|MI,  RO|AI|CE,                                    IR, IR, IR, IR },   // 0x01 LDA#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|AI,                             IR, IR, IR },   // 0x02 LDA
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|MI,         RO|AI,                  IR, IR },   // 0x03 LDA*
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     AO|RI,                             IR, IR, IR },   // 0x04 STA
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|MI,         AO|RI,                  IR, IR },   // 0x05 STA*

  { CO|MI,  RO|II|CE,  CO|MI,  RO|SI|CE,     SO|AI|S0|S3|FI,                    IR, IR, IR },   // 0x06 ADC#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|SI,         SO|AI|S0|S3|FI,         IR, IR },   // 0x07 ADC
  { CO|MI,  RO|II|CE,  CO|MI,  RO|SI|CE,     SO|AI|S1|S2|FI,                    IR, IR, IR },   // 0x08 SBC#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|SI,         SO|AI|S1|S2|FI,         IR, IR },   // 0x09 SBC
  { CO|MI,  RO|II|CE,  CO|MI,  RO|SI|CE,     SO|AI|S0|S2|S3|SM,                 IR, IR, IR },   // 0x0A AND#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|SI,         SO|AI|S0|S2|S3|SM,      IR, IR },   // 0x0B AND
  { CO|MI,  RO|II|CE,  CO|MI,  RO|SI|CE,     SO|AI|S0|S1|S2|SM,                 IR, IR, IR },   // 0x0C ORA#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|SI,         SO|AI|S0|S1|S2|SM,      IR, IR },   // 0x0D ORA
  { CO|MI,  RO|II|CE,  CO|MI,  RO|SI|CE,     SO|AI|S1|S2|SM,                    IR, IR, IR },   // 0x0E EOR#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,     RO|SI,         SO|AI|S1|S2|SM,         IR, IR },   // 0x0F EOR
  { CO|MI,  RO|II|CE,  SO|AI,                                           IR, IR, IR, IR, IR },   // 0x10 INC
  { CO|MI,  RO|II|CE,  SO|AI|S0|S1|S2|S3|SC,                            IR, IR, IR, IR, IR },   // 0x11 DEC

  { CO|MI,  RO|II|CE,  CO|MI,  RO|SI|FR|CE,  S1|S2|SC|FI,                       IR, IR, IR },   // 0x12 CMP#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|FR|CE,  RO|SI,         S1|S2|SC|FI,            IR, IR },   // 0x13 CMP
  { CO|MI,  RO|II|CE,  CE,                                              IR, IR, IR, IR, IR },   // 0x14 BCC
  { CO|MI,  RO|II|CE,  CE,                                              IR, IR, IR, IR, IR },   // 0x15 BCS
  { CO|MI,  RO|II|CE,  CE,                                              IR, IR, IR, IR, IR },   // 0x16 BEQ
  { CO|MI,  RO|II|CE,  CE,                                              IR, IR, IR, IR, IR },   // 0x17 BNE
  { CO|MI,  RO|II|CE,  FR,                                              IR, IR, IR, IR, IR },   // 0x18 CLC
  { CO|MI,  RO|II|CE,  FS,                                              IR, IR, IR, IR, IR },   // 0x19 SEC
  
  { CO|MI,  RO|II|CE,  CO|MI,  RO|CI|CE,                                    IR, IR, IR, IR },   // 0x1A JMP
  { CO|MI,  RO|II|CE,  OI,     OO|AI,         OI,                               IR, IR, IR },   // 0x1B INA

  { CO|MI,  RO|II|CE,  AO|OI,                                           IR, IR, IR, IR, IR },   // 0x1C OUT#
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,      RO|OI,                            IR, IR, IR },   // 0x1D OUT
  { CO|MI,  RO|II|CE,  CO|MI,  RO|MI|CE,      RO|MI,        RO|OI,                  IR, IR },   // 0x1E OUT*
  { CO|MI,  RO|II|CE,  HLT,                                             IR, IR, IR, IR, IR },   // 0x1F HLT
};

byte PCODE_TEMPLATE[32][8] = {
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI, OO|MI, RO|OI,                         IR, IR, IR, IR },   // 0x01 GET
  { OO|II,  OI, OO|MI, RO|OI, OO|RI, RO|OI,                   IR, IR },   // 0x02 SET
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
  { OO|II,  OI,                               IR, IR, IR, IR, IR, IR },   // 0x00 NOP
};

byte ucode[4][32][8];

static byte ADCIM = 0x06;
static byte ADCA = 0x07;
static byte SBCIM = 0x08;
static byte SBC = 0x09;

static byte BCC = 0x14;
static byte BCS = 0x15;
static byte BEQ = 0x16;
static byte BNE = 0x17;

void initUCode() {
  // run mode
  for (byte flag = 0; flag < 4; flag += 1) {
    Serial.println("Mem Copy for run mode ");
    memcpy(ucode[flag], UCODE_TEMPLATE, sizeof(UCODE_TEMPLATE));
  };

  //Special handling for conditions
  Serial.println("Perform special handling for conditions");
  // ZF = 0, CF = 0
  ucode[FLAGS_Z0C0][BNE][2] = CO|MI;
  ucode[FLAGS_Z0C0][BNE][3] = RO|CI|CE;
  ucode[FLAGS_Z0C0][BCC][2] = CO|MI;
  ucode[FLAGS_Z0C0][BCC][3] = RO|CI|CE;

  ucode[FLAGS_Z0C0][ADCIM][4] |= SC; // unset carry flag. AND with INVERTed mask for carry bit.
  ucode[FLAGS_Z0C0][ADCA][5] |= SC;
  ucode[FLAGS_Z0C0][SBCIM][4] |= SC;
  ucode[FLAGS_Z0C0][SBC][5] |= SC;

  // ZF = 0, CF = 1
  ucode[FLAGS_Z0C1][BNE][2] = CO|MI;
  ucode[FLAGS_Z0C1][BNE][3] = RO|CI|CE;
  ucode[FLAGS_Z0C1][BCS][2] = CO|MI;
  ucode[FLAGS_Z0C1][BCS][3] = RO|CI|CE;

  ucode[FLAGS_Z0C1][ADCIM][4] &= ~SC; // unset carry flag. AND with INVERTed mask for carry bit.
  ucode[FLAGS_Z0C1][ADCA][5] &= ~SC;
  ucode[FLAGS_Z0C1][SBCIM][4] &= ~SC;
  ucode[FLAGS_Z0C1][SBC][5] &= ~SC;

  // ZF = 1, CF = 0
  ucode[FLAGS_Z1C0][BEQ][2] = CO|MI;
  ucode[FLAGS_Z1C0][BEQ][3] = RO|CI|CE;
  ucode[FLAGS_Z1C0][BCC][2] = CO|MI;
  ucode[FLAGS_Z1C0][BCC][3] = RO|CI|CE;

  ucode[FLAGS_Z1C0][ADCIM][4] |= SC;
  ucode[FLAGS_Z1C0][ADCA][5] |= SC;
  ucode[FLAGS_Z1C0][SBCIM][4] |= SC;
  ucode[FLAGS_Z1C0][SBC][5] |= SC;

  // ZF = 1, CF = 1
  ucode[FLAGS_Z1C1][BEQ][2] = CO|MI;
  ucode[FLAGS_Z1C1][BEQ][3] = RO|CI|CE;
  ucode[FLAGS_Z1C1][BCS][2] = CO|MI;
  ucode[FLAGS_Z1C1][BCS][3] = RO|CI|CE;

  ucode[FLAGS_Z1C1][ADCIM][4] &= ~SC; // unset carry flag. AND with INVERTed mask for carry bit.
  ucode[FLAGS_Z1C1][ADCA][5] &= ~SC;
  ucode[FLAGS_Z1C1][SBCIM][4] &= ~SC;
  ucode[FLAGS_Z1C1][SBC][5] &= ~SC;
}

void initPCode() {
  // program mode
  for (byte flag = 0; flag < 4; flag += 1) {
    Serial.println("Mem copy for programming mode ");
    memcpy(ucode[flag], PCODE_TEMPLATE, sizeof(PCODE_TEMPLATE));
  };

}
/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  delay(10);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (byte pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (byte pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  for (byte pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  setAddress(address, /*outputEnable*/ false);

  for (byte pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
}


/*
 * Read the contents of the EEPROM and prbyte them to the serial monitor.
 */
void printContents(int start, int length) {
  for (int base = start; base < length; base += 16) {
    byte data[16];
    for (byte offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


void setup() {
  Serial.begin(57600);
  Serial.println("Setup pins");

  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);

  // Program data bytes
  Serial.println("Init PCode");
  initPCode();

  // Program data bytes
  Serial.print("Programming EEPROM for PCode");

  // Program the 8 high-order bits of microcode into the first 128 bytes of EEPROM
  for (int address = 0; address < 1024; address += 1) {
    byte flags       = (address & 0b1100000000) >> 8;
    byte instruction = (address & 0b0011111000) >> 3;
    byte step        = (address & 0b0000000111);

    writeEEPROM(address, ucode[flags][instruction][step]);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  Serial.println("Init UCode");
  initUCode();

  // Program data bytes
  Serial.print("Programming EEPROM for UCode");
  // Program the 8 high-order bits of microcode into the first 128 bytes of EEPROM
  for (int address = 1024; address < 2048; address += 1) {
    byte flags       = (address & 0b1100000000) >> 8;
    byte instruction = (address & 0b0011111000) >> 3;
    byte step        = (address & 0b0000000111);

    writeEEPROM(address, ucode[flags][instruction][step]);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }

  Serial.println(" done");


  // Read and prbyte out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents(0, 2048);
}


void loop() {
  // put your main code here, to run repeatedly:

}
