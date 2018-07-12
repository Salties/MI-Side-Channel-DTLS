#/bin/bash -x

TARGET=$1
AUXFILES="fdb_latexmk fls dvi ps"

latexmk ${TARGET} -shell-escape
for EXT in $AUXFILES
do
	rm -f ${TARGET}.${EXT}
done

eog ${TARGET}.png
