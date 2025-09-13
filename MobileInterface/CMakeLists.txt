cmake_minimum_required(VERSION 3.28)
project("MobileInterface")

set(CMAKE_EXPORT_COMPILE_COMMANDS "yes")

set(ANDROIDSDK $ENV{ANDROID_HOME})
set(ANDROIDTARGET 30)
set(ANDROIDVERSION 30)
set(APKFILE ${APPNAME}.apk)
set(APPNAME MobileInterface)
set(LABEL ${APPNAME})
set(BUILD_TOOLS_VER 30.0.2)
set(NDK_VER 21.3.6528147)
set(PACKAGENAME com.WDH.${APPNAME})
set(TARGET_ARCH arm64-v8a)

set(RAWDRAWANDROID rawdrawandroid)

# CMake commands created based off of rawdrawandroid/Makefile

set(ANDROID_FULLSCREEN y)
set(ADB $ENV{ADB})
string(TOLOWER ${CMAKE_SYSTEM_NAME}-x86_64 OS_NAME)
set(NDK ${ANDROIDSDK}/ndk/${NDK_VER})
set(BUILD_TOOLS ${ANDROIDSDK}/build-tools/${BUILD_TOOLS_VER})

add_compile_options(
    -ffunction-sections
    -Os
    -fvisibility=hidden
    -DRDALOGFNCB=LogFunction
    -m64
)
add_compile_definitions(
    ANDROID
    APPNAME="${APPNAME}"
    ANDROIDVERSION=${ANDROIDVERSION}
)
if(${ANDROID_FULLSCREEN} STREQUAL "y")
    add_compile_definitions(ANDROID_FULLSCREEN)
endif()

# Add the library that has all our app stuff
include_directories(
    ${RAWDRAWANDROID}/rawdraw
    ${NDK}/sysroot/usr/include
    ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/sysroot/usr/include
    ${RAWDRAWANDROID}
)
link_libraries(
    m
    GLESv3
    EGL
    android
    log
)
add_link_options(-uANativeActivity_onCreate)

set(CMAKE_C_COMPILER
    ${NDK}/toolchains/llvm/prebuilt/${OS_NAME}/bin/aarch64-linux-android${ANDROIDVERSION}-clang
)

include_directories(${APPNAME} PRIVATE ../WirelessMouseDesktop/src)
add_subdirectory(src)
target_sources(${APPNAME} PRIVATE ${RAWDRAWANDROID}/android_native_app_glue.c)
add_link_options(${APPNAME} PRIVATE -s)

# Packaging process
set(STOREPASS password)
set(DNAME "CN=example.com, OU=ID, O=Example, L=Doe, S=John, C=GB")
set(KEYSTOREFILE my-release-key.keystore)
set(ALIASNAME standkey)

add_custom_command(
    OUTPUT ${KEYSTOREFILE}
    COMMAND
        keytool -genkey -v -keystore ${KEYSTOREFILE} -alias ${ALIASNAME} -keyalg
        RSA -keysize 2048 -validity 10000 -storepass ${STOREPASS} -keypass
        ${STOREPASS} -dname ${DNAME}
)

add_custom_command(
    OUTPUT AndroidManifest.xml
    COMMAND
        PACKAGENAME=${PACKAGENAME} ANDROIDVERSION=${ANDROIDVERSION}
        ANDROIDTARGET=${ANDROIDTARGET} APPNAME=${APPNAME} LABEL=${LABEL}
        envsubst '$$ANDROIDTARGET $$ANDROIDVERSION $$APPNAME $$PACKAGENAME
        $$LABEL' < ${CMAKE_CURRENT_LIST_DIR}/AndroidManifest.xml.template >
        AndroidManifest.xml
)

set(ICONS "${CMAKE_CURRENT_LIST_DIR}/Sources/assets/icon.png" "${CMAKE_CURRENT_LIST_DIR}/Sources/res/mipmap/icon.png")

add_custom_command(
    OUTPUT ${APKFILE}
    COMMAND rm -rf makecapk
    COMMAND mkdir -p makecapk/assets
    COMMAND mkdir -p makecapk/lib/${TARGET_ARCH}
    COMMAND cp $<TARGET_FILE:${APPNAME}> makecapk/lib/${TARGET_ARCH}
    COMMAND cp -r ${CMAKE_BINARY_DIR}/${KEYSTOREFILE} makecapk/
    COMMAND cp -r ${CMAKE_CURRENT_LIST_DIR}/Sources/assets/* makecapk/assets
    COMMAND rm -rf temp.apk
    COMMAND
        ${BUILD_TOOLS}/aapt package -f -F temp.apk -I
        ${ANDROIDSDK}/platforms/android-${ANDROIDVERSION}/android.jar -M
        AndroidManifest.xml -S ${CMAKE_CURRENT_LIST_DIR}/Sources/res -A
        makecapk/assets -v --target-sdk-version ${ANDROIDTARGET}
    COMMAND unzip -o temp.apk -d makecapk
    COMMAND rm -rf makecapk.apk
    COMMAND
        cd makecapk && zip -D9r ../makecapk.apk . && zip -D0r ../makecapk.apk
        ./resources.arsc ./AndroidManifest.xml && cd ..
    COMMAND
        jarsigner -sigalg SHA1withRSA -digestalg SHA1 -verbose -keystore
        ${KEYSTOREFILE} -storepass ${STOREPASS} ${CMAKE_BINARY_DIR}/makecapk.apk
        ${ALIASNAME}
    COMMAND rm -rf ${APKFILE}
    COMMAND
        ${BUILD_TOOLS}/zipalign -v 4 ${CMAKE_BINARY_DIR}/makecapk.apk ${APKFILE}
    #Using the apksigner in this way is only required on Android 30+
    COMMAND
        ${BUILD_TOOLS}/apksigner sign --key-pass pass:${STOREPASS} --ks-pass
        pass:${STOREPASS} --ks ${KEYSTOREFILE} ${APKFILE}
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/temp.apk
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/makecapk.apk
    COMMAND ls -l ${APKFILE}
    DEPENDS ${APPNAME} AndroidManifest.xml ${KEYSTOREFILE} ${ICONS}
)
add_custom_target(push COMMAND ${ADB} install -r ${APKFILE} DEPENDS ${APKFILE})
