/*! @file mfio_sample.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  Multi-Function I/O API usage in a Linux environment.
 *  Shows example usage of the APIs available for controlling the MFIO pins
 *  on the vehicle/FC.
 *
 *  @copyright
 *  2017 DJI. All rights reserved.
 * */

#include "mfio_sample.hpp"

using namespace DJI::OSDK;

bool
pwmOutputBlockingApiDemo(Vehicle* vehicle)
{

  int responseTimeout = 1;

  // Set SDK4 to output PWM at 50Hz
  // Parameters: initialValue - duty cycle
  //             freq - PWM freq
  uint32_t initOnTimeUs = 10000; // us
  uint16_t pwmFreq      = 50;    // Hz

  // Setup the channel 4
  std::cout << "Configuring channel\n";
  vehicle->mfio->config(MFIO::MODE_PWM_OUT, MFIO::CHANNEL_3, initOnTimeUs,
                        pwmFreq, responseTimeout);
  std::cout << "Channel 4 configured to output 50Hz PWM with 50% duty cycle.\n";
  sleep(5);

  // AFter 5s, change the duty cycle from 30%, then 70%.

  std::cout << "Setting 30% duty cycle\n";
  initOnTimeUs = 6000; // us, 30% duty cycle
  vehicle->mfio->setValue(MFIO::CHANNEL_3, initOnTimeUs, responseTimeout);
  sleep(5);

  std::cout << "Setting 70% duty cycle\n";
  initOnTimeUs = 14000; // us, 70% duty cycle
  vehicle->mfio->setValue(MFIO::CHANNEL_3, initOnTimeUs, responseTimeout);
  sleep(5);

  std::cout << "Turning off the PWM signal\n";
  uint32_t digitalValue = 0;
  uint16_t digitalFreq  = 0;
  vehicle->mfio->config(MFIO::MODE_GPIO_OUT, MFIO::CHANNEL_3, digitalValue,
                        digitalFreq, responseTimeout);
}

bool
pwmOutputNonBlockingApiDemo(Vehicle* vehicle)
{
  // Set SDK4 to output PWM at 50Hz
  // Parameters: initialValue - duty cycle
  //             freq - PWM freq
  uint32_t initOnTimeUs = 10000; // us
  uint16_t pwmFreq      = 50;    // Hz
  // CHANNEL  channelUsed=MFIO::CHANNEL_3;

  // Setup the channe4
  std::cout << "Configuring channel\n";
  vehicle->mfio->config(MFIO::MODE_PWM_OUT, MFIO::CHANNEL_3, initOnTimeUs,
                        pwmFreq);
  std::cout << "Channel 4 configured to output 50Hz PWM with 50% duty cycle.\n";
  sleep(5);

  // AFter 4s, change the duty cycle from 30%, then 70%.

  std::cout << "Setting 30% duty cycle\n";
  initOnTimeUs = 6000; // us, 30% duty cycle
  vehicle->mfio->setValue(MFIO::CHANNEL_3, initOnTimeUs);
  sleep(5);

  std::cout << "Setting 70% duty cycle\n";
  initOnTimeUs = 14000; // us, 70% duty cycle
  vehicle->mfio->setValue(MFIO::CHANNEL_3, initOnTimeUs);
  sleep(5);

  std::cout << "Turning off the PWM signal\n";
  uint32_t digitalValue = 0;
  uint16_t digitalFreq  = 0;
  vehicle->mfio->config(MFIO::MODE_GPIO_OUT, MFIO::CHANNEL_3, digitalValue,
                        digitalFreq);
}

bool
gpioLoopbackBlockingApiDemo(Vehicle* vehicle)
{

  int responseTimeout = 1;

  // Set SDK4 to GPO output
  uint32_t initHL  = 0;  // low
  uint16_t pwmFreq = 50; // Hz not used

  // Setup the channel 4
  std::cout << "Configuring channel\n";
  vehicle->mfio->config(MFIO::MODE_GPIO_OUT, MFIO::CHANNEL_3, initHL, pwmFreq,
                        responseTimeout);
  std::cout << "Channel 4 configured to GPO.\n";
  sleep(5);

  // AFter 5s, change to high.

  std::cout << "Setting to high\n";
  initHL = 1; // high
  vehicle->mfio->setValue(MFIO::CHANNEL_3, initHL, responseTimeout);
  sleep(1);

  std::cout << "config channel 5 to GPI\n";
  uint32_t digitalValue = 1; // not used... Does not matter for GPI
  uint16_t digitalFreq  = 0; //  not used....Does not matter for GPIO
  vehicle->mfio->config(MFIO::MODE_GPIO_IN, MFIO::CHANNEL_4, digitalValue,
                        digitalFreq, responseTimeout);

  ACK::MFIOGet ack;
  ack = vehicle->mfio->getValue(MFIO::CHANNEL_4, responseTimeout);

  std::cout << "\n GPI status:" << ack.ack.data << std::endl;
  std::cout << "\n GPI value:" << ack.value << std::endl;
}

bool
gpioLoopbackNonBlockingApiDemo(Vehicle* vehicle)
{

  // Set SDK4 to GPO
  uint32_t initHL  = 0;  //
  uint16_t pwmFreq = 50; // Hz  not used

  // Setup the channel
  std::cout << "Configuring channel\n";
  vehicle->mfio->config(MFIO::MODE_GPIO_OUT, MFIO::CHANNEL_3, initHL, pwmFreq);
  std::cout << "Channel configured to GPO low.\n";
  sleep(5);

  // AFter 5s, change the duty cycle from 30%, then 70%.

  std::cout << "Setting to high\n";
  initHL = 1; // us, 30% duty cycle
  vehicle->mfio->setValue(MFIO::CHANNEL_3, initHL);
  sleep(1);

  std::cout << "config channel 5 to GPI\n";
  uint32_t digitalValue = 0; // not used... Does not matter for GPI
  uint16_t digitalFreq  = 0; // not used....Does not matter for GPIO
  vehicle->mfio->config(MFIO::MODE_GPIO_IN, MFIO::CHANNEL_4, digitalValue,
                        digitalFreq);

  vehicle->mfio->getValue(MFIO::CHANNEL_4, getGpiCallBack);
  sleep(5);
}

static void
getGpiCallBack(DJI::OSDK::Vehicle* vehicle, DJI::OSDK::RecvContainer recvFrame,
               DJI::OSDK::UserData userData)
{
  uint16_t ack_length =
    recvFrame.recvInfo.len - static_cast<uint16_t>(OpenProtocol::PackageMin);
  uint8_t* ackPtr = recvFrame.recvData.raw_ack_array;

  uint8_t  result;
  uint32_t value;

  memcpy((uint8_t*)&result, ackPtr, 1);
  memcpy((uint8_t*)&value, ackPtr + 1, 4);

  std::cout << "\n GPI status:" << result << std::endl;
  std::cout << " GPI value:" << value << std::endl;
}

bool
adcBlockingApiDemo(Vehicle* vehicle)
{

  int responseTimeout = 1;
  // Set SDK5 to ADC input
  uint32_t initOnTimeUs = 0; // us
  uint16_t pwmFreq      = 0; // Hz

  // Setup the channel 5
  std::cout << "Configuring channel\n";
  vehicle->mfio->config(MFIO::MODE_ADC, MFIO::CHANNEL_4, initOnTimeUs, pwmFreq,
                        responseTimeout);
  std::cout << "Channel 5 configured to ADC input.\n";

  ACK::MFIOGet ack;
  ack = vehicle->mfio->getValue(MFIO::CHANNEL_4, responseTimeout);

  std::cout << "ADC status:" << ack.ack.data << std::endl;
  std::cout << "ADC value:" << ack.value << std::endl;
}

bool
adcNonBlockingApiDemo(Vehicle* vehicle)
{

  // Set F5 as SDK5 for  ADC input
  uint32_t initOnTimeUs = 0; // any number
  uint16_t pwmFreq      = 0; // any number

  // Setup the channel
  std::cout << "Configuring channel\n";
  vehicle->mfio->config(MFIO::MODE_ADC, MFIO::CHANNEL_4, initOnTimeUs, pwmFreq);
  std::cout << "Channel 5 configured to ADC input\n";

  vehicle->mfio->getValue(MFIO::CHANNEL_4, getAdcCallBack);

  sleep(5);
}

static void
getAdcCallBack(DJI::OSDK::Vehicle* vehicle, DJI::OSDK::RecvContainer recvFrame,
               DJI::OSDK::UserData userData)
{

  uint16_t ack_length =
    recvFrame.recvInfo.len - static_cast<uint16_t>(OpenProtocol::PackageMin);
  uint8_t* ackPtr = recvFrame.recvData.raw_ack_array;

  uint8_t  result;
  uint32_t value;

  memcpy((uint8_t*)&result, ackPtr, 1);
  memcpy((uint8_t*)&value, ackPtr + 1, 4);

  std::cout << "ADC status:" << result << std::endl;
  std::cout << "ADC value:" << value << std::endl;
}
