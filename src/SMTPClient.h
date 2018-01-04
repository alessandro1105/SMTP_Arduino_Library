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
#define DEBUG // Print only the SMTP server responses
#define DEBUG_ALL // Print both the commands sent to the SMTP server and the responses from it

// Template for Mail class
class Mail;

// SMTP errors
typedef enum smtp_error {
   SMTP_OK,
   SMTP_CLIENT_NOT_CONNECTED, // Error throwned when the client has not previusly connected to the SMTP server
   SMTP_CLIENT_CONNECTION_ERROR, // Error throwned when the client cannot connect to the SMTP server
   SMTP_CONNECTION_ERROR, // Error throwned if the server not respond 220 to the connection
   SMTP_IDENTIFICATION_ERROR, // Error throwned in response the EHLO command
   SMTP_AUTH_ERROR, // Error throwned if the AUTH LOGIN command fails
   SMTP_MAIL_FROM_ERROR, // Error throwned if the MAIL TO command fails
   SMTP_RCPT_TP_ERRPR, // Error throwned if the RCPT TO command fails
   SMTP_DATA_ERROR, // Error throwned if the DATA command fails
   SMTP_BODY_ERROR, // Error throwned if the body is not valid
   SMTP_QUIT_ERROR // Error throwned if the QUIT command fails
};



//class D7S
class SMTPClient {

   public: 

      // Constructor
      SMTPClient(Client &client, const char *server, int port);

      // Connect to the SMTP server
      smtp_error connect();

      // --- SMTP commands ---
      // EHLO name
      smtp_error ehlo(const char *name);
      // AUTH LOGIN followed by login and passwd
      smtp_error authLogin(const char *login, const char *passwd);
      // MAIL FROM: <email> (the from argument must be an email address)
      smtp_error mailFrom(const char *from);
      // RCPT TO: <email> (the to argument must be an email address)
      smtp_error rcptTo(const char *to);
      // DATA
      smtp_error data();
      // send the body after the DATA command and the headers
      smtp_error body(const char *body);
      //CRLF . CRLF
      smtp_error finishData();
      // QUIT
      smtp_error quit();

      // --- SMTP HEADERS ---
      // send a header
      smtp_error header(const char *name, const char *value);

      // --- SMTP STATUS ---
      // Await for the SMTP server to respond
      bool awaitSMTPResponse(const char *response = NULL, uint16_t timeout = 10000);

      // --- MAIL ---
      // Send the email prepared using Mail class
      bool send(Mail &mail);


   private:

      // SMTP server address
      const char *_server;
      // SMTP server port
      int _port;

      // Client to be used for the connection
      Client *_client;
     
};

#endif