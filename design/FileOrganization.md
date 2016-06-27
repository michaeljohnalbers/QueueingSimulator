# File Organization #
Ideas for how to organize the source code.

QueueingSimulator  
  |  
  -- Visualization  (OpenGL)  
  |   |  
  |   -- src  
  |   -- inc  
  |   -- obj  
  |   -- test  
  |      |  
  |      -- src  
  |  
  -- Control (GTK)  
  |  
  -- Engine (Simulation core)  
  |  
  -- Plugins (plugin data, including base Actor/Object/Behavior classes)  
  |  
  -- Main  
  |  
  -- doc (contains generated documenation)  
  |  
  -- build (convenience directory for CMake)  
  |  
  -- CmakeLists.txt  

Everything except main will be built into a library, linked into executable. Main will have simple main.cpp.

