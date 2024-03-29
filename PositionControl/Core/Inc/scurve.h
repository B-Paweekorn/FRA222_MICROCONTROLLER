/*
 * scurve.h
 *
 *  Created on: 22 May 2023
 *      Author: pawee
 */

#ifndef INC_SCURVE_H_
#define INC_SCURVE_H_

#include "main.h"
#include "math.h"

float Trajectory_Generator(int initial_p,int target_p,float vmax,float amax,float jmax);
struct ScurveEvaluated Trajectory_Evaluated(float vmax, float amax, float jmax);

#endif /* INC_SCURVE_H_ */
