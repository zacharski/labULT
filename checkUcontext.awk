BEGIN{
  print("************************************************************");
  print("WARNING: I think these sanity checks should work for everyone");
  print("but they are just heuristics. If you think you are right, but");
  print("the test is not happy, TALK TO THE TA ASAP rather than banging");
  print("your head against the wall.");
  print("************************************************************");
  
  OKCount = 0;
}

($1 == "A" && $2 == "ucontext_t" && $3 == "is" && $5 == "bytes"){
  if($4 > 200 && $4 < 500){
    print("ucontext_t size might be OK");
    OKCount++;
  }
  else{
    printf("ucontext_t size looks suspicious");
  }
}

($1 == "The" && $2 == "memory" && $3 == "address" && $7=="main()"){
  mainaddr = strtonum($9);
  if(mainaddr > 0x8000000 && mainaddr < 0x8100000){
    print("main addr might be OK");
    OKCount++;
  }
  else{
    print("main addr looks suspicious");
  }
}

($1 == "The" && $2 == "memory" && $3 == "address" && $7=="counter"){
  eipaddr = strtonum($13);
  if(eipaddr > 0x8000000 && eipaddr < 0x8100000  && eipaddr > mainaddr && eipaddr < mainaddr + 2000){
    print("eip addr might be OK");
    OKCount++;
  }
  else{
    print("eip addr looks suspicious");
  }
}


($1 == "The" && $2 == "memory" && $3 == "address" && $6=="local" && $7 == "variable"){
  erraddr = strtonum($10);
  if(erraddr > 0xb0000000 && erraddr < 0xc0000000){
    print("err addr might be OK");
    OKCount++;
  }
  else{
    print("err addr looks suspicious");
  }
}


($1 == "The" && $2 == "memory" && $3 == "address" && $6=="argument" && $7 == "argc"){
  argcaddr = strtonum($9);
  if(argcaddr > 0xb0000000 && argcaddr < 0xc0000000 && argcaddr > erraddr && argcaddr < erraddr + 1000){
    print("argc addr might be OK");
    OKCount++;
  }
  else{
    print("argc addr looks suspicious");
  }
}


($1 == "The" && $2 == "number" && $3 == "of" && $10=="argc" && $12 == "err"){
  argcBytesErr = strtonum($14);
  if(argcBytesErr > 4 && argcBytesErr < 0x30){
    print("argc-to-err bytes might be OK");
    OKCount++;
  }
  else{
    print("argc-to-er bytes looks suspicious");
  }
}



($1 == "The" && $2 == "number" && $3 == "of" && $10=="err" && $18 == "mycontext"){
  errBytesMyContext = strtonum($20);
  if(errBytesMyContext > 4 && errBytesMyContext < 0x500){
    print("err-to-save bytes might be OK");
    OKCount++;
  }
  else{
    print("err-to-save bytes looks suspicious " errBytesMyContext);
  }
}


END{
  print("Might-be-OK-count: " OKCount);
}