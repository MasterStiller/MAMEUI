###########################################################################
#
#   winui.mak
#
#   MAME Windows-specific makefile
#
###########################################################################

# build the executable names
RCFLAGS += -DMAME

EMU_EXE = $(PREFIX)$(NAME)$(SUFFIX)$(SUFFIX64)$(SUFFIXDEBUG)$(EXE)
BUILD += $(EMU_EXE)

MAME_WINUISRC = $(SRC)/mame/osd/winui
MAME_WINUIOBJ = $(OBJ)/mame/osd/winui
WINUISRC = $(SRC)/osd/winui
WINUIOBJ = $(OBJ)/osd/winui
OSDOBJ = $(OBJ)/osd

RESFILE = $(MAME_WINUIOBJ)/mameui.res

INCPATH += \
	-I$(MAMESRC)/osd/winui

OBJDIRS += \
	$(MAMEOBJ)/osd \
	$(MAMEOBJ)/osd/winui


$(MAME_WINUIOBJ)/mameui.res:	$(WINUISRC)/mameui.rc $(MAME_WINUISRC)/messui.rc $(WINUISRC)/resource.h $(WINUISRC)/resourcems.h $(WINUIOBJ)/mamevers.rc
	@echo Compiling resources $<...
	$(RC) $(RCDEFS) $(RCFLAGS) --include-dir $(WINUISRC) --include-dir $(MAME_WINUISRC) --include-dir $(WINUIOBJ) -o $@ -i $(MAME_WINUISRC)/messui.rc

$(WINUIOBJ)/mamevers.rc: $(OBJ)/build/verinfo$(EXE) $(SRC)/version.c
	@echo Emitting $@...
	@"$(VERINFO)" -b mame $(SRC)/version.c  > $@

$(LIBOSD): $(OSDOBJS)

$(LIBOCORE): $(OSDCOREOBJS)

$(LIBOCORE_NOMAIN): $(OSDCOREOBJS:$(WINOBJ)/main.o=)

#-------------------------------------------------
# OSD Windows library
#-------------------------------------------------

WINOSDOBJS = \
	$(WINOBJ)/d3d9intf.o \
	$(WINOBJ)/drawd3d.o \
	$(WINOBJ)/drawdd.o \
	$(WINOBJ)/d3dhlsl.o \
	$(WINOBJ)/drawgdi.o \
	$(WINOBJ)/drawnone.o \
	$(WINOBJ)/input.o \
	$(WINOBJ)/output.o \
	$(OSDOBJ)/modules/lib/osdlib_win32.o \
	$(WINOBJ)/video.o \
	$(WINOBJ)/window.o \
	$(OSDOBJ)/modules/debugger/debugwin.o \
	$(OSDOBJ)/modules/lib/osdobj_common.o \
	$(OSDOBJ)/modules/midi/portmidi.o \
	$(OSDOBJ)/modules/sync/work_osd.o \
	$(OSDOBJ)/modules/osdmodule.o \
	$(OSDOBJ)/modules/sound/js_sound.o  \
	$(OSDOBJ)/modules/sound/direct_sound.o  \
	$(OSDOBJ)/modules/sound/sdl_sound.o  \
	$(OSDOBJ)/modules/sound/none.o  \
	$(OSDOBJ)/modules/font/font_sdl.o \
	$(OSDOBJ)/modules/font/font_windows.o \
	$(OSDOBJ)/modules/font/font_osx.o \
	$(OSDOBJ)/modules/font/font_none.o \
	$(OSDOBJ)/modules/debugger/debugwin.o \
	$(OSDOBJ)/modules/debugger/debugint.o \
	$(OSDOBJ)/modules/debugger/debugqt.o \
	$(OSDOBJ)/modules/debugger/none.o \
	$(WINOBJ)/winmain.o \
	$(WINOBJ)/winmenu.o \

ifndef DONT_USE_NETWORK
WINOSDOBJS += \
	$(WINOBJ)/netdev.o \
	$(WINOBJ)/netdev_pcap.o
endif

$(EMU_EXE): $(VERSIONOBJ) $(EMUINFOOBJ) $(DRIVLISTOBJ) $(DRVLIBS) $(WINOSDOBJS) $(LIBBUS) $(LIBOPTIONAL) $(LIBEMU) $(LIBDASM) $(LIBUTIL) $(EXPAT) $(SOFTFLOAT) $(JPEG_LIB) $(FLAC_LIB) $(7Z_LIB) $(FORMATS_LIB) $(LUA_LIB) $(SQLITE3_LIB) $(WEB_LIB) $(ZLIB) $(LIBOCORE) $(MIDI_LIB) $(RESFILE)
	@echo Linking $@...
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
