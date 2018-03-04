A) AI Physics :  A lot of times, the game has an AI car that is driving onscreen, but it drives crazy. You average out the values and feed them to REV with this component.
- Initialize with gear ratios, 
- Update with velocity, 
- Generate a Gear & RPM value to feed to REV.
- You should be able to infer Throttle from the change in the velocity being +/-. Smooth out or use a sticky on/off'latch' if even thats too noisy a signal.
- Example: Average the input throttle over 10 frames. If > 0.5f, send output throttle of 1.0f into REV. Lock that throttle at 1.0f for a minimum of 1 second. Then revaluate.
- This way the AI cars won't sound like they're tapping the throttle like maniacs.

b) Physics Simulator : This is what powers the REV.Tool driving simulator as well as the Wwise driving simulator.
- Initialize with Gear ratios/Car weight/torque, 
- Update with a Throttle/Brake value 
- generates RPM/Throttle/Gear/Velocity to feed to rev.

Example Values:
	Gear Ratios [3.50f, 2.0f, 1.40f, 1.0f, 0.70f]
	Weight = 900.0f;
    EngineTorque = 2500.0f;
	BreakingHorsePower = 6000.0f;

Additionally, there's engine_model_streamdata.csv which is a stream representing an ideal acceleration run of a car. 
This is useful for reference purposes.
