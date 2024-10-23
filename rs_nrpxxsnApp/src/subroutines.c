#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <epicsTypes.h>
#include <dbAccess.h>
#include <registryFunction.h>

#define MAXPOINTS 5001

/*
 * parsing waveform
 *
 * Parameters:
 *
 *  INA: measurement control
 *  INB: measurement mode
 *  INC: waveform data in watts
 *  IND: trace time
 *  INE: trace offset time
 *  INF: trigger source
 *  ING: trigger level in watts
 *  INH: eacc factor
 *
 *  VALA: @param [out] [waveform] xaxis in second
 *  VALB: @param [out] [waveform] yaxis in dbm
 *  VALC: @param [out] [waveform] yaxis in MV/m
 *  VALD: @param [out] [scalar]   yaxis unit
 *  VALE: @param [out] [waveform] trigger array in watts
 *  VALF: @param [out] [waveform] trigger array in dbm
 *
 */
static int parse_waveform(aSubRecord *precord)
{
    const epicsInt16 mesWF = *(epicsInt16*)precord->a;
    const epicsInt16 mode  = *(epicsInt16*)precord->b;
    if (mesWF==0 || (mesWF==1 && mode==0)) {
        memset(precord->vala, 0, precord->nova * dbValueSize((&precord->ftva)[0]));
        precord->neva=0;
        memset(precord->valb, 0, precord->novb * dbValueSize((&precord->ftvb)[0]));
        precord->nevb=0;
        memset(precord->valc, 0, precord->novc * dbValueSize((&precord->ftvc)[0]));
        precord->nevc=0;
        memset(precord->vale, 0, precord->nove * dbValueSize((&precord->ftve)[0]));
        precord->neve=0;
        memset(precord->valf, 0, precord->novf * dbValueSize((&precord->ftvf)[0]));
        precord->nevf=0;
    }
    else if (mesWF==1 && mode==1) {
        const epicsInt32 points = precord->nec;
        if (points > MAXPOINTS) {
            printf("[aSub ERROR] data points is too high");
            return -1;
        }
        if (points > precord->nova) {
            printf("[aSub Error] data points is bigger than NOVA");
            return -1;
        }
        epicsFloat64 *xaxis = malloc(points*dbValueSize((&precord->ftva)[0]));
        if (xaxis == NULL) {
            printf("[asub Error] cannot allocate memory");
            return -1;
        }
        epicsFloat64 *wf_dbm = malloc(points*dbValueSize((&precord->ftvb)[0]));
	if (wf_dbm == NULL) {
            printf("[asub Error] cannot allocate memory");
            return -1;
        }
        epicsFloat64 *wf_eacc = malloc(points*dbValueSize((&precord->ftvc)[0]));
	if (wf_eacc == NULL) {
            printf("[asub Error] cannot allocate memory");
            return -1;
        }
        epicsFloat64 *trig_wf_watts = malloc(points*dbValueSize((&precord->ftve)[0]));
        if (trig_wf_watts == NULL) {
            printf("[asub Error] cannot allocate memory");
            return -1;
        }
        epicsFloat64 *trig_wf_dbm = malloc(points*dbValueSize((&precord->ftvf)[0]));
        if (trig_wf_dbm == NULL) {
            printf("[asub Error] cannot allocate memory");
            return -1;
        }
        // parsing parameters
        epicsFloat64 *wf_watts = (epicsFloat64*)precord->c;
        const epicsFloat64 trace_time	= *(epicsFloat64*)precord->d;
        const epicsFloat64 trace_offset	= *(epicsFloat64*)precord->e;
        const char *source;
        source = (char*)precord->f;
        const epicsFloat64 triglvl_watts= *(epicsFloat64*)precord->g;
        const epicsFloat64 cavity_kt	= *(epicsFloat64*)precord->h;
        int i;
        double pmax = wf_watts[0];
        for (i=0; i<points; i++) {
            xaxis[i]   = 1E-3*(i*(trace_time/points)+trace_offset);
            wf_dbm[i]  = 10*log10(fabs(wf_watts[i]))+30;
            wf_eacc[i] = cavity_kt*sqrt(fabs(wf_watts[i]));
            if (wf_watts[i]>pmax) {pmax = wf_watts[i];}
        }
        int bit=1;
        if (pmax>=1000) {bit=2;}
        else if (pmax<=1) {bit=0;}
        memcpy(precord->vala, xaxis,	points*dbValueSize((&precord->ftva)[0]));
        precord->neva=points;
        memcpy(precord->valb, wf_dbm,	points*dbValueSize((&precord->ftvb)[0]));
        precord->nevb=points;
        memcpy(precord->valc, wf_eacc,	points*dbValueSize((&precord->ftvc)[0]));
        precord->nevc=points;
        *((epicsInt16*)precord->vald)=bit;
        precord->nevd=1;
       // parsing trigger waveform...
        int j;
        if (strcmp(source,"INT")==0) {
            for (j=0; j<points; j++) {
                trig_wf_watts[j]= triglvl_watts;
                trig_wf_dbm[j]	= 10*log10(triglvl_watts)+30;
            }
            memcpy(precord->vale, trig_wf_watts,points*dbValueSize((&precord->ftve)[0]));
            precord->neve=points;
            memcpy(precord->valf, trig_wf_dbm,	points*dbValueSize((&precord->ftvf)[0]));
            precord->nevf=points;
        }
        else if (strcmp(source,"INT")!=0) {
            memset(precord->vale, 0, precord->nove * dbValueSize((&precord->ftve)[0]));
            precord->neve=0;
            memset(precord->valf, 0, precord->novf * dbValueSize((&precord->ftvf)[0]));
            precord->nevf=0;
        }
        free(xaxis);
	free(wf_dbm);
	free(wf_eacc);
	free(trig_wf_watts);
	free(trig_wf_dbm);
    }

    /* Done! */
    return 0;
}
epicsRegisterFunction(parse_waveform);
