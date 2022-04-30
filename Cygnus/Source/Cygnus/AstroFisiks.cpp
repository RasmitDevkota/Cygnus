#include "AstroFisiks.h"

static float pi = 3.1415926535897932384626433832795f;
static float c = 3e5f;
static float G = 6.67e-13f;
static float h = 6.628e-34;

static float kSB = 5.67e-5f;

static float Me = 5.97e24f;
static float Re = 6378.0f;

static float Ms = 1.99e30f;
static float Rs = 6.96e5f;
static float Ts = 5778.0f;

static float AU = 1.5e8f;
static float ly = 9.46e12f;
static float pc = 3.09e12f;
static float arcmin = 1.0f/60.0f;
static float arcsec = 1.0f/3600.0f;

static float H0 = 70.0f;

float FAstroFisiks::SmallAngleFormula_AngularDiameter(float Diameter, float Distance)
{
	return 206265.0f * Diameter / Distance;
}

float FAstroFisiks::SmallAngleFormula_Diameter(float AngularDiameter, float Distance)
{
	return AngularDiameter * Distance / 206265.0f;
}

float FAstroFisiks::SmallAngleFormula_Distance(float AngularDiameter, float Diameter)
{
	return 206265.0f * Diameter / AngularDiameter;
}

float FAstroFisiks::WiensLaw_PeakWavelength(float Temperature)
{
	return 2.9e6f / Temperature;
}

float FAstroFisiks::WiensLaw_Temperature(float PeakWavelength)
{
	return 2.9e6f / PeakWavelength;
}

float FAstroFisiks::StefanBoltzmannLaw_Flux(float Temperature)
{
	return kSB * pow(Temperature, 4);
}
	
float FAstroFisiks::StefanBoltzmannLaw_Luminosity(float Radius, float Temperature)
{
	return 4 * pi * kSB * pow(Radius, 2) * pow(Temperature, 4);
}

float FAstroFisiks::StefanBoltzmannLaw_Radius(float Luminosity, float Temperature)
{
	return pow(Luminosity / (4 * pi * kSB * pow(Temperature, 4)), 0.5f);
}

float FAstroFisiks::StefanBoltzmannLaw_Temperature(float Luminosity, float Radius)
{
	return pow(Luminosity / (4 * pi * kSB * Radius * Radius), 0.25f);
}

float FAstroFisiks::CosmologicalRedshift_Redshift(float ObservedWavelength, float EmittedWavelength)
{
	return ObservedWavelength / EmittedWavelength - 1;
}

float FAstroFisiks::CosmologicalRedshift_ObservedWavelength(float Redshift, float EmittedWavelength)
{
	return (Redshift + 1) * EmittedWavelength;
}

float FAstroFisiks::CosmologicalRedshift_EmittedWavelength(float Redshift, float ObservedWavelength)
{
	return ObservedWavelength / (Redshift + 1);
}

float FAstroFisiks::RelativisticRedshift_Redshift(float Velocity)
{
	return pow((c + Velocity) / (c - Velocity), 0.5) - 1;
}

float FAstroFisiks::RelativisticRedshift_Velocity(float Redshift)
{
	return 0.0f;
}

float FAstroFisiks::DopplerFormula_Velocity(float Redshift)
{
	return c * Redshift;
}

float FAstroFisiks::DopplerFormula_Redshift(float Velocity)
{
	return Velocity / c;
}

float FAstroFisiks::MassEnergyEquivalence_Energy(float Mass)
{
	return Mass * pow(c, 2);
}

float FAstroFisiks::MassEnergyEquivalence_Mass(float Energy)
{
	return Energy / pow(c, 2);
}

float FAstroFisiks::GravitationalForce_Force(float MassA, float MassB, float Distance)
{
	return G * MassA * MassB / pow(Distance, 2);
}

float FAstroFisiks::GravitationalForce_MassA(float Force, float MassB, float Distance)
{
	return Force / (G * MassB / pow(Distance, 2));
}

float FAstroFisiks::GravitationalForce_MassB(float Force, float MassA, float Distance)
{
	return Force / (G * MassA / pow(Distance, 2));
}

float FAstroFisiks::GravitationalForce_Distance(float Force, float MassA, float MassB)
{
	return pow(G * MassA * MassB / Force, 0.5);
}

float FAstroFisiks::GravitationalAcceleration_Acceleration(float Mass, float Radius)
{
	return G * Mass / pow(Radius, 2);
}

float FAstroFisiks::HubblesLaw_Velocity(float Distance)
{
	return H0 * Distance;
}

float FAstroFisiks::HubblesLaw_Distance(float Velocity)
{
	return Velocity / H0;
}
