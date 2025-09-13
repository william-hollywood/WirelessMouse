
APPNAME=MobileInterface
RAWDRAWANDROID=rawdrawandroid
CFLAGS:=-I. -I../WirelessMouseDesktop/src/ -ffunction-sections -Os -fvisibility=hidden -DRDALOGFNCB=LogFunction
LDFLAGS:=-s
PACKAGENAME?=com.WDH.$(APPNAME)
SRC:=src/main.c src/app.c
SRC+=src/ui/rect.c src/ui/button.c src/ui/dragger.c

ANDROIDVERSION=30
ANDROIDTARGET=30

TARGETS:=makecapk/lib/arm64-v8a/lib$(APPNAME).so makecapk/lib/armeabi-v7a/lib$(APPNAME).so

include rawdrawandroid/Makefile

# Because the rawdrawandroid/Makefile includes this, I don't use c++ (ew, amirite)
# May as well remove it so I don't get a warnings
LDFLAGS := $(filter-out -static-libstdc++,$(LDFLAGS))

clean-manifest :
	rm -rf AndroidManifest.xml

deploy: clean clean-manifest all push run
