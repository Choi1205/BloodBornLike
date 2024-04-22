// Fill out your copyright notice in the Description page of Project Settings.


#include "BBLegacyCameraShake.h"

UBBLegacyCameraShake::UBBLegacyCameraShake()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.15f;

 	//RotOscillation.Pitch.Amplitude = 25.f;
 	//RotOscillation.Pitch.Frequency = 10.f;
 	//RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
 	//RotOscillation.Pitch.Waveform = EOscillatorWaveform::SineWave;
 
 	RotOscillation.Yaw.Amplitude = 50.f;
 	RotOscillation.Yaw.Frequency = 10.f;
 	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_MAX;
 	RotOscillation.Yaw.Waveform = EOscillatorWaveform::SineWave;

//	FOVOscillation.Amplitude = -50.0f;
//	FOVOscillation.Frequency = 5.0f;
//	FOVOscillation.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
//	FOVOscillation.Waveform = EOscillatorWaveform::SineWave;

}