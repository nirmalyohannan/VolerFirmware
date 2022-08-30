/* M1 M4 high =  roll +ve
M2 M3 high = roll -ve

M1 M2 high =  yaw decreasing
M3 M4 high =  yaw increasing

M1 M3 HIGH  =  Pitch +ve
M2 M4 High  =  pitch -ve

SetPointRoll,SetPointYaw,SetPointPitch */

void Runner_Motor1()
{

	if (roll < SetPointRoll) // PWMROLL
	{
		PWMrollM1 = PWMrollM1 - (compensator * HorizontalcompensatorWeight);
	}
	elseif(roll > SetPointRoll)
	{
		PWMrollM1 = PWMrollM1 + (compensator * HorizontalcompensatorWeight);
	}
	else
	{
		PWMrollM1 = 0;
	}

	if (pitch < SetPointPitch) // PWMPitch
	{
		PWMpitchM1 = PWMpitchM1 - (compensator * HorizontalcompensatorWeight);
	}
	elseif(roll > SetPointRoll)
	{
		PWMpitchM1 = PWMpitchM1 + (compensator * HorizontalcompensatorWeight);
	}
	else
	{
		PWMpitchM1 = 0;
	}

	if (altitude < setPointAltitude) // PWMaltitude
	{
		PWMaltitudeM1 = PWMaltitudeM1 + (compensator * VerticalCompensatorWeight);
	}
	elseif(altitude > setPointAltitude)
	{
		PWMaltitudeM1 = PWMaltitudeM1 - (compensator * VerticalCompensatorWeight);
	}
	// if altitude=setpointaltitude : no change for PWMaltitudeM1

	PWM_M1 = PWMroll + PWMyaw + PWMpitch + PWMaltitude;
}