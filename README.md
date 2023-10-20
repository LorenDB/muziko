# Muziko

![Muziko logo](./qml/icons/logo-128.png)

[![Donate using Liberapay](https://img.shields.io/badge/donate-liberapay-yellow)](https://liberapay.com/LorenDB/donate) [![Donate using Paypal](https://img.shields.io/badge/donate-paypal-blue)](https://paypal.me/lorendev)

Muziko is an app designed to help you practice playing any musical instrument. It does so by taking all the songs you know and giving you a list of songs every day based on how well you know each song.

## Get the app

Muziko is available for Android devices (and Chromebooks) on the [Google Play Store](https://play.google.com/store/apps/details?id=dev.lorendb.muziko).

Muziko is not available for iPhone; I don't want to spend 600-1000 USD on a Mac plus 100 USD yearly on an Apple developer account. After all, this app is just a hobby and has not proved itself to be a wildly successful app with huge revenue streams or anything like that. However, if you really want Muziko on your iPhone, building from source shouldn't be too hard as long as you use Qt Creator.

For other platforms (Windows, Mac, Linux, etc.), binaries are not available right now, but you should be able to build it from source on pretty much any platform Qt supports.

## Building

Muziko is built with C++ and Qt, so once you have installed a standard Qt toolchain, run the normal `mkdir build && cd build && cmake .. && cmake --build .` to build Muziko. Or you could open the project in Qt Creator and build it that way, which probably would be the easiest way to build for Android or iOS.

## Screenshots

See the [Google Play Store listing](https://play.google.com/store/apps/details?id=dev.lorendb.muziko) for a few screenshots of the UI. Please note that as Muziko is still a work in progress, the UI is likely to change as different elements get cleaned up and new features get added.
