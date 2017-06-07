/*
 * File name: ComBDRate.cpp.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/9/20.
 */

#include <stdlib.h>
#include <cmath>
#include <cstring>
#include <assert.h>

#include "ComBDRate.h"

ComBDRate::ComBDRate()
{

}
ComBDRate::~ComBDRate()
{

}

void ComBDRate::InitParam(int iNumQp, double *pdAnchorRate, double *pdAnchorPSNRY, double *pdAnchorPSNRU, double *pdAnchorPSNRV, 
  double *pdTestRate, double *pdTestPSNRY, double *pdTestPSNRU, double *pdTestPSNRV)
{
  assert(iNumQp == 4);
  // Initialize memory.
  InitMemory();

  // Copy anchor data.
  assert(m_pdAnchorRate != NULL && pdAnchorRate != NULL);
  memcpy_s(m_pdAnchorRate, 4 * sizeof(double), pdAnchorRate, 4 * sizeof(double));

  assert(m_pdAnchorPSNRY != NULL && pdAnchorPSNRY != NULL);
  memcpy_s(m_pdAnchorPSNRY, 4 * sizeof(double), pdAnchorPSNRY, 4 * sizeof(double));

  assert(m_pdAnchorPSNRU != NULL && pdAnchorPSNRU != NULL);
  memcpy_s(m_pdAnchorPSNRU, 4 * sizeof(double), pdAnchorPSNRU, 4 * sizeof(double));

  assert(m_pdAnchorPSNRV != NULL && pdAnchorPSNRV != NULL);
  memcpy_s(m_pdAnchorPSNRV, 4 * sizeof(double), pdAnchorPSNRV, 4 * sizeof(double));

  // Copy test data.
  assert(m_pdTestRate != NULL && pdTestRate != NULL);
  memcpy_s(m_pdTestRate, 4 * sizeof(double), pdTestRate, 4 * sizeof(double));

  assert(m_pdTestPSNRY != NULL && pdTestPSNRY != NULL);
  memcpy_s(m_pdTestPSNRY, 4 * sizeof(double), pdTestPSNRY, 4 * sizeof(double));

  assert(m_pdTestPSNRU != NULL && pdTestPSNRU != NULL);
  memcpy_s(m_pdTestPSNRU, 4 * sizeof(double), pdTestPSNRU, 4 * sizeof(double));

  assert(m_pdTestPSNRV != NULL && pdTestPSNRV != NULL);
  memcpy_s(m_pdTestPSNRV, 4 * sizeof(double), pdTestPSNRV, 4 * sizeof(double));

  // Set result as 0.
  m_dBDRateResultY = m_dBDRateResultV = m_dBDRateResultU = 0;
}

void ComBDRate::InitMemory()
{
  m_pdAnchorRate = new double [4];
  m_pdAnchorPSNRY = new double [4];
  m_pdAnchorPSNRU = new double [4];
  m_pdAnchorPSNRV = new double [4];

  m_pdTestRate = new double [4];
  m_pdTestPSNRY = new double [4];
  m_pdTestPSNRU = new double [4];
  m_pdTestPSNRV = new double [4];
}

void ComBDRate::DestroyMemory()
{
  delete [] m_pdAnchorRate; m_pdAnchorRate = NULL;
  delete [] m_pdAnchorPSNRY; m_pdAnchorPSNRY =NULL;
  delete [] m_pdAnchorPSNRU; m_pdAnchorPSNRU = NULL;
  delete [] m_pdAnchorPSNRV; m_pdAnchorPSNRV = NULL;

  delete [] m_pdTestRate; m_pdTestRate = NULL;
  delete [] m_pdTestPSNRY; m_pdTestPSNRY = NULL;
  delete [] m_pdTestPSNRU; m_pdTestPSNRU = NULL;
  delete [] m_pdTestPSNRV; m_pdTestPSNRV = NULL;
}

double ComBDRate::Pchipend(double dH1, double dH2, double dDelta1, double dDelta2)
{
  double dD;

  dD = ((2 * dH1 + dH2) * dDelta1 - dH1 * dDelta2) / (dH1 + dH2);

  if (dD * dDelta1 < 0)
  {
    dD = 0;
  }
  else if((dDelta1 * dDelta2 < 0) && (abs(dD) > abs(3 * dDelta1)))
  {
    dD = 3 * dDelta1;
  }

  return dD;
}

double ComBDRate::BDRint(double *pdRate, double *pdPSNR, double dLow, double dHigh)
{
  double dLogRate[4];
  double dLogDistortion[4];
  double dH[4];
  double dDelta[4];
  double dD[4];
  double dC[4];
  double dB[4];

  for (int i = 0; i < 4; ++i)
  {
    dLogRate[i] = log10(pdRate[3 - i]);
    dLogDistortion[i] = pdPSNR[3 - i];
  }

  for (int i = 0; i < 3; ++i)
  {
    dH[i] = dLogDistortion[i + 1] - dLogDistortion[i];
    dDelta[i] = (dLogRate[i + 1] - dLogRate[i]) / dH[i];
  }

  dD[0] = Pchipend(dH[0], dH[1], dDelta[0], dDelta[1]);
  for (int i = 1; i < 3; ++i)
  {
    dD[i] = (3 * dH[i - 1] + 3 * dH[i]) / ((2 * dH[i] + dH[i - 1]) / dDelta[i - 1] + (dH[i] + 2 * dH[i - 1]) / dDelta[i]);
  }
  dD[3] = Pchipend(dH[2], dH[1], dDelta[2], dDelta[1]);

  for (int i = 0; i < 3; ++i)
  {
    dC[i] = (3 * dDelta[i] - 2 * dD[i] - dD[i + 1]) / dH[i];
    dB[i] = (dD[i] - 2 * dDelta[i] + dD[i + 1]) / (dH[i] * dH[i]);
  }

  double dResult = 0;
  double dS0, dS1;
  for (int i = 0; i < 3; ++i)
  {
    dS0 = dLogDistortion[i];
    dS1 = dLogDistortion[i + 1];

    dS0 = Max(dS0, dLow);
    dS0 = Min(dS0, dHigh);

    dS1 = Max(dS1, dLow);
    dS1 = Min(dS1, dHigh);

    dS0 = dS0 - dLogDistortion[i];
    dS1 = dS1 - dLogDistortion[i];

    if(dS1 > dS0)
    {
      dResult = dResult + (dS1 - dS0) * dLogRate[i];
      dResult = dResult + (dS1 * dS1 - dS0 * dS0) * dD[i] / 2;
      dResult = dResult + (dS1 * dS1 * dS1 - dS0 * dS0 * dS0) * dC[i] / 3;
      dResult = dResult + (dS1 * dS1 * dS1 * dS1 - dS0 * dS0 * dS0 * dS0) * dB[i] / 4;
    }
  }

  return dResult;
}

double ComBDRate::BDRateCalc(double *pdAnchorRate, double *pdAnchorPSNR, double *pdTestRate, double *pdTestPSNR)
{
  double dMaxAnchorPSNR = pdAnchorPSNR[0];
  double dMinAnchorPSNR = pdAnchorPSNR[0];
  double dMaxTestPSNR = pdTestPSNR[0];
  double dMinTestPSNR = pdTestPSNR[0];

  // Find max and min PSNR.
  for (int i = 1; i < 4; ++i)
  {
    if (dMaxAnchorPSNR < pdAnchorPSNR[i])
    {
      dMaxAnchorPSNR = pdAnchorPSNR[i];
    }

    if (dMinAnchorPSNR > pdAnchorPSNR[i])
    {
      dMinAnchorPSNR = pdAnchorPSNR[i];
    }

    if (dMaxTestPSNR < pdTestPSNR[i])
    {
      dMaxTestPSNR = pdTestPSNR[i];
    }

    if (dMinTestPSNR > pdTestPSNR[i])
    {
      dMinTestPSNR = pdTestPSNR[i];
    }
  }

  double dMinPSNR = Max(dMinAnchorPSNR, dMinTestPSNR);
  double dMaxPSNR = Min(dMaxAnchorPSNR, dMaxTestPSNR);

  double dVA = BDRint(pdAnchorRate, pdAnchorPSNR, dMinPSNR, dMaxPSNR);
  double dVB = BDRint(pdTestRate, pdTestPSNR, dMinPSNR, dMaxPSNR);

  double dAverage = (dVB - dVA) / (dMaxPSNR - dMinPSNR);

  return pow(10, dAverage) - 1;
}

void ComBDRate::BDRateCalcYUV()
{
  m_dBDRateResultY = BDRateCalc(m_pdAnchorRate, m_pdAnchorPSNRY, m_pdTestRate, m_pdTestPSNRY);
  assert(m_dBDRateResultY != 0);
  m_dBDRateResultU = BDRateCalc(m_pdAnchorRate, m_pdAnchorPSNRU, m_pdTestRate, m_pdTestPSNRU);
  assert(m_dBDRateResultU != 0);
  m_dBDRateResultV = BDRateCalc(m_pdAnchorRate, m_pdAnchorPSNRV, m_pdTestRate, m_pdTestPSNRV);
  assert(m_dBDRateResultV != 0);
}