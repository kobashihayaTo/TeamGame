
#include <math.h>

double PI = 3.141592;

//-------------------easeioSine------------------//
//easeIn�̏���
double easeInSine(double time)
{
	return 1 - cos((time * PI) / 2);
}

//easeOut�̏���
double easeOutSine(double time)
{
	return sin((time * PI) / 2);
}

//easeInOut�̏���
double easeInOutSine(double time)
{
	return -(cos(PI * time) - 1) / 2;
}



//-------------------easeioCubic------------------//
//easeInCubic�̏���
double easeInCubic(double time)
{
	return time * time * time;
}

//easeOutCubic�̏���
double easeOutCubic(double time)
{
	return 1 - pow(1 - time, 3);
}

//easeInOut�̏���
double easeInOutCubic(double time)
{
	return time < 0.5 ? 4 * time * time * time : 1 - pow(-2 * time + 2, 3) / 2;
}



//-------------------easeioQuint------------------//
//easeInQuint�̏���
double easeInQuint(double time)
{
	return time * time * time * time * time;
}

//easeOutQuint�̏���
double easeOutQuint(double time)
{
	return 1 - pow(1 - time, 5);
}

//easeInOutQuint�̏���
double easeInOutQuint(double time)
{
	return time < 0.5 ? 16 * time * time * time * time * time : 1 - pow(-2 * time + 2, 5) / 2;
}



//-------------------easeioCirc------------------//
//easeInCirc�̏���
double easeInCirc(double time)
{
	return 1 - sqrt(1 - pow(time, 2));
}

//easeOutCirc�̏���
double easeOutCirc(double time)
{
	return sqrt(1 - pow(time - 1, 2));
}

//easeInOutCirc�̏���
double easeInOutCirc(double time)
{
	return time < 0.5
		? (1 - sqrt(1 - pow(2 * time, 2))) / 2
		: (sqrt(1 - pow(-2 * time + 2, 2)) + 1) / 2;
}



//-------------------easeioElastic------------------//
//easeInElastic
double easeInElastic(double time)
{
	const double c4 = (2 * PI) / 3;

	return time == 0
		? 0
		: time == 1
		? 1
		: -pow(2, 10 * time - 10) * sin((time * 10 - 10.75) * c4);
}

//easeOutElastic
double easeOutElastic(double time)
{
	const  double c4 = (2 * PI) / 3;

	return time == 0
		? 0
		: time == 1
		? 1
		: pow(2, -10 * time) * sin((time * 10 - 0.75) * c4) + 1;
}

//easeInOutElastic
double easeInOutElastic(double time)
{
	const double c5 = (2 * PI) / 4.5;

	return time == 0
		? 0
		: time == 1
		? 1
		: time < 0.5
		? -(pow(2, 20 * time - 10) * sin((20 * time - 11.125) * c5)) / 2
		: (pow(2, -20 * time + 10) * sin((20 * time - 11.125) * c5)) / 2 + 1;
}



//-------------------easeioQuad------------------//
//easeInQuad�̏���
double easeInQuad(double time)
{
	return time * time;
}

//easeOutQuad�̏���
double easeOutQuad(double time)
{
	return 1 - (1 - time) * (1 - time);
}

//easeInOutQuad�̏���
double easeInOutQuad(double time)
{
	return time < 0.5 ? 2 * time * time : 1 - pow(-2 * time + 2, 2) / 2;
}



//-------------------easeioQuart------------------//
//easeInQuart�̏���
double easeInQuart(double time)
{
	return time * time * time * time;
}

//easeOutQuart�̏���
double easeOutQuart(double time)
{
	return 1 - pow(1 - time, 4);
}

//easeInOutQuart�̏���
double easeInOutQuart(double time)
{
	return time < 0.5 ? 8 * time * time * time * time : 1 - pow(-2 * time + 2, 4) / 2;
}



//-------------------easeioExpo------------------//
//easeInExpo�̏���
double easeInExpo(double time)
{
	return time == 0 ? 0 : pow(2, 10 * time - 10);
}

//easeOutExpo�̏���
double easeOutExpo(double time)
{
	return time == 1 ? 1 : 1 - pow(2, -10 * time);
}

//easeInOutExpo�̏���
double easeInOutExpo(double time)
{
	return time == 0
		? 0
		: time == 1
		? 1
		: time < 0.5 ? pow(2, 20 * time - 10) / 2
		: (2 - pow(2, -20 * time + 10)) / 2;
}



//-------------------easeioBack------------------//
//easeInBack�̏���
double easeInBack(double time)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	return c3 * time * time * time - c1 * time * time;
}

//easeOutBack�̏���
double easeOutBack(double time)
{
	const double c1 = 1.70158;
	const double c3 = c1 + 1;

	return 1 + c3 * pow(time - 1, 3) + c1 * pow(time - 1, 2);
}

//easeInOutBack�̏���
double easeInOutBack(double time)
{
	const double c1 = 1.70158;
	const double c2 = c1 * 1.525;

	return time < 0.5
		? (pow(2 * time, 2) * ((c2 + 1) * 2 * time - c2)) / 2
		: (pow(2 * time - 2, 2) * ((c2 + 1) * (time * 2 - 2) + c2) + 2) / 2;
}



//-------------------easeioBounse------------------//
//easeOutBounce�̏���
double easeOutBounce(double time)
{
	const double n1 = 7.5625;
	const double d1 = 2.75;

	if (time < 1 / d1) {
		return n1 * time * time;
	}
	else if (time < 2 / d1) {
		return n1 * (time -= 1.5 / d1) * time + 0.75;
	}
	else if (time < 2.5 / d1) {
		return n1 * (time -= 2.25 / d1) * time + 0.9375;
	}
	else {
		return n1 * (time -= 2.625 / d1) * time + 0.984375;
	}
}

//easeInBounce�̏���
double easeInBounce(double time)
{
	return 1 - easeOutBounce(1 - time);
}

//easeInOutBounce�̏���
double easeInOutBounce(double time)
{
	return time < 0.5
		? (1 - easeOutBounce(1 - 2 * time)) / 2
		: (1 + easeOutBounce(2 * time - 1)) / 2;
}
