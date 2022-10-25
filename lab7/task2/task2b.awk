#! /usr/bin/awk -f
function foo(lunch){
  if (lunch ~ "bachelor"){
    return 1
  }
  return 0
}

BEGIN { FS = ","
        cnt=0
      } 
      {   
        if (foo($3)){
          cnt++
        }
        
      }
END {
  print "number of students whose parents have bachelor degree = " cnt
} 