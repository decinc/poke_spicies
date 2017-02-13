/* This file is part of PKSM

Copyright (C) 2016 Bernardo Giordano

>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.
*/

#pragma once

#include <stdlib.h>
#include <3ds.h>

#define POKEBANK 0x00040000000C9B00

#define VERSION "v4.2.0"

#define GAME_X 0
#define GAME_Y 1
#define GAME_OR 2
#define GAME_AS 3
#define GAME_SUN 4
#define GAME_MOON 5
#define GAME_DIAMOND 6
#define GAME_PEARL 7
#define GAME_PLATINUM 8
#define GAME_HG 9
#define GAME_SS 10
#define GAME_B1 11
#define GAME_W1 12
#define GAME_B2 13
#define GAME_W2 14


#define         MASKBITS                0x3F
#define         MASKBYTE                0x80
#define         MASK2BYTES              0xC0
#define         MASK3BYTES              0xE0
#define         MASK4BYTES              0xF0
#define         MASK5BYTES              0xF8
#define         MASK6BYTES              0xFC


void checkMaxValue(u8* pkmn, int byteEntry, int value, int max);
void loadLines(u8 *src, wchar_t *dst, u8 strlen,  u32 size);
bool checkFile(char* path);
void loadPersonal();
void loadFile(u8* buf, char* path);
void injectFromFile(u8* mainbuf, char* path, u32 offset);
void printfile(char* path);
bool isHBL();
void fsStart();
void fsEnd();
bool openSaveArch(FS_Archive *out, u64 id);
void settingsMenu(u8* mainbuf, int game);