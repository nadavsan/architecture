#! /usr/bin/awk -f
function foo(line){
        if (line ~ "English")
            return 1
        return 0
    }
BEGIN {
    }
    {
        total = total + foo($0)
    }
END{
    print "Count=" total
}