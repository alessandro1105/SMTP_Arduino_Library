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

#include "Mail.h"

// Constructor
Mail::Mail() {
   // Initializing variables
   _from[0] = NULL;
   _from[1] = NULL;
   _subject = NULL;
   _body = NULL;
   _replyTo = NULL;
}

// Set the sender of the mail
bool Mail::from(const char *address, const char *name) {
   _from[0] = address;
   _from[1] = name;

   return true;
}

// Set the reply to of the mail
bool Mail::replyTo(const char *replyTo) {
   _replyTo = replyTo;

   return true;
}
// Add a recipient to the mail

bool Mail::to(const char *address, const char *name) {
   // Check if there is space to add a new recipient
   if (_recipientIndex > MAIL_MAX_RECIPIENTS) {
      return false;
   }

   // Save the header
   _recipients[_recipientIndex][0] = address;
   _recipients[_recipientIndex][1] = name;
   _recipientsType[_recipientIndex] = MAIL_RECIPIENT_TO;

   // Increment the index
   _recipientIndex++;
   
   return true;
}

// Add a CC recipient
bool Mail::cc(const char *address, const char *name) {
   // Check if there is space to add a new recipient
   if (_recipientIndex > MAIL_MAX_RECIPIENTS) {
      return false;
   }

   // Save the header
   _recipients[_recipientIndex][0] = address;
   _recipients[_recipientIndex][1] = name;
   _recipientsType[_recipientIndex] = MAIL_RECIPIENT_CC;

   // Increment the index
   _recipientIndex++;
   
   return true;
}

// Add BCC recipient
bool Mail::bcc(const char *address) {
   // Check if there is space to add a new recipient
   if (_recipientIndex > MAIL_MAX_RECIPIENTS) {
      return false;
   }

   // Save the header
   _recipients[_recipientIndex][0] = address;
   _recipients[_recipientIndex][1] = NULL;
   _recipientsType[_recipientIndex] = MAIL_RECIPIENT_BCC;

   // Increment the index
   _recipientIndex++;
   
   return true;
}

// Subjecto of the mail
bool Mail::subject(const char *subject) {
   _subject = subject;

   return true;
}

// Set the body of the mail
bool Mail::body(const char *body) {
   _body = body;

   return true;
}

// Set personalized header
bool Mail::header(const char *name, const char *value) { // do not include the colon symbol in the name of the header
   // Check if there is space to add a new headers
   if (_headerIndex > MAIL_MAX_HEADERS) {
      return false;
   }

   // Save the header
   _headers[_headerIndex][0] = name;
   _headers[_headerIndex][1] = value;

   // Increment the index
   _headerIndex++;

   return true;
}

