/*
* Apple Macbook Pro LCD backlight control
*
* Copyright (C) 2006 Nicolas Boichat <nicolas @boichat.ch>
* Copyright (C) 2006 Felipe Alfaro Solana <felipe_alfaro @linuxmail.org>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

#include <stdio.h>
#include <sys/io.h>
#include <stdlib.h>

void init()
{
    if (ioperm(0xB2, 0xB3, 1) < 0)
    {
        perror("ioperm failed (you should be root).");
        exit(2);
    }
}

int get_current_value()
{
    outb(0x03, 0xB3);
    outb(0xBF, 0xB2);
    char t = inb(0xB3) >> 4;
    return t;
}

int calculate_new_value(const char *arg)
{
    int val, new = atoi(arg);

    if (arg[0] == '+' || arg[0] == '-')
        val = new + get_current_value();
    else
        val = new;

    if (val > 15)
        val = 15;
    else if (val < 0)
        val = 1;

    return val;
}

int main(int argc, char** argv)
{
    if (argc > 2)
    {
        printf("Usage:\n");
        printf("%s : read current value\n", argv[0]);
        printf("%s value : write value [0-15]\n", argv[0]);
        exit(1);
    }

    init();

    if (argc < 2)
    {
        printf("Current value : %d\n", get_current_value());
        exit(0);
    }

    if (argc == 2)
    {
        int value = calculate_new_value(argv[1]);
        outb(0x04 | (value << 4), 0xB3);
        outb(0xBF, 0xB2);
        printf("new value: %d\n", value);
    }

    return 0;
}
