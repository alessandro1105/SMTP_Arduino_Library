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

   // Constructor
SMTPClient::SMTPClient(Client &client, const char *server, int port): _client(&client), _server(server), _port(port) {}

// Connect to the SMTP server
smtp_error SMTPClient::connect() {

}

// --- SMTP commands ---
// EHLO name
smtp_error SMTPClient::ehlo(const char *name) {
   // Check if the client is connected
   if (_client->connected()) {
      return SMTP_CLIENT_NOT_CONNECTED;
   }

   #if defined(DEBUG_ALL)
      Serial->print("EHLO ");
      Serial->println(name);
   #endif

   _client->print("EHLO ");
   _client->println(name);

   if(!awaitSMTPResponse("250")) {
      return SMTP_IDENTIFICATION_ERROR;
   }

   return SMTP_OK;
}
// AUTH LOGIN followed by login and passwd
smtp_error SMTPClient::authLogin(const char *login, const char *passwd) {

}
// MAIL FROM: <email> (the from argument must be an email address)
smtp_error SMTPClient::mailFrom(const char *from) {

}
// RCPT TO: <email> (the to argument must be an email address)
smtp_error SMTPClient::rcptTo(const char *to) {

}
// DATA
smtp_error SMTPClient::data() {

}
// send the body after the DATA command and the headers
smtp_error SMTPClient::body(const char *body) {

}
//CRLF . CRLF
smtp_error SMTPClient::finishData() {

}
// QUIT
smtp_error SMTPClient::quit() {

}

// --- SMTP HEADERS ---
// send a header
smtp_error SMTPClient::header(const char *name, const char *value) {

}

// --- SMTP STATUS ---
// Await for the SMTP server to respond
bool SMTPClient::awaitSMTPResponse(const char *response = NULL, uint16_t timeout = 10000) {

}

// --- MAIL ---
// Send the email prepared using Mail class
bool SMTPClient::send(Mail &mail) {

}