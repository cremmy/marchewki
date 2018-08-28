/*
 * rules.cpp
 *
 *  Created on: 28 sie 2018
 *      Author: crm
 */

#include "rules.h"

Rules ruleset;

Rules getRuleset()
	{
	return ruleset;
	}

bool isRuleEnabled(Rules rule)
	{
	return (ruleset&rule)==rule;
	}

void enableRule(Rules rule)
	{
	ruleset|=rule;
	}

void disableRule(Rules rule)
	{
	ruleset&=~rule;
	}



