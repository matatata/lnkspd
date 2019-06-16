
# Use my fork https://github.com/matatata/pciutils, otherwise the libpci.a lib will not be present
PCIUTILS_DIR="/Users/matteo/Downloads/pciutils/"

gcc -lpci  -L"$PCIUTILS_DIR/lib" -I"$PCIUTILS_DIR" -framework IOKit main.c -o lnkspd

