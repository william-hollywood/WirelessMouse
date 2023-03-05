
APPNAME=MobileInterfaceV2
RAWDRAWANDROID=rawdrawandroid
CFLAGS:=-I. -ffunction-sections -Os -fvisibility=hidden -DRDALOGFNCB=example_log_function
LDFLAGS:=-s
PACKAGENAME?=com.WDH.$(APPNAME)
SRC:=test.c

ANDROIDVERSION=28
ANDROIDTARGET=28

TARGETS:=makecapk/lib/arm64-v8a/lib$(APPNAME).so makecapk/lib/armeabi-v7a/lib$(APPNAME).so

include rawdrawandroid/Makefile

# Because the rawdrawandroid/Makefile includes this, I don't use c++ (ew, amirite)
# May as well remove it so I don't get a warnings
LDFLAGS := $(filter-out -static-libstdc++,$(LDFLAGS))

clean-manifest :
	rm -rf AndroidManifest.xml

deploy: clean clean-manifest all push run