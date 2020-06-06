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
}

MedalServer::~MedalServer()
{
	if (connected)
	{
		close();
	}
}

bool MedalServer::connect(const std::string &privateKey)
{
	if (SDLNet_ResolveHost(&ip, MEDAL_SERVER_HOST, MEDAL_SERVER_PORT) == -1)
	{
		printf("ERROR: SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return false;
	}

	debug("Connected {} to {}:{}\n", privateKey, MEDAL_SERVER_HOST, MEDAL_SERVER_PORT);

	this->privateKey = privateKey;
	connected = true;

	return true;
}

int MedalServer::postMedal(const std::string &str)
{
	if (!connected)
	{
		return 0;
	}

	std::string medal = str;

	for (auto &&c: medal)
	{
		if (c == ' ' || c == '#' || c == ',')
		{
			c = '_';
		}
	}

	debug("Attempting to post medal 'MBS_{}'\n", medal);

	TCPsocket socket = SDLNet_TCP_Open(&ip);

	if (!socket)
	{
		printf("ERROR: SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return 0;
	}

	std::string out = fmt::format("GET /addMedal/{}/MBS_{} HTTP/1.1\nHost: {}\nUser-Agent:BWMBS{:.2f}-{}\n\n", privateKey, medal, MEDAL_SERVER_HOST, VERSION, RELEASE);

	if (SDLNet_TCP_Send(socket, (void *)out.data(), out.size()) < (int)out.size())
	{
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		printf("Disconnected\n");
		SDLNet_TCP_Close(socket);
		close();
		return 0;
	}

	std::vector<char> in;
	in.reserve(1024);

	int len = SDLNet_TCP_Recv(socket, in.data(), 512);

	if (len <= 0)
	{
		printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		printf("Disconnected\n");
		SDLNet_TCP_Close(socket);
		close();
		return 0;
	}

	in[len] = 0;
	in.resize(len + 1);

	int response = 0;
	char tmpMessage[512];

	for (auto token: split(in, '\n'))
	{
		if (contains(token, "MEDAL_RESPONSE"))
		{
			scan(token, "%*s %d %[^\n\r]", &response, tmpMessage);
			message = tmpMessage;

			if (response == 4)
			{
				rubyMessage = message;
				gotRuby = true;
			}
			else
			{
				break;
			}
		}
	}

	debug("MedalServer Response: {} '{}'\n", response, message);

	                SDLNet_TCP_Close(socket);

	return response;
}

std::string MedalServer::getMessage()
{
	return message;
}

bool MedalServer::hasRuby()
{
	return gotRuby;
}

std::string MedalServer::getRubyMessage()
{
	gotRuby = false;

	return rubyMessage;
}

void MedalServer::close()
{
	connected = false;

	SDL_DestroyMutex(lock);
}
