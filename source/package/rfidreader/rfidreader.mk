RFIDREADER_VERSION = 1.0
RFIDREADER_SITE = $(BR2_SRC_DIR)/source/rfidreader
RFIDREADER_SITE_METHOD = local

RFIDREADER_MAKE_ENV = PKG_CONFIG_SYSROOT_DIR="$(STAGING_DIR)" PKG_CONFIG_PATH="$(STAGING_DIR)/usr/lib/pkgconfig"

define RFIDREADER_BUILD_CMDS
    $(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(@D)
endef

define RFIDREADER_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 -D $(@D)/rfidreader $(TARGET_DIR)/usr/bin/rfidreader
endef

$(eval $(generic-package))
