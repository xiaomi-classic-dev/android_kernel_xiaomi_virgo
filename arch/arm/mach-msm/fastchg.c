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

#include <linux/kobject.h>
#include <linux/sysfs.h>

#include <linux/fastchg.h>

int current_charge_level;
int force_fast_charge = true;
int ac_charge_level;
int usb_charge_level;
int failsafe;

static ssize_t
force_fast_charge_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", force_fast_charge);
}

static ssize_t
force_fast_charge_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	int new_force_fast_charge;

	sscanf(buf, "%du", &new_force_fast_charge);

	switch (new_force_fast_charge) {
		case FAST_CHARGE_DISABLED:
		case FAST_CHARGE_FORCE_AC:
		case FAST_CHARGE_FORCE_CUSTOM_MA:
			force_fast_charge = new_force_fast_charge;
			return count;
		default:
			return -EINVAL;
	}
}

static ssize_t
ac_charge_level_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%d\n", ac_charge_level);
}

static ssize_t
ac_charge_level_store(struct kobject *kobj, struct kobj_attribute *attr,
			const char *buf, size_t count)
{
	int new_ac_charge_level;

	sscanf(buf, "%du", &new_ac_charge_level);

	if (failsafe == FAIL_SAFE_DISABLED &&
	    new_ac_charge_level <= MAX_CHARGE_LEVEL &&
	    new_ac_charge_level >= MIN_CHARGE_LEVEL) {
		ac_charge_level = new_ac_charge_level;
		return count;
	} else {
		switch (new_ac_charge_level) {
			case AC_CHARGE_1000:
			case AC_CHARGE_1100:
			case AC_CHARGE_1200:
			case AC_CHARGE_1300:
			case AC_CHARGE_1400:
			case AC_CHARGE_1500:
			case AC_CHARGE_1600:
			case AC_CHARGE_1700:
			case AC_CHARGE_1800:
			case AC_CHARGE_1900:
			case AC_CHARGE_2000:
				ac_charge_level = new_ac_charge_level;
				return count;
			default:
				return -EINVAL;
		}
	}

	return -EINVAL;
}

static ssize_t
usb_charge_level_show(struct kobject *kobj, struct kobj_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%d\n", usb_charge_level);
}

static ssize_t
usb_charge_level_store(struct kobject *kobj, struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int new_usb_charge_level;

	sscanf(buf, "%du", &new_usb_charge_level);

	if (failsafe == FAIL_SAFE_DISABLED &&
	    new_usb_charge_level <= MAX_CHARGE_LEVEL &&
	    new_usb_charge_level >= MIN_CHARGE_LEVEL) {
		usb_charge_level = new_usb_charge_level;
		return count;
	} else {
		switch (new_usb_charge_level) {
			case USB_CHARGE_500:
			case USB_CHARGE_600:
			case USB_CHARGE_700:
			case USB_CHARGE_800:
			case USB_CHARGE_900:
			case USB_CHARGE_1000:
				usb_charge_level = new_usb_charge_level;
				return count;
			default:
				return -EINVAL;
		}
	}

	return -EINVAL;
}

static ssize_t
failsafe_show(struct kobject *kobj, struct kobj_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%d\n", failsafe);
}

static ssize_t
failsafe_store(struct kobject *kobj, struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	int new_failsafe;

	sscanf(buf, "%du", &new_failsafe);

	switch (new_failsafe) {
		case FAIL_SAFE_ENABLED:
			usb_charge_level = USB_CHARGE_500;
			ac_charge_level = AC_CHARGE_1500;
			failsafe = new_failsafe;
			return count;
		case FAIL_SAFE_DISABLED:
			failsafe = new_failsafe;
			return count;
		default:
			return -EINVAL;
	}
}

static ssize_t
ac_levels_show(struct kobject *kobj, struct kobj_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%s\n", AC_LEVELS);
}

static ssize_t
usb_levels_show(struct kobject *kobj, struct kobj_attribute *attr,
				char *buf)
{
	return sprintf(buf, "%s\n", USB_LEVELS);
}

static struct kobj_attribute force_fast_charge_attribute =
	__ATTR(force_fast_charge, 0666,
	       force_fast_charge_show, force_fast_charge_store);

static struct kobj_attribute ac_charge_level_attribute =
	__ATTR(ac_charge_level, 0666,
	       ac_charge_level_show, ac_charge_level_store);

static struct kobj_attribute usb_charge_level_attribute =
	__ATTR(usb_charge_level, 0666,
	       usb_charge_level_show, usb_charge_level_store);

static struct kobj_attribute failsafe_attribute =
	__ATTR(failsafe, 0666,
	       failsafe_show, failsafe_store);

static struct kobj_attribute ac_levels_attribute =
	__ATTR(ac_levels, 0444,
	       ac_levels_show, NULL);

static struct kobj_attribute usb_levels_attribute =
	__ATTR(usb_levels, 0444,
	       usb_levels_show, NULL);

static struct kobject *force_fast_charge_kobj;

static struct attribute *force_fast_charge_attrs[] = {
	&force_fast_charge_attribute.attr,
	&ac_charge_level_attribute.attr,
	&usb_charge_level_attribute.attr,
	&failsafe_attribute.attr,
	&ac_levels_attribute.attr,
	&usb_levels_attribute.attr,
	NULL,
};

static struct attribute_group force_fast_charge_attr_group = {
	.attrs = force_fast_charge_attrs,
};

int force_fast_charge_init(void)
{
	int ret;

	force_fast_charge = FAST_CHARGE_DISABLED;
	ac_charge_level = AC_CHARGE_2000;
	usb_charge_level = USB_CHARGE_1000;
	current_charge_level = NOT_FAST_CHARGING;
	failsafe = FAIL_SAFE_ENABLED;

	force_fast_charge_kobj = kobject_create_and_add("fast_charge",
					kernel_kobj);
	if (!force_fast_charge_kobj)
		return -ENOMEM;

	ret = sysfs_create_group(force_fast_charge_kobj,
			&force_fast_charge_attr_group);
	if (ret)
		kobject_put(force_fast_charge_kobj);

	return ret;
}

void force_fast_charge_exit(void)
{
	kobject_put(force_fast_charge_kobj);
}

module_init(force_fast_charge_init);
module_exit(force_fast_charge_exit);
