/*
Copyright (C) 2005 Parallel Realities

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

String::String()
{
	setText("");
}

String::String(char *text)
{
	int size = strlen(text);

	this->text = new char[size + 1];

	strcpy(this->text, text);

	this->length = size;
}

String::~String()
{
	if (this->text != NULL)
	{
		delete[] this->text;
		this->text = NULL;
	}
}

/*
Got this off Google... not sure I entirely trust it but it seems
to be okay.
*/
void String::trim()
{
	if (text == NULL)
	{
		return;
	}

	char *tempText = new char[length + 1];

	char *c = text;

	while (*c == ' ')
	{
		c++; // LOL!!! :)
	}

	strcpy(tempText, c);

	int len = strlen(tempText);
	
	while ((len > 0) && ((tempText[len - 1] == ' ') || (tempText[len - 1] == '\0')))
	{
		tempText[--len] = 0;
	}

	delete[] this->text;

	this->text = tempText;
}

void String::operator= (char *text)
{
	if (this->text != NULL)
	{
		delete[] this->text;
		this->text = NULL;
	}

	if (text == NULL)
	{
		printf("WARNING: String - Can't set NULL!\n");
		return;
	}

	int size = strlen(text);

	this->text = new char[size + 1];

	strcpy(this->text, text);

	this->length = size;
}

bool String::operator== (char *text)
{
	if (strcmp(this->text, text) == 0)
	{
		return true;
	}

	return false;
}

bool String::operator== (String string)
{
	if (strcmp(this->text, string.getText()) == 0)
	{
		return true;
	}

	return false;
}

bool String::operator!= (char *text)
{
	if (strcmp(this->text, text) != 0)
	{
		return true;
	}

	return false;
}

bool String::operator!= (String string)
{
	if (strcmp(this->text, string.getText()) != 0)
	{
		return true;
	}

	return false;
}

void String::setText(char *text, ...)
{
	strcpy(tmpString, "");
	
	va_list argp;
	va_start(argp, text);
	vsprintf(tmpString, text, argp);
	va_end(argp);
	
	int size = strlen(tmpString);

	this->text = new char[size + 1];

	strcpy(this->text,  tmpString);
}

char *String::getText()
{
	if (text == NULL)
	{
		printf("WARNING: String::getText() - text is NULL!\n");
		return NULL;
	}

	return text;
}

int String::getLength()
{
	return length;
}

char String::tmpString[1024];
