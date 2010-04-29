#ifndef PARAMETERS_H
#define PARAMETERS_H

#ifdef __cplusplus
extern "C" {
#endif
  
  enum  type_check {CHECK_NONE, CHECK_SIZE, CHECK_MIN, CHECK_MAX, CHECK_BOTH, CHECK_VALUES};
  
  int initPList(int Pos, int ** param_addr);
  int checkPList(int * param_addr);
  int hasLabelInPList(int * param_addr, const char * Label);
  int hasPartialLabelInPList(int * param_addr, const char * Label);
  int getIntInPList(int * param_addr, const char * Label, int * value, int * found, int default_value, int log, type_check check, ...);
  int getDoubleInPList(int * param_addr, const char * Label, double * value, int * found, double default_value, int log, type_check check, ...);
  int getStringInPList(int * param_addr, const char * Label, char ** value, int * found, const char * default_value, int log, type_check check, ...);
  int getVectorOfIntInPList(int * param_addr, const char * Label, int * value, int * found, 
			    int default_value, int default_size, int * size, int Log, type_check check, ...);
  int getVectorOfDoubleInPList(int * param_addr, const char * Label, double * value, int * found, 
			       double default_value, int default_size, int * size, int Log, type_check check, ...);
  
  int createPList(int Pos, int ** param_addr, char ** LabelNames, int nb_params);
  int createIntInPList(int iVar, int * param_addr, char * LabelName, int value);
  int createDoubleInPList(int iVar, int * param_addr, char * LabelName, double value);
  int createStringInPList(int iVar, int * param_addr, char * LabelName, char * value);
  int createVectorOfIntInPList(int iVar, int * param_addr, char * LabelName, int nb_values, int * value);
  int createVectorOfDoubleInPList(int iVar, int * param_addr, char * LabelName, int nb_values, double * value);
  
#ifdef __cplusplus
}
#endif
#endif
