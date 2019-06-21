
# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present




FLAGS = -lpci  -framework IOKit -framework DirectHW -F/Library/Frameworks



# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present

lnkspd: main.c
	gcc -lpci  $(FLAGS) main.c -o lnkspd
clean:
	rm lnkspd

install: lnkspd
	install -c -m 755 lnkspd /usr/local/sbin
	install -c -m 755 lnkspd.sh /usr/local/sbin





