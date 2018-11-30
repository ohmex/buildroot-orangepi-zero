#!/usr/bin/env bash

$(pwd)/support/scripts/genimage.sh "$@"

## START == UBOOT BUILD SEGMENT ##



## END == UBOOT BUILD SEGMENT ##

## START == KERNEL BUILD SEGMENT ##

export current=$PWD
echo "Current build directory = $current"

tools=$current/sachin/tools
spibin=$current/sachin/spibin

kernel_version=4.17.15
kernel_build_dir=$current/output/build/linux-$kernel_version
kernel_image_path=$kernel_build_dir/arch/arm/boot
kernel_image=$kernel_image_path/Image
kernel_zimage=$kernel_image_path/zImage

addr=0x40008000
mkimage=mkimage
comps="gzip lzo lzma none"
image="Image"
tmp="/tmp"

fsize() {
    size=-1
    size=`stat --printf %s $1`
}

for c in $comps ; do

    out="$tmp/$image.$c"

    [ "" ] || {
    case $c in
    gzip)
	[ "$have" ] || $c -7 -c $kernel_image_path/$image > $out
    ;;
    lzma)
	[ "$have" ] || {
	$c -9 -c $kernel_image_path/$image > $out
    }
    ;;
    lzo)
	[ "$have" ] || {
	which lzop >/dev/null 2>&1 || {
	    echo "[e] lzop not found, try to install it or just fix it">&2
	    exit 1
	}
	lzop -9 -c $kernel_image_path/$image > $out
    }
    ;;
    lz4)
	echo "$c ignored"
	## not working 
    ;;
    none)
	[ "$have" ] || cp $kernel_image_path/$image $out
    ;;
    *)
	echo "[i] undef $c compress type ignored"
    ;;
    esac
    }


[ -f $out ] && {
	fsize $out
	uout="$spibin/u$image.$c"

	$mkimage -A arm -O linux -C $c \
		-T kernel -a $addr -e $addr \
		-n 'Linux-4.17.15' -d $tmp/$image.$c $uout 1>/dev/null 2>/dev/null 

	ksize=$size
	fsize $uout

    which file 1>/dev/null && file $uout
    echo "$c => $uout $ksize bytes + 64 = $size"
}

done

echo " "
echo "You can program MMC hybrid using command"
echo "sudo img_size=16M MMC=1 ./builder sachin.conf.fast"
echo "sudo dd of=/dev/sde < /home/sachin/build/spiboot/buildroot/sachin/spibin/mmc_image.bin"

## END == KERNEL BUILD SEGMENT ##
