#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <network.h>

#include "sockettest.h"

static	lwp_t httd_handle = (lwp_t)NULL;

struct in_addr bba_localip;
struct in_addr bba_netmask;
struct in_addr bba_gateway;

int InitSocket() {
	s32 ret;

	//char localip[16] = {"192.168.86.235"};
	char localip[16] = {0};
	char gateway[16] = {0};
	char netmask[16] = {0};
	
	printf ("\nlibogc network demo\n");
	printf("Configuring network ...\n");

	// Configure the network interface
	ret = if_config ( localip, netmask, gateway, TRUE);
	if (ret>=0) {
		printf ("network configured, ip: %s, gw: %s, mask %s\n", localip, gateway, netmask);

		LWP_CreateThread(	&httd_handle,	/* thread handle */ 
							httpd,			/* code */ 
							NULL,			/* arg pointer for thread */
							NULL,			/* stack base */ 
							24*1024,		/* stack size */
							40				/* thread priority */ );
	} else {
		printf ("network configuration failed!\n");
	}

	return 0;
}

const static char http_200[] = "HTTP/1.1 200 OK\r\n";

const static char indexdata[] = "<html> \
                               <head><title>A test page</title></head> \
                               <body> \
                               This small test page has had %d hits. \
                               </body> \
                               </html>";

const static char http_html_hdr[] = "Content-type: text/html\r\n\r\n";
const static char http_get_index[] = "GET / HTTP/1.1\r\n";

void *httpd (void *arg) {

	s32 sock, csock;
	int ret;
	u32	clientlen;
	struct sockaddr_in client;
	struct sockaddr_in server;
	char temp[1026];
	static int hits=0;
	
	clientlen = sizeof(client);

	sock = net_socket (AF_INET, SOCK_STREAM, IPPROTO_IP);
	printf("Got net_socket %d!\n", sock);

	if (sock == INVALID_SOCKET) {
      printf ("Cannot create a socket!\n");
    } else {

		memset (&server, 0, sizeof (server));
		memset (&client, 0, sizeof (client));

		server.sin_family = AF_INET;
		server.sin_port = htons (80);
		server.sin_addr.s_addr = INADDR_ANY;
		ret = net_bind (sock, (struct sockaddr *) &server, sizeof (server));
		
		if ( ret ) {

			printf("Error %d binding socket!\n", ret);

		} else {

			if ( (ret = net_listen( sock, 5)) ) {

				printf("Error %d listening!\n", ret);

			} else {
			
				while(1) {
	
					csock = net_accept (sock, (struct sockaddr *) &client, &clientlen);
					printf("Got socket %d!\n", csock);
				
					if ( csock < 0 ) {
						printf("Error connecting socket %d!\n", csock);
						while(1);
					}

					printf("Connecting port %d from %s\r\n", client.sin_port, inet_ntoa(client.sin_addr));
					memset (temp, 0, 1026);
					ret = net_recv (csock, temp, 1024, 0);
					printf("Received %d bytes\n", ret);

					if ( !strncmp( temp, http_get_index, strlen(http_get_index) ) ) {
						hits++;
						net_send(csock, http_200, strlen(http_200), 0);
						net_send(csock, http_html_hdr, strlen(http_html_hdr), 0);
						sprintf(temp, indexdata, hits);
						net_send(csock, temp, strlen(temp), 0);
					}

					net_close (csock);

				}
			}
		}
	}
	return NULL;
}