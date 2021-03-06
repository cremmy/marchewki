/*
 * rules.cpp
 *
 *  Created on: 28 sie 2018
 *      Author: crm
 */

#include "rules.h"

static int ruleset;

Rules getRuleset()
	{
	return (Rules)ruleset;
	}

bool isRuleEnabled(Rules rule)
	{
	return (ruleset&rule)==rule;
	}

void setRuleset(Rules rules)
	{
	ruleset=rules;
	}

void enableRule(Rules rule)
	{
	ruleset|=rule;
	}

void disableRule(Rules rule)
	{
	ruleset&=~rule;
	}



