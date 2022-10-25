#! /usr/bin/awk -f
function foo(lunch){
  if (lunch ~ "spring"){
    return 1
  }
  return 0
}

BEGIN {
    OFS = "---"
    FS = ","
      } 
      {   
        if (foo($0)){
          print $2, $3, $4 "\n"
        }
        
      }
END {
} 