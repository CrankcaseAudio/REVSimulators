/*
*
Copyright (c) 2012 CrankcaseAudio Inc
All Rights Reserved.
*
*/
#ifndef PHYSICSIMULATOR_HPP
#define PHYSICSIMULATOR_HPP

#include "Configuration.h"


namespace CrankcaseAudio
{

	const int	KMaxNumberOfGear = 5;
	const float KEngineGearRatio = 50.0f;
	const float kWindResistancePerVelocity = 600.0f/40000.000f;
	const float kFrictionResistance = 1.0f;;
	const float KUpShiftingRPM = 0.97f;
	const float KDownShiftRPM = 0.94f;
	const float KThrottleInterpolationTime = 0.050f;

	struct VehiclePhysicsControlData
	{
		VehiclePhysicsControlData()
		{
			 Weight = 900.0f;
			 EngineTorque = 2500.0f;
			 BreakingHorsePower = 6000.0f;
			 GearRatios[0] = 3.50f;
			 GearRatios[1] = 2.0f;
			 GearRatios[2] = 1.40f;
			 GearRatios[3] = 1.0f;
			 GearRatios[4] = 0.70f;

		}

		void SetGearRatio(int i, float value){CRANKCASE_ASSERT(i >=0 && i < KMaxNumberOfGear); GearRatios[i] = value;}
		float  GetGearRatio(int i ) {CRANKCASE_ASSERT(i >=0 && i < KMaxNumberOfGear); return GearRatios[i];}

		uint16_t EndianStatus;
		uint16_t SizeOf; //safety check to ensure out filesizes stay consistant accross compilers.


		float Weight;
		float EngineTorque;
		float BreakingHorsePower;
		float GearRatios[KMaxNumberOfGear];	

		int GetDataSize(){return sizeof(VehiclePhysicsControlData);}
	};


	class  PhysicsUpdateParams
	{
		public:
		PhysicsUpdateParams()
		{
			Throttle = 0.0f;
			Break = 0.0f;
		}

		float Throttle;
		float Break;

	};

	
	class  PhysicsOutputParameters
	{
		public:

		PhysicsOutputParameters()
		{
			Rpm =0.0f;
			Throttle = 0.0f;
			Gear =1 ;
			Velocity = 0.0f; 
		}

		float Rpm; //[0,1]
		float Throttle; //[0,1]
		int   Gear;//[1,6]
		float Velocity;//[1, ~150]
		
	};

	//This class is meant to emulate the likely data we would receive from the Physics engine
	//of a vehicle simulator. By feeding in the users control of Throttle and Break, we can
	//read the resulting RPM and Gears of a given car.
	//This simulator Shifts INSTANTLY like so many physics simulators.
	//Ideally the output data from the 
	class PhysicsSimulator
	{
	public:

		PhysicsSimulator();

		void Initialize(const  VehiclePhysicsControlData & params);


		//Delta time in seconds. (ie 0.03s per frame);
		PhysicsOutputParameters Update(
			PhysicsUpdateParams & params,
			float deltaTime);


		VehiclePhysicsControlData * GetControlDataPtr()				{	return & mVehiclePhysicsControlData;	}
		void SetControlData(const VehiclePhysicsControlData * pData)	{	mVehiclePhysicsControlData = *pData;	}

		void Reset();

	private:

		PhysicsUpdateParams				mUpdateParams;
		VehiclePhysicsControlData	    mVehiclePhysicsControlData;
		float mElapsedTime;
		

		void ApplyForces(float deltaTime);

		void SmoothInputs(PhysicsUpdateParams & params);
		
		PhysicsOutputParameters mPhysicsParams;

		float GetDownShiftRPM(float velocity, int gear);

	};

}
#endif
