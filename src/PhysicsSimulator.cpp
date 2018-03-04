/*
*
Copyright (c) 2012 CrankcaseAudio Inc
All Rights Reserved.
*
*/
#include "PhysicsSimulator.h"

namespace CrankcaseAudio
{
	

	PhysicsSimulator::PhysicsSimulator()
	{
		Reset();
	}

	void PhysicsSimulator::Initialize(const  VehiclePhysicsControlData & params)
	{
		mVehiclePhysicsControlData = params;

	}
	
	
	void PhysicsSimulator::SmoothInputs(
		PhysicsUpdateParams & params)
	{
		PhysicsUpdateParams tempParams = params;
	
		mUpdateParams = tempParams;
	}

	
	PhysicsOutputParameters PhysicsSimulator::Update(
		PhysicsUpdateParams & params,
		float deltaTime)
	{
		SmoothInputs(params);

		mElapsedTime += deltaTime;
		ApplyForces(deltaTime);
		
		return mPhysicsParams;
	}

	//This is the RPM we'd be at if we downshifted.
	float PhysicsSimulator::GetDownShiftRPM( float velocity, int gear)
	{
		CRANKCASE_ASSERT(gear >= 2);

		float downShiftRPM  = 0.0f;
		
		if(mPhysicsParams.Gear > 1)
			downShiftRPM = velocity * mVehiclePhysicsControlData.GearRatios[gear - 2]/ KEngineGearRatio;

		return downShiftRPM	;
	}

	
	void PhysicsSimulator::Reset()
	{
		mElapsedTime = 0.0f;
		mPhysicsParams = PhysicsOutputParameters();
	
	}

	void PhysicsSimulator::ApplyForces(float deltaTime)
	{
		//Normalized
		float normalThrottle = mUpdateParams.Throttle * (1.0f  - mUpdateParams.Break);
		float normalBreakForce = mUpdateParams.Break;
	
		mPhysicsParams.Throttle = normalThrottle;
	
		//Forces in Newtons.
		float engineTorque =  mVehiclePhysicsControlData.EngineTorque * normalThrottle * mVehiclePhysicsControlData.GearRatios[mPhysicsParams.Gear -1];
		float breakingForce = normalBreakForce * mVehiclePhysicsControlData.BreakingHorsePower;
		float windResistance = kWindResistancePerVelocity * mPhysicsParams.Velocity * mPhysicsParams.Velocity;
		float frictionResistance = kFrictionResistance * mVehiclePhysicsControlData.Weight;
	
		float netForce =  engineTorque - breakingForce - windResistance - frictionResistance;


		//Change in Velocity;	
		float velocity = mPhysicsParams.Velocity + (netForce / mVehiclePhysicsControlData.Weight)*deltaTime;
	
		if(velocity < 0.0f)
			velocity = 0.0f;
	
		//Now working backwords, what is the engine RPM as a results of this velocity and gear ratio.
		float rpm = velocity * mVehiclePhysicsControlData.GearRatios[mPhysicsParams.Gear -1] / KEngineGearRatio;


		//Now determine if we need to shift.
		if(rpm > KUpShiftingRPM && mPhysicsParams.Gear != 5)
		{
			mPhysicsParams.Gear = mPhysicsParams.Gear + 1;
			rpm = velocity * mVehiclePhysicsControlData.GearRatios[mPhysicsParams.Gear -1] / KEngineGearRatio;

		}
		
		
		//If the RPM that we'd be at if we downshift is below KDownShiftRPM, then it makes sense to downshift.
		if(mPhysicsParams.Gear >= 2 && KDownShiftRPM > GetDownShiftRPM(velocity, mPhysicsParams.Gear))
		{
			mPhysicsParams.Gear = mPhysicsParams.Gear - 1;
			rpm = velocity * mVehiclePhysicsControlData.GearRatios[mPhysicsParams.Gear -1] / KEngineGearRatio;
		}

		if(rpm > 1.0f && mPhysicsParams.Gear == 5)
		{
			//Max speed. Do not change anything.		
		}
		else
		{
			mPhysicsParams.Velocity = velocity;
			mPhysicsParams.Rpm = rpm;
		}
	}
}