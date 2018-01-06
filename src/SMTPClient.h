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

#ifndef SMTPClient_H
#define SMTPClient_H

#include <Arduino.h>
#include <Client.h>
#include <Mail.h>

// --- DEBUG ----
// Comment this lines to disable all debug information
//#define DEBUG // Print only the SMTP server responses
#define DEBUG_ALL // Print both the commands sent to the SMTP server and the responses from it

// Circular dependencies
class Mail;
enum mail_recipient_type: uint8_t;

// SMTP errors
typedef enum smtp_status: uint8_t {
   SMTP_OK,                         // OK
   SMTP_CLIENT_NOT_CONNECTED,       // Error throwned when the client has not previusly connected to the SMTP server
   SMTP_CLIENT_ALREADY_CONNECTED,   // Error throwned when the client is alredy connected
   SMTP_CLIENT_CANNOT_CONNECT,      // Error throwned when the client cannot connect to the SMTP server
   SMTP_CONNECTION_ERROR,           // Error throwned if the server not respond 220 to the connection
   SMTP_IDENTIFICATION_ERROR,       // Error throwned in response the EHLO command
   SMTP_AUTH_ERROR,                 // Error throwned if the AUTH LOGIN command fails
   SMTP_MAIL_FROM_ERROR,            // Error throwned if the MAIL TO command fails
   SMTP_RCPT_TP_ERROR,              // Error throwned if the RCPT TO command fails
   SMTP_DATA_ERROR,                 // Error throwned if the DATA command fails
   SMTP_BODY_ERROR,                 // Error throwned if the body is not valid
   SMTP_QUIT_ERROR,                 // Error throwned if the QUIT command fails
   SMTP_DATA_NOT_SENT,              // Error throwned if the command DATA has not been sent before sending header
   SMTP_BODY_ALREADY_SENT,          // Error throwned if body has been already sent before header
   SMTP_AUTH_NOT_SET                // Error throwned if the Auth Login information has not been set
};


//class D7S
class SMTPClient {

   public: 

      // Constructor
      SMTPClient(Client &client, const char *server, int port);

      // Connect to the SMTP server
      smtp_status connect();

      // Set the AUTH LOGIN credentials
      void setAuthLogin(const char *login, const char *passwd);

      // --- SMTP commands ---
      // AUTH LOGIN followed by login and passwd
      smtp_status authLogin(const char *login, const char *passwd);
      // AUTH LOGIN followed by login and passwd
      smtp_status authLogin();
      // EHLO name
      smtp_status ehlo(const char *name);
      // MAIL FROM: <email> (the from argument must be an email address)
      smtp_status mailFrom(const char *from);
      // RCPT TO: <email> (the to argument must be an email address)
      smtp_status rcptTo(const char *to);
      // DATA
      smtp_status data();
      // send the body after the DATA command and the headers
      smtp_status body(const char *body);
      //CRLF . CRLF
      smtp_status finishData();
      // QUIT
      smtp_status quit();

      // --- SMTP HEADERS ---
      // send a header
      smtp_status header(const char *name, const char *value);

      // --- SMTP STATUS ---
      // Await for the SMTP server to respond
      bool awaitSMTPResponse(const char *response = NULL, uint16_t timeout = 10000);

      // --- MAIL ---
      // Send the email prepared using Mail class
      smtp_status send(Mail &mail);


   private:

      // Compile address in the form "Name <address>"
      void compileAddress(char *buffer, const char *address, const char *name);
      // Compile addresses in the form "Name <address>, Name <address>..."
      void compileAddresses(char *buffer, size_t length, const char *recipients[][2], mail_recipient_type recipientsType[], size_t size, mail_recipient_type type);
      // Length of the addresses
      size_t countAddressesLength(const char *recipients[][2], mail_recipient_type recipientsType[], size_t size, mail_recipient_type type);

      // SMTP server address
      const char *_server;
      // SMTP server port
      int _port;

      // Client to be used for the connection
      Client *_client;

      // Auth Login
      const char *_login = NULL;
      const char *_passwd = NULL;

      // Data command
      bool _dataSent = false;
      bool _bodySent = false;
     
};

#endif