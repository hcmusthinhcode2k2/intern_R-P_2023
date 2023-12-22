#include "SST25xx.h"



// MAX_MEM_ADDR=(Capacity in Mbit *1024^2)/8 - 1 (address start at 0)
#define SECTOR_LENGTH   4096
#define PAGE_LENGTH     256
#define PAGE_IN_SECTOR  16
#if defined(SST25PF040B)
#define MAX_MEM_ADDR    0x7FFFF                                                                                                   
#elif defined(SST25VF020B)
#define MAX_MEM_ADDR    0x3FFFF  
#elif defined SST25VF016B
#define MAX_MEM_ADDR    0x1FFFFF 
#else
#warning "Please define your flash memory"
#endif

/*********************************************************************COMMANDS*/
#define SST_READ_DATA        0x03
#define SST_FAST_READ_DATA   0x0B
#define SST_SECTOR_ERASE     0x20
#define SST_SECTOR_32_ERASE  0x52
#define SST_SECTOR_64_ERASE  0xD8
#define SST_BULK_ERASE       0xC7
#define SST_BYTE_PROG        0x02
#define SST_WORD_PROG        0xAD
#define SST_READ_STATUS      0x05
#define SST_WRITE_STATUS_EN  0x50
#define SST_WRITE_STATUS     0x01
#define SST_WRITE_EN         0x06
#define SST_WRITE_DIS        0x04
#define SST_READ_JDECID      0x9F
#define SST_HW_WR_STATUS     0x70
#define SST_POLL_WR_STATUS   0x80
#define FLASH_PROTECT_LOCK   0x80
#define FLASH_PROTECT_UNLOCK 0x00

uint8_t sstCMD[6];

/* [Chip select On/Off=1/0][data buffer][write length][read length][Chip select On/Off=1/0] */
void SPI_Xfer(bool enCS, uint8_t *data, uint8_t lenIn, uint8_t lenOut, bool dnCS)
{
    uint8_t i;

    if(enCS)
        CS_SetLow();

    for(i=0; i<lenIn; i++)
        SPI_Exchange8bit(data[i]);

    for(i=0; i<lenOut; i++)
        data[i]=SPI_Exchange8bit(0xFF);

    if(dnCS)
        CS_SetHigh();
}

void SST_Enable_Write(void)// Private function
{
    sstCMD[0]=SST_WRITE_EN;
    SPI_Xfer(1, sstCMD, 1, 0, 1);
}

void SST_Disable_Write(void)// Private function
{
    sstCMD[0]=SST_WRITE_DIS;
    SPI_Xfer(1, sstCMD, 1, 0, 1);
}

void SST_Poll_WR_Status(void)// Private function
{
    sstCMD[0]=SST_POLL_WR_STATUS;
    SPI_Xfer(1, sstCMD, 1, 0, 1);
}

static uint8_t SST_Read_Status(void)// Private function
{
    sstCMD[0]=SST_READ_STATUS;
    SPI_Xfer(1, sstCMD, 1, 1, 1);

    return sstCMD[0];
}

void SST_Write_Status(uint8_t stt)// Private function
{
    sstCMD[0]=SST_WRITE_STATUS_EN;
    SPI_Xfer(1, sstCMD, 1, 0, 1);
    sstCMD[0]=SST_WRITE_STATUS;
    sstCMD[1]=stt;
    SPI_Xfer(1, sstCMD, 2, 0, 1);
}

void SST_Check_Busy(void)// Private function
{
    while(SST_Read_Status()&0x01);
}

void SST_Make_Address(uint32_t address)// Private function
{
    sstCMD[1]=(uint8_t) (address>>16);
    sstCMD[2]=(uint8_t) (address>>8);
    sstCMD[3]=(uint8_t) address;
}

void SST_Chip_Erase(void)// Public function
{
    SST_Enable_Write();
    sstCMD[0]=SST_BULK_ERASE;
    SPI_Xfer(1, sstCMD, 1, 0, 1);
    SST_Check_Busy();
    SST_Disable_Write();
}

void SST_Sector_Erase(uint32_t BAddr)// Public function
{
    SST_Enable_Write();
    sstCMD[0]=SST_SECTOR_ERASE;
    SST_Make_Address(BAddr);
    SPI_Xfer(1, sstCMD, 4, 0, 1);
    SST_Check_Busy();
    SST_Disable_Write();
}

void SST_Read_nByte(uint32_t BAddr, uint32_t len, uint8_t *buffer)// Public function
{
    SST_Check_Busy();
    sstCMD[0]=SST_FAST_READ_DATA;
    SST_Make_Address(BAddr);
    sstCMD[4]=0x00;
    SPI_Xfer(1, sstCMD, 5, 0, 0);
    SPI_Xfer(0, buffer, 0, len, 1);
}

uint8_t SST_Read_Byte(uint32_t BAddr)// Public function
{
    uint8_t data;

    SST_Check_Busy();
    sstCMD[0]=SST_FAST_READ_DATA;
    SST_Make_Address(BAddr);
    sstCMD[4]=0x00;
    SPI_Xfer(1, sstCMD, 5, 0, 0);
    SPI_Xfer(0, &data, 0, 1, 1);

    return data;
}

void SST_Write_Byte(uint32_t Addr, uint8_t data)// Public function
{
    SST_Check_Busy();
    SST_Enable_Write();
    sstCMD[0]=SST_BYTE_PROG;
    SST_Make_Address(Addr);
    sstCMD[4]=data;
    SPI_Xfer(1, sstCMD, 5, 0, 1);
    SST_Check_Busy();
    SST_Disable_Write();
}

void SST_Write_nByte(uint32_t BAddr, uint16_t len, uint8_t *data)// Public function
{
    uint16_t i;
    // Auto Address Increment (AAI) Word-Program
    SST_Check_Busy();
    SST_Enable_Write();
    sstCMD[0]=SST_WORD_PROG;
    SST_Make_Address(BAddr);
    sstCMD[4]=data[0];
    sstCMD[5]=data[1];
    SPI_Xfer(1, sstCMD, 6, 0, 1);
    SST_Check_Busy();

    for(i=2; i<len; i+=2)
    {
        sstCMD[0]=SST_WORD_PROG;
        sstCMD[1]=data[i];
        sstCMD[2]=data[i+1];
        SPI_Xfer(1, sstCMD, 3, 0, 1);
        SST_Check_Busy();
    }

    SST_Disable_Write();
}

void SST_Protect(sst_protect_range_t range)// Public function
{
    SST_Enable_Write();
    sstCMD[0]=SST_WRITE_STATUS;
    sstCMD[1]=FLASH_PROTECT_UNLOCK;
    SPI_Xfer(1, sstCMD, 2, 0, 1);
    SST_Enable_Write();
    sstCMD[0]=range|FLASH_PROTECT_LOCK;
    SPI_Xfer(1, sstCMD, 1, 0, 1);
}

uint32_t SST_Get_JDECID(void)// Public function
{
    uint32_t ID;

    sstCMD[0]=SST_READ_JDECID;
    SPI_Xfer(1, sstCMD, 1, 3, 1);
    ID=sstCMD[0];
    ID<<=8;
    ID|=sstCMD[1];
    ID<<=8;
    ID|=sstCMD[2];
    
    return ID;
}

bool SST_Init(void)// Public function
{
    uint8_t tryTimes=0;

LOOP:
    SPI_Init();
    sstCMD[0]=SST_READ_JDECID;
    SPI_Xfer(1, sstCMD, 1, 3, 1);

    if((sstCMD[0]!=0xBF)||(sstCMD[1]!=0x25))// SST JDEC ID is always 0xBF25xx
    {
        if(++tryTimes==0)
            return 1; // Try 256 times after return error

        goto LOOP;
    }

    SST_Write_Status(0); // BP0,1,2,3: not write protect, read/writable

    return 0;
}