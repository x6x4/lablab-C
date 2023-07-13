#!/bin/bash

############################################
#  data preprocessing 
preproc() 
{
    ../timing.bin > ../test 
    rm ord_del #ord_ins #ord_search #ord_ins # #ord_trav
    EPOCHS=10
}

############################################
#  select the lines corresponding to each operation
selection() 
{
    #awk 'NR%1==0' ../test | awk '{print $5}' > search
    #awk 'NR%1==0' ../test | awk '{print $5}' > insertion
    awk 'NR%1==0' ../test | awk '{print $5}' > deletion
    #awk 'NR%4==0' ../test | awk '{print $5}' > traversal
}

############################################
# sort lines by the number of nodes in tests
sort ()
{
    for i in $(seq 1 $((EPOCHS - 1)))
    do 
        #awk 'NR%'$EPOCHS'=='${i} search >> ord_search
        #awk 'NR%'$EPOCHS'=='${i} insertion >> ord_ins
        awk 'NR%'$EPOCHS'=='${i} deletion >> ord_del
    #    awk 'NR%'$EPOCHS'=='${i} traversal >> ord_trav
    done 
        #awk 'NR%'$EPOCHS'==0' search >> ord_search
        #awk 'NR%'$EPOCHS'==0' insertion >> ord_ins
        awk 'NR%'$EPOCHS'==0' deletion >> ord_del
    #    awk 'NR%'$EPOCHS'==0' traversal >> ord_trav
}

############################################
#  calculate medium for each operation
medium()
{
    #medium_search
    #medium_ins
    medium_del
    #medium_trav
}
############################################
medium_search() 
{
    echo search
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_s=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" ord_search | awk '{sum_s += $1} END {x=sum_s/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_ins()
{
    echo ins
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_i=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" ord_ins | awk '{sum_i += $1} END {x=sum_i/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_del()
{
    echo del
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_d=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" ord_del | awk '{sum_d += $1} END {x=sum_d/'$EPOCHS'*100; print x}'
    done
}
############################################
medium_trav()
{
    echo trav
    for i in $(seq 1 $((EPOCHS)))
    do
        sum_t=0
        sed -n "$(($EPOCHS*i-$EPOCHS + 1)), $(($EPOCHS*i))p" ord_trav | awk '{sum_t += $1} END {x=sum_t/'$EPOCHS'; print x}'
    done
}

############################################
main()
{
	preproc
    selection
    sort
    medium
}
############################################
main

    
    

