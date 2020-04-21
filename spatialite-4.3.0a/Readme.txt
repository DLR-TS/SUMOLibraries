This folder contains spatialite an extension to sqlite similar to PostGIS which can be used either standalone or from python.
To use it from python you need to add this directory to your PATH and either remove or rename the existing DLLs\sqlite3.dll in your python installation.
In your python code you need to activate the extension like that:
	conn = sqlite3.connect(":memory:")
	conn.enable_load_extension(True)
	conn.execute("SELECT load_extension('mod_spatialite')")
