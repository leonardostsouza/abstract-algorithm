//#!/usr/bin/env node

var fs = require("fs");
var path = require("path");
var L = require("./lambda-calculus.js");

try {
  var args = [].slice.call(process.argv, 2);
  var stats = args.indexOf("-s") !== -1 || args.indexOf("--stats") !== -1;
  var bruijn = args.indexOf("-b") !== -1 || args.indexOf("--bruijn") !== -1;
  var file = args[args.length - 1];
  var base = fs.readFileSync(path.join(__dirname, "..", "lib", "base.lam"), "utf8");
  var code = fs.readFileSync("./" + (file.indexOf(".") === -1 ? file + ".lam" : file), "utf8");
} catch (e) {
  console.log("Absal evaluates λ-terms optimally (no oracle).");
  console.log("Usage:");
  console.log("  absal [--stats] [--bruijn] fileName[.lam]");
  console.log("Syntax:");
  console.log("  #arg body      : lambda expression");
  console.log("  /fn arg        : applies fn to arg");
  console.log("  @name val expr : let name be val in expr");
  console.log("Example:");
  console.log("  @four #f #x /f /f /f /f x");
  console.log("  /four four");
  console.log("Stats:");
  console.log("  - loops: how many times the main loop was executed");
  console.log("  - rewrites : total graph rewrites (dupls + annis)");
  console.log("    - dupls  : different color rewrites (duplications)");
  console.log("    - annis  : equal color rewrites (annihilations)");
  console.log("    - betas  : annis with color=0 (lam/app nodes)");
  process.exit();
}

/*const reduce_ = (src) => {
    const lambda = L.fromString(src);
    const origNet = L.toNet(lambda);
    const reducNet = L.net.reduce(origNet);

    const netToStr = net => JSON.stringify(net.mem);

    return {origNet: netToStr(origNet), reducNet: netToStr(reducNet), ptr: origNet.ptr, stats: reducNet.stats};
}
*/
const statsStr = (stats) => {
    return `${stats.loops},${stats.rewrites},${stats.dupls},${stats.annis},${stats.betas}`;
}

const toCsv = (netStr, ptr) => {
    netStr = netStr.replace("[", "");
    netStr = netStr.replace("]", "");
    netStr = ptr + "," + netStr;
    return netStr;
}

var start = Date.now();
//var result = reduce_(`${base} ${code}`);
var result = L.reduce(`${base} ${code}`, 1, bruijn);
var execTime = ((Date.now() - start) / 1000);

console.log(result.ptr);
// Print results
//console.log("Original Network: \n");
fs.writeFile(`origNet.csv`, toCsv(result.origNet, result.ptr), function(err){if(err) return console.log(err);});
console.log("ORIGINAL: " + result.origNet + "\n\n");
//console.log("\n=========================================\n");
//console.log("Reduced Network: \n")
fs.writeFile(`reducNet.csv`, toCsv(result.reducNet, result.ptr), function(err){if(err) return console.log(err);});
console.log("REDUCED: " + result.reducNet + "\n\n");
fs.writeFile(`stats.mem`, statsStr(result.stats), function(err){if(err) return console.log(err);});
if (stats) {
  console.log("");
  console.log("- time     : " + (execTime + "s"));
  console.log("- loops    : " + result.stats.loops);
  console.log("- rewrites : " + result.stats.rewrites);
  console.log("  - dupls  : " + result.stats.dupls);
  console.log("  - annis  : " + result.stats.annis);
  console.log("  - betas  : " + result.stats.betas);
}
