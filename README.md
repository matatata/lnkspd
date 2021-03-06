# lnkspd


I created this to force my PCI M2 NVMe Carrier Card to PCI2.0 speed in my MacPro3,1 (PCIe Slot 2).

The `lnkspd` executable uses the DirectHW framework and KEXT to access the PCIe config space. I'm using the pciutils library to actually read and write the relevant bits whereas in my first attempt I used a more direct approach https://github.com/matatata/rwmem/blob/master/lnkspd.c .

The `lnkspd.sh` script loads the DirectHW.kext, executed lnkspd and unloads the kext afterwards.

To set the target speed to 2 (5 GT/s) in slot 2 (that's 00:01.0) on my MacPro3,1 I use:

```
sudo /usr/local/sbin/lnkspd.sh -s 00:01.0 -t 2
```

Check the current linkspeed:
```
sudo /usr/local/sbin/lnkspd.sh -s 00:01.0
```

The ```-s``` parameter lets you filter for specific slots, busses etc. like the pcitools do. Without parameter all slots will be listed.

To build it yourself you'd first have to build and install:

https://github.com/matatata/directhw and

https://github.com/matatata/pciutils

I've created a [pkg installer file](https://github.com/matatata/lnkspd/raw/master/matatata.lnkspd.pkg) that also includes a 
LaunchDaemon (the plist file) that executes the script on boot. Enable it with `sudo launchctl load -w /Library/LaunchDaemons/com.github.matatata.lnkspd.plist`


