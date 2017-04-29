/*
 * Copyright (C) 2017, Chad Froebel <chadfroebel@gmail.com>
 *		       Jean-Pierre Rasquin <yank555.lu@gmail.com>
 *		       Alex Saiko <solcmdr@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _LINUX_FASTCHG_H
#define _LINUX_FASTCHG_H

#define AC_LEVELS  "1000 1100 1200 1300 1400 1500 1600 1700 1800 1900 2000"
#define USB_LEVELS "500 600 700 800 900 1000"

extern int force_fast_charge;
#define FAST_CHARGE_DISABLED 0
#define FAST_CHARGE_FORCE_AC 1
#define FAST_CHARGE_FORCE_CUSTOM_MA 2

extern int ac_charge_level;
#define AC_CHARGE_1000 1000
#define AC_CHARGE_1100 1100
#define AC_CHARGE_1200 1200
#define AC_CHARGE_1300 1300
#define AC_CHARGE_1400 1400
#define AC_CHARGE_1500 1500
#define AC_CHARGE_1600 1600
#define AC_CHARGE_1700 1700
#define AC_CHARGE_1800 1800
#define AC_CHARGE_1900 1900
#define AC_CHARGE_2000 2000

extern int usb_charge_level;
#define USB_CHARGE_500 500
#define USB_CHARGE_600 600
#define USB_CHARGE_700 700
#define USB_CHARGE_800 800
#define USB_CHARGE_900 900
#define USB_CHARGE_1000 1000

#define MIN_CHARGE_LEVEL 100
#define MAX_CHARGE_LEVEL 2000

extern int current_charge_level;
#define NOT_FAST_CHARGING 0

extern int failsafe;
#define FAIL_SAFE_DISABLED 0
#define FAIL_SAFE_ENABLED 1

#endif
