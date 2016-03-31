#ifndef OSUCALC_H
#define OSUCALC_H

double CS2px(double _CS);

double px2CS(double _px);

double AR2ms(double _AR);

double ms2AR(double _ms);

// for 300 score
double OD2ms(double _OD);

// for 300 score
double ms2OD(double _ms);

// Slider osu!px/s
double SV2px_s(double _SV, double _BPM);

// Approach Circle at t
double ApCS2px_ps(double _CS, double _AR, double _t);

#endif