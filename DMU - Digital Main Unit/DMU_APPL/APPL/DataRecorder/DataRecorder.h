#ifndef DATARECORDER_H_
#define DATARECORDER_H_

#include "Std_Types.h"

#define DATARECORDER_CCM_MAXNUMBER 	39
#define DATARECORDER_DCY_INIT  		0x01
#define DATARECORDER_DCY_START 		0x02
#define DATARECORDER_DCY_END   		0x03

extern void DataRecorder_MainFunction(void);

#endif /* DATARECORDER_H_ */