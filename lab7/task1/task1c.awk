#! /usr/bin/awk -f

BEGIN { FS = ","
      } 
      {   
        if ($2+0 >1969){
          print "Actor name: " $4 "\nMovie name: " $5
        }
        
      } 