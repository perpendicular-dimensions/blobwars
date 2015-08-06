/*
Copyright (C) 2010-2011 Parallel Realities

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "headers.h"

MedalServer::MedalServer()
{
	lock = SDL_CreateMutex();
	
	connected = false;
	gotRuby = false;
	
	message[0] = 0;
	rubyMessage[0] = 0;
}

MedalServer::~MedalServer()
{
	if (connected)
	{
		close();
	}
}

bool MedalServer::connect(const char *privateKey)
{
	if (SDLNet_ResolveHost(&ip, MEDAL_SERVER_HOST, MEDAL_SERVER_PORT) == -1)
	{
		printf("ERROR: SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return false;
	}
	
	debug(("Connected %s to %s:%d\n", privateKey, MEDAL_SERVER_HOST, MEDAL_SERVER_PORT));
	
	strlcpy(this->privateKey, privateKey, sizeof this->privateKey);
	connected = true;
	
	return true;
}

#if !UNIX
extern char *strtok_r(char *s1, const char *s2, char **lasts);
#endif
		
int MedalServer::postMedal(const char *str)
{
	if (!connected)
	{
		return 0;
	}
	
	char *store;
	
	char medal[128];
	strlcpy(medal, str, sizeof medal);
	
	for (unsigned int i = 0 ; i < strlen(medal) ; i++)
	{
		if (medal[i] == ' ' || medal[i] == '#' || medal[i] == ',')
		{
			medal[i] = '_';
		}
	}
	
	debug(("Attempting to post medal 'MBS_%s'\n", medal));
	
	TCPsocket socket = SDLNet_TCP_Open(&ip);
	
	if (!socket)
	{
		printf("ERROR: SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return 0;
	}
	
	char out[1024];
	
	snprintf(out, sizeof out, "GET /addMedal/%s/MBS_%s HTTP/1.1\nHost: %s\nUser-Agent:BWMBS%.2f-%d\n\n", privateKey, medal, MEDAL_SERVER_HOST, VERSION, RELEASE);
	
	//printf("%s\n", out);
	
	int len = strlen(out) + 1;
	
	if (SDLNet_TCP_Send(socket, (void*)out, len) < len)
	{
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		printf("Disconnected\n");
		SDLNet_TCP_Close(socket);
		close();
		return 0;
	}
	
	char *in = new char[1024];

	SDLNet_TCP_Recv(socket, in, 512);
	
	//printf("%s\n", in);
	
	int response = 0;
	char *token = strtok_r(in, "\n", &store);
	
	while (token != NULL)
	{
		if (strstr(token, "MEDAL_RESPONSE"))
		{
			sscanf(token, "%*s %d %[^\n\r]", &response, message);
			
			if (response == 4)
			{
				strlcpy(rubyMessage, message, sizeof rubyMessage);
				gotRuby = true;
			}
			else
			{
				break;
			}
		}
		
		token = strtok_r(NULL, "\n", &store);
	}
	
	debug(("MedalServer Response: %d '%s'\n", response, message))
	
	delete[] in;
	
	SDLNet_TCP_Close(socket);
	
	return response;
}

const char *MedalServer::getMessage()
{
	return message;
}

bool MedalServer::hasRuby()
{
	return gotRuby;
}

const char *MedalServer::getRubyMessage()
{
	gotRuby = false;
	
	return rubyMessage;
}

void MedalServer::close()
{
	connected = false;
	
	SDL_DestroyMutex(lock);
}
