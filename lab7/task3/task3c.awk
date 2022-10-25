#! /usr/bin/awk -f
function foo(lunch){
  if (lunch ~ "happy"){
    return 1
  }
  return 0
}

BEGIN {
    OFS = "---"
    FS = ","
      } 
      {   
        if (foo($2)){
          print $2, $3, $4 "\n"
        }
        
      }
END {
} 