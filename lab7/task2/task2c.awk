#! /usr/bin/awk -f
function foo(x){
  if (x+0>79){
    return 1
  }
  return 0
}

BEGIN { FS = ","
        cnt=0
        print "Success Student List"
        print "-------"
      } 
      {   
        if (foo($6) && foo($7) && foo($8)){
          print $0
          cnt++
        }
        
      }
END {

  print "-------"
  print "\n\nThe number of students: " cnt "\n"
} 