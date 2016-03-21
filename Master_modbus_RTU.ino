/**
 *  Modbus master example 2:
 *  The purpose of this example is to query several sets of data
 *  from an external Modbus slave device. 
 *  The link media can be USB or RS232.
 *
 *  Recommended Modbus slave: 
 *  diagslave http://www.modbusdriver.com/diagslave.html
 *
 *  In a Linux box, run 
 *  "./diagslave /dev/ttyUSB0 -b 19200 -d 8 -s 1 -p none -m rtu -a 1"
 *  This is:
 *   serial port /dev/ttyUSB0 at 19200 baud 8N1
 *  RTU mode and address @1
 *
  */

//// http://pdacontrol.blogspot.com.co/2016/03/pruebas-libreria-modbus-rtu-maestro-con.html
  
  
  
#include <ModbusRtu.h>


/// oled debuging

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4 // not used / nicht genutzt bei diesem Display
Adafruit_SSD1306 display(OLED_RESET);


///  

uint16_t au16data[32]; //!< data array for modbus network sharing  Array
uint8_t u8state; //!< machine state
uint8_t u8query; //!< pointer to message query

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus master(0,0,7); // this is master and RS-232 or USB-FTDI  ///  7 = PIN ENEABLE rs485

/**
 * This is an structe which contains a query to an slave device
 */
modbus_t telegram[2];

unsigned long u32wait;



void setup() {

  // initialize with the I2C addr 0x3C / mit I2C-Adresse 0x3c initialisieren
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(0.5);
  display.setTextColor(INVERSE); 

  //////////////////////////////
    
  // telegram 0: read registers
  telegram[0].u8id = 1;                 // slave address
  telegram[0].u8fct = 3;                // function code (registers read multiple  3) 
  telegram[0].u16RegAdd = 0;            // start address in slave -  direccion de Inicio 0
  telegram[0].u16CoilsNo = 10;          // number of elements (coils or registers) to read  0 - 16 
  telegram[0].au16reg = au16data;       // pointer to a memory array in the Arduino - Almacenamiento en Array de memoria de arduino

  // telegram 1: write a multiple  register = function 16
  telegram[1].u8id = 1;                 // slave address
  telegram[1].u8fct = 16;               // function code (write multiple registers 16 )
  telegram[1].u16RegAdd = 10;           // start address in slave  -  direccion de Inicio 10
  telegram[1].u16CoilsNo = 10;          // number of elements (coils or registers) to read
  telegram[1].au16reg = au16data+10;    // pointer to a memory array in the Arduino - Almacenamiento en Array de memoria de arduino 10 posiciones despues

   
  master.begin( 19200 ); // baud-rate at 19200
  master.setTimeOut( 5000 ); // if there is no answer in 5000 ms, roll over
  u32wait = millis() + 1000;
  u8state = u8query = 0;   
}

void loop() {
  switch( u8state ) {
  case 0: 
    if (millis() > u32wait) u8state++; // wait state
    break;
  case 1: 
    master.query( telegram[u8query] ); // send query (only once)
    u8state++;
 u8query++;
 if (u8query > 2) u8query = 0;
    break;
  case 2:
    master.poll(); // check incoming messages
    if (master.getState() == COM_IDLE) {
      u8state = 0;
      u32wait = millis() + 1000; 
    }
    break;
  }
  
   display.clearDisplay();

  //Read Multiple Holding Register  - Lectura de multiples registros 
   
   display.setCursor(0,0); 
   display.print(au16data[0]);    // Read Holding Register [0]
   display.print(",");    
   display.print(au16data[1]);    // Read Holding Register [1]
   display.print(",");    
   display.print(au16data[2]);    // Read Holding Register [2]
   display.print(","); 
   display.print(au16data[3]);    // Read Holding Register [3]
   display.print(",");     
   display.print(au16data[4]);    // Read Holding Register [4]
   display.print(" Hold 0-4");
   
   display.setCursor(0,8);  
   display.print(au16data[5]);   // Read Holding Register [5]
   display.print(",");     
   display.print(au16data[6]);   // Read Holding Register [6]
   display.print(",");   
   display.print(au16data[7]);   // Read Holding Register [7]
   display.print(",");   
   display.print(au16data[8]);   // Read Holding Register [8]
   display.print(",");    
   display.print(au16data[9]);   // Read Holding Register [9]
   display.print(" Hold 5-9");



     
  //Write Multiple Holding Register  - Escritura de multiples registros 

   au16data[10] = analogRead( 0 ); // Write Holding Register [10]
   
   au16data[11] = analogRead( 0 ); // Write Holding Register [11]
   
   au16data[12] = analogRead( 0 ); // Write Holding Register [12]
   
   au16data[13] = analogRead( 0 ); // Write Holding Register [13]
   
   au16data[14] = analogRead( 0 ); // Write Holding Register [14]
   
   au16data[15] = analogRead( 0 ); // Write Holding Register [15]
   
   au16data[16] = analogRead( 0 ); // Write Holding Register [16]
   
   au16data[17] = analogRead( 0 ); // Write Holding Register [17]
   
   au16data[18] = analogRead( 0 ); // Write Holding Register [18]
      
   au16data[19] = analogRead( 0 ); // Write Holding Register [19]
   

// Print Oled Write Holding Register 
   
   display.setCursor(0,16); 
   display.print(au16data[10]);    
   display.print(",");    
   display.print(au16data[11]);
   display.print(",");    
   display.print(au16data[12]);    
   display.print(","); 
   display.print(au16data[13]);    
   display.print(",");     
   display.print(au16data[14]);
   display.print(",");    
   display.print(au16data[15]);    
   display.print(",");    
   display.print(au16data[16]);
   display.print(",");    
   display.print(au16data[17]);    
   display.print(","); 
   display.print(au16data[18]);    
   display.print(",");     
   display.print(au16data[19]);
   display.print(" Hold 10-19");
   
   
   display.display();  
              
 


  
} 

