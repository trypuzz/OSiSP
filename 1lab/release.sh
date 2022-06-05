
 #!/bin/bash
err="$0.err"
filik="$4.txt"
 #:> $err

if [ $# \< 4 ]
then
        echo "$(basename $0): Too few arguments" >&2 >"$err"
        exit 1
fi

IFS=$'\n'; 

if [ $# -eq 4 ]

if [ -f "$err" ]
	then
	rm "$err"
fi

echo -n "">"$filik"
then

count=0

find $(realpath $1) -type f -size +$2c -size -$3c -printf "%p %s\n" >> "$filik" 2>$err 
wc -l  < "$filik"

sed "s/.[a-zA-Z]*:/`(basename $0)`:/" < $err 1>&2


fi
