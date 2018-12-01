#!/usr/bin/env bash

#Generate base sd card image
#$(pwd)/support/scripts/genimage.sh "$@"

export current=$PWD

builder_path=$current/source/builder
image_path=$current/output/images
fex_path=$current/source/fex
fex_file=ZeroPi.fex
dtb_file=script.bin

kernel_image_path=$image_path

addr=0x40008000
mkimage=mkimage
comps="gzip lzo lzma none"
image="Image"
tmp="/tmp"

## START == SCRIPT BUILD SEGMENT ##

if [ ! -f $image_path/script.bin ]; then
    echo "Creating $image_path/$dtb_file file"
    fex2bin $fex_path/$fex_file > $image_path/$dtb_file
fi

## START == KERNEL BUILD SEGMENT ##

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
	uout="$image_path/u$image.$c"

	$mkimage -A arm -O linux -C $c \
		-T kernel -a $addr -e $addr \
		-n 'Linux-3.4.113+' -d $tmp/$image.$c $uout 1>/dev/null 2>/dev/null 

	ksize=$size
	fsize $uout

    which file 1>/dev/null && file $uout
    echo "$c => $uout $ksize bytes + 64 = $size"
}

done

## START == IMAGE BUILD SEGMENT ##

echo "Building images..."
cd $builder_path/
sudo img_size=16M MMC=1 ./builder zeropi.conf.fast
sudo img_size=16M ./builder zeropi.conf.fast
cd -
echo "---------------------------"
echo "To check MMC image use below"
echo "sudo dd count=1 skip=1 < $image_path/ZeroPi-16M.8192-mmc.bin 2>/dev/null | head -n 3"
echo "To check image hybrid image use below"
echo "sudo dd count=1 < $image_path/ZeroPi-16M.bin 2>/dev/null | grep -q -o eGON.BT0 && echo \"OK its hybryd image\""
echo "---------------------------"
echo "To burn MMC image use below"
echo "sudo dd of=/dev/sde < $image_path/ZeroPi-16M.8192-mmc.bin"
echo "To burn hybrid image to SDCARD"
echo "sudo dd seek=16 bs=512 of=/dev/sde < $image_path/ZeroPi-16M.bin"
echo "---------------------------"
echo "DONE"
## END == IMAGE BUILD SEGMENT ##

