#! /usr/bin/awk -f

function goo(line) {
  if (line ~ "fun" || line ~ "fan"){
    return 1
  }
  return 0
}

BEGIN {
    OFS = "---"
    FS = ","
    cnt = 0
      } 
      {   
        if (goo($0)){
          cnt++
        }
        
      }
END {
  print "the number of haikus with fan or fun in them is: " cnt
} 