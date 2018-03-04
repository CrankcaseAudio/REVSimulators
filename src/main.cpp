//
//  Main.cpp
//  REVSimulators
//
//  Created by John Twigg on 2/11/2017
//  Copyright (c) 2017 Crankcase Audio Inc
//

#include "AIPhysicsSimulator.h"
#include <cstdio>


void RunAISimulation()
{
	static const float gearRatios[] = { 3.50f, 2.0f, 1.40f, 1.0f, 0.70f };

	CrankcaseAudio::AIPhysicsSimulator aiSimulator;
	aiSimulator.Initialize(gearRatios);

	float elapsedTime = 0.0f;
	const float deltaTime = 0.015f;
	const float MagicNumber = 7.0f; //Control how fast/slow the car feels like its accelerating.

	CrankcaseAudio::AIPhysicsUpdateParams params;
	params.Velocity = 0.0f;
	for (; elapsedTime < 15.0f; elapsedTime += deltaTime)
	{
		params.Velocity += MagicNumber * deltaTime;
		aiSimulator.Update(params);
		CrankcaseAudio::AIOutputParameters outputParams = aiSimulator.GetOuputParams();
		printf("T:%0.2f : vel %0.2f  -> Gear:%i  RPM:%0.2f\n", elapsedTime, params.Velocity, outputParams.Gear, outputParams.RPM);
	}
}

void RunPhysicsSimulator()
{
	static const float gearRatios[] = { 3.50f, 2.0f, 1.40f, 1.0f, 0.70f };

	CrankcaseAudio::PhysicsSimulator simulator;

	CrankcaseAudio::VehiclePhysicsControlData controlParams;
	controlParams.BreakingHorsePower = 6000.0f;
	controlParams.EngineTorque = 2500.0f;
	controlParams.Weight = 900.0f;
	controlParams.GearRatios[0] = gearRatios[0];
	controlParams.GearRatios[1] = gearRatios[1];
	controlParams.GearRatios[2] = gearRatios[2];
	controlParams.GearRatios[3] = gearRatios[3];
	controlParams.GearRatios[4] = gearRatios[4];

	simulator.Initialize(controlParams);

	float elapsedTime = 0.0f;
	const float deltaTime = 0.015f;

	CrankcaseAudio::PhysicsUpdateParams updateParams;

	updateParams.Throttle = 1.0f;
	updateParams.Break= 0.0f;

	for (; elapsedTime < 10.0f; elapsedTime += deltaTime)
	{
		CrankcaseAudio::PhysicsOutputParameters outputParams = simulator.Update(updateParams,deltaTime);
		printf("T:%0.2f : Throttle:%0.2f, Break:%0.2f -> Vel:%0.2f Throttle:%0.2f Gear:%i RPM:%0.2f\n", elapsedTime, updateParams.Throttle, updateParams.Break, outputParams.Velocity, outputParams.Throttle, outputParams.Gear, outputParams.Rpm);
	}

	updateParams.Throttle = 0.0f;
	updateParams.Break = 0.0f;

	for (; elapsedTime < 20; elapsedTime += deltaTime)
	{
		CrankcaseAudio::PhysicsOutputParameters outputParams = simulator.Update(updateParams, deltaTime);
		printf("T:%0.2f : Throttle:%0.2f, Break:%0.2f -> Vel:%0.2f Throttle:%0.2f Gear:%i RPM:%0.2f\n", elapsedTime, updateParams.Throttle, updateParams.Break, outputParams.Velocity, outputParams.Throttle, outputParams.Gear, outputParams.Rpm);
	}

	updateParams.Throttle = 0.0f;
	updateParams.Break = 1.0f;

	for (; elapsedTime < 25; elapsedTime += deltaTime)
	{
		CrankcaseAudio::PhysicsOutputParameters outputParams = simulator.Update(updateParams, deltaTime);
		printf("T:%0.2f : Throttle:%0.2f, Break:%0.2f -> Vel:%0.2f Throttle:%0.2f Gear:%i RPM:%0.2f\n", elapsedTime, updateParams.Throttle, updateParams.Break, outputParams.Velocity, outputParams.Throttle, outputParams.Gear, outputParams.Rpm);
	}
}

int main()
{
	//RunAISimulation();
	RunPhysicsSimulator();


	
	
}
