SDLSUITE_VERSION = 1.0
SDLSUITE_SITE = $(BR2_SRC_DIR)/source/sdlsuite
SDLSUITE_SITE_METHOD = local

SDLSUITE_MAKE_ENV = PKG_CONFIG_SYSROOT_DIR="$(STAGING_DIR)" PKG_CONFIG_PATH="$(STAGING_DIR)/usr/lib/pkgconfig"

define SDLSUITE_CONFIGURE_CMDS
	(cd $(@D); $(TARGET_MAKE_ENV) $(QT5_QMAKE) PREFIX=/usr)
endef

define SDLSUITE_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)
endef

define SDLSUITE_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 -D $(@D)/serialserver $(TARGET_DIR)/usr/bin/sdlsuite
endef

$(eval $(generic-package))
