#include <stdint.h>
#include <stdio.h>

#include "td-usb.h"

void throw_exception(td_context_t *context, int exitcode, const char *msg)
{
	if (msg != NULL) fprintf(stderr, "%s\n", msg);

#ifndef NDEBUG
	if (context != NULL)
	{
		if (context->handle != NULL) {
			TdHidCloseDevice(context->handle);
			context->handle = NULL;
		}

		if (context->device_type != NULL) {
			delete_device_type(context->device_type);
			context->device_type = NULL;
		}
	}

	exit(exitcode);
#endif
}
