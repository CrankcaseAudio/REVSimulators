//
//  AIPhysicsSimulator.cpp
//  REVVehicle
//
//  Created by John Twigg on 10/11/15.
//  Copyright (c) 2015 EpicGames. All rights reserved.
//

#include "AIPhysicsSimulator.h"

namespace CrankcaseAudio
{
	AIPhysicsSimulator::AIPhysicsSimulator()
	{	
	}
	
	void AIPhysicsSimulator::Initialize(const float gearRatios[KMaxNumberOfGear])
	{
		for(int i=0; i < KMaxNumberOfGear; i++)
		{
			GearRatios[i] = gearRatios[i];
		}
	
		
	}
	
	AIOutputParameters AIPhysicsSimulator::Update(CrankcaseAudio::AIPhysicsUpdateParams &params)
	{
		if(params.Velocity < 0.0f)
			params.Velocity = 0.0f;
		//Now working backwords, what is the engine RPM as a results of this velocity and gear ratio.
		float rpm = params.Velocity * GearRatios[mPhysicsParams.Gear -1] / KEngineGearRatio;
		
		
		//Now determine if we need to shift.
		if(rpm > KUpShiftingRPM && mPhysicsParams.Gear != 5)
		{
			mPhysicsParams.Gear = mPhysicsParams.Gear + 1;
			rpm = params.Velocity * GearRatios[mPhysicsParams.Gear -1] / KEngineGearRatio;
			
		}
		//If the RPM that we'd be at if we downshift is below KDownShiftRPM, then it makes sense to downshift.
		if(mPhysicsParams.Gear >= 2 && KDownShiftRPM > GetDownShiftRPM(params.Velocity, mPhysicsParams.Gear))
		{
			mPhysicsParams.Gear = mPhysicsParams.Gear - 1;
			rpm = params.Velocity * GearRatios[mPhysicsParams.Gear -1] / KEngineGearRatio;
		}
		
		if(rpm > 1.0f && mPhysicsParams.Gear == 5)
		{
			//Max speed. Do not change anything.
			mPhysicsParams.RPM = 1.0f;
		}
		else
		{
			mPhysicsParams.RPM = rpm;
		}
		

		
		return mPhysicsParams;
	}
	
	//This is the RPM we'd be at if we downshifted.
	float AIPhysicsSimulator::GetDownShiftRPM( float velocity, int gear)
	{
		CRANKCASE_ASSERT(gear >= 2);
		
		float downShiftRPM  = 0.0f;
		
		if(mPhysicsParams.Gear > 1)
			downShiftRPM = velocity * GearRatios[gear - 2]/ KEngineGearRatio;
		
		return downShiftRPM	;
	}

							   
}
