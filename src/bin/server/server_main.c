// C standard libraries
#include <stdio.h>   /* `perror` */
#include <assert.h>  /* `assert` */
#include <string.h>  /* `memset` */
// POSIX libraries
#include <unistd.h>  /* standard unix functions */
#include <sys/msg.h> /* for message buffer functions / defines */
// Local
#include <interface.h>

#include "interface.h"
#include "server.h"

int
main(int argc, char *argv[])
{
   struct test_msgbuf buf;
   key_t key = 0x88A4;  // This is the initial msg_queue ID

   // open/create the msgbuf
   int msg_q_id = msgget(key, 0666 | IPC_CREAT);
   if (msg_q_id == -1) { perror("msgget"); }

   // start the run loop
   bool running = true;
   bool countdown_started = false;
   int countdown_ticks = COUNTDOWN_START_TICKS;
   printf("starting run loop\n");
   while (running)
   {
      // user info
      if (countdown_started)
      {
         if (countdown_ticks ==COUNTDOWN_START_TICKS)
         {
            printf("starting countdown\n");
         }
         printf("start tick %d\n", countdown_ticks);
         countdown_ticks -= 1;
         if (countdown_ticks <= 0)
         {
            running = false;
         }
      }

      // read from the msg queue...
      bool have_message = true;
      while (have_message)
      {
         ssize_t rcv_result = msgrcv(msg_q_id,
                                     &buf,
                                     sizeof(buf.msg_data),
                                     SERVER_MSG_TYPE,
                                     IPC_NOWAIT);

         // ...and read the result
         if (rcv_result < 0)
         {
            perror("rcv_result");
            have_message = false;
         }
         else if (rcv_result > 0)
         {
            assert(buf.msg_type == SERVER_MSG_TYPE);
            int snd_result = 0;
            struct info_client *data = (struct info_client *) buf.msg_data;
            switch (data->type)
            {
               case START_COUNTDOWN:
                  printf("Starting countdown\n");
                  countdown_started = true;
                  break;

               case SEND_PING:
                  printf("got `PING`, sending `PONG`\n");
                  buf.msg_type = CLIENT_MSG_TYPE;
                  data->type = SEND_PONG;
                  data->data.plang.has_ping = true;
                  data->data.plang.has_pong = false;
                  snd_result = msgsnd(msg_q_id, &buf, MSG_DATA_SIZE, 0);
                  if (snd_result == -1)
                  {
                     perror("send ping");
                  }
                  break;

               case SEND_PONG:
                  printf("got `PONG`, sending `PING`\n");
                  buf.msg_type = CLIENT_MSG_TYPE;
                  data->type = SEND_PING;
                  data->data.plang.has_ping = false;
                  data->data.plang.has_pong = true;
                  snd_result = msgsnd(msg_q_id, &buf, MSG_DATA_SIZE, 0);
                  if (snd_result == -1)
                  {
                     perror("send ping");
                  }
                  break;
            }
         } // end message parse branch
         else
         {
            have_message = false;
         }
      } // end read msg loop

      // sleep a bit
      sleep(1);
   }

   return 0;
}
