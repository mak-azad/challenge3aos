#ifndef VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/BeveragePreferenceService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

// get beverage service handler 

namespace vending_machine{

class BeveragePreferenceServiceHandler : virtual public BeveragePreferenceServiceIf {
	public:
		BeveragePreferenceServiceHandler();
		~BeveragePreferenceServiceHandler() override=default;
		Beverages::type GetBeverage(const BeverageType::type btype) override;
	};

	BeveragePreferenceServiceHandler::BeveragePreferenceServiceHandler(){
	}

	Beverages::type BeveragePreferenceServiceHandler::GetBeverage(const BeverageType::type btype)
	{
		// get a rondom number and return a corresponding beverage
		int rnumber = rand() % 3;
		if(btype == BeverageType::type::COLD){
		// if the type is cold then cold drinks
			if (rnumber == 1)
				return (Beverages::type::soda);
			else if(rnumber == 2)
				 return (Beverages::type::lemonade);
			else
				 return (Beverages::type::icetea);
		}
		else if(btype == BeverageType::type::HOT){
		// if type is hot then hot drinks
			if(rnumber  == 1)
				return (Beverages::type::cappuccino);
			else if(rnumber == 2)
				 return (Beverages::type::espresso);
			else
				 return (Beverages::type::latte);
		}
	}
}
#endif //VENDING_MACHINE_MICROSERVICES_WEATHERHANDLER_H
