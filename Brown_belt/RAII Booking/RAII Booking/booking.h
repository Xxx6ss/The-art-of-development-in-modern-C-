//
//  booking.h
//  RAII Booking
//
//  Created by Andrew Kireev on 08.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef booking_h
#define booking_h

#include <utility>

namespace RAII {

template <typename Provider>
class Booking {
public:
    Booking(Provider* prov, int count) : provider_(prov), counter_(count) {
    }
    
    Booking(Booking&& prov) {
        provider_ = prov.provider_;
        prov.provider_ = nullptr;

    }
    
    Booking(const Booking& prov) = delete;
    
    Booking& operator= (Booking&& prov) {
        provider_ = prov.provider_;
        prov.provider_ = nullptr;
        return *this;
    }
    
    Booking& operator= (const Booking& prov) = delete;
    
    ~Booking() {
        if (provider_)
            provider_->CancelOrComplete(*this);
    }
    
private:
    Provider* provider_;
     int counter_;
};
}

#endif /* booking_h */
