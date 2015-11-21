PROG = blobwars
PAKNAME = blobwars.pak
DOCS = doc/*
ICONS = icons/
DATA = data gfx sound music

VERSION = 2.00
RELEASE ?= 0
USEPAK ?= 0

PREFIX ?= /usr
BINDIR ?= $(PREFIX)/games/
DATADIR ?= $(PREFIX)/share/games/blobwars/
DOCDIR ?= $(PREFIX)/share/doc/$(PROG)/
ICONDIR ?= $(PREFIX)/share/icons/hicolor/
DESKTOPDIR ?= $(PREFIX)/share/applications/
LOCALEDIR ?= $(PREFIX)/share/locale/
MEDAL_SERVER_HOST = www.parallelrealities.co.uk
MEDAL_SERVER_PORT = 80

CXXFLAGS += `pkg-config --cflags sdl2 SDL2_mixer SDL2_image SDL2_ttf SDL2_net` -DVERSION=$(VERSION) -DRELEASE=$(RELEASE) -DUSEPAK=$(USEPAK)
CXXFLAGS += -DPAKNAME=\"$(PAKNAME)\" -DPAKLOCATION=\"$(DATADIR)\" -DUNIX -DGAMEPLAYMANUAL=\"$(DOCDIR)index.html\" -Wall
CXXFLAGS += -DLOCALEDIR=\"$(LOCALEDIR)\" -DMEDAL_SERVER_HOST=\"$(MEDAL_SERVER_HOST)\" -DMEDAL_SERVER_PORT=$(MEDAL_SERVER_PORT)
CXXFLAGS += $(CFLAGS) -Werror
LIBS = `pkg-config --libs sdl2 SDL2_mixer SDL2_image SDL2_ttf SDL2_net` -lz
PAKLIBS = -lz

OBJS += CAudio.o
OBJS += CBoss.o
OBJS += CCollision.o CCutscene.o  CConfig.o
OBJS += CData.o 
OBJS += CEffect.o CEngine.o CEntity.o 
OBJS += CFileData.o 
OBJS += CGame.o CGameData.o CGameObject.o CGraphics.o
OBJS += CHub.o 
OBJS += CKeyboard.o 
OBJS += CJoystick.o 
OBJS += CLineDef.o
OBJS += CList.o
OBJS += CMap.o CMath.o CMedalServer.o
OBJS += CObjective.o
OBJS += CPak.o CParticle.o CPersistant.o CPersistData.o
OBJS += CRadarBlip.o CReplayData.o
OBJS += CSpawnPoint.o CSprite.o CSwitch.o
OBJS += CTeleporter.o CTrain.o CTrap.o
OBJS += CWeapon.o CWidget.o

OBJS += aquaBoss.o
OBJS += bosses.o bullets.o 
OBJS += cutscene.o
OBJS += droidBoss.o 
OBJS += effects.o enemies.o entities.o explosions.o 
OBJS += finalBattle.o 
OBJS += galdov.o game.o graphics.o 
OBJS += hub.o 
OBJS += info.o init.o intro.o items.o 
OBJS += lineDefs.o loadSave.o
OBJS += map.o mapData.o mias.o mission.o
OBJS += objectives.o obstacles.o options.o
OBJS += particles.o player.o 
OBJS += resources.o 
OBJS += spawnPoints.o switches.o 
OBJS += tankBoss.o teleporters.o title.o trains.o traps.o triggers.o 
OBJS += weapons.o widgets.o

GAMEOBJS = $(OBJS) main.o
MAPOBJS = $(OBJS) mapEditor.o
PAKOBJS = CFileData.o pak.o

LOCALE_MO = $(patsubst %.po,%.mo,$(wildcard locale/*.po))

ALL = $(PROG) $(LOCALE_MO)
ifeq ($(USEPAK), 1)
	ALL += $(PAKNAME)
endif

# top-level rule to create the program.
all: $(ALL)

# compiling other source files.
%.o: src/%.cpp src/%.h src/defs.h src/defines.h src/headers.h
	$(CXX) $(CXXFLAGS) -c $<

# linking the program.
$(PROG): $(GAMEOBJS)
	$(CXX) $(LDFLAGS) $(GAMEOBJS) -o $(PROG) $(LIBS)
	
pak: $(PAKOBJS)
	$(CXX) $(LDFLAGS) $(PAKOBJS) -o pak $(PAKLIBS)

%.mo: %.po
	msgfmt -c -o $@ $<

mapeditor: $(MAPOBJS)
	$(CXX) $(LDFLAGS) $(MAPOBJS) -o mapeditor $(LIBS)

# cleaning everything that can be automatically recreated with "make".
clean:
	$(RM) $(GAMEOBJS) mapEditor.o pak.o $(PROG) $(PAKNAME) pak mapeditor $(LOCALE_MO)
	
$(PAKNAME): pak
	./pak $(DATA) $(PAKNAME)

buildpak: $(PAKNAME)

# install
install: $(ALL)
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(DATADIR)
	mkdir -p $(DESTDIR)$(DOCDIR)
	mkdir -p $(DESTDIR)$(ICONDIR)16x16/apps
	mkdir -p $(DESTDIR)$(ICONDIR)32x32/apps
	mkdir -p $(DESTDIR)$(ICONDIR)48x48/apps
	mkdir -p $(DESTDIR)$(ICONDIR)64x64/apps
	mkdir -p $(DESTDIR)$(DESKTOPDIR)

	install -m 755 $(PROG) $(DESTDIR)$(BINDIR)$(PROG)
ifeq ($(USEPAK), 1)
	install -m 644 $(PAKNAME) $(DESTDIR)$(DATADIR)$(PAKNAME)
else
	cp -pr $(DATA) $(DESTDIR)$(DATADIR)
endif
	cp -p $(DOCS) $(DESTDIR)$(DOCDIR)
	cp -p $(ICONS)$(PROG)16x16.png $(DESTDIR)$(ICONDIR)16x16/apps/$(PROG).png
	cp -p $(ICONS)$(PROG)32x32.png $(DESTDIR)$(ICONDIR)32x32/apps/$(PROG).png
	cp -p $(ICONS)$(PROG)48x48.png $(DESTDIR)$(ICONDIR)48x48/apps/$(PROG).png
	cp -p $(ICONS)$(PROG)64x64.png $(DESTDIR)$(ICONDIR)64x64/apps/$(PROG).png
	cp -p $(ICONS)$(PROG).desktop $(DESTDIR)$(DESKTOPDIR)

	@for f in $(LOCALE_MO); do \
		lang=`echo $$f | sed -e 's/^locale\///;s/\.mo$$//'`; \
		mkdir -p $(DESTDIR)$(LOCALEDIR)$$lang/LC_MESSAGES; \
		echo "cp $$f $(DESTDIR)$(LOCALEDIR)$$lang/LC_MESSAGES/$(PROG).mo"; \
		cp $$f $(DESTDIR)$(LOCALEDIR)$$lang/LC_MESSAGES/$(PROG).mo; \
	done

uninstall:
	$(RM) $(BINDIR)$(PROG)
	$(RM) $(DATADIR)$(PAKNAME)
	$(RM) -r $(DOCDIR)
	$(RM) $(ICONDIR)$(ICONS)$(PROG).png
	$(RM) $(ICONDIR)16x16/apps/$(PROG).png
	$(RM) $(ICONDIR)32x32/apps/$(PROG).png
	$(RM) $(ICONDIR)64x64/apps/$(PROG).png
	$(RM) $(DESKTOPDIR)$(PROG).desktop

	@for f in $(LOCALE_MO); do \
		lang=`echo $$f | sed -e 's/^locale\///;s/\.mo$$//'`; \
		echo "$(RM) $(LOCALEDIR)$$lang/LC_MESSAGES/$(PROG).mo"; \
		$(RM) $(LOCALEDIR)$$lang/LC_MESSAGES/$(PROG).mo; \
	done

.PHONY: all install uninstall clean buildpak
