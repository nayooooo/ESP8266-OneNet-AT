#ifndef __AT_H__
#define __AT_H__

#include <Arduino.h>
#include <stdlib.h>

enum At_Type{
    AT_TYPE_NULL = 0,
    AT_TYPE_CMD,
    AT_TYPE_DATA,
};
typedef enum At_Type At_Type_t;

enum At_Err{
    AT_OK = 0,
    AT_ERROR,
    AT_ERROR_NOT_FIND,
    AT_ERROR_NO_ACT,
    AT_ERROR_CANNOT_CUT,
};
typedef enum At_Err At_Err_t;

struct At_State{
    String atLable;
    At_Type_t type;
    At_Err_t (*act)(int argc, char *argv[]);
};
typedef struct At_State * At_State_t;

#define AT_LABLE_TAIL ""

#define AT_PARAM_MAX_NUM        (3)
struct At_Param{
    String cmd;
    int argc;
    char *argv[AT_PARAM_MAX_NUM];
};
typedef struct At_Param * At_Param_t;

class At_Class
{
public:
    At_Class(const At_State_t atTable);

private:
    At_Err_t At_Cut_String(struct At_Param &param, const String &atLable);
    At_State_t At_Check_String(struct At_Param &param, const At_State_t atTable, const String &atLable);
    
public:
    String At_ErrorToString(At_Err_t error);
    int At_Get_Param_Max_Num(void);
    At_Err_t At_Handle(const String &atLable);
    
protected:
    At_State_t _atTable;
    int _param_max_nun;
};

#endif /* __AT_H__ */
