#! /usr/bin/awk -f
function foo(line){
        if (line+0 > 86)
            return 1
        return 0
    }
BEGIN {
    }
    {
        if (foo($4)){
            print $0
        }
        
    }
END{
}