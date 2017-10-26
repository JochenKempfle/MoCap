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


#include "ReceiverFile.h"
#include "FileHandler.h"
#include "SensorManager.h"
#include "SensorNodeIMURaw.h"
#include "SensorNodeIMU.h"

#include "wx_pch.h"
#include <wx/wfstream.h>
#include <wx/stdstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>


REGISTER_RECEIVER("File Receiver", ReceiverFile)

ReceiverFile::ReceiverFile()
{
    _binary = false;
    _pos = 0;
    _startTime = -1;
    _time = 0;
    setThreaded();
}

ReceiverFile::ReceiverFile(wxString filename) : _filename(filename)
{
    _binary = false;
    _pos = 0;
    _startTime = -1;
    _time = 0;
}

ReceiverFile::~ReceiverFile()
{
    //dtor
}


// -----------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Definitions

#define sampleFreqDef	50.0f		// sample frequency in Hz
#define betaDef		0.4f		// 2 * proportional gain

//---------------------------------------------------------------------------------------------------
// Variable definitions

volatile float sampleFreq = sampleFreqDef;
volatile float beta = betaDef;								// 2 * proportional gain (Kp)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	// quaternion of sensor frame relative to auxiliary frame
volatile unsigned int numIterations = 4;

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

// AHRS algorithm update

void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
		// MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);
		return;
	}

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
	qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
	qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
	qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q0 * mx;
		_2q0my = 2.0f * q0 * my;
		_2q0mz = 2.0f * q0 * mz;
		_2q1mx = 2.0f * q1 * mx;
		_2q0 = 2.0f * q0;
		_2q1 = 2.0f * q1;
		_2q2 = 2.0f * q2;
		_2q3 = 2.0f * q3;
		_2q0q2 = 2.0f * q0 * q2;
		_2q2q3 = 2.0f * q2 * q3;
		q0q0 = q0 * q0;
		q0q1 = q0 * q1;
		q0q2 = q0 * q2;
		q0q3 = q0 * q3;
		q1q1 = q1 * q1;
		q1q2 = q1 * q2;
		q1q3 = q1 * q3;
		q2q2 = q2 * q2;
		q2q3 = q2 * q3;
		q3q3 = q3 * q3;

		// Reference direction of Earth's magnetic field
		hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
		hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q0 += qDot1 * (1.0f / sampleFreq);
	q1 += qDot2 * (1.0f / sampleFreq);
	q2 += qDot3 * (1.0f / sampleFreq);
	q3 += qDot4 * (1.0f / sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= recipNorm;
	q1 *= recipNorm;
	q2 *= recipNorm;
	q3 *= recipNorm;
}

// -----------------------------------------------------------------------------------------------------------------------------------------
#include <wx/numdlg.h> // temporary include, remove!
bool ReceiverFile::setup()
{
    // temporary code to convert multiple IMU data files to files containing Madgwick filtered quaternions (uncomment to activate)
    /*
    long sampleFreqUser = wxGetNumberFromUser(_("Enter Madgwick sample frequency"), _("Frequency in Hz"), _("Number"), 200, 1, 10000);
    if (sampleFreqUser == -1)
    {
        return false;
    }
    long numIterationsUser = wxGetNumberFromUser(_("Enter Madgwick iterations"), _("Number of iterations"), _("Number"), 4, 1, 100);
    if (numIterationsUser == -1)
    {
        return false;
    }
    long betaUser = wxGetNumberFromUser(_("Enter Madgwick beta (in form of beta*1000)"), _("Beta in range (0, 1) * 1000"), _("Number"), 200, 1, 1000);
    if (betaUser == -1)
    {
        return false;
    }

    sampleFreq = sampleFreqUser;
    numIterations = numIterationsUser;
    beta = float(betaUser) / 1000.0f;

    // show a file dialog to prompt the user for the files
    wxFileDialog* multiFileDialog = new wxFileDialog(nullptr, _("Open file"), _(""), _(""), _("csv files (*.csv)|*.csv"), wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);
    if (multiFileDialog->ShowModal() == wxID_CANCEL)
    {
        multiFileDialog->Destroy();
        return false;
    }

    wxArrayString paths;
    multiFileDialog->GetPaths(paths);

    multiFileDialog->Destroy();

    for (size_t i = 0; i < paths.size(); ++i)
    {
        _filename = paths[i];
        if (connect())
        {
            wxFFileInputStream input(_filename);
            wxTextInputStream textIn(input);

            wxString newFilename = _filename.BeforeLast('.') + _("_Quaternion.csv");

            wxFFileOutputStream output(newFilename);
            wxTextOutputStream textOut(output);

            if (!input.IsOk() || input.Eof())
            {
                continue;
            }

            // consume first line
            wxString nextLine = textIn.ReadLine();
            textOut << nextLine << _(";q0;q1;q2;q3\n");

            size_t j = 0;
            while (input.IsOk() && !input.Eof() && j < _data.size())
            {
                Vector3 acc = _data[j].getAcceleration();
                Vector3 gyro = _data[j].getGyroscope();
                Vector3 magnet = _data[j].getMagnetometer();
                for (unsigned int k = 0; k < numIterations; ++k)
                {
                    MadgwickAHRSupdate(-acc.x(), -acc.y(), acc.z(), -gyro.x(), -gyro.y(), gyro.z(), -magnet.x(), -magnet.y(), -magnet.z());
                }
                ++j;

                wxString nextLine = textIn.ReadLine();
                nextLine << _(";") << q0 << _(";") << q1 << _(";") << q2 << _(";") << q3 << _("\n");
                textOut << nextLine;
            }
        }
    }
    // end of temporary code, return false to avoid further data processing
    return false;
    */

    // show a file dialog to prompt the user for a file
    wxFileDialog* fileDialog = new wxFileDialog(nullptr, _("Open file"), _(""), _(""), _("csv files (*.csv)|*.csv"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (fileDialog->ShowModal() == wxID_CANCEL)
    {
        fileDialog->Destroy();
        return false;
    }

    _filename = fileDialog->GetPath();

    fileDialog->Destroy();

    return true;
}

bool ReceiverFile::onUpdate()
{
    if (_data.size() == 0)
    {
        return false;
    }


    if (_startTime < 0)
    {
        // get time point at which the data arrived
        wxLongLong time = wxGetUTCTimeMillis();
        uint64_t receiveTime = (uint64_t(time.GetHi()) << 32) + time.GetLo();

        _startTime = receiveTime;
    }

    Vector3 acc = _data[_pos].getAcceleration();
    Vector3 gyro = _data[_pos].getGyroscope();
    Vector3 magnet = _data[_pos].getMagnetometer();
    for (size_t i = 0; i < 4; ++i)
    {
        MadgwickAHRSupdate(-acc.x(), -acc.y(), acc.z(), -gyro.x(), -gyro.y(), gyro.z(), -magnet.x(), -magnet.y(), -magnet.z());
    }

    SensorDataOrientation data;
    data.setOrientation(Quaternion(q0, q1, q2, q3));
    data.setTimestamp(_time);
    // data.setReceiveTime()
    theSensorManager.updateSensor(_id, &data, _startTime + _time);

    if (++_pos >= _data.size())
    {
        _pos = 0;
        _startTime = -1;
        _time = 0;
    }
    _time += 200;

    return true;
}

bool ReceiverFile::onConnect()
{
    if (_filename.length() == 0 || !wxFileExists(_filename))
    {
        return false;
    }
    MotionSequence* sequence = FileHandler::read(_filename);
    if (sequence != nullptr)
    {
        // handle sequence
    }

    // TODO(JK#2#2017-09-12): generalize file parsing in ReceiverFile (maybe also use FileHandler for parsing)

    // handle different files (.txt .csv etc.)
    wxFFileInputStream input(_filename);
    wxTextInputStream textIn(input);

    if (!input.IsOk() || input.Eof())
    {
        return nullptr;
    }

    wxStringTokenizer tokenizer;

    while (input.IsOk() && !input.Eof())
    {
        tokenizer.SetString(textIn.ReadLine(), _(" ,;\r\t\n"), wxTOKEN_STRTOK);
        int i = 0;
        int j = 0;
        Vector3 acc, gyro, magnet;

        if (tokenizer.CountTokens() == 10)
        {
            wxString token = tokenizer.GetNextToken();
            double val;
            if (token.ToDouble(&val))
            {
                // consume
            }
        }
        while (tokenizer.HasMoreTokens())
        {
            wxString token = tokenizer.GetNextToken();
            double val;
            if (!token.ToDouble(&val))
            {
                continue;
            }
            if (j == 0)
            {
                acc(i) = val;
            }
            else if (j == 1)
            {
                gyro(i) = val;
            }
            else if (j == 2)
            {
                magnet(i) = val;
            }
            ++i;
            if (i >= 3)
            {
                i = 0;
                ++j;
            }
            // wxLogDebug(token + _(" is ") << val);
        }
        // in order to skip invalid lines (no 3x3 entries) check if j had a chance to count up to three
        if (j >= 2)
        {
            /*
            wxString msg;
            msg << acc.x() << _(" ") << acc.y() << _(" ") << acc.z() << _(" ") << gyro.x() << _(" ") << gyro.y() << _(" ") << gyro.z() << _(" ") << magnet.x() << _(" ") << magnet.y() << _(" ") << magnet.z();
            wxLogDebug(msg);
            */

            acc *= ratio_acc;
            gyro *= ratio_gyro;
            magnet *= ratio_mag;

            _data.push_back(SensorDataIMURaw(acc, gyro, magnet));
        }
    }
    _pos = 0;
    _id = theSensorManager.createSensorNode<SensorNodeIMU>(_filename.ToStdString());
    return true;
}

void ReceiverFile::onDisconnect()
{
    _data.clear();
}

bool ReceiverFile::isConnected() const
{
    return (_data.size() > 0);
}

