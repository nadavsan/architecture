#! /usr/bin/awk -f
function foo(lunch){
  if (lunch ~ "standard"){
    return 1
  }
  return 0
}

BEGIN { FS = ","
        stndrdLunchCnt = 0
        mathSum = 0
        readSum = 0
        writeSum = 0
      } 
      {   
        if (foo($4)){
          stndrdLunchCnt++
          mathSum = mathSum + ($6+0)
          readSum = readSum + ($7+0)
          writeSum = writeSum + ($8+0)
        }

        
      }
END {
  mathSum = (mathSum/stndrdLunchCnt)
  readSum = (readSum/stndrdLunchCnt)
  writeSum = (writeSum/stndrdLunchCnt)
  print "math avg=" mathSum ", read avg=" readSum ", write avg=" writeSum
} 