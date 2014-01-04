// This code is released under the MIT license (see below).
//
// The MIT License
// 
// Copyright (c) 2012 Dominique Wurtz (www.blaukraut.info)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// see http://www.kvraudio.com/forum/printview.php?t=346155&start=0
// and http://pastebin.com/THe5JG5f

#ifndef __DIODE_LADDER_FILTER_HPP__
#define __DIODE_LADDER_FILTER_HPP__

#include <cassert>
#include <cmath>
#include <algorithm>

// Emulation of Diode ladder lowpass filter as found in Roland TB303 or EMS VCS3
//
// Version 0.2 (04/05/2012) greatly simplified equations; add highpass filter in feedback path
// Version 0.1 (04/03/2012) initial version

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class DiodeLadderFilter
{
public:

	DiodeLadderFilter()
	{
		std::fill(z, z + 5, 0);
		set_q(0);
	}

	// fc: normalized cutoff frequency in the range [0..1] => 0 HZ .. Nyquist
	void set_feedback_hpf_cutoff(const double fc)
	{
		const double K = fc * M_PI;
		ah = (K - 2) / (K + 2);
		bh = 2 / (K + 2);
	}

	void reset()
	{
		if (k < 17) std::fill(z, z + 5, 0);
	}

	// q: resonance in the range [0..1]
	void set_q(const double q)
	{
		assert(q >= 0 && q <= 1.);
		k = 20 * q;
		A = 1 + 0.5*k; // resonance gain compensation 
	}

	void set_fc(double cutoff)
	{
		cutoff = ( cutoff * cutoff );
		fc = cutoff <= 0 ? .02 : (cutoff >= 1.0 ? .999 : cutoff);
	}

	// Process one sample.
	//
	// x: input signal
	// fc: normalized cutoff frequency in the range [0..1] => 0 HZ .. Nyquist
	__forceinline double tick(const double x)
	{
		assert(fc > 0 && fc < 1);
		const double a = M_PI * fc; // PI is Nyquist frequency 
		// a = 2 * tan(0.5*a); // dewarping, not required with 2x oversampling
		const double ainv = 1/a;
		const double a2 = a*a;
		const double b = 2*a + 1;
		const double b2 = b*b;
		const double c = 1 / (2*a2*a2 - 4*a2*b2 + b2*b2);
		const double g0 = 2*a2*a2*c;
		const double g = g0 * bh;

		// current state
		const double s0 = (a2*a*z[0] + a2*b*z[1] + z[2]*(b2 - 2*a2)*a + z[3]*(b2 - 3*a2)*b) * c;
		const double s = bh*s0 - z[4];

		// solve feedback loop (linear)
		double y5 = (g*x + s) / (1 + g*k);

		// input clipping
		const double y0 = clip(x - k*y5);
		y5 = g*y0 + s;

		// compute integrator outputs
		const double y4 = g0*y0 + s0;
		const double y3 = (b*y4 - z[3]) * ainv;
		const double y2 = (b*y3 - a*y4 - z[2]) * ainv;
		const double y1 = (b*y2 - a*y3 - z[1]) * ainv;

		// update filter state
		z[0] += 4*a*(y0 - y1 + y2);
		z[1] += 2*a*(y1 - 2*y2 + y3);
		z[2] += 2*a*(y2 - 2*y3 + y4);
		z[3] += 2*a*(y3 - 2*y4);
		z[4] = bh*y4 + ah*y5;

		return A*y4;
	}
	
private:
	double k, A;
	double z[5]; // filter memory (4 integrators plus 1st order HPF)
	double ah, bh; // feedback HPF coeffs
	double fc; // filter cutoff

	static __forceinline double clip(const double x)
	{
		return x / (1 + abs(x));
	}
};

#endif // __DIODE_LADDER_FILTER_HPP__