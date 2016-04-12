//
//  FacebookX.cpp
//  FacebookXTest
//
//  Created by Thuy Dong Xuan on 4/12/16.
//
//

#include "FacebookX.hpp"

namespace h102 {
  
  FacebookListener* FacebookX::listener = NULL;
  
  void FacebookX::init() {
    
  }
  
  void FacebookX::setListener(h102::FacebookListener *listener) {
    FacebookX::listener = listener;
  }
}