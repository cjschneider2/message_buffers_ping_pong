#ifndef INTERFACE_H
#define INTERFACE_H 1

#include <stdbool.h>

// server message index
#define SERVER_MSG_TYPE 1
#define CLIENT_MSG_TYPE 2


// shared buffer objects
enum commands {
   START_COUNTDOWN,
   SEND_PING,
   SEND_PONG,
};

struct ping_pong {
   bool has_ping;
   bool has_pong;
};

struct info_client {
   enum commands type;
   union {
      struct ping_pong plang;
   } data;
};

// Shared buffer description
#define MSG_DATA_SIZE sizeof(struct info_client)
struct test_msgbuf {
   long msg_type;
   char msg_data[MSG_DATA_SIZE];
};

#endif // INTERFACE_H
