#define POWER_PIN 19   
#define DO_PIN 21     

void  setup () { 
Serial.begin(9600); 
pinMode(POWER_PIN, OUTPUT);   
pinMode (DO_PIN, INPUT );

}  

void  loop () { 
digitalWrite (POWER_PIN, LOW );   
  
  delay(10);
  int rain_state = digitalRead(DO_PIN);

  digitalWrite (POWER_PIN, LOW );   

  
  if (rain_state == HIGH ){
Serial.println ( "A chuva NÃO foi detectada" );
  } 
  else{ Serial . println ( "A chuva foi detectada" );
  }    
  
    

  delay (1000);  
 }
