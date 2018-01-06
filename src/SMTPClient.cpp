/*
   Copyright 2016 Alessandro Pasqualini
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
     http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   
   @author         Alessandro Pasqualini <alessandro.pasqualini.1105@gmail.com>
   @url            https://github.com/alessandro1105
*/

#include "SMTPClient.h"
#include <Mail.h>

// Constructor
SMTPClient::SMTPClient(Client &client, const char *server, int port): _client(&client), _server(server), _port(port) {}

// Connect to the SMTP server
smtp_status SMTPClient::connect() {
   // Check if the client is connected
   if (_client->connected()) {
      return SMTP_CLIENT_ALREADY_CONNECTED;
   }
   // Connect to the SMTP server
   if (!_client->connect(_server, _port)) {
      // The client cannot connect to the SMTP server
      return SMTP_CLIENT_CANNOT_CONNECT;
   }
   // Check the server response
   if (!awaitSMTPResponse("220")) {
      return SMTP_CONNECTION_ERROR;
   }

   return SMTP_OK;
}

// Set the AUTH LOGIN credentials
void SMTPClient::setAuthLogin(const char *login, const char *passwd) {
   // Save the Auth Login information
   _login = login;
   _passwd = passwd;
}

// --- SMTP commands ---
// AUTH LOGIN followed by login and passwd
smtp_status SMTPClient::authLogin(const char *login, const char *passwd) {
   // Save the Auth Login information
   _login = login;
   _passwd = passwd;

   // Call the authLogin
   authLogin();
}

// AUTH LOGIN followed by login and passwd
smtp_status SMTPClient::authLogin() {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }
   // Check if the Auth Login information has been set
   if (_login == NULL || _passwd == NULL) {
      return SMTP_AUTH_NOT_SET;
   }

   #if defined(DEBUG_ALL)
      Serial.println("AUTH LOGIN ");
   #endif
   // Send the command AUTH LOGIN
   _client->println("AUTH LOGIN");
   // Wait fo the confirmation
   if(!awaitSMTPResponse("334")) {
      return SMTP_AUTH_ERROR;
   }

   #if defined(DEBUG_ALL)
      Serial.println(_login);
   #endif
   // Send the login
   _client->println(_login);
   // Wait fo the confirmation
   if(!awaitSMTPResponse("334")) {
      return SMTP_AUTH_ERROR;
   }

   #if defined(DEBUG_ALL)
      Serial.println(_passwd);
   #endif
   // Send the passwd
   _client->println(_passwd);
   // Wait fo the confirmation
   if(!awaitSMTPResponse("235")) {
      return SMTP_AUTH_ERROR;
   }

   return SMTP_OK;
}

// EHLO name
smtp_status SMTPClient::ehlo(const char *name) {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.print("EHLO ");
      Serial.println(name);
   #endif
   // Send the command 
   _client->print("EHLO ");
   _client->println(name);
   // Wait fo the confirmation
   if(!awaitSMTPResponse("250")) {
      return SMTP_IDENTIFICATION_ERROR;
   }

   return SMTP_OK;
}

// MAIL FROM: <email> (the from argument must be an email address)
smtp_status SMTPClient::mailFrom(const char *from) {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.print("MAIL FROM: <");
      Serial.print(from);
      Serial.println(">");
   #endif
   // Send the command 
   _client->print("MAIL FROM: <");
   _client->print(from);
   _client->println(">");
   // Wait fo the confirmation
   if(!awaitSMTPResponse("250")) {
      return SMTP_MAIL_FROM_ERROR;
   }

   return SMTP_OK;
}

// RCPT TO: <email> (the to argument must be an email address)
smtp_status SMTPClient::rcptTo(const char *to) {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.print("RCPT TO: <");
      Serial.print(to);
      Serial.println(">");
   #endif
   // Send the command 
   _client->print("RCPT TO: <");
   _client->print(to);
   _client->println(">");
   // Wait fo the confirmation
   if(!awaitSMTPResponse("250")) {
      return SMTP_RCPT_TP_ERROR;
   }

   return SMTP_OK;
}

// DATA
smtp_status SMTPClient::data() {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.println("DATA");
   #endif
   // Send the command 
   _client->println("DATA");
   // Wait fo the confirmation
   if(!awaitSMTPResponse("354")) {
      return SMTP_DATA_ERROR;
   }

   // Data command has been sent
   _dataSent = true;

   return SMTP_OK;
}

// send the body after the DATA command and the headers
smtp_status SMTPClient::body(const char *body) {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.println(body);
   #endif
   // Send the command 
   _client->println(body);

   _bodySent = true;

   return SMTP_OK;
}

//CRLF . CRLF
smtp_status SMTPClient::finishData() {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.println(".");
   #endif
   // Send the command 
   _client->println(".");
   // Wait fo the confirmation
   if(!awaitSMTPResponse("250")) {
      return SMTP_BODY_ERROR;
   }

   // Data command has been closed
   _dataSent = false;
   _bodySent = false;

   return SMTP_OK;
}

// QUIT
smtp_status SMTPClient::quit() {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial.println("QUIT");
   #endif
   // Send the command 
   _client->println("QUIT");
   // Wait fo the confirmation
   if(!awaitSMTPResponse("221")) {
      return SMTP_QUIT_ERROR;
   }

   return SMTP_OK;
}

// --- SMTP HEADERS ---
// send a header
smtp_status SMTPClient::header(const char *name, const char *value) {
   // Check if the client is connected
   if (!_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }
   // Check id the command data has been sent
   if (!_dataSent) {
      return SMTP_DATA_NOT_SENT;
   }
   // Check if the body has been already sent
   if (_bodySent) {
      return SMTP_BODY_ALREADY_SENT;
   }

   #if defined(DEBUG_ALL)
      Serial.print(name);
      Serial.print(": ");
      Serial.println(value);
   #endif
   // Send the command 
   _client->print(name);
   _client->print(": ");
   _client->println(value);

   return SMTP_OK;
}

// --- SMTP STATUS ---
// Await for the SMTP server to respond
bool SMTPClient::awaitSMTPResponse(const char *response, uint16_t timeout) {
   uint32_t time = millis();

  // Wait until the SMTP server respond or the timeout expires
  while (!_client->available()) {
    if (millis() > (time + timeout)) {
      return false;
    }
  }

  String serverReponse = _client->readStringUntil('\n');

  #if defined(DEBUG) || defined(DEBUG_ALL)
    Serial.println(serverReponse);
  #endif

  // Check if the response given from the server is wrong
  if (response != NULL && strstr(serverReponse.c_str(), response) == NULL) {
    // If there are other characters on the buffer try again
    if (_client->available()) {
      return awaitSMTPResponse(response, timeout);
    }

    return false;
  }

  // Emptying the buffer
  while (_client->available()) {
    char c = _client->read();
    
    #if defined(DEBUG) || defined(DEBUG_ALL)
      Serial.print(c);
    #endif
  }

  return true;
}

// --- MAIL ---
// Send the email prepared using Mail class
smtp_status SMTPClient::send(Mail &mail) {
   // Status 
   smtp_status status;
   // Connect to the SMTP server
   if ((status = connect()) != SMTP_OK) {
      return status;
   }
   // EHLO friend
   if ((status = ehlo("friend")) != SMTP_OK) {
      return status;
   }
   // AUTH LOGIN
   if ((status = authLogin()) != SMTP_OK) {
      return status;
   }
   // MAIL FROM: <address>
   if ((status = mailFrom(mail._from[0])) != SMTP_OK) {
      return status;
   }
   // RCPT TO: <address>
   for (int i = 0; i < mail._recipientIndex; i++) {
      if ((status = rcptTo(mail._recipients[i][0])) != SMTP_OK) {
         return status;
      }
   }
   // DATA
   if ((status = data()) != SMTP_OK) {
      return status;
   }

   // HEADERS
   // From: Name <address>
   char from[strlen(mail._from[0]) + strlen (mail._from[1]) + 3 + 1];
   from[0] = 0;
   compileAddress(from, mail._from[0], mail._from[1]);
   if ((status = header("From", (const char *) from)) != SMTP_OK) {
      return status;
   }

   // To: Name <address>
   size_t length = countAddressesLength(mail._recipients, mail._recipientsType, mail._recipientIndex, MAIL_RECIPIENT_TO);
   if (length > 0) {
      char to[length];
      compileAddresses(to, length, mail._recipients, mail._recipientsType, mail._recipientIndex, MAIL_RECIPIENT_TO);
      if ((status = header("To", (const char *) to)) != SMTP_OK) {
         return status;
      }
   }

   // CC: Name <address>
   length = countAddressesLength(mail._recipients, mail._recipientsType, mail._recipientIndex, MAIL_RECIPIENT_CC);
   if (length > 0) {
      char cc[length];
      compileAddresses(cc, length, mail._recipients, mail._recipientsType, mail._recipientIndex, MAIL_RECIPIENT_CC);
      if ((status = header("CC", (const char *) cc)) != SMTP_OK) {
         return status;
      }
   }

   // Subject
   if (mail._subject != NULL && (status = header("Subject", mail._subject)) != SMTP_OK) {
      return status;
   }

   // Subject
   if (mail._replyTo != NULL && (status = header("ReplyTO", mail._replyTo)) != SMTP_OK) {
      return status;
   }

   // Body
   if ((status = body(mail._body)) != SMTP_OK) {
      return status;
   }

   // FinishData
   if ((status = finishData()) != SMTP_OK) {
      return status;
   }

   // Quit
   return quit();
}

// Compile address in the form "Name <address>"
void SMTPClient::compileAddress(char *buffer, const char *address, const char *name) {
   strcat(buffer, name);
   strcat(buffer, " <");
   strcat(buffer, address);
   strcat(buffer, ">");
}

// Compile addresses in the form "Name <address>, Name <address>..."
void SMTPClient::compileAddresses(char *buffer, size_t length, const char *recipients[][2], mail_recipient_type recipientsType[], size_t size, mail_recipient_type type) {
   buffer[0] = 0;
   
   // Determine the length of the string 
   for (int i = 0; i < size; i++) {
      // If the type is right
      if (recipientsType[i] == type) {
         compileAddress(buffer, recipients[i][0], recipients[i][1]);
         strcat(buffer, ", ");
      }
   }

   buffer[length -2] = 0;
}

// Length of the addresses
size_t SMTPClient::countAddressesLength(const char *recipients[][2], mail_recipient_type recipientsType[], size_t size, mail_recipient_type type) {
   size_t length = 0;
   // Determine the length of the string 
   for (int i = 0; i < size; i++) {
      // If the type is right
      if (recipientsType[i] == type) {
         length += strlen(recipients[i][0]) + strlen(recipients[i][1]) + 3 + 2;
      }
   }

   return length;
}