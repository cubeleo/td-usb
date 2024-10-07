/**
* @file iwt120.c
* @author s-dz, Tokyo Devices, Inc. (tokyodevices.jp)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../td-usb.h"
#include "../tdhid.h"
#include "../tddevice.h"


#define REPORT_SIZE		16


static int write(td_context_t* context)
{
	uint8_t report_buffer[REPORT_SIZE + 1];
	memset(report_buffer, 0, REPORT_SIZE + 1);

	if (context->c != 1)
	{
		throw_exception(context, EXITCODE_INVALID_OPTION, "Only one value can be set.");
	}

	report_buffer[0] = 0x00; // Dummy report id
	report_buffer[1] = 0x31; // Set mode command
	report_buffer[2] = (uint8_t)atoi(context->v[0]); // Register value

	if (TdHidSetReport(context->handle, report_buffer, REPORT_SIZE + 1, USB_HID_REPORT_TYPE_FEATURE))
		throw_exception(context, EXITCODE_DEVICE_IO_ERROR, "USB I/O Error.");

	return 0;
}


static int read(td_context_t* context)
{
	uint8_t report_buffer[REPORT_SIZE + 1];
	memset(report_buffer, 0, REPORT_SIZE + 1);
	if (TdHidGetReport(context->handle, report_buffer, REPORT_SIZE + 1, USB_HID_REPORT_TYPE_FEATURE))
		throw_exception(context, EXITCODE_DEVICE_IO_ERROR, "USB I/O Error.");

	if (context->c == 0 || strcmp(context->v[0], "MODE") == 0)
	{
		printf("%d\n", report_buffer[2]);
	}
	else if (strcmp(context->v[0], "FIRMWARE_VERSION") == 0)
	{
		printf("%d\n", report_buffer[1]);
	}
	else
	{
		throw_exception(context, EXITCODE_INVALID_OPTION, "Unknown option.");
	}

	fflush(stdout);

	return 0;
}


static td_device_t *export_type(void)
{
	td_device_t* dt = (td_device_t *)malloc(sizeof(td_device_t));
	memset(dt, 0, sizeof(td_device_t));

	dt->product_name = (char *)malloc(7);
	strcpy(dt->product_name, "IWT120");

	dt->vendor_id = 0x16c0;
	dt->product_id = 0x05df;
	dt->output_report_size = REPORT_SIZE;
	dt->init = tddev1_init_operation;
	dt->set = write;
	dt->get = read;

	return dt;
}

td_device_t *(*iwt120_import)(void) = export_type;
