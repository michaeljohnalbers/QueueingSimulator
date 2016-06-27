# Startup Sequence #
Describes the high level startup sequence for QS.
1. User starts program
2. Initialize GTK
3. Initialize OpenGL
4. Initialize Engine
   1. Parse custom command line args/options
5. Create GTK control GUI (separate thread)
6. Create OpenGL visualization window
