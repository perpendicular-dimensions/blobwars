/*
Copyright (C) 2004-2011 Parallel Realities
Copyright (C) 2011-2015 Perpendicular Dimensions

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

#include "widgets.h"

/**
* Draws the options for a radio button widget with the currently selected
* option highlighted
* @param widget The radio button widget
* @param maxWidth The width of the largest widget (used for spacing)
*/
void drawOptions(Widget *widget, int maxWidth)
{
	int x = widget->x + maxWidth + 55;

	int count = 0;

	char *c = widget->options;

	SDL_Surface *text;

	while (*c)
	{
		char token[100];
		unsigned int token_len;

		char *eow = strchr (c, '|');
		if (!eow)
			eow = strchr (c, 0);

		token_len = eow - c;
		if (token_len > sizeof (token) - 1)
			token_len = sizeof (token) - 1;
		memcpy (token, c, token_len);
		token [token_len] = 0;

		c = eow;
		if (*c)
			c++;

		if (widget->enabled)
		{
			graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
			if (count == *widget->value)
				graphics.setFontColor(0x00, 0x00, 0x00, 0x00, 0xff, 0x00);
		}
		else
		{
			graphics.setFontColor(0x77, 0x77, 0x77, 0x00, 0x00, 0x00);
		}

		text = graphics.getString(token, false);

		if ((widget->enabled) && (count == *widget->value))
			graphics.drawWidgetRect(x, widget->y, text->w, text->h);

		graphics.blit(text, x, widget->y, graphics.screen, false);

		x += text->w + 25;
		count++;
	}
}

/**
* Draws a horizontal slider widget with its current value
* used to fill the bar
* @param widget The slider widget
* @param maxWidth The width of the largest widget (used for spacing)
*/
void drawSlider(Widget *widget, int maxWidth)
{
	int x = widget->x + maxWidth + 50;

	if (widget->enabled)
	{
		graphics.drawRect(x, widget->y, 300, 18, graphics.white, graphics.screen);
	}
	else
	{
		graphics.drawRect(x, widget->y, 300, 18, graphics.grey, graphics.screen);
	}

	graphics.drawRect(x + 1, widget->y + 1, 298, 16, graphics.black, graphics.screen);

	float width = 296.00 / widget->max;

	width *= *widget->value;

	if (widget->enabled)
	{
		graphics.drawRect(x + 2, widget->y + 2, (int)width, 14, graphics.green, graphics.screen);
	}
	else
	{
		graphics.drawRect(x + 2, widget->y + 2, (int)width, 14, graphics.darkGreen, graphics.screen);
	}

}

/**
* Draws a widget used to represent joypad buttons. A widget with a value of
* -1000 or less appears as ...
* @param widget The Joypad Button widget
*/
void drawJoypadButtonOption(Widget *widget)
{
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	
	int x = 300;
	
	char text[25];
	text[0] = 0;
	
	// joysticks have a button 0 so we can't
	// do the same thing as the keyboard(!)
	if (*widget->value < -2)
	{
		snprintf(text, sizeof text, "...");
	}
	else if (*widget->value == -2)
	{
		snprintf(text, sizeof text, "N/A");
	}
	else
	{
		snprintf(text, sizeof text, "Button #%d", *widget->value);
	}
	
	graphics.drawString(text, x, widget->y, TXT_LEFT, graphics.screen);
}

/**
* Draws a widget used to represent a key based on the widget's current value
* @param widget The Widget
*/
void drawKeyOption(Widget *widget)
{
	graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	
	int x = 300;
	
	graphics.drawString(Keyboard::translateKey(*widget->value),
			    x, widget->y, TXT_LEFT, graphics.screen);
}

/**
* Creates an image for the specified image. The text colour and background
* colour as set depending on the status of the widget
* @param widget The Widget to create the image for
*/
void generateWidgetImage(Widget *widget)
{
	if (widget == engine.highlightedWidget)
	{
		graphics.setFontColor(0x00, 0x00, 0x00, 0x00, 0xff, 0x00);
	}
	else
	{
		if (widget->enabled)
		{
			graphics.setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
		}
		else
		{
			graphics.setFontColor(0x77, 0x77, 0x77, 0x00, 0x00, 0x00);
		}
	}

	widget->image = graphics.getString(widget->label, false);
}

/**
* Draws all the widgets in their specified positions. Widgets that
* have no image have an image created for them. Invisible widgets
* are not drawn.
*/
void drawWidgets()
{
	graphics.setFontSize(0);

	Widget *widget = (Widget*)engine.widgetList.getHead();

	int maxWidth = 0;

	while (widget->next != NULL)
	{
		widget = (Widget*)widget->next;

		if (widget->image == NULL)
		{
			generateWidgetImage(widget);
		}
			
		maxWidth = max(maxWidth, widget->image->w);
	}

	widget = (Widget*)engine.widgetList.getHead();

	while (widget->next != NULL)
	{
		widget = (Widget*)widget->next;

		if (!widget->visible)
		{
			continue;
		}

		if ((!widget->value) && (widget->type != WG_LABEL))
		{
			debug(("WARNING: Widget variable for '%s' not set!\n", widget->name));
			continue;
		}

		if (widget->x == -1)
		{
			widget->x = (640 - widget->image->w) / 2;
		}

		if (widget == engine.highlightedWidget)
		{
			graphics.drawWidgetRect(widget->x, widget->y, widget->image->w, widget->image->h);
		}
		else
		{
			graphics.drawRect(widget->x - 3, widget->y - 2, widget->image->w + 6, widget->image->h + 4, graphics.black, graphics.screen);
		}

		graphics.blit(widget->image, widget->x, widget->y, graphics.screen, false);

		switch (widget->type)
		{
			case WG_RADIO:
				drawOptions(widget, maxWidth);
				break;
			case WG_SLIDER:
			case WG_SMOOTH_SLIDER:
				drawSlider(widget, maxWidth);
				break;
			case WG_KEYBOARD:
				drawKeyOption(widget);
				break;
			case WG_JOYPAD:
				drawJoypadButtonOption(widget);
				break;
		}

		widget->changed = false;
	}
}
