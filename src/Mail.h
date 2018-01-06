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

#ifndef Mail_H
#define Mail_H

#include <Arduino.h>
#include <SMTPClient.h>

#define MAIL_MAX_RECIPIENTS 9
#define MAIL_MAX_HEADERS 3

//--- DEBUG ----
//comment this line to disable all debug information
#define DEBUG

// Mail recipient type
typedef enum mail_recipient_type: uint8_t {
  MAIL_RECIPIENT_TO,    // To field
  MAIL_RECIPIENT_CC,    // CC field
  MAIL_RECIPIENT_BCC    // BCC field
};

//class D7S
class Mail {

   public: 

      // Constructor
      Mail();

      // Set the sender of the mail
      bool from(const char *address, const char *name = NULL);
      // Set the reply to of the mail
      bool replyTo(const char *replyTo);
      // Add a recipient to the mail
      bool to(const char *address, const char *name = NULL);
      // Add a CC recipient
      bool cc(const char *address, const char *name = NULL);
      // Add BCC recipient
      bool bcc(const char *address);
      // Subjecto of the mail
      bool subject(const char *subject);
      // Set the body of the mail
      bool body(const char *body);
      // Set personalized header
      bool header(const char *name, const char *value); // do not include the colon symbol in the name of the header


   protected:

      // From field
      const char *_from[2];
      // Subject of the mail
      const char *_subject;
      // Body of the message
      const char *_body;
      // ReplyTo
      const char *_replyTo;
      // Recipients
      const char *_recipients[MAIL_MAX_RECIPIENTS][2];
      mail_recipient_type _recipientsType[MAIL_MAX_RECIPIENTS];
      // Headers
      const char *_headers[MAIL_MAX_HEADERS][2];

      // Indexes
      size_t _recipientIndex = 0;
      size_t _headerIndex = 0;

   friend class SMTPClient;
     
};

#endif