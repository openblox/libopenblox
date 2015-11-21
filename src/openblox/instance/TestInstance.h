#ifndef OPENBLOX_INSTANCE_TESTINSTANCE_H_
#define OPENBLOX_INSTANCE_TESTINSTANCE_H_

#include "Instance.h"

BEGIN_INSTANCE

class TestInstance: public Instance{
	public:
		TestInstance();
		virtual ~TestInstance();

		DECLARE_CLASS(TestInstance);
};

END_INSTANCE

#endif
