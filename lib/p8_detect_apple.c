/*
 * This file is partially derived from code that carried the following copyright
 * licensing information:
 *
 * Copyright (C) 2011-2012 Pulse-Eight Limited.  All rights reserved.
 *
 * This program is dual-licensed; you can redistribute it and/or modify
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <dirent.h>
#include <stdio.h>              /* For debugging */
#include <sys/param.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <CoreFoundation/CoreFoundation.h>

#include "proto.h"
#include "p8.h"

/*
 * We implement here a very convoluted way to find the right USB device.
 *
 * We first look for IOSerialBSDClients, i.e. devices that have a serial
 * node as /dev/tty.*.  Then, we iterate over them, and have a look at
 * if they are USB nodes and have the right vendor and product id.
 */

#define P8_CEC_VID  0x2548
#define P8_CEC_PID  0x1001
#define P8_CEC_PID2 0x1002

/*
 * Returns the Unix-level path to the serial port.
 * Buffer must point to a memory area of MAXPATHLEN.
 */
static char *
fetch_device_path(io_object_t serial, char *buffer) {
    // fetch the device path.
    CFTypeRef path;

    path = IORegistryEntryCreateCFProperty(serial, CFSTR(kIOCalloutDeviceKey),
                                           kCFAllocatorDefault, 0);
    if (!path)
        return NULL;

    // Place the path into the buffer
    CFStringGetCString((CFStringRef)path, buffer, MAXPATHLEN - 1, kCFStringEncodingUTF8);
    CFRelease(path);
    return buffer;
}

/*
 * Returns the first parent that conforms to the class IOUSBSerial
 */
static io_object_t
find_usb_parent(io_object_t serial) {
    io_object_t child = serial, parent;
    IOObjectRetain(serial);
    while (KERN_SUCCESS == IORegistryEntryGetParentEntry(child, kIOServicePlane, &parent)) {
        if (IOObjectConformsTo(parent, kIOUSBInterfaceClassName)) {
            return parent;
        }
        (void) IOObjectRelease(child);
        child = parent;
    }
    return IO_OBJECT_NULL;
}

/*
 * Matches a serial port I/Okit object for VID and PID
 */
static int
match_usb_vid_and_pid(io_object_t serial) {
    io_object_t usb;
    CFTypeRef vidRef = NULL;
    CFTypeRef pidRef = NULL;
    int vid = -1, pid = -1;

    DEBUG("Considering io_object %d\n", serial);

    usb = find_usb_parent(serial);
    if (usb == IO_OBJECT_NULL)
        return 0;

    DEBUG("Found USB parent %d\n", usb);

    vidRef = IORegistryEntrySearchCFProperty(usb, kIOServicePlane,
                                             CFSTR(kUSBVendorID),  kCFAllocatorDefault, 0);
    if (vidRef) {
        CFNumberGetValue((CFNumberRef)vidRef, kCFNumberIntType, &vid);
        CFRelease(vidRef);
        DEBUG("VID=%d\n", vid);
    }

    pidRef = IORegistryEntrySearchCFProperty(usb, kIOServicePlane,
                                             CFSTR(kUSBProductID), kCFAllocatorDefault, 0);
    if (pidRef) {
        CFNumberGetValue((CFNumberRef)pidRef, kCFNumberIntType, &pid);
        CFRelease(pidRef);
        DEBUG("PID=%d\n", pid);
    }

    IOObjectRelease(usb);

    return (vid == P8_CEC_VID && (pid == P8_CEC_PID || pid == P8_CEC_PID2));
}

/*
 * Places the adapter device file path name to the buffer.
 * Returns the buffer on success, NULL on failure.
 */
char *
p8_detect_path(char *buffer) {
    kern_return_t kr;
    io_iterator_t serialPortIterator;

    CFMutableDictionaryRef matchDict = IOServiceMatching(kIOSerialBSDServiceValue);
    if (!matchDict)
        return NULL;
    CFDictionarySetValue(matchDict, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDModemType));

    // Consumes matchDict
    kr = IOServiceGetMatchingServices(kIOMasterPortDefault, matchDict, &serialPortIterator);
    if (KERN_SUCCESS != kr)
        return NULL;

    for (
            io_object_t serial = IO_OBJECT_NULL;
            (serial = IOIteratorNext(serialPortIterator));
            IOObjectRelease(serial)
        ) {

        if (!match_usb_vid_and_pid(serial))
            continue;

        if (fetch_device_path(serial, buffer))
            break;
    }
    IOObjectRelease(serialPortIterator);
    return buffer;
}

