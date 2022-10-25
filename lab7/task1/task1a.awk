#! /usr/bin/awk -f
BEGIN {
    i = 0
    while (i<ARGC){
        print "ARGV[" i "]=" ARGV[i]
        i++
    }
}

