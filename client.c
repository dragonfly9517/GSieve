#include <glib.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

char ip[25], recvBuffer[1000], sendBuffer[1000], command[1000];






void clearBuffers()
{
		memset(sendBuffer,'\0',sizeof(sendBuffer));
		memset(recvBuffer,'\0',sizeof(recvBuffer));
}



void client_function(int clientPort)
{

	printf("Enter IP\n");
	//scanf("%s",ip);
	strcpy(ip ,"127.0.0.1");
	printf("I have entered the client\n");
	//creating the socket
	GSocket *socket;
	GError *error=NULL, *error2=NULL;
	socket = g_socket_new (G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM,0, &error);


	//checking if the socket is formed
	if(socket == NULL)
	{
		printf("Error in creating the socket.\n");
		return;
	}

	GSocketAddress *address;
	address = g_inet_socket_address_new_from_string (ip,clientPort);


	//Binding the socket
/*	if(!(g_socket_bind (socket,address, FALSE ,&error)))
	{
		printf("Error in binding socket\n");
		return;
	}
	printf("Socket binding to client successful\n");*/


	//Attempting to connect to server
	GCancellable *cancellable = g_cancellable_new ();
	g_socket_connect (socket,address,cancellable,&error);
	if(error!= NULL)
	{
		printf("Connection failed\n");
		return;
	}


	printf("Connection successful\n");

	//Starting the protocol

	//Initial greeting
	int len;
	len= g_socket_receive (socket,recvBuffer,1000,cancellable,&error);
	recvBuffer[len]='\0';
	printf("%s\n",recvBuffer);
	clearBuffers();

	//Subsequent interaction
	while(1)
	{

		scanf("%s",command);
		//printf("command was:%s\n", command);
		strcpy(sendBuffer,command);
		//printf("sending: %s\n",sendBuffer );
		g_socket_send (socket,sendBuffer,strlen(sendBuffer),cancellable,&error);
		clearBuffers();		
		//printf("Message sent. Awaiting response\n");
		g_socket_receive (socket,recvBuffer,1000,cancellable,&error);
		printf("S: %s\n",recvBuffer);
		clearBuffers();
		/*g_socket_receive (socket,recvBuffer,1000,cancellable,&error);
		printf("S: %s\n",recvBuffer);
		clearBuffers();*/


		//Clearing the value of the send and recv buffers at the end of each iteration
		memset(sendBuffer,'\0',sizeof(sendBuffer));
		memset(recvBuffer,'\0',sizeof(recvBuffer));
	}



}




int main()
{
	g_type_init();
	unsigned int clientPort;
	printf("Welcome \nPlease Enter the port on which you want the client: ");
	scanf("%d", &clientPort);
	client_function(clientPort);

}