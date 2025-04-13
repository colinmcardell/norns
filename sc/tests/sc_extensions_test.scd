// SuperCollider Extensions Test File
// This file demonstrates how to test the Norns SC extensions

// First, make sure the Norns extensions are loaded
(
// Check if Norns class exists
if(\Norns.asClass.isNil) {
    "ERROR: Norns class not found. Make sure the extensions are properly installed.".postln;
    "You may need to run 'make install_sc_extensions' or restart SuperCollider.".postln;
} {
    "Norns extensions found and loaded.".postln;
}
)

// Test basic Crone functionality
(
// Check if Crone class exists
if(\Crone.asClass.isNil) {
    "ERROR: Crone class not found.".postln;
} {
    "Crone class found.".postln;
    
    // Print some information about the Crone class
    "Crone class methods:".postln;
    \Crone.asClass.methods.collect(_.name).sort.postln;
    
    "Crone class variables:".postln;
    \Crone.asClass.classVarNames.sort.postln;
}
)

// Test CroneEngine functionality
(
// Check if CroneEngine class exists
if(\CroneEngine.asClass.isNil) {
    "ERROR: CroneEngine class not found.".postln;
} {
    "CroneEngine class found.".postln;
    
    // List all available engines
    "Available engines:".postln;
    CroneEngine.allSubclasses.select { |class|
        class.name.asString.beginsWith("Engine_");
    }.collect { |class|
        class.name.asString.split($_).drop(1).join($_);
    }.sort.postln;
}
)

// Test a specific engine (Engine_TestSine)
(
// Check if Engine_TestSine class exists
if(\Engine_TestSine.asClass.isNil) {
    "ERROR: Engine_TestSine class not found.".postln;
} {
    "Engine_TestSine class found.".postln;
    
    // Print information about the Engine_TestSine class
    "Engine_TestSine class methods:".postln;
    \Engine_TestSine.asClass.methods.collect(_.name).sort.postln;
}
)

// Test utility classes
(
// Check if BufUtil class exists
if(\BufUtil.asClass.isNil) {
    "ERROR: BufUtil class not found.".postln;
} {
    "BufUtil class found.".postln;
    
    // Print information about the BufUtil class
    "BufUtil class methods:".postln;
    \BufUtil.asClass.methods.collect(_.name).sort.postln;
}
)

// Print a summary of the test results
(
"Test Summary:".postln;
"------------".postln;
[\Norns, \Crone, \CroneEngine, \Engine_TestSine, \BufUtil].do { |className|
    if(className.asClass.isNil) {
        "FAIL: % class not found.".format(className).postln;
    } {
        "PASS: % class found and loaded.".format(className).postln;
    }
};
)

// Note: To run a more comprehensive test that actually initializes Crone and tests audio,
// you would need to have the full Norns system running, as Crone depends on OSC communication
// with the C++ components of the system.
