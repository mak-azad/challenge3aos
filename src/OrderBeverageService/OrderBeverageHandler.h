#ifndef VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/OrderBeverageService.h"
#include "../../gen-cpp/WeatherService.h"
#include "../../gen-cpp/BeveragePreferenceService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

// order beverage handler


namespace vending_machine{

class OrderBeverageServiceHandler : public OrderBeverageServiceIf {
 public:
  OrderBeverageServiceHandler(
		  ClientPool<ThriftClient<WeatherServiceClient>> *, ClientPool<ThriftClient<BeveragePreferenceServiceClient>> * ) ;
  ~OrderBeverageServiceHandler() override=default;

  void PlaceOrder(std::string& _return, const int64_t city) override;
 private:
 // get the client pool for weather and beveragepreference service 
  ClientPool<ThriftClient<WeatherServiceClient>> *_weather_client_pool;
  ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *_beveragepreference_client_pool;
};

// Constructor
OrderBeverageServiceHandler::OrderBeverageServiceHandler(
		ClientPool<ThriftClient<WeatherServiceClient>> *weather_client_pool, ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *beveragepreference_client_pool) {

     // Storing the clientpool
     _weather_client_pool = weather_client_pool;
     _beveragepreference_client_pool = beveragepreference_client_pool;  

}

// Remote Procedure "PlaceOrder"
void OrderBeverageServiceHandler::PlaceOrder(std::string& _return, const int64_t city){
     printf("PlaceOrder\n");

     //return BeverageType::type::COLD;
#if 1   
    // 1. get the weather service client pool
    auto weather_client_wrapper = _weather_client_pool->Pop();
    if (!weather_client_wrapper) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
      se.message = "Failed to connect to weather-service";
      throw se;
    }  
    auto weather_client = weather_client_wrapper->GetClient();
   
   // follow same as weather service

    auto beveragepreference_client_wrapper = _beveragepreference_client_pool->Pop();
    if (!beveragepreference_client_wrapper){
	ServiceException se;
	se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
	se.message = "Failed to connect to beverage-preference-service";
	throw se;
	}
   auto beveragepreference_client = beveragepreference_client_wrapper->GetClient();

    // by default get cold
    WeatherType::type weatherType = WeatherType::type::COLD;

    // 2. call the remote procedure : GetWeather
    try {
      weatherType = weather_client->GetWeather(city);
    } catch (...) {
      _weather_client_pool->Push(weather_client_wrapper);
      LOG(error) << "Failed to send call GetWeather to weather-client";
      throw;
    }
    _weather_client_pool->Push(weather_client_wrapper);

    // now call the getbeverage service to return specificc drinks

    // follow same as weatehr call
	try {
		  if(weatherType == WeatherType::type::WARM)
		  {
			 Beverages::type drink1 = beveragepreference_client->GetBeverage(BeverageType::type::COLD);
			 if(drink1 == Beverages::type::lemonade)
				_return = "lemonade";
			 else if (drink1 == Beverages::type::icetea)
				_return = "ice-tea";
			 else
				_return = "soda";  
		  }else {
			 Beverages::type drink2 = beveragepreference_client->GetBeverage(BeverageType::type::HOT);
			if(drink2 == Beverages::type::cappuccino)
				_return = "cappuccino";
			else if (drink2 == Beverages::type::latte)
				 _return = "latte";
			else
				_return = "espresso";
			
	    }
	  }
	 catch(...){
		_beveragepreference_client_pool->Push(beveragepreference_client_wrapper);
		LOG(error) << "Failed to send call GetBeverage to beverage-preference-client";	
		throw;
	}	
	_beveragepreference_client_pool->Push(beveragepreference_client_wrapper);

#endif
}

} // namespace vending_machine


#endif //VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H

