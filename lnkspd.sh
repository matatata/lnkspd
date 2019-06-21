#!/bin/bash

kextload -v /usr/local/share/DirectHW.kext


/usr/local/sbin/lnkspd "$@"

kextunload -v -b com.apple.driver.DirectHW
