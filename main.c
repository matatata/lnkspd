//
//  main.c
//  mypci
//
//  Created by Matteo on 16.06.19.
//  Copyright © 2019 Matteo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include <pciutils.h>


struct pci_access *pacc;

const char program_name[] = "lnkspd";

const char * USAGE = "Usage: %s [-s [dom:][bus:][slot][.func]] [-t target_speed]\n";

int set_target_speed(struct pci_dev *dev,unsigned int target_speed );


int main(int argc, char * argv[])
{
    
    struct pci_filter filter;
    struct pci_dev *dev;
    
    char namebuf[1024];
    
    pacc = pci_alloc();
    pci_init(pacc);
    pci_scan_bus(pacc);        /* We want to get the list of devices */
    
    pci_filter_init(pacc, &filter);
    
    unsigned int target_speed = 0;
    int verbose=0;
    int c;
    opterr = 0;
    while ((c = getopt (argc, argv, "vs:t:")) != -1)
        switch (c)
    {
        case 'v':
            verbose = 1;
            break;
        case 's':
            if(pci_filter_parse_slot(&filter, optarg)){
                fprintf(stderr,"Unable to parse filter %s\n", optarg);
                fprintf (stderr,USAGE,program_name);
                return 1;
            }
            break;
        case 't':{
            char *d;
            target_speed = (unsigned int) strtoul (optarg, &d, 10) ;
            // Ensure argument was okay.
            if ((d == optarg) || (*d != '\0') || target_speed < 1 || target_speed > 3) {
                fprintf(stderr,"Invalid number for -%c\n",optopt);
                return 1;
            }
            
        }
            break;
        case '?':
            fprintf (stderr,USAGE,program_name);
            return 1;
        default:
            abort ();
    }
    
    
    if(verbose)
        printf ("will try to set target speed = %d\n",target_speed);
    
    if(optind < argc){
        fprintf (stderr,USAGE,program_name);
        return 1;
    }
    /*
     for (int index = optind; index < argc; index++) {
     printf ("Non-option argument %s\n", argv[index]);
     }*/
    if(verbose)
        printf("Filter dom=%04x bus=%02x slot=%02x func=%d\n",filter.domain,filter.bus,filter.slot,filter.func);
    
    printf("dom:bus:slot.f\t\tVendor\tDevice\tSpeed\tWidth\tName | Vendor\t\t\n");
    
    struct pci_dev *selected_dev=NULL;
    int matches=0;
    for (dev=pacc->devices; dev; dev=dev->next)    /* Iterate over all devices */
    {
        if (!pci_filter_match(&filter, dev))
            continue;
        matches++;
        
        if(matches==1){
            selected_dev = dev;
        }
        
        pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);    /* Fill in header info we need */
        
        
        struct pci_cap *cap;
        cap = pci_find_cap(dev, PCI_CAP_ID_EXP, PCI_CAP_NORMAL);
        if (cap)
        {
            unsigned int linkstatus = pci_read_word(dev, cap->addr + 0x12);
            unsigned int linkspeed = linkstatus & 0xF;
            unsigned int linkwidth = linkstatus >> 4 & 0x3F;
            
            printf("%04x:%02x:%02x.%d\t\t%04x\t%04x\t",
                   dev->domain, dev->bus, dev->dev, dev->func, dev->vendor_id, dev->device_id);
            printf("PCIe%d.0\t%dx\t", linkspeed,linkwidth);
            
            printf("%s", pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id));
            printf(" | %s\n", pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_VENDOR, dev->vendor_id, dev->device_id));
        }
        
    }
    
    
    
    if(target_speed){
        
        if(matches!=1){
            fprintf(stderr,"To set a link target speed the filter set via -s must match exactly one device\n");
            return 1;
        }
        if(selected_dev)
            set_target_speed(selected_dev,target_speed);

    }
    
    
    pci_cleanup(pacc);        /* Close everything */
    return 0;
}

int set_target_speed(struct pci_dev *dev,unsigned int target_speed ){
    
    struct pci_cap *cap;
    cap = pci_find_cap(dev, PCI_CAP_ID_EXP, PCI_CAP_NORMAL);
    if (!cap){
        fprintf(stderr,"no CAP_ID_EXP");
        return 1;
    }
    
    unsigned int lnk_ctrl_reg2 = pci_read_word(dev, cap->addr+0x30);
    lnk_ctrl_reg2 &= ~0xF; // Clear lower 4 bits.
    lnk_ctrl_reg2 |= target_speed & 0xF; // set target speed
    pci_write_word(dev, cap->addr+0x30, lnk_ctrl_reg2);
    
    sleep(1);
    
    // poll until training bit says 0
    unsigned int training_bit = 0;
    while((training_bit = (pci_read_word(dev,cap->addr+0x12) ) & 0x800)!=0){
        printf("wait for training bit to return 0\n");
        usleep(1000);
    }
    
    
    // Set Retrain Link bit 5 of offset 10
    unsigned int lnkctr=pci_read_word(dev, cap->addr+0x10);
    lnkctr |= 0x20;
    pci_write_word(dev,cap->addr+0x10,lnkctr);
    
    
    sleep(1);
    
    while((training_bit = (pci_read_word(dev,cap->addr+0x12) ) & 0x800)!=0){
        printf("after setting retrain bit wait for training bit to return 0\n");
        usleep(1000);
    }
    
    unsigned int linkstatus = pci_read_word(dev, cap->addr + 0x12);
    unsigned int linkspeed = linkstatus & 0xF;
    unsigned int linkwidth = linkstatus >> 4 & 0x3F;
    
    
    printf("Now PCIe%d.0\t%dx\t\n", linkspeed,linkwidth);
    return 0;
}
