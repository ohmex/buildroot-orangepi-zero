################################################################################
#
# xr819-xradio
#
################################################################################

XR819_XRADIO_VERSION = 705c792d2d40544b884dab7796934502256a8fdd
XR819_XRADIO_SITE = $(call github,ohmex,xradio,$(XR819_XRADIO_VERSION))
XR819_XRADIO_LICENSE = GPL-2.0
XR819_XRADIO_LICENSE_FILES = LICENSE

$(eval $(kernel-module))
$(eval $(generic-package))
