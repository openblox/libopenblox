/*
 * TestInstance.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: johnmh
 */

#include "TestInstance.h"

BEGIN_INSTANCE
	DEFINE_CLASS(TestInstance, true, false, Instance);

	TestInstance::TestInstance(){}
	TestInstance::~TestInstance(){}

	Instance* TestInstance::cloneImpl(Instance* newOne){
		if(newOne == NULL){
			newOne = new TestInstance();
		}
		return newOne;
	}

END_INSTANCE
