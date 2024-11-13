// Funktionsprototypen
void BandVor();
void BandHalt();
void Analysieren();
void EingaengeLesen();
void ErgebnisAusgeben();
bool Fehlererkennung();

// Globale Variablen
// Eigenschaften Block
  bool Metall = 0;
  bool Lang = 0;
  bool Hoch = 0;
// Zustände Eingänge
  int KAS_End = 0;
  int LS_Hinten = 0;
  int IND_Sensor = 0;
  int inputStateA3 = 0;
  int inputStateA4 = 0;
  int KAS_Start = 0;
  int LS_Hoehe = 0;
// Variablen für Zeitmessung
  unsigned long Previous_Millis = 0;
  unsigned long Elapsed_Millis = 0; 
  int LS_Hinten_besetzt = 0;
  unsigned long Compare_Time = 650;
// Variable für Fehlererkennung
  bool BandBelegt;
  bool Fehler;


void setup() {
  // Serielle Kommunikation starten
  Serial.begin(9600);
  // Initialisierung Pins
  // Output Pins
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    
  //Input Pins
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);

  // Kein Fehler
  Fehler = 0;
  BandBelegt = 0;

}

void loop() {

  EingaengeLesen();

  // Wenn Block eingelegt
  if (KAS_Start == 1 && !Fehler){
    // Band starten
    BandVor();
    // Band Belegen
    BandBelegt = 1;
    // Warten bis Block weg
    delay(500);

    // Blockstatus zurücksetzen
    Metall = 0;
    Lang = 0;
    Hoch = 0;

    // Block analysieren bis er am Ende ankommt
    do {
      EingaengeLesen();
      //Fehler = Fehlererkennung();
      //if (Fehler == 1){
      //  Serial.println("Fehler erkannt!");
      //  break;
      //}
      Analysieren();
    } while (KAS_End != 1);

    BandHalt();
    // Band Entbelegen
    BandBelegt = 0;
    //Ergebnisse Ausgeben
    ErgebnisAusgeben();
  }

}

void Analysieren(){
  // Metall erkennen
  if (IND_Sensor == 1){
    Metall = 1;
  }
  // Hohe Blöcke erkennen
  if (LS_Hoehe == 1) {
    Hoch = 1;
  }
  
  // Zeitmessung zur Längenbestimmung
  if(LS_Hinten == 1 && LS_Hinten_besetzt == 0){
    Previous_Millis = 0;
    Elapsed_Millis = 0; 
    LS_Hinten_besetzt = 1;
    Previous_Millis = millis();
  }
  if(LS_Hinten == 0 && LS_Hinten_besetzt == 1){
    Elapsed_Millis = millis() - Previous_Millis;
    Serial.println("Vergangene Zeit in ms: " + String(Elapsed_Millis));
    LS_Hinten_besetzt = 0;
    if(Elapsed_Millis >= Compare_Time){
      Lang = 1;
    }
  }
}

void ErgebnisAusgeben(){
    if (Metall == 1){
      Serial.println("Kurzer Block mit Metallkern erkannt!");
    }
    if (Hoch == 1){
      Serial.println("Länglicher Block hochkant erkannt!");
    }
    // Großer Block erkennen
    if (Lang == 1){
      Serial.println("Länglicher Block liegend erkannt!");
    }
    if (Metall == 0 && Hoch == 0 && Lang == 0){
      Serial.println("Kurzer Block ohne Metallkern erkannt!");      
    }
}

bool Fehlererkennung (){
  if (KAS_Start && BandBelegt){
    return 1;
  }
  else if (!KAS_Start && !BandBelegt){
    BandBelegt = 1;    
    Serial.println("Band Belegt!.");
  }

  return 0;
}

void BandVor(){
  digitalWrite(D0, HIGH);
  Serial.println("Band gestartet.");
}

void BandHalt(){
  digitalWrite(D0, LOW);  
  Serial.println("Band gestoppt.");
}

void EingaengeLesen(){
    // read all input States
  KAS_End = digitalRead(A0);
  LS_Hinten = digitalRead(A1);
  IND_Sensor = digitalRead(A2);
  inputStateA3 = digitalRead(A3);
  inputStateA4 = digitalRead(A4);
  KAS_Start = digitalRead(A5);
  LS_Hoehe = digitalRead(A6);
}
