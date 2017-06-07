/*
 * File name: ComBDRate.h.
 * Function:
 * Author: Ylonge from MCL-ZJU.
 * Date: 2015/9/20.
 */
#pragma once

#include <stdlib.h>

#include "ComClass/TypeDef.h"

class ComBDRate
{
private:

  double *m_pdAnchorRate;
  double *m_pdAnchorPSNRY;
  double *m_pdAnchorPSNRU;
  double *m_pdAnchorPSNRV;

  double *m_pdTestRate;
  double *m_pdTestPSNRY;
  double *m_pdTestPSNRU;
  double *m_pdTestPSNRV;

  double m_dBDRateResultY;
  double m_dBDRateResultU;
  double m_dBDRateResultV;

public:
  ComBDRate();
  ~ComBDRate();

  /************************************************************************/
  /* Initialization
  /************************************************************************/

  void InitParam(int iNumQp, double *pdAnchorRate, double *pdAnchorPSNRY, double *pdAnchorPSNRU, double *pdAnchorPSNRV, 
    double *pdTestRate, double *pdTestPSNRY, double *pdTestPSNRU, double *pdTestPSNRV);
  void InitMemory();
  void DestroyMemory();

  /************************************************************************/
  /* Fetch value.
  /************************************************************************/

  double GetBDRateResultY() {return m_dBDRateResultY;};
  double GetBDRateResultU() {return m_dBDRateResultU;};
  double GetBDRateResultV() {return m_dBDRateResultV;};

  /************************************************************************/
  /* BDrate computation.
  /************************************************************************/

  void BDRateCalcYUV();

protected:
  double BDRateCalc(double *pdAnchorRate, double *pdAnchorPSNR, double *pdTestRate, double *pdTestPSNR);
  double BDRint(double *pdRate, double *pdPSNR, double dLow, double dHigh);
  double Pchipend(double dH1, double dH2, double dDelta1, double dDelta2);
};