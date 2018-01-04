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
#include <Client.h>
#include <SMTPClient.h>

//--- DEBUG ----
//comment this line to disable all debug information
#define DEBUG

//class D7S
class Mail {

   public: 

      // Constructor
      Mail();

      // Set the sender of the mail
      void from(const char *from); // Must be of the type "Name <address>" or "address"
      // Set the reply to of the mail
      void replyTo(const char *to); // Must be of the type "Name <address>" or "address"
      // Add a recipient to the mail
      void to(const char *to); // Must be of the type "Name <address>" or "address"
      // Add a CC recipient
      void cc(const char *cc); // Must be of the type "Name <address>" or "address"
      // Add BCC recipient
      void bcc(const char *bcc); // Must be of the type "Name <address>" or "address"
      // Subjecto of the mail
      void subject(const char *subject);
      // Set the body of the mail
      void body(const char *body);
      // Set personalized header
      void header(const char *name, const char *value); // do not include the colon symbol in the name of the header


   protected:

      // From field
      const char *_from;
      // Subject of the mail
      const char *_subject;
      // Body of the message
      const char *_body;

   friend class SMTPClient;
     
};

#endif