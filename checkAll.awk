BEGIN{
  phase = 1;
  inLine = 0;
  ok = 1;
}

($1 == "XYZ_BREAK_TOKEN_XYZ"){
  if(student[inLine-1] != "Done."){
    print "Unexpected stuff (or missng Done) at end of output: ", student[inLine - 1];
    ok = 0;
  }
  phase = 2;
}

($1 != "XYZ_BREAK_TOKEN_XYZ"){
  if(phase == 1){
    student[inLine++] = $0;
  }
  else{
    thisOK = 0;
    for(ii = 0; ii < inLine; ii++){
      if(student[ii] == $0){
        thisOK = 1;
      }
    }
    if(thisOK == 0){
      print "Missing: ", $0;
      ok = 0;
    }
  }
}

END{
  if(ok == 1){
    print "checkAll-OK\n";
  }
  else{
    print "Appears to be broken.\n";
  }
}
