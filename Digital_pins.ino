//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
/*////////////////////////////////////////////////////////////////////////////////////////////////
//        ██████╗ ██╗ ██████╗ ██╗████████╗ █████╗ ██╗         ██████╗ ██╗███╗   ██╗███████╗     //
//        ██╔══██╗██║██╔════╝ ██║╚══██╔══╝██╔══██╗██║         ██╔══██╗██║████╗  ██║██╔════╝     //
//        ██║  ██║██║██║  ███╗██║   ██║   ███████║██║         ██████╔╝██║██╔██╗ ██║███████╗     //
//        ██║  ██║██║██║   ██║██║   ██║   ██╔══██║██║         ██╔═══╝ ██║██║╚██╗██║╚════██║     //
//        ██████╔╝██║╚██████╔╝██║   ██║   ██║  ██║███████╗    ██║     ██║██║ ╚████║███████║     //
//        ╚═════╝ ╚═╝ ╚═════╝ ╚═╝   ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═╝     ╚═╝╚═╝  ╚═══╝╚══════╝     //
*/////////////////////////////////////////////////////////////////////////////////////////////////          

/************************************************************************************************/

#include <Arduino.h>
#include <MIDI.h>

/************************************************************************************************/

const int PadPin1 = 22; // Pino do botão 1
const int PadPin2 = 24; // Pino do botão 2
const int PadPin3 = 26; // Pino do botão 3

int Pad1State = LOW; // Estado atual do botão 1
int Pad2State = LOW; // Estado atual do botão 2
int Pad3State = LOW; // Estado atual do botão 3

int lastPad1State = LOW; // Estado anterior do botão 1
int lastPad2State = LOW; // Estado anterior do botão 2
int lastPad3State = LOW; // Estado anterior do botão 3

bool note1Activated = false; // Estado de ativação da nota 1
bool note2Activated = false; // Estado de ativação da nota 2
bool note3Activated = false; // Estado de ativação da nota 3

/************************************************************************************************/

void setupDigital() {
  // Configuração específica para o modo digital
  pinMode(PadPin1, INPUT); // Configura o pino do botão 1 como INPUT_PULLUP
  pinMode(PadPin2, INPUT); // Configura o pino do botão 2 como INPUT_PULLUP
  pinMode(PadPin3, INPUT); // Configura o pino do botão 3 como INPUT_PULLUP

  // Inicializa a comunicação MIDI
  MIDI.begin();
}
/************************************************************************************************/

void loopDigital() {
  
  int reading1 = digitalRead(PadPin1);
  int reading2 = digitalRead(PadPin2);
  int reading3 = digitalRead(PadPin3);

  if (reading1 == LOW && lastPad1State == HIGH && !note1Activated) {
    MIDI.sendNoteOn(60, 127, 10);
    MIDI.sendNoteOff(60, 0, 10);
    note1Activated = true;
  } else if (reading1 == HIGH && lastPad1State == LOW && note1Activated) {
    note1Activated = false;
  }

  if (reading2 == LOW && lastPad2State == HIGH && !note2Activated) {
    MIDI.sendNoteOn(61, 127, 10);
    MIDI.sendNoteOff(61, 0, 10);
    note2Activated = true;
  } else if (reading2 == HIGH && lastPad2State == LOW && note2Activated) {
    note2Activated = false;
  }

  if (reading3 == LOW && lastPad3State == HIGH && !note3Activated) {
    MIDI.sendNoteOn(62, 127, 10);
    MIDI.sendNoteOff(62, 0, 10);
    note3Activated = true;
  } else if (reading3 == HIGH && lastPad3State == LOW && note3Activated) {
    note3Activated = false;
  }

  lastPad1State = reading1;
  lastPad2State = reading2;
  lastPad3State = reading3;

  
}
/************************************************************************************************/