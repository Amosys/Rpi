#!/bin/bash

function re(){
    echo "enter into dir $1"
    cd $1
    for file1 in $(ls ./)
    do
        if [ -d $file1 ]
        then
            re $file1
        else
            mv $file1 ${file1%".bin"}
            echo "create ${file1%".bin"}"
        fi
    done
    cd ..
    echo "exit dir $1"
}

function copy(){
    echo "enter into dir $1"
    cd $1
    for file1 in $(ls ./)
    do
        if [ -d $file1 ]
        then
            copy $file1
        else
            cat $file1 > "$file1.bin"
			rm $file1
            echo "create $file1.bin"
        fi
    done
    cd ..
    echo "exit dir $1"
}

if [ $# -ne 1 ]
then
	echo "plz input one arg"
else
	if [ $1 = "-h" ]
	then
		echo "help:-c copy, -r restore"
	elif [ $1 = "-c" ]
	then 
		copy "./"
	elif [ $1 = "-r" ]
	then
		re "./"
	fi
fi
