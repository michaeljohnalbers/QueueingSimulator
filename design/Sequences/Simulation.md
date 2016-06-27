# Simulation Sequence #
Describes the steps needed to run a simulation. See [Startup Sequence](Startup.md) for steps to start the program itself.

* Precondition: User has already selected config file and (somehow) passed it to the engine.
* On any error the process is aborted and notification provided to the user.

1. Simulation Loader
2. Plugin Loader
3. Simulation Initialization
4. Simulation
   1. Set appropriate OpenGL display/idle callbacks (batch v. real-time)
5. Report Generation


