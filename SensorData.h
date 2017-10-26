/*
Copyright (c) 2016, Jochen Kempfle
All rights reserved.


Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/


#ifndef SENSORDATA_H
#define SENSORDATA_H

#include "SensorDataType.h"
#include "SensorDataFactory.h"
#include "SensorBuffer.h"
#include "Quaternion.h"
#include "Vector3.h"
#include <string>
#include <set>
#include <initializer_list>


// forward declaration of typed sensor buffer
template <class T>
class SensorBufferType;


// sensor data base class
class SensorData
{
  public:
    SensorData();
    SensorData(unsigned int timestamp/*, uint64_t receiveTime, int dataType*/);
    SensorData(const SensorData &other);
    virtual ~SensorData();

    // inline void setType(unsigned int dataType) { _dataType = dataType; }
    // inline unsigned int getType() const { return _dataType; }

    inline void setTimestamp(unsigned int timestamp) { _timestamp = timestamp; }
    inline unsigned int getTimestamp() const { return _timestamp; }

    // virtual const SensorDataType getType() const = 0;
    // inline void setReceiveTime(uint64_t receiveTime) { _receiveTime = receiveTime; }
    // inline uint64_t getReceiveTime() const { return _receiveTime; }

  protected:

  private:
    unsigned int _timestamp;
    // uint64_t _receiveTime;
    // unsigned int _dataType;
};




// maybe use instead of the #define mixin the curiously recurring template pattern (with abstract base class)
#define DECLARE_SENSOR_DATA_TYPE(SENSOR_DATA_TYPE_CLASS) \
  public: \
    static SensorData* createSelf() { return new SENSOR_DATA_TYPE_CLASS(); } \
    static const int getTypeId() { return _sensorDataTypeId; } \
    static const std::string getTypeName() { return _sensorDataTypeName; } \
    static const int _sensorDataTypeId; \
    static const std::string _sensorDataTypeName; \
    static const SensorDataType getType() { return SensorDataType(_sensorDataTypeId); }

#define REGISTER_SENSOR_DATA_TYPE(SENSOR_DATA_TYPE_NAME, SENSOR_DATA_TYPE_CLASS) \
    const int SENSOR_DATA_TYPE_CLASS::_sensorDataTypeId = SensorDataFactory::registerSensorDataType(SENSOR_DATA_TYPE_NAME, SENSOR_DATA_TYPE_CLASS::createSelf, SensorBufferType<SENSOR_DATA_TYPE_CLASS>::createSelf); \
    const std::string SENSOR_DATA_TYPE_CLASS::_sensorDataTypeName = SENSOR_DATA_TYPE_NAME;


// sensor data types
class SensorDataOrientation : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataOrientation)

  public:
    SensorDataOrientation();
    SensorDataOrientation(const SensorData* data);
    SensorDataOrientation(const SensorData &data);
    SensorDataOrientation(const SensorDataOrientation &orientation);
    SensorDataOrientation(const Quaternion &orientation);

    // void operator=(const SensorData &other);

    inline void setOrientation(const Quaternion &orientation) { _orientation = orientation; }
    inline Quaternion getOrientation() const { return _orientation; }

  private:
    Quaternion _orientation;
};


class SensorDataPosition : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataPosition)

  public:
    SensorDataPosition();
    SensorDataPosition(const SensorData* data);
    SensorDataPosition(const SensorData &data);
    SensorDataPosition(const SensorDataPosition &position);
    SensorDataPosition(const Vector3 &position);

    // void operator=(const SensorData &other);

    inline void setPosition(const Vector3 &position) { _position = position; }
    inline Vector3 getPosition() const { return _position; }

  private:
    Vector3 _position;
};


class SensorDataAcceleration : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataAcceleration)

  public:
    SensorDataAcceleration();
    SensorDataAcceleration(const SensorData* data);
    SensorDataAcceleration(const SensorData &data);
    SensorDataAcceleration(const SensorDataAcceleration &acc);
    SensorDataAcceleration(const Vector3 &acc);

    void operator=(const SensorData &other);

    inline void setAcceleration(const Vector3 &acceleration) { _acceleration = acceleration; }
    inline Vector3 getAcceleration() const { return _acceleration; }

  private:
    Vector3 _acceleration;
};


class SensorDataGyroscope : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataGyroscope)

  public:
    SensorDataGyroscope();
    SensorDataGyroscope(const SensorData* data);
    SensorDataGyroscope(const SensorData &data);
    SensorDataGyroscope(const SensorDataGyroscope &gyro);
    SensorDataGyroscope(const Vector3 &gyro);

    void operator=(const SensorData &other);

    inline void setGyroscope(const Vector3 &gyroscope) { _gyroscope = gyroscope; }
    inline Vector3 getGyroscope() const { return _gyroscope; }

  private:
    Vector3 _gyroscope;
};


class SensorDataMagnetometer : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataMagnetometer)

  public:
    SensorDataMagnetometer();
    SensorDataMagnetometer(const SensorData* data);
    SensorDataMagnetometer(const SensorData &data);
    SensorDataMagnetometer(const SensorDataMagnetometer &magnet);
    SensorDataMagnetometer(const Vector3 &magnet);

    void operator=(const SensorData &other);

    inline void setMagnetometer(const Vector3 &magnetometer) { _magnetometer = magnetometer; }
    inline Vector3 getMagnetometer() const { return _magnetometer; }

  private:
    Vector3 _magnetometer;
};

/*
class SensorDataHeight : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataHeight)

  public:
    inline void setHeight(float height) { _height = height; }
    inline float getHeight() const { return _height; }

  private:
    float _height;
};


class SensorDataDistance : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataDistance)

  public:
    inline void setDistance(float distance) { _distance = distance; }
    inline float getDistance() const { return _distance; }

  private:
    float _distance;
};


class SensorDataImageRGB : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataImageRGB)

  public:
    inline void setImageRGB(char* image) { _imageRGB = image; }
    inline char* getImageRGB() const { return _imageRGB; }

  private:
    char* _imageRGB;
};


class SensorDataImageDepth : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataImageDepth)

  public:
    inline void setImageDepth(char* image) { _imageDepth = image; }
    inline char* getImageDepth() const { return _imageDepth; }

  private:
    char* _imageDepth;
};


class SensorDataPointCloud : public virtual SensorData
{
    DECLARE_SENSOR_DATA_TYPE(SensorDataPointCloud)

  public:
    inline void setPointCloud(const std::vector<Vector3> &pointCloud) { _pointCloud = pointCloud; }
    inline std::vector<Vector3> getPointCloud() const { return _pointCloud; }

  private:
    std::vector<Vector3> _pointCloud;
};
*/

// TODO(JK#5#2017-09-13): SensorDataComposition requires lots of memory (due to RTTI). Rewrite and provide sensorData_cast<class T>(data*)

// templates for data composition. To get more template parameters just add SensorDataComposition[X+1] templates like shown below.
// This style guarantees that each Composition can be downcasted to any other subset of compositions

template <class... Args>
class SensorDataComposition : public virtual Args...
{
  public:
    SensorDataComposition();
    SensorDataComposition(const Args&... args);
    template <class T>
    SensorDataComposition(const T &other);
    SensorDataComposition(const SensorData* other);

    static size_t getNumTypeIds() { return _type.numIds(); }
    static const SensorDataType getType() { return _type; }

  private:
    static const SensorDataType _type;
    static const int _sensorDataTypeId;
};


template <class... Args>
const SensorDataType SensorDataComposition<Args...>::_type = {Args::getTypeId()...};

template <class... Args>
const int SensorDataComposition<Args...>::_sensorDataTypeId = SensorDataFactory::registerSensorDataCompositionType(_type, SensorBufferType<SensorDataComposition<Args...> >::createSelf);
// sensorData_cast provided for convenience, rather not use it
/*
template <class... Args1, class... Args2>
inline SensorDataComposition<Args1...> sensorData_cast(const SensorDataComposition<Args2...> &data)
{
    static_assert(sizeof...(Args1) > 0, "sensorData_cast must be called with template parameters specified");
    return SensorDataComposition<Args1...>(static_cast<Args1>(data)...);
}
*/



template <class... Args>
struct isBaseOfSensorData : std::true_type
{
};

template <class T, class... Args>
struct isBaseOfSensorData<T, Args...> : std::integral_constant<bool, std::is_base_of<SensorData, T>() && isBaseOfSensorData<Args...>{} >
{
};


template <class... Args>
SensorDataComposition<Args...>::SensorDataComposition()
{
    static_assert(isBaseOfSensorData<Args...>{}, "Template parameter in SensorDataComposition must be derived from SensorData");
}

template <class... Args>
SensorDataComposition<Args...>::SensorDataComposition(const Args&... args) : Args(args)...
{
    static_assert(isBaseOfSensorData<Args...>{}, "Template parameter in SensorDataComposition must be derived from SensorData");
}


template <class... Args>
template <class T>
SensorDataComposition<Args...>::SensorDataComposition(const T &other) : Args(static_cast<Args>(other))...
{
    static_assert(isBaseOfSensorData<Args...>{}, "Template parameter in SensorDataComposition must be derived from SensorData");
    static_assert(isBaseOfSensorData<T>{}, "Template parameter to be assigned to SensorDataComposition must be derived from SensorData");
}

template <class... Args>
SensorDataComposition<Args...>::SensorDataComposition(const SensorData* data) : Args(*static_cast<const Args*>(data))...
{
    static_assert(isBaseOfSensorData<Args...>{}, "Template parameter in SensorDataComposition must be derived from SensorData");
}


// typedefs for convenience
typedef SensorDataComposition<SensorDataOrientation, SensorDataPosition> SensorDataOrientationPosition;

typedef SensorDataComposition<SensorDataAcceleration, SensorDataGyroscope, SensorDataMagnetometer> SensorDataIMURaw;

#endif // SENSORDATA_H
