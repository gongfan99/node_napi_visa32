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