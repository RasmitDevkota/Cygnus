#pragma once

#include "UObject/NameTypes.h"

class FAstroFisiks
{
	static float SmallAngleFormula_AngularDiameter(float Diameter, float Distance);
	static float SmallAngleFormula_Diameter(float AngularDiameter, float Distance);
	static float SmallAngleFormula_Distance(float AngularDiameter, float Diameter);
	
	static float WiensLaw_PeakWavelength(float Temperature);
	static float WiensLaw_Temperature(float PeakWavelength);

	static float StefanBoltzmannLaw_Flux(float Temperature);
	
	static float StefanBoltzmannLaw_Luminosity(float Radius, float Temperature);
	static float StefanBoltzmannLaw_Radius(float Luminosity, float Temperature);
	static float StefanBoltzmannLaw_Temperature(float Luminosity, float Radius);

	static float CosmologicalRedshift_Redshift(float ObservedWavelength, float EmittedWavelength);
	static float CosmologicalRedshift_ObservedWavelength(float Redshift, float EmittedWavelength);
	static float CosmologicalRedshift_EmittedWavelength(float Redshift, float ObservedWavelength);

	static float RelativisticRedshift_Redshift(float Velocity);
	static float RelativisticRedshift_Velocity(float Redshift);

	static float DopplerFormula_Velocity(float Redshift);
	static float DopplerFormula_Redshift(float Velocity);

	static float MassEnergyEquivalence_Energy(float Mass);
	static float MassEnergyEquivalence_Mass(float Energy);

	static float GravitationalForce_Force(float MassA, float MassB, float Distance);
	static float GravitationalForce_MassA(float Force, float MassB, float Distance);
	static float GravitationalForce_MassB(float Force, float MassA, float Distance);
	static float GravitationalForce_Distance(float Force, float MassA, float MassB);

	static float HubblesLaw_Velocity(float Distance);
	static float HubblesLaw_Distance(float Velocity);
};
