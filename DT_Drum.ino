//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////
//                   ██████╗ ████████╗    ██████╗ ██████╗ ██╗   ██╗███╗   ███╗                  //
//                   ██╔══██╗╚══██╔══╝    ██╔══██╗██╔══██╗██║   ██║████╗ ████║                  //
//                   ██║  ██║   ██║       ██║  ██║██████╔╝██║   ██║██╔████╔██║                  //
//                   ██║  ██║   ██║       ██║  ██║██╔══██╗██║   ██║██║╚██╔╝██║                  //
//                   ██████╔╝   ██║       ██████╔╝██║  ██║╚██████╔╝██║ ╚═╝ ██║                  //
//                   ╚═════╝    ╚═╝       ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝                  //
//                                                                                              //
//                                                                                              //
//          ██╗  ██╗███████╗██╗     ██╗      ██████╗ ██████╗ ██████╗ ██╗   ██╗███╗   ███╗       //        
//          ██║  ██║██╔════╝██║     ██║     ██╔═══██╗██╔══██╗██╔══██╗██║   ██║████╗ ████║       //
//          ███████║█████╗  ██║     ██║     ██║   ██║██║  ██║██████╔╝██║   ██║██╔████╔██║       //
//          ██╔══██║██╔══╝  ██║     ██║     ██║   ██║██║  ██║██╔══██╗██║   ██║██║╚██╔╝██║       //
//          ██║  ██║███████╗███████╗███████╗╚██████╔╝██████╔╝██║  ██║╚██████╔╝██║ ╚═╝ ██║       //
//          ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝       //
//                                                                                              //
//                                                                                              //
//                                                                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////

/************************************************************************************************/
/*   

    Este é um controlador MIDI desenvolvido por Guilhermino Angelim, 
    baseado no projeto "HelloDrum" de Ryo Kosaka. O controlador MIDI permite interagir com VST MIDI 
    através de sensores piezoelétricos, convertendo os toques em sinais MIDI para integração
    com software de música ou dispositivos compatíveis.                     
    Personalize suas batidas e ajuste a sensibilidade dos sensores.              
                           
                                                                                                                  
                                                                                                                 
    Este código foi desenvolvido com base "HelloDrum" de Ryo Kosaka.                 
    Todos os direitos autorais e créditos do projeto original são devidamente atribuídos a Ryo Kosaka.               
    https://github.com/RyoKosaka                 
                                                                                                                                                                                                                                  
    TCRT5000 para controlador hihat para A0                                                                                           
    Circuito de Botões:                                                                                                               
    Botão para EDIT no pino digital 6                                                                                                 
    Botão para UP para o pino digital 7                                                                                                
    Botão para DOWN para o pino digital 8                                                                                              
    Botão para NEXT do pino digital 9                                                                                                  
    Botão para VOLTAR ao pino digital 10                                                                                              
*/                                                                                                                                  
/************************************************************************************************/



#include <hellodrum.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
/************************************************************************************************/
//Usando a Biblioteca MIDI. Se você quiser usar USB-MIDI, comente as próximas duas linhas.
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
/************************************************************************************************/
//Desmarque as próximas duas linhas para usar USB-MIDI com atmega32u4 ou Teensy
//#include <USB-MIDI.h>
//USBMIDI_CREATE_DEFAULT_INSTANCE();
/************************************************************************************************/
// Multiplexador 4067 
/************************************************************************************************/
//Definição do pino do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  
/************************************************************************************************/
//Por favor, nomeie seu pad e controlador.
HelloDrum hihatPedal(0); // PAD 1
HelloDrum hihat(1);      // PAD 2
HelloDrum kick(2);       // PAD 3
HelloDrum snare(3);      // PAD 4
HelloDrum RackTom_1(4);  // PAD 5
HelloDrum RackTom_2(5);  // PAD 6
HelloDrum RackTom_3(6);  // PAD 7
HelloDrum FloorTom_X(7); // PAD 8
HelloDrum FloorTom_XX(8);// PAD 9
HelloDrum Cymbal_1(9);   // PAD 10
HelloDrum Cymbal_2(10);  // PAD 11
HelloDrum Cymbal_3(11);  // PAD 12
HelloDrum Cymbal_4(12);  // PAD 13
HelloDrum Ride_1(13);    // PAD 14
HelloDrum Ride_2(14);    // PAD 15
HelloDrum Ride_3(15);    // PAD 16
/************************************************************************************************/
//Configura o número do pino DIGITAL ao qual os botões estão conectados.
HelloDrumButton button(6, 7, 8, 9, 10); //(EDIT,UP,DOWN,NEXT,BACK)
/************************************************************************************************/


/************************************************************************************************/
/************************************************************************************************/
struct MidiNote {
  int note;
  int velocity;
};

const int MAX_ACTIVE_NOTES = 16; // Defina o tamanho máximo do vetor de notas ativas

MidiNote activeNotes[MAX_ACTIVE_NOTES]; // Vetor para armazenar as notas MIDI ativas
int numActiveNotes = 0; // Variável para acompanhar o número atual de notas ativas
/************************************************************************************************/
/************************************************************************************************/


/************************************************************************************************/
/// lastInteractionTime >= 10000) { LCD lcd.noBacklight(); 
unsigned long lastInteractionTime = 0;
/************************************************************************************************/


void slideText(const char* text, int delayTime) {
  int textLength = strlen(text);
  int startColumn = (16 - textLength) / 2;  // Centraliza o texto no LCD

  for (int i = textLength - 1; i >= 0; i--) {
    lcd.clear();
    lcd.setCursor(startColumn, 0);
    for (int j = i; j < textLength; j++) {
      lcd.print(text[j]);
    }
    delay(delayTime);
  }
}

void progressBar(int width, int delayTime) {
  lcd.setCursor(0, 1);
  lcd.print(".");  

  for (int i = 0; i < width; i++) {
    lcd.print(".");  
    delay(50);
  }

  lcd.print(".");  
}

void formText(const char* text, int delayTime) {
  int textLength = strlen(text);
  int startColumn = (16 - textLength) / 2;  // Centraliza o texto no LCD

  for (int i = 0; i < textLength; i++) {
    lcd.setCursor(startColumn + i, 0);
    lcd.print(text[i]);
    delay(delayTime);
  }
}
/************************************************************************************************/

void setup()
{
  //setupDigital(); //Pinos digitas
  
/************************************************************************************************/

  //se você usa ESP32, tem que descomentar a próxima linha.
  //EEPROM_ESP.begin(512);

  //Se você usar Hairless MIDI ou loop midi, você deve comentar a próxima linha.
  MIDI.begin(31250 );
  //E descomente as próximas duas linhas. Por favor, defina a taxa de transmissão de Hairless para 38400.
  //MIDI.begin();
  //Serial.begin(115200);
  Wire.begin();
  
/************************************************************************************************/

  // Nome para ser exibido no LCD.
 

  hihatPedal.settingName("HIHAT PEDAL"); // PAD 1
  hihat.settingName("HIHAT");            // PAD 2
  kick.settingName("KICK");              // PAD 3
  snare.settingName("SNARE");            // PAD 4
  RackTom_1.settingName("RackTom 1");    // PAD 5
  RackTom_2.settingName("RackTom 2");    // PAD 6
  RackTom_3.settingName("RackTom 3");    // PAD 7
  FloorTom_X.settingName("FloorTom 1");  // PAD 8
  FloorTom_XX.settingName("FloorTom 2"); // PAD 9
  Cymbal_1.settingName("Cymbal 1");      // PAD 10
  Cymbal_2.settingName("Cymbal 2");      // PAD 11
  Cymbal_3.settingName("Cymbal 3");      // PAD 12
  Cymbal_4.settingName("Cymbal 4");      // PAD 13
  Ride_1.settingName("Ride 1");          // PAD 14
  Ride_2.settingName("Ride 2");          // PAD 15
  Ride_3.settingName("Ride 3");          // PAD 16
  
/************************************************************************************************/
  //Load  EEPROM.
  
    
  hihatPedal.loadMemory();               // PAD 1
  hihat.loadMemory();                    // PAD 2
  kick.loadMemory();                     // PAD 3
  snare.loadMemory();                    // PAD 4
  RackTom_1.loadMemory();                // PAD 5
  RackTom_2.loadMemory();                // PAD 6
  RackTom_3.loadMemory();                // PAD 7
  FloorTom_X.loadMemory();               // PAD 8
  FloorTom_XX.loadMemory();              // PAD 9
  Cymbal_1.loadMemory();                 // PAD 10
  Cymbal_2.loadMemory();                 // PAD 11
  Cymbal_3.loadMemory();                 // PAD 12
  Cymbal_4.loadMemory();                 // PAD 13
  Ride_1.loadMemory();                   // PAD 14
  Ride_2.loadMemory();                   // PAD 15
  Ride_3.loadMemory();                   // PAD 16

/************************************************************************************************/
  
  lcd.begin(16, 2);  // Inicializa o LCD
  lcd.backlight();  // Acende a luz de fundo do LCD

 
  slideText("LOADING!", 30);  // Desliza o texto "INICIANDO!" no LCD
  
  progressBar(20, 200);  // Exibe uma animação de barra de progresso
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   PERCUSSION");
  progressBar(20, 200);
  
  

  lcd.clear();
  formText("  DTT87 DRUM", 100);  // Forma o texto "PROTOTIPO" no LCD
  lcd.setCursor(0, 1);
  lcd.noBacklight();
  delay(100);
  lcd.backlight();
  lcd.print("By: GUILHERMINO");
  delay(100);
  lcd.noBacklight();
  delay(100);
  lcd.backlight();

  delay(7000);
  lcd.noBacklight();
}
/************************************************************************************************/


void loop()
{
/************************************************************************************************/

  //loopDigital(); //Pinos digitas

/************************************************************************************************/

  bool buttonPush = button.GetPushState();
  bool editStart = button.GetEditState();
  bool editDone = button.GetEditdoneState();
  bool display = button.GetDisplayState();

  const char *padName = button.GetPadName();
  const char *item = button.GetSettingItem();
  int settingValue = button.GetSettingValue();

  button.readButtonState();

/************************************************************************************************/

  hihatPedal.settingEnable();          // PAD 1
  hihat.settingEnable();               // PAD 2
  kick.settingEnable();                // PAD 3
  snare.settingEnable();               // PAD 4
  RackTom_1.settingEnable();           // PAD 5
  RackTom_2.settingEnable();           // PAD 6
  RackTom_3.settingEnable();           // PAD 7
  FloorTom_X.settingEnable();          // PAD 8
  FloorTom_XX.settingEnable();         // PAD 9
  Cymbal_1.settingEnable();            // PAD 10
  Cymbal_2.settingEnable();            // PAD 11
  Cymbal_3.settingEnable();            // PAD 12
  Cymbal_4.settingEnable();            // PAD 13
  Ride_1.settingEnable();              // PAD 14
  Ride_2.settingEnable();              // PAD 15
  Ride_3.settingEnable();              // PAD 16

  if (buttonPush == true)
{
  lcd.backlight();
  lcd.clear();
  lcd.print(padName);
  lcd.setCursor(0, 1);
  lcd.print(item);
  lcd.setCursor(13, 1);
  lcd.print(settingValue);

  lastInteractionTime = millis();
  }

  if (millis() - lastInteractionTime >= 10000) {
    lcd.noBacklight();
  }

  if (editStart == true)
  {
    lcd.backlight();
    lcd.clear();
    lcd.print("EDIT START");
    delay(500);
    lcd.clear();
    lcd.print(padName);
    lcd.setCursor(0, 1);
    lcd.print(item);
    lcd.setCursor(13, 1);
    lcd.print(settingValue);
  lastInteractionTime = millis();
  }

  if (millis() - lastInteractionTime >= 10000) {
    lcd.noBacklight();
  }

  if (editDone == true)
  {
    
    lcd.clear();
    lcd.print("EDIT DONE");
    delay(500);
    lcd.clear();
    lcd.print(padName);
    lcd.setCursor(0, 1);
    lcd.print(item);
    lcd.setCursor(13, 1);
    lcd.print(settingValue);

  lastInteractionTime = millis();
  }

  if (millis() - lastInteractionTime >= 10000) {
    lcd.noBacklight();
  }

  // Mostra o nome do pad atingido e a velocidade para o LCD
  if (display == true)
  {
    int velocity = button.GetVelocity();
    const char *hitPad = button.GetHitPad();

    lcd.clear();
    lcd.print(hitPad);
    lcd.setCursor(0, 1);
    lcd.print(velocity);

    if (hihat.hit == true)
    {
      //open
      if (hihatPedal.openHH == true)
      {
        lcd.setCursor(15, 0);
        lcd.print("C");
      }
      //close
      else if (hihatPedal.closeHH == true)
      {
        lcd.setCursor(15, 0);
        lcd.print("O");
      }
    }
  }
/************************************************************************************************/
/*                            2. SENSING & SENDING MIDI                                         */

  hihatPedal.hihatControl();         // PAD 1
  hihat.HH();                        // PAD 2
  kick.singlePiezo();                // PAD 3
  snare.singlePiezo();               // PAD 4
  RackTom_1.singlePiezo();           // PAD 5
  RackTom_2.singlePiezo();           // PAD 6
  RackTom_3.singlePiezo();           // PAD 7
  FloorTom_X.singlePiezo();          // PAD 8
  FloorTom_XX.singlePiezo();         // PAD 9
  Cymbal_1.singlePiezo();            // PAD 10
  Cymbal_2.singlePiezo();            // PAD 11
  Cymbal_3.singlePiezo();            // PAD 12
  Cymbal_4.singlePiezo();            // PAD 13
  Ride_1.singlePiezo();              // PAD 14
  Ride_2.singlePiezo();              // PAD 15
  Ride_3.singlePiezo();              // PAD 16

/************************************************************************************************/
/*                                     HIHAT CONTROLLER                                        */
  //Quando o hihat está fechado

  if (hihatPedal.hit == true)
  {
    MIDI.sendNoteOn(hihatPedal.note, hihatPedal.velocity, 10); //(note of pedal, velocity, channel)
    MIDI.sendNoteOff(hihatPedal.note, 0, 10);
    
  }

  //sending state of pedal with controll change
  if (hihatPedal.moving == true)
  {
    MIDI.sendControlChange(4, hihatPedal.pedalCC, 10);
  }
/************************************************************************************************/
//HIHAT

  if (hihat.hit == true)
  {
    //check open or close
    //1.open
    if (hihatPedal.openHH == true)
    {
      MIDI.sendNoteOn(hihat.noteOpen, hihat.velocity, 10); //(note of open, velocity, channel)
      MIDI.sendNoteOff(hihat.noteOpen, 0, 10);
      
    }
    //2.close
    else if (hihatPedal.closeHH == true)
    {
      MIDI.sendNoteOn(hihat.noteClose, hihat.velocity, 10); //(note of close, velocity, channel)
      MIDI.sendNoteOff(hihat.noteClose, 0, 10);
      
    }
  }
/************************************************************************************************/ 
/*                                   Sending MIDI signals.                                      */
if (kick.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {kick.note, kick.velocity};
  }
}

// Exemplo para a peça Snare
if (snare.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {snare.note, snare.velocity};
  }
}

// Exemplo para a peça RackTom_1
if (RackTom_1.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {RackTom_1.note, RackTom_1.velocity};
  }
}

// Exemplo para a peça RackTom_2
if (RackTom_2.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {RackTom_2.note, RackTom_2.velocity};
  }
}

// Exemplo para a peça RackTom_3
if (RackTom_3.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {RackTom_3.note, RackTom_3.velocity};
  }
}

// Exemplo para a peça FloorTom_X
if (FloorTom_X.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {FloorTom_X.note, FloorTom_X.velocity};
  }
}

// Exemplo para a peça FloorTom_XX
if (FloorTom_XX.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {FloorTom_XX.note, FloorTom_XX.velocity};
  }
}

// Exemplo para a peça Cymbal_1
if (Cymbal_1.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Cymbal_1.note, Cymbal_1.velocity};
  }
}

// Exemplo para a peça Cymbal_2
if (Cymbal_2.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Cymbal_2.note, Cymbal_2.velocity};
  }
}

// Exemplo para a peça Cymbal_3
if (Cymbal_3.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Cymbal_3.note, Cymbal_3.velocity};
  }
}

// Exemplo para a peça Cymbal_4
if (Cymbal_4.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Cymbal_4.note, Cymbal_4.velocity};
  }
}

// Exemplo para a peça Ride_1
if (Ride_1.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Ride_1.note, Ride_1.velocity};
  }
}

// Exemplo para a peça Ride_2
if (Ride_2.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Ride_2.note, Ride_2.velocity};
  }
}

// Exemplo para a peça Ride_3
if (Ride_3.hit == true) {
  if (numActiveNotes < MAX_ACTIVE_NOTES) {
    activeNotes[numActiveNotes++] = {Ride_3.note, Ride_3.velocity};
  }
}

for (int i = 0; i < numActiveNotes; i++) {
  MIDI.sendNoteOn(activeNotes[i].note, activeNotes[i].velocity, 10); // Envia as mensagens Note On
}

for (int i = 0; i < numActiveNotes; i++) {
  MIDI.sendNoteOff(activeNotes[i].note, 0, 10); // Envia as mensagens Note Off
}

// Limpe o número de notas ativas para o próximo ciclo
numActiveNotes = 0;

}
