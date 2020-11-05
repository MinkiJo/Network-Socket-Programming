#include <sys/types.h>




#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>

 

#include <pthread.h>

#include <stdio.h>

#include <string.h>
#include<stdlib.h>

 

void * socketthread(void * parm);       /* thread function prototype    */

 
pthread_mutex_t  mut; 

#define PROTOPORT         5193          // port 기본값 

#define QLEN              6             /* size of request queue        */
#define BUF_SIZE 1000
char msg[BUF_SIZE];
 
int visits =  0;                        /* counts client connections     */

 

main (int argc, char *argv[])

{

     struct   hostent   *ptrh;     /* pointer to a host table entry */
     struct   protoent  *ptrp;     /* pointer to a protocol table entry */
     struct   sockaddr_in sad;     /* structure to hold server's address */
     struct   sockaddr_in cad;     /* structure to hold client's address */
     int      sd, sd2;             /* socket descriptors */
     int      port;                /* protocol port number */
     int      alen;                /* length of address */
     pthread_t  tid;           /* variable to hold thread ID */

     pthread_mutex_init(&mut, NULL);
     memset((char  *)&sad,0,sizeof(sad)); /* clear sockaddr structure   */
     sad.sin_family = AF_INET;            /* set family to Internet     */
     sad.sin_addr.s_addr = INADDR_ANY;    /* set the local IP address */


 if (argc > 1) {                        /* if argument specified     */

                     port = atoi (argv[1]); /* convert argument to binary*/

     } else {

                      port = PROTOPORT;     /* use default port number   */

     }

     if (port > 0)                          /* test for illegal value    */

                      sad.sin_port = htons((u_short)port);

     else {                                /* print error message and exit */

                      fprintf (stderr, "bad port number %s/n",argv[1]);

                      exit (1);

     }

/* Map TCP transport protocol name to protocol number */

     

     if ( ((int)(ptrp = getprotobyname("tcp"))) == 0)  {

                     fprintf(stderr, "cannot map \"tcp\" to protocol number");

                     exit (1);

     }

 

     /* Create a socket */

     sd = socket (PF_INET, SOCK_STREAM, ptrp->p_proto);

     if (sd < 0) {

                       fprintf(stderr, "socket creation failed\n");

                       exit(1);

     }

 

     /* Bind a local address to the socket */

     if (bind(sd, (struct sockaddr *)&sad, sizeof (sad)) < 0) {

                        fprintf(stderr,"bind failed\n");

                        exit(1);

     }

 

     /* Specify a size of request queue */ 

     if (listen(sd, QLEN) < 0) {  //listen

                        fprintf(stderr,"listen failed\n");

                         exit(1);

     }

 

     alen = sizeof(cad);

 /* Main server loop - accept and handle requests */

     fprintf( stderr, "Server up and running.\n");

     while (1) {

 

         printf("SERVER: Waiting for contact ...\n");

         

         if (  (sd2=accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {

                         fprintf(stderr, "accept failed\n");

                              exit (1);

    }

	printf("새로운 클라이언트가 접속하였습니다\n");
    pthread_create(&tid, NULL, socketthread, (void *) sd2 );

     }

     close(sd);

}

void * socketthread(void * parm)
{
	
   int tsd, tvisits;
   int str_len=0; 
   char  buf[100];           /* buffer for string the server sends */
   tsd = (int) parm;
   pthread_mutex_lock(&mut);
        tvisits = ++visits;
   pthread_mutex_unlock(&mut);

   
   while((str_len=read(tsd,msg,sizeof(msg)))!=0)
   {

	
	printf("메세지가 왔습니다 %s\n",msg);
		
	send(tsd,msg,str_len,0);
	
	memset(msg,'\0',sizeof(msg));

	
}
   
   close(tsd);
   pthread_exit(0);
    
}


