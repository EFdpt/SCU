/** 
 *  @file           SCU.ino
 *  @author         Arella Matteo <br/>
 *                  (mail: arella.1646983@studenti.uniroma1.it)
 *  @date           2018
 *  @brief          Main module file
 */

#include "common.h"
#include "states.h"
#include "nmt.h"
#include "radio.h"

/**
 *  @mainpage FastChargeSAE SCU firmware
 *
 *  La rete CAN nasce dall’esigenza di digitalizzare tutti quei segnali 
 *  necessari al funzionamento della vettura.
 *
 *  Sono state adottate due schede di prototipazione Arduino Due per la 
 *  digitalizzazione dei segnali: una posta nella parte anteriore del veicolo, 
 *  riservata all’acquisizione dei pedali, delle sospensioni anteriori e ai 
 *  gruppi ruota anteriori, l’altra posta nella parte posteriore, per acquisire 
 *  sospensioni, gruppi ruota posteriori e accelerometri.
 *  Le schede di acquisizione dei sensori verranno d’ora in poi denominate 
 *  SCU (Sensors Control Unit) e SCU FRONTAL, SCU REAR rispettivamente per le
 *  SCU poste nella parte anteriore e posteriore del veicolo.
 *
 *  Ogni scheda effettua principalmente due azioni:
 *      - Acquisizione sensori
 *      - Trasmissione dati via CAN bus e via radio
 *
 *  Viene implementato un layer protocollare superiore al layer di data link
 *  offerto dal protocollo CAN, ispirato al protocollo di comunicazione CANOpen;
 *  ogni scheda è indirizzabile a livello di rete mediante un ID specifico per
 *  ogni nodo.
 *
 *  Ogni scheda SCU è rappresentabile mediante una macchina a stati finiti con 
 *  i seguenti stati: Initialisation, Pre-operational, Operational, Stopped.
 *  In fase di accensione ogni scheda si trova in fase di Inizializzazione. 
 *  Al termine di tale fase essa tenta di inviare un messaggio di boot-up. Non 
 *  appena esso è stato inviato con successo si pone nello stato Pre-operazional.
 *  Usando un messaggio master NMT, la VCU può far transitare le varie SCU tra 
 *  i diversi stati Pre-operational, Operational e Stopped.
 *  
 *  Ciascuna SCU invia PDO con i dati dei sensori in modalità sincrona 
 *  solamente se si trova nello stato Operational.
 *  
 * 
 *  Il firmware per ogni scheda è selezionabile in fase di precompilazione del
 *  codice dalle direttive presenti in SCU_selection.
 *
 */

/**
 *  @brief      This function perform basic board setup.
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *  @param      None
 *  @retval     None
 */
void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    while(!Serial);
    Serial.println("Serial configured");
    Serial.flush();
    setNodeId(NODE_ID);
    initialisation();
    slaveSendBootUp();  
    preOperational();
    operational();
}

#define ATOMIC          0

/**
 *  @brief      This function is called into endless while main loop
 *  @author     Arella Matteo <br/>
 *              (mail: arella.1646983@studenti.uniroma1.it)
 *  @param      None
 *  @retval     None
 */
void loop() {
    /* Go in Wait For Interrupt mode for reducing power consumption */
    //__asm__("WFI");
    volatile bool radio_transmit_curr;
    #if ATOMIC
    noInterrupts();
    #endif
    radio_transmit_curr = radio_transmit;
    #if ATOMIC
    interrupts();
    #endif
    if (radio_transmit_curr) {
        #if ATOMIC
        noInterrupts();
        #endif
        radio_transmit = false;
        #if ATOMIC
        interrupts();
        #endif
        radio_send_model();
    }
}
