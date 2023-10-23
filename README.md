<!--
SPDX-FileCopyrightText: 2023 Loren Burkholder <computersemiexpert@outlook.com>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Muziko

![Muziko logo](./qml/icons/logo-128.png)

[![Linux build](https://github.com/LorenDB/muziko/actions/workflows/linux-build.yml/badge.svg)](https://github.com/LorenDB/muziko/actions/workflows/linux-build.yml) [![Windows build](https://github.com/LorenDB/muziko/actions/workflows/windows-build.yml/badge.svg)](https://github.com/LorenDB/muziko/actions/workflows/windows-build.yml) [![macOS build](https://github.com/LorenDB/muziko/actions/workflows/macos-build.yml/badge.svg)](https://github.com/LorenDB/muziko/actions/workflows/macos-build.yml) [![Linting](https://github.com/LorenDB/muziko/actions/workflows/linting.yml/badge.svg)](https://github.com/LorenDB/muziko/actions/workflows/linting.yml) [![CodeQL](https://github.com/LorenDB/muziko/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/LorenDB/muziko/actions/workflows/codeql-analysis.yml) [![Donate using Liberapay](https://img.shields.io/badge/donate-liberapay-yellow)](https://liberapay.com/LorenDB/donate) [![Donate using Paypal](https://img.shields.io/badge/donate-paypal-blue)](https://paypal.me/lorendev)
[![Get it on Google Play](https://img.shields.io/badge/get%20it%20on-google%20play-green)](https://play.google.com/store/apps/details?id=dev.lorendb.muziko)

Muziko is an app designed to help you practice playing any musical instrument. It does so by taking all the songs you know and giving you a list of songs every day based on how well you know each song.

## How it works

Muziko works more or less on the principle of spaced repetition; however, the approach is unique enough to warrant a full explanation.

When you start Muziko, you will need to enter the name of your instrument. Once done, you can enter as many songs as you would like into Muziko. You will be asked for the song name and your level of proficiency (low, medium, or high). Muziko will then give you a list of songs every day (by default, 5 songs) to practice. The song choosing algorithm works as follows:

- Low proficiency songs are practiced daily.
- Medium proficiency songs are practiced several times per week.
- High proficiency songs are not practiced at a fixed interval; instead, Muziko cycles through them at the rate of 1-2 per day. The more high proficiency songs you have, the longer it will take you to cycle through them all.

Muziko also lets you store links to aid your practice; for example, you might save a link to a jam track YouTube video or to an online learning platform for each song.

## Get the app

Muziko is available for Android devices (and Chromebooks) on the [Google Play Store](https://play.google.com/store/apps/details?id=dev.lorendb.muziko).

Muziko is not available for iPhone; I don't want to spend 600-1000 USD on a Mac plus 100 USD yearly on an Apple developer account. After all, this app is just a hobby and has not proved itself to be a wildly successful app with huge revenue streams or anything like that. However, if you really want Muziko on your iPhone, building from source shouldn't be too hard as long as you use Qt Creator.

For other platforms (Windows, Mac, Linux, etc.), binaries are not available right now, but you should be able to build it from source on pretty much any platform Qt supports.

## Building

Muziko is built with C++ and Qt, so once you have installed a standard Qt toolchain, run the normal `mkdir build && cd build && cmake .. && cmake --build .` to build Muziko. Or you could open the project in Qt Creator and build it that way, which probably would be the easiest way to build for Android or iOS.

## Contact

You can chat about Muziko at [#muziko:nheko.im](https://matrix.to/#/#muziko:nheko.im).

## Screenshots

See the [Google Play Store listing](https://play.google.com/store/apps/details?id=dev.lorendb.muziko) for a few screenshots of the UI. Please note that as Muziko is still a work in progress, the UI is likely to change as different elements get cleaned up and new features get added.
