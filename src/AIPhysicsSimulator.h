//
//  AIPhysicsSimulator.h
//  REVVehicle
//
//  Created by John Twigg on 10/11/15.
//  Copyright (c) 2015 Crankcase Audio Inc
//

#ifndef AIPHYSICSSIMULATOR_CPP
#define AIPHYSICSSIMULATOR_CPP

#include "Configuration.h"
#include "PhysicsSimulator.h"


namespace CrankcaseAudio
{
	struct AIOutputParameters
	{
		AIOutputParameters()
		{
			Gear = 1;
			RPM = 0.0f;
		}
		int Gear;
		float RPM;
	};
	
	struct AIPhysicsUpdateParams
	{
		float Velocity; //KPH [0,200 ish]
	};
	
	//This is a helpful component that generates a reasonable RPM and Gear suitable
	//for rev, given a forward velocity. Its helpful when the physics coming from
	//the game (RPM and Gear) makes assumtions that are different than REV, OR when
	//the physics from the game is unrealistic in many respects, except for Velocity which
	// is normally reliable.
	class AIPhysicsSimulator
	{
	public:
		AIPhysicsSimulator();
		
		void Initialize(const float gearRatios[KMaxNumberOfGear]);
		
		AIOutputParameters Update(
					AIPhysicsUpdateParams & params);
		
		AIOutputParameters GetOuputParams(){return mPhysicsParams;}
		
		
	private:
		
		float GetDownShiftRPM( float velocity, int gear);

		float GearRatios[KMaxNumberOfGear];
		AIPhysicsUpdateParams			mUpdateParams;
		AIOutputParameters				mPhysicsParams;
		
	};
	
}


#endif /* AIPHYSICSSIMULATOR_CPP */
