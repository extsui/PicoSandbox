#include "tusb.h"
#include <cstring>
#include "Util.h"

const uint8_t DeviceDescriptor[] = {
    0x12,                   // bLength
    TUSB_DESC_DEVICE,       // bDescriptorType
    0x10, 0x01,             // bcdUSB 1.10
    0x00,                   // bDeviceClass (個別指定)
    0x00,                   // bDeviceSubClass
    0x00,                   // bDeviceProtocol
    CFG_TUD_ENDPOINT0_SIZE, // bMaxPacketSize0
    0xCA, 0xFE,             // idVendor (適当な値に)
    0x00, 0x01,             // idProduct
    0x00, 0x01,             // bcdDevice
    0x01,                   // iManufacturer
    0x02,                   // iProduct
    0x03,                   // iSerialNumber
    0x01                    // bNumConfigurations
};

uint8_t const ConfigurationDescriptor[] = {
    0x09,                               // bLength
    TUSB_DESC_CONFIGURATION,            // bDescriptorType
    32, 0x00,                           // wTotalLength
    0x01,                               // bNumInterfaces
    0x01,                               // bConfigurationValue
    0x00,                               // iConfiguration
    0x80,                               // bmAttributes (バスパワー)
    0x32,                               // bMaxPower (100mA)

    // Interface Descriptor (MSC)
    0x09,                               // bLength
    TUSB_DESC_INTERFACE,                // bDescriptorType
    0x00,                               // bInterfaceNumber
    0x00,                               // bAlternateSetting
    0x02,                               // bNumEndpoints
    TUSB_CLASS_MSC,                     // bInterfaceClass
    0x06,                               // bInterfaceSubClass (SCSI)
    0x50,                               // bInterfaceProtocol (Bulk-Only)
    0x00,                               // iInterface

    // Endpoint Descriptor (Bulk OUT)
    0x07,                               // bLength
    TUSB_DESC_ENDPOINT,                 // bDescriptorType
    0x01,                               // bEndpointAddress (OUT)
    TUSB_XFER_BULK,                     // bmAttributes
    0x40, 0x00,                         // wMaxPacketSize (64)
    0x00,                               // bInterval

    // Endpoint Descriptor (Bulk IN)
    0x07,                               // bLength
    TUSB_DESC_ENDPOINT,                 // bDescriptorType
    0x81,                               // bEndpointAddress (IN)
    TUSB_XFER_BULK,                     // bmAttributes
    0x40, 0x00,                         // wMaxPacketSize (64)
    0x00                                // bInterval
};

char const* StringDescriptors[] = {
    (const char[]) {0x09, 0x04},        // 0: English (US)
    "Manufacturer",
    "MSC Sample",
    "123456"
};

// --- コールバック関数群 ---

extern "C" {

// USBデバイス記述子を返す
uint8_t const* tud_descriptor_device_cb(void)
{
    return DeviceDescriptor;
}

// USB設定記述子を返す
uint8_t const* tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return ConfigurationDescriptor;
}

// 文字列記述子を返す
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;

    static uint16_t _desc_str[32];
    uint8_t chr_count;

    if (index == 0)
    {
        memcpy(&_desc_str[1], StringDescriptors[0], 2);
        chr_count = 1;
    }
    else
    {
        if (!(index < sizeof(StringDescriptors) / sizeof(StringDescriptors[0])))
        {
            return NULL;
        }
        const char* str = StringDescriptors[index];
        chr_count = (uint8_t)strlen(str);
        if(chr_count > 31)
        {
            chr_count = 31;
        }
        for (uint8_t i = 0; i < chr_count; i++)
        {
            _desc_str[1+i] = str[i];
        }
    }
    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);
    return _desc_str;
}

// --- MSCコールバック ---

// RAMディスクの設定
#define RAMDISK_BLOCK_SIZE 512
#define RAMDISK_BLOCK_COUNT 128
static uint8_t g_RamDisk[RAMDISK_BLOCK_COUNT * RAMDISK_BLOCK_SIZE];

// ストレージ容量を返す
void tud_msc_capacity_cb(uint8_t lun, uint32_t* block_count, uint16_t* block_size)
{
    INFO_LOG("%s\n", __func__);

    (void) lun;
    *block_count = RAMDISK_BLOCK_COUNT;
    *block_size  = RAMDISK_BLOCK_SIZE;
}

// 準備完了確認（常にReady）
bool tud_msc_test_unit_ready_cb(uint8_t lun)
{
    INFO_LOG("%s\n", __func__);

    (void) lun;
    return true;
}

// INQUIRY応答
void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4])
{
    INFO_LOG("%s\n", __func__);

    (void) lun;
    memcpy(vendor_id, "PICOSDK ", 8);
    memcpy(product_id, "TinyUSB MSC     ", 16);
    memcpy(product_rev, "1.00", 4);
}

// MSC読み込み処理（RAMディスクから）
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void* buffer, uint32_t bufsize)
{
    INFO_LOG("%s\n", __func__);

    (void) lun;
    if (lba >= RAMDISK_BLOCK_COUNT)
    {
        return -1;
    }
    memcpy(buffer, &g_RamDisk[lba * RAMDISK_BLOCK_SIZE + offset], bufsize);
    return (int32_t) bufsize;
}

// MSC書き込み処理（RAMディスクへ）
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t* buffer, uint32_t bufsize)
{
    INFO_LOG("%s\n", __func__);

    (void) lun;
    if (lba >= RAMDISK_BLOCK_COUNT)
    {
        return -1;
    }
    memcpy(&g_RamDisk[lba * RAMDISK_BLOCK_SIZE + offset], buffer, bufsize);
    return (int32_t) bufsize;
}

// SCSIコマンド処理（未対応）
int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void* buffer, uint16_t bufsize)
{
    INFO_LOG("%s\n", __func__);

    (void) lun; (void) scsi_cmd; (void) buffer; (void) bufsize;
    return 0;   // TODO:
}

} // extern "C"
