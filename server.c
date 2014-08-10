#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int auth=0, tls=0;		//Flags to keep track of whether or not user is authenticated/ tls is started 
char ip[25], recvBuffer[1000], sendBuffer[1000];
GSocket *socket;
GError *error=NULL, *error2=NULL;
GCancellable *cancellable;
GSocket *newSocket;
GSocketAddress *address;




void clearBuffers()
{
		memset(sendBuffer,'\0',sizeof(sendBuffer));
		memset(recvBuffer,'\0',sizeof(recvBuffer));
}

void respond(char* resp)
{
			clearBuffers();
			strcpy(sendBuffer,resp);
			g_socket_send (newSocket,sendBuffer,strlen(sendBuffer),cancellable,&error2);
			memset(sendBuffer,'\0',sizeof(sendBuffer));
			clearBuffers();
}

void capability_function()
{
	clearBuffers();
	if(auth == 1)
	{

	}
	else
	{
		if(tls==1)
		{

		}

		else
		{
			respond("YES\n");
			clearBuffers();			
			strcpy(sendBuffer,"Capabilities: capability starttls authenticate");
			g_socket_send (newSocket,sendBuffer,strlen(sendBuffer),cancellable,&error2);
			memset(sendBuffer,'\0',sizeof(sendBuffer));			

		}
	}
}







void server_function(int serverPort)
{

	printf("Enter IP\n");
	//scanf("%s",ip);
	strcpy(ip ,"127.0.0.1");
	printf("I have entered the server\n");
	//creating the socket

	socket = g_socket_new (G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM,0, &error);


	//checking if the socket is formed
	if(socket == NULL)
	{
		printf("Error in creating the socket.\n");
		return;
	}
	cancellable = g_cancellable_new ();
	
	address = g_inet_socket_address_new_from_string (ip,serverPort);

	//Binding the socket
	if(!(g_socket_bind (socket,address, TRUE ,&error)))
	{
		printf("Error in binding socket\n");
		return;
	}

	printf("Socket binding to server successful\n");

	//Begin listening
	g_socket_listen (socket, &error);
	if(error!= NULL)
	{
		printf("Error in listening\n");
		return;
	}
	printf("TCPServer waiting for an client on port %d\n", serverPort);


	//Accepting connection from client
	newSocket = g_socket_accept (socket,cancellable,&error2);

	if(newSocket == NULL)
	{
		printf("No connection\n");
		return;
	}











	////////////////////Starting the protocol////////////////////////

	//Initial greeting from the server to client
	strcpy(sendBuffer, "Hello Client\nYou are currently in the non-authenticated state.\nCapabilities: starttls authenticate capability\n");
	g_socket_send (newSocket,sendBuffer,strlen(sendBuffer),cancellable,&error2);
	clearBuffers();
	


	//Subsequent interaction
	while(1)	
	{
		g_socket_receive (newSocket,recvBuffer,1000,cancellable,&error2);
		if(strcmp(recvBuffer,"capability") == 0)
		{
			capability_function();
		}
		//Clearing the value of the send and recv buffers at the end of each iteration
		memset(sendBuffer,'\0',sizeof(sendBuffer));
		memset(recvBuffer,'\0',sizeof(recvBuffer));
	}






}


int main()
{
	g_type_init();
	unsigned int serverPort;
	printf("Welcome \nPlease Enter the port on which you want the server: ");
	scanf("%d", &serverPort);
	server_function(serverPort);

}