/**
 * This sketch is for programming my 8bit computer
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define BUS_D0 5
#define BUS_D7 12
#define ACK_IO 13

// Change in USART.h or HardwareSerial.h in ~/Libraries/Arduino15/...
// #define SERIAL_RX_BUFFER_SIZE 256 
// #define SERIAL_TX_BUFFER_SIZE 256 
// #define SERIAL_BUFFER_SIZE 256 

void(* reset) (void) = 0;  // declare reset at address 0

// Assembly to Op Code Table
typedef struct {
    char* ascode;
    byte opcode;
    bool twobytes;
} code;

code codes[] = {
{"HLT", 0x00, false},
{"NOP", 0x04, false},
{"LDA# ", 0x05, true},
{"LDA ", 0x06, true},
{"LDA* ", 0x07, true},
{"STA ", 0x08, true},
{"STA* ", 0x09, true},
//skip 0A to 0F control characters
{"ADD# ", 0x10, true},
{"ADD ", 0x11, true},
{"SUB# ", 0x12, true},
{"SUB ", 0x13, true},
{"ADC# ", 0x14, true},
{"ADC ", 0x15, true},
{"SBC# ", 0x16, true},
{"SBC ", 0x17, true},
{"AND# ", 0x18, true},
{"AND ", 0x19, true},
{"ORA# ", 0x1A, true},
{"ORA ", 0x1B, true},
{"EOR# ", 0x1C, true},
{"EOR ", 0x1D, true},
{"INC", 0x1E, false},
{"DEC", 0x1F, false},

{"CMP# ", 0x20, true},
{"CMP ", 0x21, true},
{"BCC ", 0x22, true},
{"BCS ", 0x23, true},
{"BEQ ", 0x24, true},
{"BNE ", 0x25, true},
{"CLC", 0x26, false},
{"SEC", 0x27, false},

{"JMP ", 0x2A, true},
{"JSR ", 0x2B, true},
{"RET", 0x2C, false},
{"PSH", 0x2D, false},
{"POP", 0x2E, false},

{"INA", 0x30, false},
{"OUTA", 0x31, false},
{"OUT# ", 0x32, true},
{"OUT ", 0x33, true},
{"OUT* ", 0x34, true},
};

size_t size = sizeof(codes) / sizeof(code);

code* searchByAssemblyCode(const char* ascode) {
    for (size_t i=0; i<size; i++) {
        if (strcmp(codes[i].ascode, ascode) == 0) {
            return &codes[i];
        }
    }
    return NULL;
}

code* searchByOpCode(const byte opcode) {
    for (size_t i=0; i<size; i++) {
        if (codes[i].opcode == opcode) {
            return &codes[i];
        }
    }
    return NULL;
}

/*
   Output the address or data bits using shift registers.
*/
void setData(byte data) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, data);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, 0x0);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  delay(5);
  digitalWrite(SHIFT_LATCH, LOW);
}

/*
   Read a byte from memory at the specified address.
*/
byte getMemory(byte address) {
  wait_for_ack();
  setData(0x01);
  wait_for_ack();
  setData(address);
  wait_for_ack();
  return readBus();
}

/*
   Write a byte to memory at the specified address.
*/
byte setMemory(byte address, byte data) {
  wait_for_ack();
  setData(0x02);
  wait_for_ack();
  setData(address);
  wait_for_ack();
  setData(data);
  wait_for_ack();
  return readBus();
}

/*
   Set Run Mode
*/
bool runMode = false;

void setRunMode() {
  runMode = true;
  wait_for_ack();
  setData(0x03);
  wait_for_ack();
  setData(0x0);
}

/*
   set value in register
*/
void setRegister(byte value) {
  wait_for_ack();
  setData(value);
  wait_for_ack();
  setData(0x0);
}

byte readBus() {
  byte data = 0;
  for (int pin = BUS_D0; pin <= BUS_D7; pin += 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

bool waiting = false;
bool ack = false;

void wait_for_ack() {
  waiting = true;
  while (!ack) {
    delay(1);
  }
  waiting = false;
  ack = false;
}

void interrupted() {
  if (waiting) {
    ack = true;
  }
  if (runMode) {
    byte value = readBus();
    Serial.write(value);
  }
}

/*
   Get the contents of memory and print them to the serial monitor.
*/
bool getOpCode(int address) {
  char buf[16];
  byte data = getMemory(address);
  code* result = searchByOpCode(data);
  if (result) {  
    if (result->twobytes) {
      setData(0x0);
      byte operand = getMemory(address + 1);
      sprintf(buf, "%02x: %s%02x", address, result->ascode, operand);
    } else {
      sprintf(buf, "%02x: %s", address, result->ascode);
    };
    Serial.println(buf);
    return result->twobytes;
  } else {
    sprintf(buf, "%02x: %02x", address, data);
    Serial.println(buf);
  };
  return false;
}

/*
   Get the contents of memory and print them to the serial monitor.
*/
void getContents(int address) {
  char buf[8];
  byte data = getMemory(address);
  sprintf(buf, "%02x: %02x", address, data);
  Serial.println(buf);
}

/*
   Set the contents of memory and print them to the serial monitor.
*/
void setContents(int address, byte value) {
  byte data = setMemory(address, value);
  char buf[8];
  sprintf(buf, "%02x: %02x", address, data);
  Serial.println(buf);
}

void prompt() {
    Serial.println("Ready.");
    Serial.print(">");
}

void clear() {
  // clear the contents of RAM
  Serial.println("--- New ---");
  for (int base = 0; base <= 255; base += 1) {
    setContents(base, 0x0);
  }
  setData(0x0);
}


void list(int address, byte length) {
  // Read and print out the contents of RAM
  
  if (length == 0) length = 255;

  Serial.println("--- List ---");
  for (int base = 0; base <= length; base += 1) {
    bool skip = getOpCode(address + base);
    if (skip) base += 1;
    setData(0x0);
  }
}

void upload(int address) {
  Serial.println("--- Upload ---");
  Serial.print(">");
  while(Serial.available() == 0) {
    delay(250);
  }

  int size = 0;
  byte buffer[256];
  while (Serial.available() > 0) {
    buffer[size++] = (byte) Serial.read();
  }
  
  for(int addr=0; addr < size; addr++) {
    setContents(address+addr, buffer[addr]);
    setData(0x0);
  }
}

void get(int address) {
  Serial.println("--- Get ---");
  getContents(address);
  setData(0x0);
}

void set(int address, byte value) {
  Serial.println("--- Set ---");
  setContents(address, value);
  setData(0x0);
}

void run() {
  Serial.println("--- Run ---");
  setRunMode();
}

void help() {
  Serial.println("n - new program");
  Serial.println("l - list program");
  Serial.println("u - upload program");
  Serial.println("g {addr} - get a value at an address");
  Serial.println("s {addr} {value} - set a value at an address");
  Serial.println("r - run the program");
  Serial.println("h - help");
}

void setup() {
  Serial.begin(9600);

  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);

  for (int pin = BUS_D0; pin <= BUS_D7; pin += 1) {
    pinMode(pin, INPUT);
  }

  setData(0x0);
  attachInterrupt(digitalPinToInterrupt(ACK_IO), interrupted, RISING);
  Serial.println("*** 8bit computer by Dru Jensen (2021) ***");
  Serial.println("256 bytes free");
  prompt();
}

byte idx = 0;
char rx;
char buffer[16];
char cr = '\r';
char nl = '\n';

void loop() {
  if (Serial.available()) {
    rx = Serial.read();
    if (runMode) {
      setRegister(rx);
    } else {
      Serial.print(rx);
      buffer[idx] = rx;
      if (rx == cr || rx == nl) {
        process();
        memset(buffer, 0, sizeof(buffer));      
        idx = 0;
      } else {
        idx++;
      }
    }
  }
}

void process() {
  byte command = buffer[0];
  byte address[2];
  byte value[2];

  address[0] = buffer[2];
  address[1] = buffer[3];
  int addr = (int) strtol(address, NULL, 16);

  value[0] = buffer[5];
  value[1] = buffer[6];
  byte val = (byte) (strtol(value, NULL, 16) >> 8);
    switch (command) {
        case 'n': 
          clear(); 
          prompt();
          break;
        case 'l': 
          list(addr, val); 
          prompt();
          break;
        case 'u': 
          upload(addr); 
          prompt();
          break;
        case 'g': 
          get(addr); 
          prompt();
          break;
        case 's': 
          set(addr, val); 
          prompt();
          break;
        case 'r': 
          run(); 
          break;
        case 'h':
          help();
          prompt();
          break;
        case 'q':
          Serial.println("Good bye!");
          Serial.end();
          reset();
          break;
        default:
          break;        
    }
 }