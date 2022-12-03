#!/bin/bash
echo "student_id,score" > out.csv

if [[ $# -eq 2 ]]; then
    st_num=$2
else
    st_num=5
fi

val_set() {
    if [[ $# -ne 0 ]]; then
        val=$1
    else
        val=100
    fi
}


a() {
    for f in *; do
        file_name="$f.sh"
        (./$f/$file_name > ../Outputs/$f)
    done
}

b() {
    for ((i=1; i<=$st_num; i++)); do
        f=$((1805120+i))
        if [ -f  "$f" ] ; then
            val_set $1
            diff --ignore-all-space "$f" ../AcceptedOutput.txt > temp.txt
            grep '<\|>' temp.txt > temp2.txt
            errors=$(wc -l < temp2.txt)
            error_num=$((errors * 5))

            if [[ $((val-error_num)) -le 0 ]]; then
                val=0
            else
                val=$((val-error_num))
            fi
            (cd .. && cd Submissions && c $f)
        else
            echo "$f,0" >> ../out.csv
        fi
    done
}

c() {
    target="$1.sh"
    
    id=$((1805120+st_num))
    for f in *; do
        if [[ $f != $1 ]] && [[ $f -le $id && $f -ge 1805121 ]]; then
            cd $f
            file_name="$f.sh"
            diff -w -B ../$1/$target $file_name > checker
            if [[ $(wc -l < checker) -eq 0 ]]; then
                val=$((-val))
                rm checker
                cd ..
                break
            fi
            rm checker
            cd ..
        fi
    done
    echo "$1,$val" >> ../out.csv
}


for file in *; do
    if [ "$file" = "Submissions" ]; then
        mkdir Outputs
        (cd "$file" && a)
        break
    fi
done

for file in *; do
    if [ "$file" = "Outputs" ]; then
        (cd "$file" && b $1)
        rm -r Outputs
        break
    fi
done