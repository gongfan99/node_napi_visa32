# node_napi_visa32
An node native API extension for visa32

# Introduction
This is a node native extension for visa32. It implements viOpenDefaultRM, viClose, viOpen, viPrintf, viScanf functions. National Intruments or Keysight VISA library needs to be installed first.

It has been tested on my Win7 local machine.

# Working environment
* node v8.9.1 or up

# To install
npm install

# Usage
```c
const visa32 = require('bindings')('node-napi-visa32');

(async function(){
  try {
    let rscMgr = await visa32.viOpenDefaultRM(),
    rscName = "TCP::192.168.0.10::INSTR",
    mode = 0, // default mode
    timeout = 1000, // 1 second
    vi = await visa32.viOpen(rscMgr, rscName, mode, timeout);

    await visa32.viPrintf(vi, "*IDN?\n");
    let instrDescription = await visa32.viScanf(vi);
    
    // clean up
    await visa32.viClose(vi);
    await visa32.viClose(rscMgr);
  } catch (err){
    console.log(err.message);
  }
})();
```

# To run test:
node test/test.js

# Userful links:
* https://nodejs.org/docs/latest-v8.x/api/n-api.html

* http://zone.ni.com/reference/en-XX/help/370131S-01/

# License:
MIT