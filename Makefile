
# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present
PCIUTILS_DIR="/Users/matteo/Documents/Development/pciutils"


FLAGS = -lpci   -L$(PCIUTILS_DIR)/lib -I$(PCIUTILS_DIR) -framework IOKit -framework DirectHW -F/Library/Frameworks



# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present
PCIUTILS_DIR="/Users/matteo/Documents/Development/pciutils/"

lnkspd: main.c
	gcc -lpci  $(FLAGS) main.c -o lnkspd
clean:
	rm lnkspd
