################################################################################
#
# Build the squashfs root filesystem image
#
################################################################################

ROOTFS_SQUASHFS_DEPENDENCIES = host-squashfs

ROOTFS_SQUASHFS_ARGS = -noappend -processors $(PARALLEL_JOBS)

ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_LZ4),y)
ROOTFS_SQUASHFS_ARGS += -comp lz4 -Xhc
else ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_LZO),y)
ROOTFS_SQUASHFS_ARGS += -comp lzo
else ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_LZMA),y)
ROOTFS_SQUASHFS_ARGS += -comp lzma
else ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_XZ),y)
ROOTFS_SQUASHFS_ARGS += -comp xz
else ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_XPRESS),y)
ROOTFS_SQUASHFS_ARGS += -b 512k -comp xz -Xdict-size 512k -Xbcj arm
else ifeq ($(BR2_TARGET_ROOTFS_SQUASHFS4_ZSTD),y)
ROOTFS_SQUASHFS_ARGS += -comp zstd
else
ROOTFS_SQUASHFS_ARGS += -comp gzip
endif

define ROOTFS_SQUASHFS_CMD
    #echo "TARGET_DIR="$(TARGET_DIR)
    #echo "AT="$@
    #echo "ROOTFS_SQUASHFS_ARGS="$(ROOTFS_SQUASHFS_ARGS)
	$(HOST_DIR)/bin/mksquashfs $(TARGET_DIR) $@ $(ROOTFS_SQUASHFS_ARGS)
    $(HOST_DIR)/bin/mkimage -A arm -O linux -T ramdisk -C none -a 0x43300000 -e 0x43300000 -n uInitrd -d $@ $(TOPDIR)/output/images/uInitrd.sfs
endef

$(eval $(rootfs))


# mkimage -A arm -O linux -T ramdisk -C none -a 0x43300000 -e 0x43300000 -n uInitrd -d Initrd ./uInitrd.sfs
# mkimage [-x] -A arch -O os -T type -C comp -a addr -e ep -n name -d data_file[:data_file...] image
#          -A ==> set architecture to 'arch'
#          -O ==> set operating system to 'os'
#          -T ==> set image type to 'type'
#          -C ==> set compression type 'comp'
#          -a ==> set load address to 'addr' (hex)
#          -e ==> set entry point to 'ep' (hex)
#          -n ==> set image name to 'name'
#          -d ==> use image data from 'datafile'


