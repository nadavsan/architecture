#! /usr/bin/awk -f

BEGIN { FS = ","
      } 
      {   
        if ($1+0 > 49 && $1+0 < 71){
          print "Movie name: " $5
        }
        
      } 