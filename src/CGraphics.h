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

void SDL_SetAlpha(SDL_Surface *surface, uint8_t value);

class Graphics {

	private:

		Engine *engine;
		SDL_Rect gRect;
		TTF_Font *font[5];
		SDL_Color fontForeground, fontBackground;

		List spriteList;

		int fontSize;
		
		int waterAnim;
		int slimeAnim;
		int lavaAnim;

		int currentLoading;

		int screenShotNumber;
		char screenshot[100];
		char chatString[1024];
		
		SDL_Surface *medalMessage;
		int medalMessageTimer;
		int medalType;

		SDL_Surface *fadeBlack;
		SDL_Surface *infoMessage;

	public:
		struct SurfaceCache {
			char *text;
			SDL_Surface *surface;
			SurfaceCache(): text(NULL), surface(NULL) {}	
		};

		bool takeRandomScreenShots;

		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		SDL_Surface *screen, *background;
		SDL_Surface *tile[MAX_TILES];
		
		SDL_Surface *medal[4];
		SDL_Surface *license[2];
		
		SDL_Surface *infoBar;

		int red, yellow, green, darkGreen, skyBlue, blue, cyan, white, lightGrey, grey, darkGrey, black;

	Graphics();
	void free();
	void destroy();
	void registerEngine(Engine *engine);
	void mapColors();
	Sprite *getSpriteHead();
	void setTransparent(SDL_Surface *sprite);
	void updateScreen();
	bool canShowMedalMessage() const;
	void delay(int time);
	void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v);
	void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v);
	SDL_Surface *loadImage(const char *filename, bool srcalpha = false);
	SDL_Surface *loadImage(const char *filename, int hue, int sat, int value);
	SDL_Surface *quickSprite(const char *name, SDL_Surface *image);
	void fade(int amount);
	void fadeToBlack();
	void loadMapTiles(const char *baseDir);
	void loadFont(int i, const char *filename, int pixelSize);
	Sprite *addSprite(const char *name);
	Sprite *getSprite(const char *name, bool required);
	void animateSprites();
	int getWaterAnim() const;
	int getSlimeAnim() const;
	int getLavaAnim() const;
	int getLavaAnim(int current);
	void loadBackground(const char *filename);
	void putPixel(int x, int y, Uint32 pixel, SDL_Surface *dest);
	Uint32 getPixel(SDL_Surface *surface, int x, int y);
	void drawLine(float startX, float startY, float endX, float endY, int color, SDL_Surface *dest);
	void blit(SDL_Surface *image, int x, int y, SDL_Surface *dest, bool centered);
	void drawBackground();
	void drawBackground(SDL_Rect *r);
	void drawRect(int x, int y, int w, int h, int color, SDL_Surface *dest);
	void drawRect(int x, int y, int w, int h, int color, int borderColor, SDL_Surface *dest);
	void setFontColor(int red, int green, int blue, int red2, int green2, int blue2);
	void setFontSize(int size);
	SDL_Surface *getString(const char *in, bool transparent);
	void drawString(const char *in, int x, int y, int alignment, SDL_Surface *dest);
	void drawString(const char *in, int x, int y, int alignment, SDL_Surface *dest, SurfaceCache &cache);
	void clearChatString();
	void createChatString(const char *in);
	void showMedalMessage(int type, const char *in);
	void drawChatString(SDL_Surface *surface, int y);
	void drawWidgetRect(int x, int y, int w, int h);
	SDL_Surface *createSurface(int width, int height);
	SDL_Surface *alphaRect(int width, int height, Uint8 red, Uint8 green, Uint8 blue);
	void colorize(SDL_Surface *image, int red, int green, int blue);
	void lock(SDL_Surface *surface);
	void unlock(SDL_Surface *surface);
	void resetLoading();
	void showLoading(int amount, int max);
	void showErrorAndExit(const char *error, const char *param);
	void showLicenseErrorAndExit();
	void showRootWarning();

};
