
# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present




FLAGS = -lpci  -framework IOKit -framework DirectHW -F/Library/Frameworks



# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present

lnkspd: main.c
	gcc -lpci  $(FLAGS) main.c -o lnkspd
clean:
	rm lnkspd
	sudo rm -rf ROOT
	rm matatata.lnkspd.pkg

install: lnkspd
	install -c -m 755 lnkspd /usr/local/sbin
	install -c -m 755 lnkspd.sh /usr/local/sbin


pkg:	install
	#sudo rm -rf ROOT
	#rm matatata.lnkspd.pkg
	mkdir ROOT
	install -d ROOT/usr/local/sbin
	install -d ROOT/usr/local/share
	install -d ROOT/Library/LaunchDaemons
	install -c -m 755 lnkspd ROOT/usr/local/sbin
	install -c -m 755 lnkspd.sh ROOT/usr/local/sbin
	cp -R /usr/local/share/DirectHW.kext ROOT/usr/local/share/DirectHW.kext
	sudo chmod -R 755 ROOT/usr/local/share/DirectHW.kext
	sudo chown -R root:wheel ROOT/usr/local/share/DirectHW.kext
	install -d ROOT/Library/Frameworks
	cp -R /Library/Frameworks/DirectHW.framework ROOT/Library/Frameworks/
	sudo chmod -R 755 ROOT/Library/Frameworks/DirectHW.framework
	sudo chown -R root:wheel ROOT/Library/Frameworks/DirectHW.framework
	install -d ROOT/Library/LaunchDaemons
	install -m 755 com.github.matatata.lnkspd.plist ROOT/Library/LaunchDaemons/
	pkgbuild --root ROOT --identifier com.github.matatata.lnkspd --version 1.0  matatata.lnkspd.pkg

