//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#include "build.h"
#include "game.h"

#define SHORT_MAXINTERPOLATIONS 256 // FIXME use new macro

int16_t spriteang_numinterpolations = 0;
static int16_t spriteang_bakipos[SHORT_MAXINTERPOLATIONS];
static int16_t spriteang_curipos[SHORT_MAXINTERPOLATIONS];

void spriteang_setinterpolation(int16_t spritenum)
{
    int i;

    if (spriteang_numinterpolations >= SHORT_MAXINTERPOLATIONS)
        return;

    for (i = spriteang_numinterpolations - 1; i >= 0; i--)
    {
        if (spriteang_curipos[i] == spritenum)
            return;
    }

    spriteang_curipos[spriteang_numinterpolations] = spritenum;
    spriteang_numinterpolations++;
}

void spriteang_stopinterpolation(int16_t spritenum)
{
    int i;

    for (i = spriteang_numinterpolations - 1; i >= 0; i--)
    {
        if (spriteang_curipos[i] == spritenum)
        {
            spriteang_numinterpolations--;
            spriteang_bakipos[i] = spriteang_bakipos[spriteang_numinterpolations];
            spriteang_curipos[i] = spriteang_curipos[spriteang_numinterpolations];
        }
    }
}

void spriteang_updateinterpolations(void)                  // Stick at beginning of domovethings
{
    int i;

    for (i = spriteang_numinterpolations - 1; i >= 0; i--)
        User[spriteang_curipos[i]]->oangdiff = 0;
}

// must call restore for every do interpolations
// make sure you don't exit
void spriteang_dointerpolations(int smoothratio)                      // Stick at beginning of drawscreen
{
    int i, j;

    for (i = spriteang_numinterpolations - 1; i >= 0; i--)
    {
        int16_t spritenum = spriteang_curipos[i];
        spriteang_bakipos[i] = sprite[spritenum].ang;

        j = mulscale16(User[spritenum]->oangdiff, 65536 - smoothratio);

        sprite[spritenum].ang = NORM_Q16ANGLE(sprite[spritenum].ang - j);
    }
}

void spriteang_restoreinterpolations(void)                 // Stick at end of drawscreen
{
    int i;

    for (i = spriteang_numinterpolations - 1; i >= 0; i--)
        sprite[spriteang_curipos[i]].ang = spriteang_bakipos[i];
}
