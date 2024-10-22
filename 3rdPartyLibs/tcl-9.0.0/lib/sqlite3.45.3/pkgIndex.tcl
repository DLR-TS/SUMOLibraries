if {[package vsatisfies [package provide Tcl] 9.0-]} { 
package ifneeded sqlite3 3.45.3 [list load [file join $dir tcl9sqlite3453.dll] [string totitle sqlite3]] 
} else { 
package ifneeded sqlite3 3.45.3 [list load [file join $dir sqlite3453.dll] [string totitle sqlite3]] 
} 
