#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include <stdio.h>

#include <string.h>
#include<stdlib.h>

 

void * rec_msg(void * parm);       /* thread function prototype    */
void*send_msg();
 
pthread_mutex_t  mut; 

#define PROTOPORT         5193          // port 기본값 

#define QLEN              6             /* size of request queue        */
#define BUF_SIZE 1000
char msg[BUF_SIZE];
 
int visits =  0;                        /* counts client connections     */

 char result[100];
int count = 0, count2 =0;
char RSP;
int csk[100];
 char intro[] = "<가위바위보 게임 시작> 가위[S], 바위[R], 보[P]\n";
     char win[] = "win";
     char lose[] = "lose";
     char draw[] = "again"; 
  char end[]="게임이 끝났습니다"; 



main (int argc, char *argv[])

{

     struct   hostent   *ptrh;     /* pointer to a host table entry */
     struct   protoent  *ptrp;     /* pointer to a protocol table entry */
     struct   sockaddr_in sad;     /* structure to hold server's address */
     struct   sockaddr_in cad;     /* structure to hold client's address */
     int      sd, sd2;             /* socket descriptors */
     int      port;                /* protocol port number */
     int      alen;                /* length of address */
     pthread_t  tid1, tid2;           /* variable to hold thread ID */

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
	write(sd2,intro,sizeof(intro));
	pthread_mutex_lock(&mut);
	csk[count++] = sd2;
	pthread_mutex_unlock(&mut);
   	 pthread_create(&tid1, NULL, rec_msg, (void *) sd2 );
  	pthread_create(&tid2, NULL, send_msg, NULL );

     }

     close(sd);

}

void * rec_msg(void * parm)
{
	
   int tsd;
   int str_len=0; 
   //char  buf[100];           /* buffer for string the server sends */
   tsd = (int) parm;  
   
   while((str_len=read(tsd,&result[tsd],sizeof(char)))!=0)
   {	
	printf("유저 %d이 %c을 냈습니다\n",tsd,result[tsd]);
	  pthread_mutex_lock(&mut);
	count2++;	//두번내면 게임판별
	pthread_mutex_unlock(&mut);
	//printf("%d", count2);
	
	
	
}
   
   close(tsd);
   pthread_exit(0);
    
}
void*send_msg()
{


while(1){
	
	if(count2 == 2)
	{
	
		
		pthread_mutex_lock(&mut);
		if(result[csk[0]] == result[csk[1]]){
		printf("비겼습니다\n");			
		write(csk[0], draw,sizeof(draw));
		write(csk[1], draw,sizeof(draw));	 
		count2 = 0;	
		result[csk[0]] = 'a'; result[csk[0]] = 'b'; //두개의쓰레드가 같은것을 수행 하지못하도록 함

		}
	
		else if((result[csk[0]]=='P'&&result[csk[1]]=='S') || (result[csk[0]]=='S'&&result[csk[1]]=='R') || (result[csk[0]]=='R'&&result[csk[1]]=='P')){
			printf("유저 %d가 승리했습니다\n",csk[1]);	
			write(csk[0], lose,sizeof(lose));
			write(csk[1], win,sizeof(win));
			printf("게임을 종료합니다\n");
			
			pthread_exit(0);
		}
		else if((result[csk[0]]=='S'&&result[csk[1]]=='P') || (result[csk[0]]=='R'&&result[csk[1]]=='S') || (result[csk[0]]=='P'&&result[csk[1]]=='R')){
			printf("유저 %d가 승리했습니다\n",csk[0]);	
			write(csk[1], lose,sizeof(lose));
			write(csk[0], win,sizeof(win));
			printf("게임을 종료합니다\n");
			pthread_exit(0);
		}
		
	pthread_mutex_unlock(&mut);

	}

}



}

