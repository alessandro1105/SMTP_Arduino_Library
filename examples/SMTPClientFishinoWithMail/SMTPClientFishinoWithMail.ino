/*
   Copyright 2017 Alessandro Pasqualini
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
     http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   @author    Alessandro Pasqualini <alessandro.pasqualini.1105@gmail.com>
   @url       https://github.com/alessandro1105

*/

#include <Fishino.h>
#include <SPI.h>
#include <SMTPClient.h>

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION DATA    -- ADAPT TO YOUR NETWORK !!!
// DATI DI CONFIGURAZIONE -- ADATTARE ALLA PROPRIA RETE WiFi !!!
#ifndef __MY_NETWORK_H

// here pur SSID of your network
// inserire qui lo SSID della rete WiFi
#define MY_SSID ""

// here put PASSWORD of your network. Use "" if none
// inserire qui la PASSWORD della rete WiFi -- Usare "" se la rete non ￨ protetta
#define MY_PASS ""

// here put required IP address (and maybe gateway and netmask!) of your Fishino
// comment out this lines if you want AUTO IP (dhcp)
// NOTE : if you use auto IP you must find it somehow !
// inserire qui l'IP desiderato ed eventualmente gateway e netmask per il fishino
// commentare le linee sotto se si vuole l'IP automatico
// nota : se si utilizza l'IP automatico, occorre un metodo per trovarlo !
#define IPADDR  192, 168,   1, 251
#define GATEWAY 192, 168,   1, 1
#define NETMASK 255, 255, 255, 0

#endif

//--- SMTP server settings ---
//GMAIL
// You will need a secure client to use Gmail on port 465 because it requires TLS.
// You need also to enable less secure apps from https://myaccount.google.com/lesssecureapps
#define SMTP_SERVER "smtp.gmail.com"
#define SMTP_PORT 465
//SMTP2GO
// You just need a normal client because the connection to server is not secure
//#define SMTP_SERVER "mail.smtp2go.com"
//#define SMTP_PORT 2525

//--- SMTP AUTH (must be in base64) ---
#define SMTP_LOGIN ""
#define SMTP_PASSWD ""
//                    END OF CONFIGURATION DATA                      //
//                       FINE CONFIGURAZIONE                         //
///////////////////////////////////////////////////////////////////////

// define ip address if required
// NOTE : if your network is not of type 255.255.255.0 or your gateway is not xx.xx.xx.1
// you should set also both netmask and gateway
#ifdef IPADDR
  IPAddress ip(IPADDR);
  #ifdef GATEWAY
    IPAddress gw(GATEWAY);
  #else
    IPAddress gw(ip[0], ip[1], ip[2], 1);
  #endif
  #ifdef NETMASK
    IPAddress nm(NETMASK);
  #else
    IPAddress nm(255, 255, 255, 0);
  #endif
#endif

// Use FishinoSecureClient class to create TLS connection
FishinoSecureClient client;

// Use FishinoClient class to create connection
//FishinoClient client;


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  // stampa lo SSID della rete:
  Serial.print("SSID: ");
  Serial.println(Fishino.SSID());

  // print your WiFi shield's IP address:
  // stampa l'indirizzo IP della rete:
  IPAddress ip = Fishino.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  // stampa la potenza del segnale di rete:
  long rssi = Fishino.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  // Fishino connection stuff
  // Initialize serial and wait for port to open
  // Inizializza la porta seriale e ne attende l'apertura
  Serial.begin(115200);
  
  // only for Leonardo needed
  // necessario solo per la Leonardo
  while (!Serial)
    ;

  // initialize SPI
  // inizializza il modulo SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  
  // reset and test WiFi module
  // resetta e testa il modulo WiFi
  Serial << F("Reset WiFi module...");
  Fishino.reset();
  Serial << F("OK\n");

  // go into station mode
  // imposta la modalità stazione
  Fishino.setMode(STATION_MODE);

  // Initialize the Wifi connection.
  // Inizializza la connessione WiFi
  Serial << F("Setting up the Wifi connection\n");

  // try forever to connect to AP
  // tenta la connessione finchè non riesce
  while(!Fishino.begin(MY_SSID, MY_PASS))
    Serial << F("ERROR CONNECTING TO AP, RETRYING....\n");
  
  // setup IP or start DHCP client
  // imposta l'IP statico oppure avvia il client DHCP
#ifdef IPADDR
  Fishino.config(ip, gw, nm);
#else
  Fishino.staStartDHCP();
#endif

  Serial << F("Waiting for IP..");
  while(Fishino.status() != STATION_GOT_IP)
  {
    delay(500);
    Serial << ".";
  }
  Serial << F("OK\n");
  
  // print connection status on serial port
  // stampa lo stato della connessione sulla porta seriale
  printWifiStatus();

  //SMTPClient stuff
  // Create an SMTP client
  SMTPClient smtp(client, SMTP_SERVER, SMTP_PORT);
  // Setting the AUTH LOGIN information
  smtp.setAuthLogin(SMTP_LOGIN, SMTP_PASSWD);
  
  // Creating an email container
  Mail mail;
  
  // Compiling the email
  mail.from("sender@example.com", "Some Sender");
  mail.replyTo("noreply@example.com");
  mail.to("someone@example.com", "Someone");
  mail.to("someoneelse@example.com", "Someone Else");
  mail.cc("another@example.com", "Another");
  mail.bcc("secret@example.com"); // it does not need the name
  mail.subject("Hello there");
  mail.body("Email sent from Arduino/ESP8266/Fishino!");
  
  // Send the email through the SMTP client
  if (smtp.send(mail) == SMTP_OK) {
    Serial.println("Email sent!");
  }

}

void loop() {

}

