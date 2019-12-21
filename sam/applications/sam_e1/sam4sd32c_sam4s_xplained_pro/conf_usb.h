#ifndef _CONF_USB_H_
#define _CONF_USB_H_

#include "compiler.h"

#define USB_DEVICE_EP_CTRL_SIZE		8
#define USB_DEVICE_NB_INTERFACE		3
#define USB_DEVICE_MAX_EP		7


#define UDI_CDC_DATA_EP_IN_0		(1 | USB_EP_DIR_IN)
#define UDI_CDC_DATA_EP_OUT_0		(2 | USB_EP_DIR_OUT)
#define UDI_CDC_COMM_EP_0		(3 | USB_EP_DIR_IN)

#define UDI_VENDOR_EP_ISO_IN	(4 | USB_EP_DIR_IN)
#define UDI_VENDOR_EP_ISO_OUT	(5 | USB_EP_DIR_OUT)
//#define UDI_VENDOR_EP_ISO_IN_FB	(6 | USB_EP_DIR_OUT)

#define UDI_COMPOSITE_DESC_T \
	udi_cdc_comm_desc_t	udi_cdc_comm;		\
	udi_cdc_data_desc_t	udi_cdc_data;		\
	udi_vendor_desc_t	udi_vendor;


#define UDI_COMPOSITE_DESC_FS \
	.udi_cdc_comm = UDI_CDC_COMM_DESC_0,		\
	.udi_cdc_data = UDI_CDC_DATA_DESC_0_FS,		\
	.udi_vendor = UDI_VENDOR_DESC_FS,

#define UDI_COMPOSITE_API \
	&udi_api_cdc_comm,				\
	&udi_api_cdc_data,				\
	&udi_api_vendor,



/* USB Device Configuration */

//! Device definition (mandatory)
#define  USB_DEVICE_VENDOR_ID             USB_VID_ATMEL
#define  USB_DEVICE_PRODUCT_ID            USB_PID_ATMEL_ASF_MSC
#define  USB_DEVICE_MAJOR_VERSION         1
#define  USB_DEVICE_MINOR_VERSION         0
#define  USB_DEVICE_POWER                 200 // Consumption on Vbus line (mA)
#define  USB_DEVICE_ATTR                \
	(USB_CONFIG_ATTR_BUS_POWERED)

//! USB Device string definitions (Optional)
#define  USB_DEVICE_MANUFACTURE_NAME      "sysmocom - s.f.m.c. GmbH"
#define  USB_DEVICE_PRODUCT_NAME          "osmo-e1-interface"
#define  USB_DEVICE_SERIAL_NAME           "2342"

/* USB Device Callbacks definitions (Optional) */
#define  UDC_VBUS_EVENT(b_vbus_high)
#define  UDC_SOF_EVENT()
#define  UDC_SUSPEND_EVENT()
#define  UDC_RESUME_EVENT()
//! When a extra string descriptor must be supported
//! other than manufacturer, product and serial string
// #define  UDC_GET_EXTRA_STRING()





/* USB Interface Configuration (CDC) */

#define UDI_CDC_PORT_NB 1
#define UDI_CDC_ENABLE_EXT(port)		main_cdc_enable(port)
#define UDI_CDC_DISABLE_EXT(port)		main_cdc_disable(port)
#define UDI_CDC_RX_NOTIFY(port)			main_cdc_rx_notify(port)
#define UDI_CDC_TX_EMPTY_NOTIFY(port)
#define UDI_CDC_SET_CODING_EXT(port,cfg)
#define UDI_CDC_SET_DTR_EXT(port,set)		main_cdc_set_dtr(port,set)
#define UDI_CDC_SET_RTS_EXT(port,set)

#define UDI_CDC_DEFAULT_RATE		115200
#define UDI_CDC_DEFAULT_STOPBITS	1
#define UDI_CDC_DEFAULT_PARITY		CDC_PAR_NONE
#define UDI_CDC_DEFAULT_DATABITS	8

//#define UDI_CDC_IAD_STRING_ID		4

#define UDI_CDC_COMM_IFACE_NUMBER_0	0
#define UDI_CDC_DATA_IFACE_NUMBER_0	1





/* USB Interface Configuration (Vendor) */

#define UDI_VENDOR_ENABLE_EXT()		usb_vendor_e1_enable()
#define UDI_VENDOR_DISABLE_EXT()	usb_vendor_e1_disable()
#define UDI_VENDOR_SETUP_IN_RECEIVED()	false
#define UDI_VENDOR_SETUP_OUT_RECEIVED()	false


#define UDI_VENDOR_EPS_SIZE_ISO_FS	512
#define UDI_VENDOR_EPS_SIZE_INT_FS	0
#define UDI_VENDOR_EPS_SIZE_BULK_FS	0
#define UDI_VENDOR_IFACE_NUMBER		2


/* USB Device Driver Configuration */


//! The includes of classes and other headers must be done at the end of this file to avoid compile error
#include "udi_cdc.h"
#include "udi_cdc.h"
#include "udi_vendor.h"
#include "main.h"

#endif // _CONF_USB_H_
