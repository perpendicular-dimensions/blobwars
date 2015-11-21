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

#include "headers.h"

void SDL_SetAlpha(SDL_Surface *surface, uint8_t value) {
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_SetSurfaceAlphaMod(surface, value);
}

Graphics::Graphics()
{
	for (int i = 0 ; i < MAX_TILES ; i++)
	{
		tile[i] = NULL;
	}

	background = NULL;
	infoMessage = NULL;

	fontSize = 0;
	
	medalMessageTimer = 0;
	medalType = 0;

	currentLoading = 0;

	screenShotNumber = 0;
	takeRandomScreenShots = false;

	waterAnim = 201;
	slimeAnim = 208;
	lavaAnim = 215;
}

void Graphics::free()
{
	debug(("graphics.free: Background\n"));
	if (background != NULL)
	{
		SDL_FreeSurface(background);
	}
	debug(("graphics.free: Background - Done\n"));

	background = NULL;

	debug(("graphics.free: Tiles\n"));
	for (int i = 0 ; i < MAX_TILES ; i++)
	{
		if (tile[i] != NULL)
		{
			SDL_FreeSurface(tile[i]);
			tile[i] = NULL;
		}
	}
	debug(("graphics.free: Tiles - Done\n"));

	debug(("graphics.free: Sprites\n"));
	Sprite *sprite = (Sprite*)spriteList.getHead();
	while (sprite->next != NULL)
	{
		sprite = (Sprite*)sprite->next;
		//debug(("graphics.free: Sprites Sprite::Free - %s\n", sprite->name));
		sprite->free();
	}
	debug(("graphics.free: Sprites Clear()\n"));
	spriteList.clear();
	debug(("graphics.free: Sprites - Done\n"));
}

void Graphics::destroy()
{
	free();

	for (int i = 0 ; i < 5 ; i++)
	{
		if (font[i])
		{
			TTF_CloseFont(font[i]);
		}
	}
	
	if (medalMessage != NULL)
	{
		SDL_FreeSurface(medalMessage);
	}

	if (fadeBlack)
	{
		SDL_FreeSurface(fadeBlack);
	}

	if (infoBar)
	{
		SDL_FreeSurface(infoBar);
	}
	
	for (int i = 0 ; i < 4 ; i++)
	{
		if (medal[i] != NULL)
		{
			SDL_FreeSurface(medal[i]);
			medal[i] = NULL;
		}
	}
}

void Graphics::registerEngine(Engine *engine)
{
	this->engine = engine;
}

void Graphics::mapColors()
{
	red = SDL_MapRGB(screen->format, 0xff, 0x00, 0x00);
	yellow = SDL_MapRGB(screen->format, 0xff, 0xff, 0x00);
	green = SDL_MapRGB(screen->format, 0x00, 0xff, 0x00);
	darkGreen = SDL_MapRGB(screen->format, 0x00, 0x77, 0x00);
	skyBlue = SDL_MapRGB(screen->format, 0x66, 0x66, 0xff);
	blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xff);
	cyan = SDL_MapRGB(screen->format, 0x00, 0x99, 0xff);
	white = SDL_MapRGB(screen->format, 0xff, 0xff, 0xff);
	lightGrey = SDL_MapRGB(screen->format, 0xcc, 0xcc, 0xcc);
	grey = SDL_MapRGB(screen->format, 0x88, 0x88, 0x88);
	darkGrey = SDL_MapRGB(screen->format, 0x33, 0x33, 0x33);
	black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	fontForeground.r = fontForeground.g = fontForeground.b = 0xff;
	fontBackground.r = fontBackground.g = fontBackground.b = 0x00;

	fadeBlack = alphaRect(640, 480, 0x00, 0x00, 0x00);

	infoBar = alphaRect(640, 25, 0x00, 0x00, 0x00);
	
	medalMessage = NULL;
}

Sprite *Graphics::getSpriteHead()
{
	return (Sprite*)spriteList.getHead();
}

void Graphics::setTransparent(SDL_Surface *sprite)
{
	if (sprite)
		SDL_SetColorKey(sprite, SDL_TRUE, SDL_MapRGB(sprite->format, 0, 0, 0));
}

bool Graphics::canShowMedalMessage() const
{
	return (medalMessageTimer <= 0);
}

void Graphics::updateScreen()
{
	if (medalMessageTimer > 0)
	{
		int padding = 0;
		
		medalMessageTimer--;
		
		if (medalType >= 0)
		{
			padding = 18;
		}
		
		drawRect(screen->w - (medalMessage->w + 5 + padding), 5, medalMessage->w + padding - 2, 20, grey, screen);
		drawRect(screen->w - (medalMessage->w + 5 + padding - 1), 6, medalMessage->w + padding - 4, 18, black, screen);
		blit(medalMessage, screen->w - (medalMessage->w + 5), 7, screen, false);
		
		if (medalType >= 0)
		{
			blit(medal[medalType], screen->w - (medalMessage->w + 5 + 16), 7, screen, false);
		}
	}
	
	SDL_UpdateTexture(texture, NULL, screen->pixels, screen->w * 4);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

	if (takeRandomScreenShots)
	{
		if ((Math::prand() % 500) == 0)
		{
			snprintf(screenshot, sizeof screenshot, "screenshots/screenshot%.3d.bmp", screenShotNumber);
			SDL_SaveBMP(screen, screenshot);
			screenShotNumber++;
		}

		SDL_Delay(16);
	}

	if (engine->keyState[SDL_SCANCODE_F12])
	{
		snprintf(screenshot, sizeof screenshot, "screenshots/screenshot%.3d.bmp", screenShotNumber);
		SDL_SaveBMP(screen, screenshot);
		screenShotNumber++;

		engine->keyState[SDL_SCANCODE_F12] = 0;
	}

	if ((engine->keyState[SDL_SCANCODE_F10]) || ((engine->keyState[SDL_SCANCODE_RETURN]) && (engine->keyState[SDL_SCANCODE_LALT])))
	{
		engine->fullScreen = !engine->fullScreen;
		SDL_SetWindowFullscreen(window, engine->fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

		engine->keyState[SDL_SCANCODE_F10] = engine->keyState[SDL_SCANCODE_LALT] = engine->keyState[SDL_SCANCODE_RETURN] = 0;
	}
}

void Graphics::delay(int time)
{
	unsigned long then = SDL_GetTicks();

	engine->keyState[SDL_SCANCODE_ESCAPE] = 0;

	while (true)
	{
		updateScreen();
		
		if (SDL_GetTicks() >= then + time)
		{
			break;
		}

		engine->getInput();
		
		/*
		if (engine->keyState[SDL_SCANCODE_ESCAPE])
		{
			break;
		}
		*/
	}
}

void Graphics::RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	float mn, mx, delta;
	mn = min(min(r, g), b);
	mx = max(max(r, g), b);
	*v = mx;
	delta = mx - mn;

	if (mx != 0)
	{
		*s = delta / mx;
	}
	else
	{
		*s = 0;
		*h = -1;
		return;
	}

	if (r == mx)
	{
		*h = (g - b) / delta;
	}
	else if (g == mx)
	{
		*h = 2 + (b - r) / delta;
	}
	else
	{
		*h = 4 + (r - g) / delta;
	}

	*h *= 60;

	if (*h < 0)
	{
		*h += 360;
	}
}

void Graphics::HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;
	if (s == 0)
	{
		*r = *g = *b = v;
		return;
	}

	h /= 60;
	i = (int)(h);
	f = h - i;
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));

	switch (i)
	{
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;

		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;

		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;

		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;

		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;

		default:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}

SDL_Surface *Graphics::loadImage(const char *filename, bool srcalpha)
{
	SDL_Surface *image, *newImage;

	#if USEPAK
		if (!engine->unpack(filename, PAK_IMG))
			showErrorAndExit(ERR_FILE, filename);
		image = IMG_Load_RW(engine->sdlrw, 1);
	#else
		image = IMG_Load(filename);
	#endif

	if (!image)
		return showErrorAndExit(ERR_FILE, filename), image;

	newImage = SDL_ConvertSurface(image, screen->format, 0);

	if (newImage)
	{
		SDL_FreeSurface(image);
	}
	else
	{
		// This happens when we are loading the window icon image
		newImage = image;
	}

	if(srcalpha)
		SDL_SetAlpha(newImage, 255);
	else
		setTransparent(newImage);

	return newImage;
}

SDL_Surface *Graphics::loadImage(const char *filename, int hue, int sat, int value)
{
	SDL_Surface *image, *newImage;

	#if USEPAK
		if (!engine->unpack(filename, PAK_IMG))
			showErrorAndExit(ERR_FILE, filename);
		image = IMG_Load_RW(engine->sdlrw, 1);
	#else
		image = IMG_Load(filename);
	#endif

	if (!image)
		return showErrorAndExit(ERR_FILE, filename), image;

	if ((hue != 0) || (sat != 0) || (value != 0))
	{
		if (image->format->BitsPerPixel != 8)
		{
			debug(("WARNING: Could not set Hue for '%s'! Not an 8 bit image!\n", filename));
		}
		else
		{
			SDL_Color *color;
			float r, g, b, h, s, v;

			if (image->format->palette->colors != NULL)
			{
				for (int i = 1 ; i < image->format->palette->ncolors ; i++)
				{
					color = &image->format->palette->colors[i];

					r = (int)color->r;
					g = (int)color->g;
					b = (int)color->b;

					RGBtoHSV(r, g, b, &h, &s, &v);

					h += hue;
					s += sat;
					v += value;

					HSVtoRGB(&r, &g, &b, h, s, v);

					color->r = (int)r;
					color->g = (int)g;
					color->b = (int)b;

				}
			}
		}
	}

	newImage = SDL_ConvertSurface(image, screen->format, 0);

	if (newImage)
	{
		SDL_FreeSurface(image);
	}
	else
	{
		// This happens when we are loading the window icon image
		newImage = image;
	}

	setTransparent(newImage);

	return newImage;
}

SDL_Surface *Graphics::quickSprite(const char *name, SDL_Surface *image)
{
	Sprite *sprite = addSprite(name);
	sprite->setFrame(0, image, 60);

	return sprite->getCurrentFrame();
}

void Graphics::fade(int amount)
{
	SDL_SetAlpha(fadeBlack, amount);
	blit(fadeBlack, 0, 0, screen, false);
}

void Graphics::fadeToBlack()
{
	int start = 0;

	while (start < 50)
	{
		SDL_SetAlpha(fadeBlack, start);
		blit(fadeBlack, 0, 0, screen, false);
		delay(60);
		start++;
	}
}

void Graphics::loadMapTiles(const char *baseDir)
{
	bool found, autoAlpha;
	char filename[255];
	filename[0] = 0;

	autoAlpha = false;
	
	if (strcmp(baseDir, "gfx/common") == 0)
	{
		autoAlpha = true;
	}

	#if !USEPAK
	FILE *fp;
	#endif

	for (int i = 1 ; i < MAX_TILES ; i++)
	{
		found = true;

		snprintf(filename, sizeof filename, "%s/%d.png", baseDir, i);

		#if USEPAK
		
		if (!engine->getPak()->fileExists(filename))
			continue;

		#else

		fp = fopen(filename, "rb");
		if (!fp)
			continue;
		fclose(fp);

		#endif

		if (found)
		{
			tile[i] = loadImage(filename);

			if (!tile[i])
				abort();

			if (autoAlpha)
			{
				if ((i < MAP_EXITSIGN) || (i >= MAP_WATERANIM))
				{
					SDL_SetAlpha(tile[i], 130);
				}
			}
			else
			{
				if (i < MAP_DECORATION)
				{
					SDL_SetColorKey(tile[i], 0, SDL_MapRGB(tile[i]->format, 0, 0, 0));
				}
			}
		}
	}
}

void Graphics::loadFont(int i, const char *filename, int pointSize)
{
	debug(("Attempting to load font %s with point size of %d...\n", filename, pointSize));
	
	if (font[i])
	{
		debug(("Freeing Font %d first...\n", i));
		TTF_CloseFont(font[i]);
	}
	
	#if USEPAK
		(void)filename;
		char tempPath[PATH_MAX];
		snprintf(tempPath, sizeof tempPath, "%sfont.ttf", engine->userHomeDirectory);
		font[i] = TTF_OpenFont(tempPath, pointSize);
	#else
		font[i] = TTF_OpenFont(filename, pointSize);
	#endif

	if (!font[i])
	{
		engine->reportFontFailure();
	}
	
	TTF_SetFontStyle(font[i], TTF_STYLE_NORMAL);
}

Sprite *Graphics::addSprite(const char *name)
{
	Sprite *sprite = new Sprite;
	strlcpy(sprite->name, name, sizeof sprite->name);

	spriteList.add(sprite);

	return sprite;
}

Sprite *Graphics::getSprite(const char *name, bool required)
{
	Sprite *sprite = (Sprite*)spriteList.getHead();

	while (sprite->next != NULL)
	{
		sprite = (Sprite*)sprite->next;
		
		if (strcmp(sprite->name, name) == 0)
		{
			return sprite;
		}
	}

	if (required)
		showErrorAndExit("The requested sprite '%s' does not exist", name);

	return NULL;
}

void Graphics::animateSprites()
{
	Sprite *sprite = (Sprite*)spriteList.getHead();

	while (sprite->next != NULL)
	{
		sprite = (Sprite*)sprite->next;

		sprite->animate();
	}

	if ((engine->getFrameLoop() % 8) == 0)
	{
		Math::wrapInt(&(++waterAnim), 201, 204);
		Math::wrapInt(&(++slimeAnim), 207, 212);
		Math::wrapInt(&(++lavaAnim), 214, 220);
	}
}

int Graphics::getWaterAnim() const
{
	return waterAnim;
}

int Graphics::getSlimeAnim() const
{
	return slimeAnim;
}

int Graphics::getLavaAnim() const
{
	return lavaAnim;
}

int Graphics::getLavaAnim(int current)
{
	if ((engine->getFrameLoop() % 8) == 0)
		return Math::rrand(214, 220);

	return current;
}

void Graphics::loadBackground(const char *filename)
{
	if (background != NULL)
		SDL_FreeSurface(background);

	if (strcmp(filename, "@none@") == 0)
		return;

	background = loadImage(filename);

	SDL_SetColorKey(background, 0, SDL_MapRGB(background->format, 0, 0, 0));
}

void Graphics::putPixel(int x, int y, Uint32 pixel, SDL_Surface *dest)
{
	if ((x < 0) || (x > 639) || (y < 0) || (y > 479))
		return;

	int bpp = dest->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)dest->pixels + y * dest->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

Uint32 Graphics::getPixel(SDL_Surface *surface, int x, int y)
{
	if ((x < 0) || (x > (surface->w - 1)) || (y < 0) || (y > (surface->h - 1)))
		return 0;

	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
	case 1:
		return *p;

	case 2:
		return *(Uint16 *)p;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
		else
				return p[0] | p[1] << 8 | p[2] << 16;

	case 4:
		return *(Uint32 *)p;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void Graphics::drawLine(float startX, float startY, float endX, float endY, int color, SDL_Surface *dest)
{
	lock(screen);
	
	float dx, dy;
	
	Math::calculateSlope(startX, startY, endX, endY, &dx, &dy);

	while (true)
	{
		putPixel((int)startX, (int)startY, color, dest);

		if ((int)startX == (int)endX)
			break;

		startX -= dx;
		startY -= dy;
	}

	unlock(screen);
}

void Graphics::blit(SDL_Surface *image, int x, int y, SDL_Surface *dest, bool centered)
{
	if (!image)
	{
		return showErrorAndExit("graphics::blit() - NULL pointer", SDL_GetError());
	}

	if ((x < -image->w) || (x > 640 + image->w))
		return;

	if ((y < -image->h) || (y > 480 + image->h))
		return;

	// Set up a rectangle to draw to
	gRect.x = x;
	gRect.y = y;
	if (centered)
	{
		gRect.x -= (image->w / 2);
		gRect.y -= (image->h / 2);
	}

	gRect.w = image->w;
	gRect.h = image->h;

	/* Blit onto the screen surface */
	if (SDL_BlitSurface(image, NULL, dest, &gRect) < 0)
		showErrorAndExit("graphics::blit() - %s", SDL_GetError());
}

void Graphics::drawBackground()
{
	if (background != NULL)
		blit(background, 0, 0, screen, false);
	else
		SDL_FillRect(screen, NULL, black);
}

void Graphics::drawBackground(SDL_Rect *r)
{
	if (r->x < 0) r->x = 0;
	if (r->y < 0) r->y = 0;
	if (r->x + r->w > 639) r->w = 640 - r->x;
	if (r->y + r->h > 639) r->h = 480 - r->y;

	if (SDL_BlitSurface(background, r, screen, r) < 0)
		showErrorAndExit("graphics::blit() - %s", SDL_GetError());
}

void Graphics::drawRect(int x, int y, int w, int h, int color, SDL_Surface *dest)
{
	gRect.x = x;
	gRect.y = y;
	gRect.w = w;
	gRect.h = h;

	SDL_FillRect(dest, &gRect, color);
}

void Graphics::drawRect(int x, int y, int w, int h, int color, int borderColor, SDL_Surface *dest)
{
	drawRect(x - 1, y - 1, w + 2, h + 2, borderColor, dest);
	drawRect(x, y, w, h, color, dest);
}

void Graphics::setFontColor(int red, int green, int blue, int red2, int green2, int blue2)
{
	fontForeground.r = red;
	fontForeground.g = green;
	fontForeground.b = blue;

	fontBackground.r = red2;
	fontBackground.g = green2;
	fontBackground.b = blue2;
}

void Graphics::setFontSize(int size)
{
	fontSize = size;
	Math::limitInt(&fontSize, 0, 4);
}

SDL_Surface *Graphics::getString(const char *in, bool transparent)
{
	SDL_Surface *text = TTF_RenderUTF8_Shaded(font[fontSize], in, fontForeground, fontBackground);

	if (!text)
	{
		text = TTF_RenderUTF8_Shaded(font[fontSize], "FONT_ERROR", fontForeground, fontBackground);
	}

	if (!text)
	{
		fprintf(stderr, "Unable to render text: %s\n", SDL_GetError());
		abort();
	}

	if (transparent)
		setTransparent(text);

	return text;
}

void Graphics::drawString(const char *in, int x, int y, int alignment, SDL_Surface *dest)
{
	bool center = false;

	SDL_Surface *text = TTF_RenderUTF8_Shaded(font[fontSize], in, fontForeground, fontBackground);

	if (!text)
		text = TTF_RenderUTF8_Shaded(font[fontSize], "FONT_ERROR", fontForeground, fontBackground);

	if (!text)
		return;

	setTransparent(text);

	if (alignment == TXT_RIGHT) x -= text->w;
	if (alignment == TXT_CENTERED) center = true;

	blit(text, x, y, dest, center);
	SDL_FreeSurface(text);
}

void Graphics::drawString(const char *in, int x, int y, int alignment, SDL_Surface *dest, SurfaceCache &cache)
{
	bool center = false;

	if(!cache.text || strcmp(in, cache.text)) {
		if(cache.surface)
			SDL_FreeSurface(cache.surface);

		if(cache.text)
			::free(cache.text);

		cache.text = strdup(in);

		cache.surface = TTF_RenderUTF8_Shaded(font[fontSize], in, fontForeground, fontBackground);

		if (!cache.surface)
			cache.surface = TTF_RenderUTF8_Shaded(font[fontSize], "FONT_ERROR", fontForeground, fontBackground);

		if(!cache.surface)
			return;

		setTransparent(cache.surface);
	}

	if (alignment == TXT_RIGHT) x -= cache.surface->w;
	if (alignment == TXT_CENTERED) center = true;

	blit(cache.surface, x, y, dest, center);
}

void Graphics::clearChatString()
{
	chatString[0] = 0;
}

void Graphics::createChatString(const char *in)
{
	strlcat(chatString, " ", sizeof chatString);
	strlcat(chatString, in, sizeof chatString);
}

void Graphics::drawChatString(SDL_Surface *surface, int y)
{
	char *word = strtok(chatString, " ");
	char wordWithSpace[100];
	
	int r, g, b;

	int x = 10;
	int surfaceWidth = surface->w - 10;

	SDL_Surface *wordSurface;

	while (word)
	{
		if (strcmp(word, "<RGB>") == 0)
		{
			r = atoi(strtok(NULL, " "));
			g = atoi(strtok(NULL, " "));
			b = atoi(strtok(NULL, " "));

			if ((!r) && (!g) && (!b))
			{
				debug(("Parse Error in Text Color (%d:%d:%d)!!\n", r, g, b));
				exit(1);
			}

			setFontColor(r, g, b, 0, 0, 0);

			word = strtok(NULL, " ");

			continue;
		}

		snprintf(wordWithSpace, sizeof wordWithSpace, "%s ", word);

		wordSurface = getString(wordWithSpace, false);

		if (x + wordSurface->w > surfaceWidth)
		{
			y += (int)(wordSurface->h * 1.5) ;
			x = 10;
		}

		blit(wordSurface, x, y, surface, false);

		x += wordSurface->w;

		SDL_FreeSurface(wordSurface);

		word = strtok(NULL, " ");
	}
}

void Graphics::showMedalMessage(int type, const char *in)
{
	char message[1024];
	
	if (medalMessage != NULL)
	{
		SDL_FreeSurface(medalMessage);
	}
	
	setFontSize(0);
	
	switch (type)
	{
		// Bronze
		case 1:
			setFontColor(0xA6, 0x7D, 0x3D, 0x00, 0x00, 0x00);
			break;
		
		// Silver
		case 2:
			setFontColor(0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00);
			break;
			
		// Gold
		case 3:
			setFontColor(0xFF, 0xCC, 0x33, 0x00, 0x00, 0x00);
			break;
			
		// Ruby
		case 4:
			setFontColor(0xFF, 0x11, 0x55, 0x00, 0x00, 0x00);
			break;
	}
	
	medalType = type - 1; // for indexing on the image
	if (type != -1)
	{
		snprintf(message, sizeof message, "  Medal Earned - %s  ", in);
		medalMessage = getString(message, true);
	}
	else
	{
		snprintf(message, sizeof message, "  %s  ", in);
		medalMessage = getString(message, true);
	}
	medalMessageTimer = (5 * 60);
}

void Graphics::drawWidgetRect(int x, int y, int w, int h)
{
	drawRect(x - 5, y - 4, w + 10, h + 8, white, screen);
	drawRect(x - 4, y - 3, w + 8, h + 6, black, screen);
	drawRect(x - 3, y - 2, w + 6, h + 4, green, screen);
}

SDL_Surface *Graphics::createSurface(int width, int height)
{
	SDL_Surface *surface, *newImage;

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);

	if (surface == NULL)
		showErrorAndExit("CreateRGBSurface failed: %s\n", SDL_GetError());

	newImage = SDL_ConvertSurface(surface, screen->format, 0);

	SDL_FreeSurface(surface);

	return newImage;
}

SDL_Surface *Graphics::alphaRect(int width, int height, Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_Surface *surface = createSurface(width, height);

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, red, green, blue));

	SDL_SetAlpha(surface, 130);

	return surface;
}

void Graphics::colorize(SDL_Surface *image, int red, int green, int blue)
{
	SDL_Surface *alpha = alphaRect(image->w, image->h, red, green, blue);

	blit(alpha, 0, 0, image, false);

	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, red / 2, green / 2, blue / 2));
}

void Graphics::lock(SDL_Surface *surface)
{
	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(surface))
	{
		if (SDL_LockSurface(surface) < 0 )
		{
			showErrorAndExit("Could not lock surface", "");
		}
	}
}

void Graphics::unlock(SDL_Surface *surface)
{
	if (SDL_MUSTLOCK(surface))
	{
		SDL_UnlockSurface(surface);
	}
}

void Graphics::resetLoading()
{
	currentLoading = 0;
}

void Graphics::showLoading(int amount, int max)
{
	#if USEPAK
	max *= 4;

	if (max > 398)
		max = 398;

	Math::limitInt(&(currentLoading += amount), 0, max);

	drawRect(120, 420, 400, 10, black, white, screen);
	drawRect(121, 421, currentLoading, 8, red, screen);
	#else
	(void)amount;
	(void)max;
	#endif
}

void Graphics::showLicenseErrorAndExit()
{
	setFontSize(3); setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
	drawString("License Agreement Missing", 320, 50, true, screen);
	
	setFontSize(1); setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	drawString("The GNU General Public License was not found.", 320, 180, true, screen);
	drawString("It could either not be properly loaded or has been removed.", 320, 220, true, screen);
	drawString("Blob Wars : Metal Blob Solid will not run with the license missing.", 320, 260, true, screen);
	
	drawString("Blob Wars : Metal Blob Solid will now exit", 320, 420, true, screen);
	drawString("Press Escape to continue", 320, 450, true, screen);

	engine->flushInput();

	while (true)
	{
		updateScreen();
		engine->getInput();
		if (engine->keyState[SDL_SCANCODE_ESCAPE])
			exit(1);
		SDL_Delay(16);
	}
}

void Graphics::showErrorAndExit(const char *error, const char *param)
{
	SDL_FillRect(screen, NULL, black);

	if (strcmp(param, "LICENSE") == 0)
	{
		showLicenseErrorAndExit();
	}

	char message[256];
	snprintf(message, sizeof message, error, param);

	setFontSize(3); setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
	drawString("An unforseen error has occurred", 320, 50, true, screen);
	setFontSize(1); setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);
	drawString(message, 320, 90, true, screen);

	drawString("You may wish to try the following,", 50, 150, false, screen);

	setFontSize(0);
	drawString("1) Try reinstalling the game.", 75, 190, false, screen);
	drawString("2) Ensure you have SDL 1.2.5 or greater installed.", 75, 210, false, screen);
	drawString("3) Ensure you have the latest versions of additional required SDL libraries.", 75, 230, false, screen);
	drawString("4) Install using an RPM if you originally built the game from source", 75, 250, false, screen);
	drawString("or try building from source if you installed using an RPM.", 75, 270, false, screen);
	drawString("5) Visit http://www.parallelrealities.co.uk/blobWars.php and check for updates.", 75, 290, false, screen);

	setFontSize(1);

	drawString("If problems persist contact Parallel Realities. Please be aware however that we will not", 320, 360, true, screen);
	drawString("be able to assist in cases where the code or data has been modified.", 320, 380, true, screen);

	drawString("Blob Wars : Metal Blob Solid will now exit", 320, 420, true, screen);
	drawString("Press Escape to continue", 320, 450, true, screen);

	engine->flushInput();

	while (true)
	{
		updateScreen();
		engine->getInput();
		if (engine->keyState[SDL_SCANCODE_ESCAPE])
		{
			exit(1);
		}
		SDL_Delay(16);
	}
}

void Graphics::showRootWarning()
{
	setFontSize(3); setFontColor(0xff, 0x00, 0x00, 0x00, 0x00, 0x00);
	drawString("CAUTION - RUNNING AS ROOT USER!", 320, 50, true, screen);
	
	setFontSize(1); setFontColor(0xff, 0xff, 0xff, 0x00, 0x00, 0x00);

	drawString("WARNING - You appear to be running the game as the root user!", 320, 180, true, screen);
	drawString("This is not recommended and is it strongly advised that you do not run", 320, 220, true, screen);
	drawString("the game as root. You may still continue but consider running as regular user in future!", 320, 260, true, screen);
	
	drawString("Press Space to Exit", 320, 420, true, screen);
	drawString("Press Escape to Continue", 320, 450, true, screen);

	engine->flushInput();

	while (true)
	{
		updateScreen();
		engine->getInput();
		
		if (engine->keyState[SDL_SCANCODE_ESCAPE])
		{
			return;
		}
		else if (engine->keyState[SDL_SCANCODE_SPACE])
		{
			exit(0);
		}

		SDL_Delay(16);
	}
}
