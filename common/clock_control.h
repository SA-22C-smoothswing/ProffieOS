#ifndef COMMON_CLOCK_CONTROL_H
#define COMMON_CLOCK_CONTROL_H

#if VERSION_MAJOR >= 4

#include "looper.h"

class ClockControl : public Looper {
public:
  const char* name() override { return "ClockControl"; }
  void Loop() override {
    bool on = false;
    SaberBase::DoIsOn(&on);
    uint32_t now = millis();
    if (on
	|| Serial
	|| prop.NeedsPower()
	|| USBD_Connected()
	|| stm32l4_gpio_pin_read(GPIO_PIN_PB2)
#ifdef ENABLE_AUDIO
	|| amplifier.Active()
#endif
      ) {
      last_activity_ = now;
    }
    if (now - last_activity_ > 30000) {
      if (now - last_activity_ < 30200) {
	Serial3.println("SLEEPING");
      }
      stm32l4_system_sysclk_configure(1000000, 500000, 500000);
      delay(50);
      stm32l4_system_sysclk_configure(_SYSTEM_CORE_CLOCK_, _SYSTEM_CORE_CLOCK_/2, _SYSTEM_CORE_CLOCK_/2);
    }
  }
    
  private:
    uint32_t last_activity_;
};

ClockControl clock_control;

#endif

#endif
