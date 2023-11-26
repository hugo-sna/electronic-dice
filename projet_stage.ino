/*
 * Auteur: Hugo Schwab
 * Version: 1.0
 * Description: Dé électronique
 */

/**********************
 ******Constantes******
 **********************/
#define S1 0
#define S2 1
#define S_MODE_0 2
#define S_MODE_1 3

#define P1 11 // Rouge
#define P2 10 // Vert

#define DMX_A 4
#define DMX_B 5
#define DMX_C 6
#define DMX_D 7
#define DMX_CA1 8 // Dizaines
#define DMX_CA2 9 // Unités

/**********************
 ******Variables*******
 **********************/
bool s1_state, s2_state, s_mode_0_state, s_mode_1_state; // Variables pour les états des switch
byte num, last_num, bcd = 50; // Int8 pour les nombres

/**********************
 ******Fonctions*******
 **********************/
byte get_bcd(char number) {
  switch(number) {
    case '0':
      return 0;
      
    case '1':
      return 1;
      
    case '2':
      return 2;

    case '3':
      return 3;
      
    case '4':
      return 4;
      
    case '5':
      return 5;
      
    case '6':
      return 6;
      
    case '7':
      return 7;
      
    case '8':
      return 8;

    case '9':
      return 9;
      
    default:
      return 0;
  }
}

byte byte_to_bcd(byte number) {
  char buffer[2];
  sprintf(buffer, "%2u", number); // Converti number en char

  byte tmp = 0;

  tmp = get_bcd(buffer[0]) << 4;
  tmp = tmp | get_bcd(buffer[1]);
   
  return tmp;
}

void display(byte bcd_num) {
  // Test les bits et les envoies
  digitalWrite(DMX_CA2, HIGH);
  digitalWrite(DMX_A, bitRead(bcd_num, 0));
  digitalWrite(DMX_B, bitRead(bcd_num, 1));
  digitalWrite(DMX_C, bitRead(bcd_num, 2));
  digitalWrite(DMX_D, bitRead(bcd_num, 3));
  digitalWrite(DMX_CA2, LOW);
  delay(10);
  digitalWrite(DMX_CA2, HIGH);

  digitalWrite(DMX_CA1, HIGH);
  digitalWrite(DMX_A, bitRead(bcd_num, 4));
  digitalWrite(DMX_B, bitRead(bcd_num, 5));
  digitalWrite(DMX_C, bitRead(bcd_num, 6));
  digitalWrite(DMX_D, bitRead(bcd_num, 7));
  digitalWrite(DMX_CA1, LOW);
  delay(10);
  digitalWrite(DMX_CA1, HIGH);
}

/**********************
 ******Programme*******
 **********************/
void setup() {
  // Configuration des pins
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S_MODE_0, INPUT);
  pinMode(S_MODE_1, INPUT);

  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(DMX_A, OUTPUT);
  pinMode(DMX_B, OUTPUT);
  pinMode(DMX_C, OUTPUT);
  pinMode(DMX_D, OUTPUT);
  pinMode(DMX_CA1, OUTPUT);
  pinMode(DMX_CA2, OUTPUT);

  bcd = byte_to_bcd(50);
}

void loop() {
  // Mets à jour l'état des switches
  s1_state = digitalRead(S1);
  s2_state = digitalRead(S2);
  s_mode_0_state = digitalRead(S_MODE_0);
  s_mode_1_state = digitalRead(S_MODE_0);

  if(s_mode_0_state) { // Dé
    if(s1_state) {
      num = random(0, 99);
      bcd = byte_to_bcd(num);

      delay(100); // Délais pour éviter de capter le rebond
      // Bloque l'execution du programme pour éviter les doubles cliques
      while(s1_state)
        s1_state = digitalRead(S1);
    }
  } else { // Jeu
    // Plus petit
    if(s1_state) {
      num = random(0, 99);

      if(num < last_num) {
        digitalWrite(P1, LOW);
        digitalWrite(P2, HIGH);
      } else {
        digitalWrite(P1, HIGH);
        digitalWrite(P2, LOW);
      }
      
      last_num = num;
      bcd = byte_to_bcd(num);
      delay(100);
    }

    // Plus grand
    if(s2_state) {
      num = random(0, 99);

      if(num > last_num) {
        digitalWrite(P1, LOW);
        digitalWrite(P2, HIGH);
      } else {
        digitalWrite(P1, HIGH);
        digitalWrite(P2, LOW);
      }
      
      last_num = num;
      bcd = byte_to_bcd(num);
      delay(100);
      
    }

    while(s1_state || s2_state) {
      s1_state = digitalRead(S1);
      s2_state = digitalRead(S2);
    }
  }
  
  display(bcd);
}
